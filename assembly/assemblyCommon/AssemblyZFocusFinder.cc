/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyZFocusFinder.h>
#include <AssemblyUtilities.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <memory>

#include <TCanvas.h>
#include <TGraph.h>
#include <TFile.h>

int AssemblyZFocusFinder::exe_counter_ = -1;

AssemblyZFocusFinder::AssemblyZFocusFinder(const QString& output_dir_prepath, const AssemblyVUEyeCamera* camera, const LStepExpressMotionManager* motion_manager, QObject* parent)
 : QObject(parent)

 , output_dir_prepath_(output_dir_prepath)

 , camera_manager_(camera)
 , motion_manager_(motion_manager)

 , motion_enabled_(false)

 , focus_completed_(false)

 , zposi_init_(-9999.)

 , zrelm_index_(0)

 , output_dir_("")
{
  // initialization
  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyZFocusFinder", NQLog::Fatal) << "initialization error"
       << ": ApplicationConfig::instance() not initialized (null pointer), exiting constructor";

    return;
  }

  focus_pointN_max_ = config->getDefaultValue<int>   ("main", "AssemblyZFocusFinder_pointN_max", 200);
  focus_pointN_     = config->getDefaultValue<int>   ("main", "AssemblyZFocusFinder_pointN"    ,  50);

  focus_zrange_max_ = config->getDefaultValue<double>("main", "AssemblyZFocusFinder_zrange_max", 3.0);
  focus_zrange_     = config->getDefaultValue<double>("main", "AssemblyZFocusFinder_zrange"    , 0.5);

  focus_stepsize_min_ = config->getDefaultValue<double>("main", "AssemblyZFocusFinder_stepsize_min", 0.005);

  v_zrelm_vals_.clear();
  v_focus_vals_.clear();
  // --------------

  // validation
  if(camera_manager_ == nullptr)
  {
    NQLog("AssemblyZFocusFinder", NQLog::Fatal) << "initialization error"
       << ": null pointer to AssemblyVUEyeCamera object, exiting constructor";

    return;
  }

  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyZFocusFinder", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }
  // --------------

  // connection(s)
  connect(this, SIGNAL(next_zpoint()), this, SLOT(test_focus()));
  // --------------

  NQLog("AssemblyZFocusFinder", NQLog::Debug) << "constructed";
}

void AssemblyZFocusFinder::enable_motion()
{
    if(motion_enabled_ == false)
    {
      connect(this, SIGNAL(focus(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));

      connect(motion_manager_, SIGNAL(motion_finished()), camera_manager_, SLOT(acquireImage()));

      connect(camera_manager_, SIGNAL(imageAcquired(cv::Mat)), this, SLOT(process_image(cv::Mat)));

      motion_enabled_ = true;

      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "enable_motion"
         << ": AssemblyZFocusFinder connected to AssemblyVUEyeCamera and LStepExpressMotionManager";
    }

    return;
}

void AssemblyZFocusFinder::disable_motion()
{
    if(motion_enabled_ == true)
    {
      disconnect(this, SIGNAL(focus(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));

      disconnect(motion_manager_, SIGNAL(motion_finished()), camera_manager_, SLOT(acquireImage()));

      disconnect(camera_manager_, SIGNAL(imageAcquired(cv::Mat)), this, SLOT(process_image(cv::Mat)));

      motion_enabled_ = false;

      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "disable_motion"
         << ": AssemblyZFocusFinder disconnected from AssemblyVUEyeCamera and LStepExpressMotionManager";
    }

    return;
}

void AssemblyZFocusFinder::update_focus_config(const double zrange, const int pointN)
{
    if(zrange > 0.)
    {
      if(zrange > focus_zrange_max_)
      {
        NQLog("AssemblyZFocusFinder", NQLog::Warning) << "update_focus_config"
           << ": input value for z-motion range (" << zrange << ")"
           << " larger than allowed max ("   << focus_zrange_max_ << ")"
           << ", setting equal to allowed max";

        focus_zrange_ = focus_zrange_max_;
      }
      else
      {
        focus_zrange_ = zrange;
      }

      NQLog("AssemblyZFocusFinder", NQLog::Message) << "update_focus_config"
         << ": updated z-motion range to " << focus_zrange_;
    }
    else
    {
      NQLog("AssemblyZFocusFinder", NQLog::Critical) << "update_focus_config"
         << ": non-positive input value for z-motion range, value not updated";

      return;
    }

    if(pointN > 1)
    {
      if(pointN > focus_pointN_max_)
      {
        NQLog("AssemblyZFocusFinder", NQLog::Warning) << "update_focus_config"
           << ": input value for number of scans (" << pointN << ")"
           << " larger than allowed max ("    << focus_pointN_max_ << ")"
           << ", setting equal to allowed max";

        focus_pointN_ = focus_pointN_max_;
      }
      else
      {
        focus_pointN_ = pointN;
      }

      NQLog("AssemblyZFocusFinder", NQLog::Message) << "update_focus_config"
         << ": updated number of scans to " << focus_pointN_;
    }
    else
    {
      NQLog("AssemblyZFocusFinder", NQLog::Critical) << "update_focus_config"
         << ": invalid (<=1) value for number of scans, value not updated";

      return;
    }

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "update_focus_config"
       << ": emitting signal \"updated_focus_config\"";

    emit updated_focus_config();
}

void AssemblyZFocusFinder::acquire_image()
{
    if(focus_pointN_ <= 1)
    {
      NQLog("AssemblyZFocusFinder", NQLog::Fatal) << "acquire_image"
         << ": invalid (<=1) value for number of scans (" << focus_pointN_ << "), stopping AssemblyZFocusFinder";

      this->disable_motion();

      return;
    }

    const double step_size = (2. * focus_zrange_ / double(focus_pointN_ - 1));

    if(step_size < focus_stepsize_min_)
    {
      NQLog("AssemblyZFocusFinder", NQLog::Fatal) << "acquire_image"
         << ": expected step size value (" << step_size << ") smaller than min-allowed value ("
         << focus_stepsize_min_ << "), stopping AssemblyZFocusFinder";

      this->disable_motion();

      return;
    }

    this->enable_motion();

    focus_completed_ = false;

    bool  output_dir_exists(true);
    while(output_dir_exists)
    {
      ++exe_counter_;

      std::string exe_counter_str = std::to_string(exe_counter_);

      if(exe_counter_ < 1e3)
      {
        std::stringstream exe_counter_strss;
        exe_counter_strss << std::setw(3) << std::setfill('0') << exe_counter_;
        exe_counter_str = exe_counter_strss.str();
      }

      output_dir_ = output_dir_prepath_.toStdString()+"/"+exe_counter_str+"/";

      output_dir_exists = assembly::DirectoryExists(output_dir_);
    }

    assembly::QDir_mkpath(output_dir_);

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "acquire_image"
       << ": created output directory: " << output_dir_;

    zposi_init_ = motion_manager_->get_position_Z();

    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    // scan N points around initial position
    const double zmin = (zposi_init_ - focus_zrange_);
    const double zmax = (zposi_init_ + focus_zrange_);

    v_zrelm_vals_.emplace_back(zmax - zposi_init_);

    for(int i=1; i<focus_pointN_; ++i)
    {
      v_zrelm_vals_.emplace_back(-1.0 * step_size);
    }

    zrelm_index_ = -1;

    NQLog("AssemblyZFocusFinder", NQLog::Message) << "acquire_image"
       << ": initialized auto-focusing"
       << " (z-min=" << zmin << ", z-max=" << zmax << ", steps=" << focus_pointN_ << ")";

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "acquire_image"
       << ": emitting signal \"next_zpoint\"";

    emit next_zpoint();
}

void AssemblyZFocusFinder::test_focus()
{
  ++zrelm_index_;

  if(zrelm_index_ < 0)
  {
    NQLog("AssemblyZFocusFinder", NQLog::Fatal) << "test_focus"
       << ": logic error -> negative index for std::vector \"v_zrelm_vals_\", stopping AssemblyZFocusFinder";

    this->disable_motion();

    return;
  }
  else if(zrelm_index_ < int(v_zrelm_vals_.size()))
  {
    const double dz = v_zrelm_vals_.at(zrelm_index_);

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "test_focus"
       << ": emitting signal \"focus(0, 0, " << dz << ", 0)\"";

    emit focus(0., 0., dz, 0.);

    emit sig_update_progBar(int(zrelm_index_*100./v_zrelm_vals_.size())); //Update progress bar display
  }
  else
  {
    // Find best position
    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "test_focus"
       << ": finding best-focus position";

    double zposi_best(zposi_init_);
    {
      std::unique_ptr<TGraph> zscan_gra(new TGraph(v_focus_vals_.size()));
      zscan_gra->SetName("zfocus_graph");
      zscan_gra->SetTitle(";z-axis position [mm];focus discriminant");
      zscan_gra->SetMarkerColor(2);
      zscan_gra->SetMarkerStyle(20);
      zscan_gra->SetMarkerSize(1.25);

      double focus_best(-1.);
      for(unsigned int i=0; i<v_focus_vals_.size(); ++i)
      {
        const double i_zposi = v_focus_vals_.at(i).z_position;
        const double i_focus = v_focus_vals_.at(i).focus_disc;

        zscan_gra->SetPoint(i, i_zposi, i_focus);

        if((i == 0) || (i_focus > focus_best)){ focus_best = i_focus; zposi_best = i_zposi; }
      }

      std::unique_ptr<TCanvas> zscan_can(new TCanvas());
      zscan_can->SetName("zfocus_plot");
      zscan_can->cd();
      zscan_gra->Draw("alp");

      const std::string zscan_plot_path_png  = output_dir_+"/AssemblyZFocusFinder_zscan.png";
      const std::string zscan_plot_path_root = output_dir_+"/AssemblyZFocusFinder_zscan.root";

      zscan_can->SaveAs(zscan_plot_path_png.c_str());

      std::unique_ptr<TFile> zscan_fil(new TFile(zscan_plot_path_root.c_str(), "recreate"));
      zscan_fil->cd();
      zscan_can->Write();
      zscan_gra->Write();
      zscan_fil->Close();

      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "test_focus"
         << ": emitting signal \"show_zscan(" << zscan_plot_path_png << ")\"";

      emit show_zscan(QString(zscan_plot_path_png.c_str()));

      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "test_focus"
         << ": emitting signal \"text_update_request(" << zposi_best << ")\"";

      emit text_update_request(zposi_best);
    }
    // ------------------

    // text output file -
    std::ofstream txtfile(output_dir_+"/values.txt");
    if(txtfile.is_open())
    {
      txtfile << "# index z-position focus_discriminant\n";

      for(unsigned int i=0; i<v_focus_vals_.size(); ++i)
      {
        txtfile << i << " " << v_focus_vals_.at(i).z_position << " " << v_focus_vals_.at(i).focus_disc << std::endl;
      }

      txtfile.close();
    }

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "test_focus"
      << ": created output file: " << output_dir_+"/values.txt";
    // ------------------

    // auto-focus completed: move to best-focus position
    focus_completed_ = true;

    const double zposi_now = motion_manager_->get_position_Z();

    const double dz = (zposi_best-zposi_now);

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "test_focus"
       << ": emitting signal \"focus(0, 0, " << dz << ", 0)\"";

    emit focus(0., 0., dz, 0.);

    emit sig_update_progBar(int(100)); //Update progress bar display
    // ------------------
  }

  return;
}

void AssemblyZFocusFinder::emergencyStop()
{
  NQLog("AssemblyZFocusFinder", NQLog::Message) << "emergencyStop"
     << ": will stop execution of AssemblyZFocusFinder";

  this->disable_motion();

  v_zrelm_vals_.clear();

  v_focus_vals_.clear();

  focus_completed_ = false;

  zrelm_index_ = -1;

  NQLog("AssemblyZFocusFinder", NQLog::Message) << "emergencyStop"
     << ": emitting signal \"emergencyStopped\"";

  emit emergencyStopped();
}



void AssemblyZFocusFinder::process_image(const cv::Mat& img)
{
  NQLog("AssemblyZFocusFinder", NQLog::Spam) << "process_image";

  if(focus_completed_)
  {
    // disconnect z-focus-finder and motion-manager
    this->disable_motion();

    // clear internal data members
    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    focus_completed_ = false;

    zrelm_index_ = -1;

    // save best-focus image
    const std::string img_outpath = output_dir_+"/AssemblyZFocusFinder_best.png";

    cv::imwrite(img_outpath, img);

    NQLog("AssemblyZFocusFinder", NQLog::Spam) << "process_image"
       << ": emitting signal \"image_acquired\"";

    emit image_acquired(img);
  }
  else
  {
    // --- generic z-focus step ---

    // save image
    const std::string img_outpath = output_dir_+"/AssemblyZFocusFinder_"+std::to_string(v_focus_vals_.size())+".png";
    cv::imwrite(img_outpath, img);

    // save z-focus info
    AssemblyZFocusFinder::focus_info this_focus;
    this_focus.focus_disc = this->image_focus_value(img);
    this_focus.z_position = motion_manager_->get_position_Z();

    if((fabs(this_focus.z_position - zposi_init_) - focus_zrange_) > focus_stepsize_min_)
    {
      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "process_image"
         << ": logic error -> current Z-position (" << this_focus.z_position
         << ") outside z-finder bounds [" << (zposi_init_ - focus_zrange_) << ", " << (zposi_init_ + focus_zrange_)
         << "] by more than " << focus_stepsize_min_ << " mm"
         << ", sending image and terminating auto-focus routine";

      // disconnect z-focus-finder and motion-manager
      this->disable_motion();

      // clear internal data members
      v_zrelm_vals_.clear();

      v_focus_vals_.clear();

      focus_completed_ = false;

      zrelm_index_ = -1;

      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "process_image"
         << ": emitting signal \"image_acquired\"";

      emit image_acquired(img);
    }
    else
    {
      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "process_image"
         << ": image(" << exe_counter_ << "," << v_focus_vals_.size() << ")"
         << " [z = " << this_focus.z_position
         << ", focus-value = " << this_focus.focus_disc << "]";

      v_focus_vals_.emplace_back(this_focus);
      // ------------------------------

      // go to next z-focus step
      NQLog("AssemblyZFocusFinder", NQLog::Spam) << "process_image"
         << ": emitting signal \"next_zpoint\"";

      emit next_zpoint();
      // ------------------------------
    }
  }

  return;
}

// \Brief Image-focus discriminant based on Laplacian method in OpenCV
//        REF: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/laplace_operator/laplace_operator.html
double AssemblyZFocusFinder::image_focus_value(const cv::Mat& img)
{
//  // Remove noise by blurring with a Gaussian filter
//  cv::Mat img_gaus;
//  cv::GaussianBlur(img, img_gaus, cv::Size(img.cols, img.rows), 0, 0, cv::BORDER_DEFAULT);
//
//  // Convert the image to grayscale
//  cv::Mat img_gray;
//  cv::cvtColor(img_gaus, img_gray, cv::COLOR_BGR2GRAY);

  // Apply laplacian function to GS image
  cv::Mat img_lap;
  cv::Laplacian(img, img_lap, CV_64F);

  cv::Mat img_lap_abs;
//  cv::convertScaleAbs(img_lap, img_lap_abs);
  img_lap_abs = img_lap.clone();

  // Calculate standard deviation of laplace image
  cv::Scalar mean, std_dev;
  cv::meanStdDev(img_lap_abs, mean, std_dev);

  const float value = (std_dev.val[0] * std_dev.val[0]);

  return value;
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <ZFocusFinder.h>
#include <nqlogger.h>
#include <Util.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdio>
#include <memory>

#include <TGraph.h>
#include <TCanvas.h>

int ZFocusFinder::exe_counter_ = -1;
int ZFocusFinder::focus_pointN_max_ = 200;

ZFocusFinder::ZFocusFinder(AssemblyVUEyeCamera* camera, LStepExpressModel* motion_model, QObject* parent) :
  QObject(parent),
  camera_manager_(0),
  motion_manager_(0),
  motion_enabled_(false),
  focus_completed_(false),
  focus_pointN_(10),
  focus_zrange_(0.5),
  zposi_init_(-9999.),
  zposi_min_ (-99.),
  zposi_max_ (+99.),
  zrelm_index_(0),
  output_dir_("")
{
    if(!camera)
    {
      NQLog("ZFocusFinder::ZFocusFinder", NQLog::Fatal) << "null pointer to AssemblyVUEyeCamera object";
      exit(1);
    }

    if(!motion_model)
    {
      NQLog("ZFocusFinder::ZFocusFinder", NQLog::Fatal) << "null pointer to LStepExpressModel object";
      exit(1);
    }

    camera_manager_ = new AssemblyUEyeCameraManager(camera);
    motion_manager_ = new LStepExpressMotionManager(motion_model);

    ////

    connect(this , SIGNAL(next_zpoint()), this, SLOT(test_focus()));

    ////

    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    NQLog("ZFocusFinder::ZFocusFinder") << "constructed";
}

void ZFocusFinder::enable_motion()
{
    if(motion_enabled_ == false)
    {
      connect   (this           , SIGNAL(focus       (double, double, double, double)),
                 motion_manager_, SLOT  (moveRelative(double, double, double, double)));

      connect   (motion_manager_, SIGNAL(motion_finished()),
                 camera_manager_, SLOT  (acquire_image()));

      connect   (camera_manager_, SIGNAL(image_acquired(cv::Mat)),
                 this           , SLOT  (process_image (cv::Mat)));

      motion_enabled_ = true;

      NQLog("ZFocusFinder::enable_motion") << "completed";
    }

    return;
}

void ZFocusFinder::disable_motion()
{
    if(motion_enabled_ == true)
    {
      disconnect(this           , SIGNAL(focus       (double, double, double, double)),
                 motion_manager_, SLOT  (moveRelative(double, double, double, double)));

      disconnect(motion_manager_, SIGNAL(motion_finished()),
                 camera_manager_, SLOT  (acquire_image()));

      disconnect(camera_manager_, SIGNAL(image_acquired(cv::Mat)),
                 this           , SLOT  (process_image (cv::Mat)));

      motion_enabled_ = false;

      NQLog("ZFocusFinder::disable_motion") << "completed";
    }

    return;
}

void ZFocusFinder::update_focus_inputs(const double zrange, const int pointN)
{
    if(zrange > 0.)
    {
      focus_zrange_ = zrange;

      NQLog("ZFocusFinder::update_focus_inputs") << "updated z-motion range to " << focus_zrange_;
    }
    else
    {
      NQLog("ZFocusFinder::update_focus_inputs") << "non-positive input value for z-motion range"
                                                 << ", value not updated";
    }

    if(pointN > 0)
    {
      if(pointN > ZFocusFinder::focus_pointN_max_)
      {
        NQLog("ZFocusFinder::update_focus_inputs") << "input number of scans larger than allowed max"
                                                   << ", setting equal to allowed max";

        focus_pointN_ = ZFocusFinder::focus_pointN_max_;

        NQLog("ZFocusFinder::update_focus_inputs") << "updated number of scans to " << focus_pointN_;
      }
      else
      {
        focus_pointN_ = pointN;

        NQLog("ZFocusFinder::update_focus_inputs") << "updated number of scans to " << focus_pointN_;
      }
    }
    else
    {
      NQLog("ZFocusFinder::update_focus_inputs") << "non-positive input value for number of scans"
                                                 << ", value not updated";
    }

    return;
}

void ZFocusFinder::acquire_image()
{
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

      output_dir_ = Util::QtCacheDirectory().toStdString()+"/autofocus/"+exe_counter_str+"/";

      output_dir_exists = Util::DirectoryExists(output_dir_);
    }

    Util::QDir_mkpath(output_dir_);

    NQLog("ZFocusFinder::acquire_image") << "created output directory: " << output_dir_;

    zposi_init_ = motion_manager_->get_position_Z();

    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    /** scan N points around initial position
     *  TO DO: introduce pre-scan routine to
     *         pin down z-interval containing best-focus position,
     *         followed by finer scan in that interval
     */
    const double zmin = std::max(zposi_min_, zposi_init_ - (focus_zrange_/2.));
    const double zmax = std::min(zposi_max_, zposi_init_ + (focus_zrange_/2.));

    v_zrelm_vals_.emplace_back(zmax - zposi_init_);

    const double zstep = -1.0 * ((zmax - zmin) / double(focus_pointN_ - 1));

    for(int i=1; i<focus_pointN_; ++i)
    {
      v_zrelm_vals_.emplace_back(zstep);
    }

    zrelm_index_ = -1;

    NQLog("ZFocusFinder::acquire_image") << "emitting signal \"next_zpoint\"";

    emit next_zpoint();
}

void ZFocusFinder::test_focus()
{
  ++zrelm_index_;

  if(zrelm_index_ < 0)
  {
    NQLog("ZFocusFinder::test_focus", NQLog::Fatal)
         << "logic error: negative index for std::vector \"v_zrelm_vals_\"";

    exit(1);
  }
  else if(zrelm_index_ < int(v_zrelm_vals_.size()))
  {
    const double dz = v_zrelm_vals_.at(zrelm_index_);

    NQLog("ZFocusFinder::test_focus") << "emitting signal \"focus(0, 0, " << dz << ", 0)\"";

    emit focus(0., 0., dz, 0.);

    return;
  }
  else
  {
    // Find best position
    NQLog("ZFocusFinder::test_focus") << "finding best-focus position";

    double zposi_best(zposi_init_);
    {
      std::unique_ptr<TGraph> zscan_gra(new TGraph(v_focus_vals_.size()));
      zscan_gra->SetName("zfocus");
      zscan_gra->SetTitle(";z-axis position [mm];focus discriminant");
      zscan_gra->SetMarkerColor(4);
      zscan_gra->SetMarkerStyle(21);
      zscan_gra->SetMarkerSize(1.5);

      double focus_best(-1.);
      for(unsigned int i=0; i<v_focus_vals_.size(); ++i)
      {
        const double i_zposi = v_focus_vals_.at(i).z_position;
        const double i_focus = v_focus_vals_.at(i).focus_disc;

        zscan_gra->SetPoint(i, i_zposi, i_focus);

        if((i==0) || (i_focus > focus_best)){ focus_best = i_focus; zposi_best = i_zposi; }
      }

      std::unique_ptr<TCanvas> zscan_can(new TCanvas());
      zscan_can->cd();
      zscan_gra->Draw("alp");

      const std::string zscan_plot_path_png  = output_dir_+"/ZFocusFinder_zscan.png";
      const std::string zscan_plot_path_root = output_dir_+"/ZFocusFinder_zscan.root";

      zscan_can->SaveAs(zscan_plot_path_png .c_str());
      zscan_can->SaveAs(zscan_plot_path_root.c_str());

      NQLog("ZFocusFinder::test_focus") << "emitting signal \"show_zscan(" << zscan_plot_path_png << ")\"";

      emit show_zscan(zscan_plot_path_png);

      NQLog("ZFocusFinder::test_focus") << "emitting signal \"update_text(" << zposi_best << ")\"";

      emit update_text(zposi_best);
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
    // ------------------

    // auto-focus completed: move to best-focus position
    focus_completed_ = true;

    const double zposi_now = motion_manager_->get_position_Z();

    const double dz = (zposi_best-zposi_now);

    NQLog("ZFocusFinder::test_focus") << "emitting signal \"focus(0, 0, " << dz << ", 0)\"";

    emit focus(0., 0., dz, 0.);

    return;
    // ------------------
  }

  return;
}

void ZFocusFinder::process_image(const cv::Mat& img)
{
  NQLog("ZFocusFinder::process_image");

  if(focus_completed_)
  {
    // disable autofocus motion manager
    this->disable_motion();

    // clear internal data members
    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    focus_completed_ = false;

    zrelm_index_ = -1;

    // save best-focus image
    const std::string img_outpath = output_dir_+"/ZFocusFinder_best.png";

    cv::imwrite(img_outpath, img);

    // signal to end autofocus
    NQLog("ZFocusFinder::process_image") << "emitting signal \"image_acquired\"";

    emit image_acquired(img);
  }
  else
  {
    // --- generic autofocus step ---

    // save image
    const std::string img_outpath = output_dir_+"/ZFocusFinder_"+std::to_string(v_focus_vals_.size())+".png";
    cv::imwrite(img_outpath, img);

    // save focus info
    ZFocusFinder::focus_info this_focus;
    this_focus.z_position = motion_manager_->get_position_Z();
    this_focus.focus_disc = this->image_focus_value(img);

    NQLog("ZFocusFinder", NQLog::Spam) << "[process_image]"
       << " image(" << exe_counter_ << "," << v_focus_vals_.size() << "),"
       << " focus-value = " << this_focus.focus_disc;

    v_focus_vals_.emplace_back(this_focus);
    // ------------------------------

    // --- go to next autofocus step
    NQLog("ZFocusFinder", NQLog::Debug) << "[process_image]" << " emitting signal \"next_zpoint\"";

    emit next_zpoint();
    // ------------------------------
  }

  return;
}

/* \Brief Image-focus discriminant based on Laplacian method in OpenCV
 *        REF: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/laplace_operator/laplace_operator.html
 */
double ZFocusFinder::image_focus_value(const cv::Mat& img)
{
//  // Remove noise by blurring with a Gaussian filter
//  cv::Mat img_gaus;
//  cv::GaussianBlur(img, img_gaus, cv::Size(img.cols, img.rows), 0, 0, cv::BORDER_DEFAULT);
//
//  // Convert the image to grayscale
//  cv::Mat img_gray;
//  cv::cvtColor(img_gaus, img_gray, CV_BGR2GRAY);

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

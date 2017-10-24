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
#include <Log.h>

#include <iostream>
#include <vector>
#include <cstdio>

#include <TGraph.h>
#include <TCanvas.h>

int ZFocusFinder::exe_counter_ = -1;
int ZFocusFinder::focus_pointN_max_ = 20;

ZFocusFinder::ZFocusFinder(AssemblyVUEyeCamera* camera, LStepExpressModel* motion_model, QObject* parent) :
  QObject(parent),
  camera_manager_(0),
  motion_manager_(0),
  motion_enabled_(false),
  focus_completed_(false),
  focus_pointN_(5),
  focus_zsigma_(1.0),
  zposi_init_(-9999.),
  zposi_min_ (-60.),
  zposi_max_ (+99.),
  zrelm_index_(0),
  image_counter_(0),
  output_dir_("")
{
    if(!camera)
    {
      Log::KILL("ZFocusFinder::ZFocusFinder -- null pointer to AssemblyVUEyeCamera object");
    }

    if(!motion_model)
    {
      Log::KILL("ZFocusFinder::ZFocusFinder -- null pointer to LStepExpressModel object");
    }

    camera_manager_ = new AssemblyUEyeCameraManager(camera);
    motion_manager_ = new LStepExpressMotionManager(motion_model);

    ////

    connect(this , SIGNAL(next_zpoint()), this, SLOT(acquire_image()));

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

      NQLog("ZFocusFinder::enable_motion");
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

      NQLog("ZFocusFinder::disable_motion");
    }

    return;
}

void  ZFocusFinder::update_focus_inputs(const double zsigma, const int pointN)
{
    if(zsigma > 0.)
    {
      focus_zsigma_ = zsigma;
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
      }
      else
      {
        focus_pointN_ = pointN;
      }
    }
    else
    {
      NQLog("ZFocusFinder::update_focus_inputs") << "non-positive input value for number of scans"
                                                 << ", value not updated";
    }

    return;
}

void ZFocusFinder::execute()
{
    this->enable_motion();

    focus_completed_ = false;

    ++exe_counter_;

    std::string exe_counter_str = std::to_string(exe_counter_);

    if(exe_counter_ < 1000)
    {
      char tmp[256];
      sprintf(tmp, "%03d", exe_counter_);
      exe_counter_str = std::string(tmp);
    }

    const std::string output_subdir("/autofocus/"+exe_counter_str);

    output_dir_ = Util::QtCacheDirectory()+QString(output_subdir.c_str());
    Util::QDir_mkpath(output_dir_);

    zposi_init_ = motion_manager_->get_position_Z();

    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    /** scan N points around initial position
     *  TO DO: introduce pre-scan routine to
     *         pin down z-interval containing best-focus position,
     *         followed by finer scan in that interval
     */

//!!    // step-1: scan N points around initial position to find interval containing the best-focus
//!!    const unsigned int prescan_imageN = 5;
//!!
//!!    const double zposi_init = motion_manager_->get_position_Z();
//!!
//!!    const double zposi_max = zposi_init + autofocus_zrange_;
//!!    const double zposi_min = zposi_init - autofocus_zrange_;
//!!
//!!    const double prescan_zrelm = (autofocus_zrange_ * 2.0 / double(prescan_imageN - 1));

    const double zmin = std::max(zposi_min_, zposi_init_ - focus_zsigma_);
    const double zmax = std::min(zposi_max_, zposi_init_ + focus_zsigma_);

    v_zrelm_vals_.emplace_back(zmax - zposi_init_);

    const double zstep = -1.0 * ((zmax - zmin) / double(focus_pointN_ - 1));

    for(int i=1; i<focus_pointN_; ++i)
    {
      v_zrelm_vals_.emplace_back(zstep);
    }

    zrelm_index_ = -1;

    NQLog("ZFocusFinder::execute") << "emitting signal \"next_zpoint\"";

    emit next_zpoint();
}

void ZFocusFinder::acquire_image()
{
  NQLog("ZFocusFinder::acquire_image");

  ++zrelm_index_;

  if(zrelm_index_ < 0)
  {
    std::string log = "logic error: negative index for std::vector \"v_zrelm_vals_\"";
    log += "logic error: negative index for std::vector \"v_zrelm_vals_\"";

    Log::KILL("ZFocusFinder::acquire_image -- "+log);
  }
  else if(zrelm_index_ < int(v_zrelm_vals_.size()))
  {
    const double dz = v_zrelm_vals_.at(zrelm_index_);

    NQLog("ZFocusFinder::acquire_image") << "emitting signal \"focus(0, 0, " << dz << ", 0)\"";

    emit focus(0., 0., dz, 0.);

    return;
  }
  else
  {
    // Find best position
    NQLog("ZFocusFinder::acquire_image") << "finding best-focus position";

    double zposi_best(zposi_init_);
    {
      std::unique_ptr<TGraph> zscan_gra(new TGraph(v_focus_vals_.size()));

      double focus_max(-1.);
      for(unsigned int i=0; i<v_focus_vals_.size(); ++i)
      {
        const double i_zposi = v_focus_vals_.at(i).z_position;
        const double i_focus = v_focus_vals_.at(i).focus_disc;

        zscan_gra->SetPoint(i, i_zposi, i_focus);

        if((i==0) || (i_focus > focus_max)){ focus_max = i_focus; zposi_best = i_zposi; }
      }

      std::unique_ptr<TCanvas> zscan_can(new TCanvas());
      zscan_can->cd();
      zscan_gra->Draw("AC*");

      const std::string zscan_plot_path = output_dir_.toStdString()+"/ZFocusFinder_zscan.png";

      zscan_can->SaveAs(zscan_plot_path.c_str());

      NQLog("ZFocusFinder::acquire_image") << "emitting signal \"show_zscan(" << zscan_plot_path << ")\"";

      emit show_zscan(zscan_plot_path);

      NQLog("ZFocusFinder::acquire_image") << "emitting signal \"update_text(" << zposi_best << ")\"";

      emit update_text(zposi_best);
    }
    // ------------------

    // auto-focus completed: move to best-focus position
    focus_completed_ = true;

    const double zposi_now = motion_manager_->get_position_Z();

    const double dz = (zposi_best-zposi_now);

    NQLog("ZFocusFinder::acquire_image") << "emitting signal \"focus(0, 0, " << dz << ", 0)\"";

    emit focus(0., 0., dz, 0.);
  }

  return;
}

void ZFocusFinder::process_image(const cv::Mat& img)
{
  NQLog("ZFocusFinder::process_image");

  if(focus_completed_)
  {
    v_zrelm_vals_.clear();

    v_focus_vals_.clear();

    focus_completed_ = false;

    image_counter_ = 0;

    zrelm_index_ = -1;

    this->disable_motion();

    const std::string img_outpath = output_dir_.toStdString()+"/ZFocusFinder_best.png";

    cv::imwrite(img_outpath, img);

    NQLog("ZFocusFinder::process_image") << "emitting signal \"image_acquired\"";

    emit image_acquired(img);
  }
  else
  {
    ZFocusFinder::focus_info this_focus;
    this_focus.z_position = motion_manager_->get_position_Z();
    this_focus.focus_disc = this->image_focus_value(img);

    v_focus_vals_.emplace_back(this_focus);

    const std::string img_outpath = output_dir_.toStdString()+"/ZFocusFinder_"+std::to_string(image_counter_)+".png";
    cv::imwrite(img_outpath, img);

    ++image_counter_;

    NQLog("ZFocusFinder::process_image") << "emitting signal \"next_zpoint\"";

    emit next_zpoint();
  }

  return;
}

/* \Brief Image-focus discriminant based on Laplacian method in OpenCV
 *        REF: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/laplace_operator/laplace_operator.html
 */
double ZFocusFinder::image_focus_value(const cv::Mat& img)
{
  // Remove noise by blurring with a Gaussian filter
  cv::Mat img_gaus;
  cv::GaussianBlur(img, img_gaus, cv::Size(img.cols, img.rows), 0, 0, cv::BORDER_DEFAULT);

  // Convert the image to grayscale
  cv::Mat img_gray;
  cv::cvtColor(img_gaus, img_gray, CV_BGR2GRAY);

  // Apply laplacian function to GS image
  cv::Mat img_lap;
  cv::Laplacian(img_gray, img_lap, CV_64F);

  cv::Mat img_lap_abs;
//  cv::convertScaleAbs(img_lap, img_lap_abs);
  img_lap_abs = img_lap.clone();

  // Calculate standard deviation of laplace image
  cv::Scalar mean, std_dev;
  cv::meanStdDev(img_lap_abs, mean, std_dev);

  const float value = (std_dev.val[0] * std_dev.val[0]);

  NQLog("ZFocusFinder::image_focus_value") << "image(" << exe_counter_ << ", " << image_counter_ << ") focus-value = " << value;

  return value;
}

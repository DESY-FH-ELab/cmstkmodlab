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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyUtilities.h>

#include <iostream>
#include <fstream>

#include <QFile>
#include <QTextStream>

#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TH1.h>

int AssemblyObjectFinderPatRec::exe_counter_ = -1;

AssemblyObjectFinderPatRec::AssemblyObjectFinderPatRec(const QString& output_dir_path, const QString& output_subdir_name, QObject* parent) :
  QObject(parent),

  output_dir_path_   (output_dir_path   .toStdString()),
  output_subdir_name_(output_subdir_name.toStdString()),

  threshold_template_(-1),

  updated_img_master_(false),
  updated_img_master_PatRec_(false)
{
  // connections
  connect(this, SIGNAL(template_matching_request(const cv::Mat&, const cv::Mat&, const cv::Mat&, const int)),
          this, SLOT  (template_matching        (const cv::Mat&, const cv::Mat&, const cv::Mat&, const int)));
  // -----------

  NQLog("AssemblyObjectFinderPatRec", NQLog::Debug) << "constructed";
}

AssemblyObjectFinderPatRec::~AssemblyObjectFinderPatRec()
{
  NQLog("AssemblyObjectFinderPatRec", NQLog::Debug) << "destructed";
}

void AssemblyObjectFinderPatRec::Configuration::reset()
{
  template_filepath_ = "";

  thresholding_useThreshold_ = false;
  thresholding_threshold_ = -1;

  thresholding_useAdaptiveThreshold_ = false;
  thresholding_blocksize_ = -1;

  angles_prescan_vec_.clear();
  angles_finemax_  = -1.0;
  angles_finestep_ = -1.0;
}

bool AssemblyObjectFinderPatRec::Configuration::is_valid() const
{
  if(QFile::exists(template_filepath_) == false){ return false; }

  if((thresholding_useThreshold_ == true) && (thresholding_useAdaptiveThreshold_ == true))
  {
    return false;
  }
  else if(thresholding_useThreshold_ == true)
  {
    if(thresholding_threshold_ < 0){ return false; }
  }
  else if(thresholding_useAdaptiveThreshold_ == true)
  {
    if(thresholding_blocksize_ < 3){ return false; }

    if((thresholding_blocksize_ % 2) == 0){ return false; }
  }

  if(angles_prescan_vec_.size() == 0){ return false; }

  if(angles_finemax_  <= 0.){ return false; }
  if(angles_finestep_ <= 0.){ return false; }

  if(angles_finestep_ > angles_finemax_){ return false; }

  return true;
}

//!!void AssemblyObjectFinderPatRec::launch_PatRec(const AssemblyObjectFinderPatRec::Configuration& conf)
//!!{
//!!  this->set_configuration(conf);
//!!
//!!  this->launch_PatRec();
//!!}
//!!
//!!void AssemblyObjectFinderPatRec::launch_PatRec()
//!!{
//!!  if(configuration_.master_useFilePath_)
//!!  {
//!!    emit master_image_request(configuration_.master_useFilePath_fpath_);
//!!  }
//!!  else
//!!  {
//!!    emit master_image_request();
//!!  }
//!!}

void AssemblyObjectFinderPatRec::update_image_master(const cv::Mat& img)
{
//  mutex_.lock();

  if(img.channels() > 1)
  {
    img_master_ = img;
  }
  else
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    img_master_ = img_color.clone();
  }

  if(!updated_img_master_){ updated_img_master_ = true; }

  if(updated_img_master_PatRec_){ updated_img_master_PatRec_ = false; }

//  mutex_.unlock();

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "update_image_master"
     << ": emitting signal \"updated_image\"";

  emit updated_image_master(img_master_);
  emit updated_image_master();
}

void AssemblyObjectFinderPatRec::delete_image_master()
{
//  mutex_.lock();

  img_master_ = cv::Mat();

  if(updated_img_master_){ updated_img_master_ = false; }

//  mutex_.unlock();

  return;
}

void AssemblyObjectFinderPatRec::update_image_master_PatRec(const cv::Mat& img)
{
  if(updated_img_master_)
  {
//    mutex_.lock();

    img_master_PatRec_ = img.clone();

    if(!updated_img_master_PatRec_){ updated_img_master_PatRec_ = true; }

//    mutex_.unlock();

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "update_image_master_PatRec"
       << ": emitting signal \"updated_image_master_PatRec\"";

    emit updated_image_master_PatRec(img_master_PatRec_);
    emit updated_image_master_PatRec();
  }
  else
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "update_image_master_PatRec"
       << ": master image not available, no input image for PatRec produced (hint: enable camera and get an image)";

    return;
  }
}

void AssemblyObjectFinderPatRec::send_image_master()
{
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "send_image_master"
     << ": emitting signal \"sent_image_master\"";

  emit sent_image_master(img_master_);
}

void AssemblyObjectFinderPatRec::send_image_master_PatRec()
{
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "send_image_master_PatRec"
     << ": emitting signal \"sent_image_master_PatRec\"";

  emit sent_image_master_PatRec(img_master_PatRec_);
}

//!!void AssemblyObjectFinderPatRec::update_prescan_angles(QString qstr)
//!!{
//!!  const QStringList entries = qstr.remove(" ").split(",");
//!!
//!!  if(entries.length() > 0)
//!!  {
//!!//    mutex_.lock();
//!!
//!!    configuration_.angles_prescan_vec_.clear();
//!!
//!!    for(int i=0; i<entries.length(); ++i)
//!!    {
//!!      configuration_.angles_prescan_vec_.emplace_back(entries.value(i).toDouble());
//!!    }
//!!
//!!//    mutex_.unlock();
//!!
//!!    NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "update_prescan_angles"
//!!       << ": updated list of pre-scan angles: " << qstr;
//!!
//!!    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "update_prescan_angles"
//!!       << ": emitting signal \"updated_prescan_angles\"";
//!!
//!!    emit updated_prescan_angles();
//!!  }
//!!  else
//!!  {
//!!    NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "update_prescan_angles"
//!!       << ": input string with invalid format (" << qstr << "), no action taken";
//!!
//!!    return;
//!!  }
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyObjectFinderPatRec::update_angscan_parameters(QString qstr)
//!!{
//!!  const QStringList entries = qstr.remove(" ").split(",");
//!!
//!!  if(entries.length() == 2)
//!!  {
//!!//    mutex_.lock();
//!!
//!!    configuration_.angles_finemax_ = entries.value(0).toDouble();
//!!    configuration_.angles_finestep_  = entries.value(1).toDouble();
//!!
//!!//    mutex_.unlock();
//!!
//!!    NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "update_angscan_parameters"
//!!       << ": updated parameters of angular-scan:"
//!!       << " (angles_finemax=" << configuration_.angles_finemax_
//!!       << ", angles_finestep="  << configuration_.angles_finestep_
//!!       << ")";
//!!
//!!    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "update_angscan_parameters"
//!!       << ": emitting signal \"updated_angscan_parameters\"";
//!!
//!!    emit updated_angscan_parameters();
//!!  }
//!!  else
//!!  {
//!!    NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "update_angscan_parameters"
//!!       << ": input string with invalid format (" << qstr << "), no action taken";
//!!
//!!    return;
//!!  }
//!!}

void AssemblyObjectFinderPatRec::run_PatRec(const int mode_lab, const int mode_obj)
{
  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "run_PatRec"
     << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
     << ": initiated Pattern Recognition";

//  mutex_.lock();

  if(mode_lab == 1) // PRODUCTION MODE
  {
    // --- input validation
    if(updated_img_master_ == false)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": updated master image not available, no action taken";

      return;
    }
    // --------------------

    if(mode_obj == 0)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of sensor fiducial marker";

//      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/SensorPiece_1_clipC.png"   , CV_LOAD_IMAGE_COLOR);
////    img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/RawSensor_3_clipB.png"     , CV_LOAD_IMAGE_COLOR);
////    img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/RawSensor_3_clipB_temp.png", CV_LOAD_IMAGE_COLOR);
//
//      threshold_template_ = 85; // 90 for silicon marker, 88 for glass, 188 for marked-glass

//      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/markedglass_marker1_template.png", CV_LOAD_IMAGE_COLOR);
//      threshold_template_ = 165; // 90 for silicon marker, 88 for glass, 165 for marked-glass

      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/markedglass_marker1_drawing_588x588.png", CV_LOAD_IMAGE_COLOR);
      threshold_template_ = -1;
    }
    else if(mode_obj == 1)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of positioning pin not implemented yet, no action taken";

      return;
    }
    else if(mode_obj == 2)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of sensor corner";

      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/simplecorner.png"                                  , CV_LOAD_IMAGE_COLOR);
//      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorneronbaseplate_sliverpaint_A_clip.png", CV_LOAD_IMAGE_COLOR);
//      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorner_sliverpaint_D_crop.png"           , CV_LOAD_IMAGE_COLOR);

      threshold_template_ = 85; // 90 for silicon marker, 88 for glass?
    }
    else if(mode_obj == 3)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of spacer corner";

//      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/spacer_corner_tempate_crop.png", CV_LOAD_IMAGE_COLOR);
//      threshold_template_ = 85; // 90 for silicon marker, 88 for glass?

      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/spacer_corner_template_v0_BWdrawing.png", CV_LOAD_IMAGE_COLOR);
      threshold_template_ = -1;
    }
    else
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": undefined value for object-mode, no action taken";

      return;
    }
  }
  else if(mode_lab == 0) // DEMO MODE
  {
    if(mode_obj == 0)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of sensor fiducial marker";

      img_master_   = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/SensorPiece_1.png"      , CV_LOAD_IMAGE_COLOR);
      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/SensorPiece_1_clipC.png", CV_LOAD_IMAGE_COLOR);

      threshold_template_ = 85; // 90 for silicon marker, 88 for glass?
    }
    else if(mode_obj == 1)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of positioning pin not implemented yet, no action taken";

      return;
    }
    else if(mode_obj == 2)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of sensor corner";

      img_master_   = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorneronbaseplate_sliverpaint_A.png"     , CV_LOAD_IMAGE_COLOR);
      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorneronbaseplate_sliverpaint_A_clip.png", CV_LOAD_IMAGE_COLOR);

      threshold_template_ = 85; // 90 for silicon marker, 88 for glass?
    }
    else if(mode_obj == 3)
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": detection of spacer corner";

//      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/spacer_corner_tempate_crop.png", CV_LOAD_IMAGE_COLOR);
//      threshold_template_ = 85; // 90 for silicon marker, 88 for glass?

      img_template_ = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/spacer_corner_template_v0_BWdrawing.png", CV_LOAD_IMAGE_COLOR);
      threshold_template_ = -1;
    }
    else
    {
      NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "run_PatRec"
         << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
         << ": undefined value for object-mode, no action taken";

      return;
    }

    this->update_image_master(img_master_);
  }
  else
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "run_PatRec"
       << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
       << ": undefined value for lab-mode, no action taken";

    return;
  }

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "run_PatRec"
     << "(mode_lab=" << mode_lab << ", mode_obj=" << mode_obj << ")"
     << ": emitting signal \"template_matching_request\"";

//  mutex_.unlock();

  emit template_matching_request(img_master_, img_master_PatRec_, img_template_, threshold_template_);
}

void AssemblyObjectFinderPatRec::template_matching()
{
  this->template_matching(img_master_, img_master_PatRec_, img_template_, threshold_template_);

  return;
}

void AssemblyObjectFinderPatRec::template_matching(const cv::Mat& img_master, const cv::Mat& img_master_PatRec, const cv::Mat& img_templa, const int threshold_templa)
{
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching";
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Master   cols = " << img_master.cols;
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Master   rows = " << img_master.rows;
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Template cols = " << img_templa.cols;
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Template rows = " << img_templa.rows;

  if(img_master.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty master image, stopping routine";

    return;
  }

  if(img_master_PatRec.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty master image for Pattern Recognition, stopping routine";

    return;
  }

  if(img_templa.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty template image, stopping routine";

    return;
  }

  // QMutex lock (accessing data members)
//  mutex_.lock();

  // output directory
  std::string output_dir(""), output_subdir("");

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

    output_dir = output_dir_path_+"/"+exe_counter_str+"/";

    output_dir_exists = assembly::DirectoryExists(output_dir);
  }

  output_subdir = output_dir+output_subdir_name_;

  const double angle_fine_min  = -1.0 * configuration_.angles_finemax_;
  const double angle_fine_max  = +1.0 * configuration_.angles_finemax_;
  const double angle_fine_step =        configuration_.angles_finestep_;

  const std::vector<double>& prescan_angles = configuration_.angles_prescan_vec_;

//  mutex_.unlock();
  // ------------------------------------

  assembly::QDir_mkpath(output_dir);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching: created output directory: " << output_dir;

  assembly::QDir_mkpath(output_subdir);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching: created output directory: " << output_subdir;
  // -----------

  // GreyScale images
  cv::Mat img_master_gs(img_master.size(), img_master.type());
  cv::Mat img_templa_gs(img_templa.size(), img_templa.type());

  if(img_master.channels() > 1){ cv::cvtColor(img_master, img_master_gs, CV_BGR2GRAY); }
  else                         { img_master_gs = img_master.clone(); }

  if(img_templa.channels() > 1){ cv::cvtColor(img_templa, img_templa_gs, CV_BGR2GRAY); }
  else                         { img_templa_gs = img_templa.clone(); }
  // -----------

  // Input images to PatRec
  cv::Mat img_templa_PatRec(img_templa_gs.size(), img_templa_gs.type());

  if(threshold_templa >= 0)
  {
    cv::threshold(img_templa_gs, img_templa_PatRec, threshold_templa, 255, cv::THRESH_BINARY);
  }
  else
  {
    img_templa_PatRec = img_templa_gs.clone();
  }

  const std::string filepath_img_master        = output_dir+"/image_master.png";
  const std::string filepath_img_master_PatRec = output_dir+"/image_master_PatRec.png";
  const std::string filepath_img_templa        = output_dir+"/image_template.png";
  const std::string filepath_img_templa_PatRec = output_dir+"/image_template_PatRec.png";

  cv::imwrite(filepath_img_master, img_master);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved master image to " << filepath_img_master;

  cv::imwrite(filepath_img_master_PatRec, img_master_PatRec);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved PatRec-input master image to " << filepath_img_master_PatRec;

  assembly::cv_imwrite_png(filepath_img_templa, img_templa);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved template image to " << filepath_img_templa;

  cv::imwrite(filepath_img_templa_PatRec, img_templa_PatRec);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved PatRec-input template image to " << filepath_img_templa_PatRec;
  // -----------

  // --- Template Matching

  // Template-Matching method for matchTemplate() routine of OpenCV
  // For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better.
  // REF https://docs.opencv.org/2.4/modules/imgproc/doc/object_detection.html?highlight=matchtemplate#matchtemplate
  const int match_method = CV_TM_SQDIFF_NORMED;
  const bool use_minFOM = ((match_method  == CV_TM_SQDIFF) || (match_method == CV_TM_SQDIFF_NORMED));

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching" << ": initiated matching routine with angular scan";

  // First, get angle-prescan angle: best guess of central value for finer angular scan
  double angle_prescan(-9999.);

  if(prescan_angles.size() > 0)
  {
    double best_FOM(0.);

    for(unsigned int i=0; i<prescan_angles.size(); ++i)
    {
      const double i_angle = prescan_angles.at(i);

      double i_FOM(0.);
      cv::Point i_matchLoc;

      this->PatRec(i_FOM, i_matchLoc, img_master_PatRec, img_templa_PatRec, i_angle, match_method);

      const bool update = (i==0) || (use_minFOM ? (i_FOM < best_FOM) : (i_FOM > best_FOM));

      if(update){ best_FOM = i_FOM; angle_prescan = i_angle; }
    }
  }
  else
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty list of pre-scan angles, stopping Pattern Recognition";

    return;
  }

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching" << ": pre-scan estimate of best-angle yields best-angle=" << angle_prescan;
  // ----------------

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching" << ": angular scan parameters"
     << "(min="<< angle_prescan+angle_fine_min << ", max=" << angle_prescan+angle_fine_max << ", step=" << angle_fine_step << ")";

  const int N_rotations = (2 * int((angle_fine_max-angle_fine_min) / angle_fine_step));

  std::vector<std::pair<double, double> > vec_angleNfom;
  vec_angleNfom.reserve(N_rotations);

  double    best_FOM  (0.);
  double    best_angle(0.);
  cv::Point best_matchLoc;

  for(double angle_fine=angle_fine_min; angle_fine<angle_fine_max; angle_fine += angle_fine_step)
  {
    const unsigned int scan_counter = vec_angleNfom.size();

    const double i_angle = angle_prescan + angle_fine;

    double i_FOM(0.);
    cv::Point i_matchLoc;

    this->PatRec(i_FOM, i_matchLoc, img_master_PatRec, img_templa_PatRec, i_angle, match_method, output_subdir);

    const bool update = (scan_counter==0) || (use_minFOM ? (i_FOM < best_FOM) : (i_FOM > best_FOM));

    if(update)
    {
      best_FOM      = i_FOM;
      best_angle    = i_angle;
      best_matchLoc = i_matchLoc;
    }

    vec_angleNfom.emplace_back(std::make_pair(i_angle, i_FOM));

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": angular scan: [" << scan_counter << "] angle=" << i_angle << ", FOM=" << i_FOM;
  }

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching"
     << ": angular scan completed: best_angle=" << best_angle;

  // copy of master image
  cv::Mat img_master_copy = img_master.clone();

  // drawings on top of master image copy ---

  // rectangle representing template in best-match position
  this->draw_RotatedRect(img_master_copy, best_matchLoc, img_templa_PatRec.cols, img_templa_PatRec.rows, best_angle, cv::Scalar(0,0,255));

  // the circle of radius 4 is meant to *roughly* represent the x,y precision of the x-y motion stage
  // so that the user can see if the patrec results make sense
  // (the top left corner of the marker should be within the cirle)
  circle(img_master_copy, best_matchLoc,  4, cv::Scalar(255,0,0), 4, 8, 0); // 1-sigma
  circle(img_master_copy, best_matchLoc, 15, cv::Scalar(255,0,0), 4, 8, 0); // only for visualization

  // cross-lines to mark the center of the image
  line(img_master_copy, cv::Point(   img_master_copy.cols/2.0, 0), cv::Point(img_master_copy.cols/2.0, img_master_copy.rows    ), cv::Scalar(255,255,0), 2, 8, 0);
  line(img_master_copy, cv::Point(0, img_master_copy.rows/2.0   ), cv::Point(img_master_copy.cols    , img_master_copy.rows/2.0), cv::Scalar(255,255,0), 2, 8, 0);

  // label for distance scale
  line   (img_master_copy,           cv::Point(100, 125), cv::Point(100+167, 125)             , cv::Scalar(0,255,0), 2, 8, 0);
  putText(img_master_copy, "200 um", cv::Point(100, 100), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, cv::Scalar(0,255,0), 3, 8);
  // ----------------------------------------

  // FOM(angle) plot
  if(vec_angleNfom.size() > 0)
  {
    TCanvas c1("FOM", "Rotation extraction", 200, 10, 700, 500);

    TGraph gr_scan;
    for(unsigned int idx=0; idx<vec_angleNfom.size(); ++idx)
    {
      gr_scan.SetPoint(idx, vec_angleNfom.at(idx).first, vec_angleNfom.at(idx).second);
    }

//    gr_scan.Fit("pol6");

    gr_scan.Draw("AC*");
    gr_scan.SetName("PatRec_FOM");
    gr_scan.GetHistogram()->GetXaxis()->SetTitle("angle (degrees)");
    gr_scan.GetHistogram()->GetYaxis()->SetTitle("PatRec FOM");
    gr_scan.GetHistogram()->SetTitle("");

    TGraph gr_best;
    gr_best.SetPoint(0, best_angle, best_FOM);
    gr_best.SetMarkerColor(2);
    gr_best.SetMarkerStyle(22);
    gr_best.SetMarkerSize(3);
    gr_best.Draw("PSAME");
    gr_best.SetName("PatRec_FOM_best");

    const std::string filepath_FOM_base = output_dir+"/RotationExtraction";

    const std::string filepath_FOM_png  = filepath_FOM_base+".png";
    const std::string filepath_FOM_root = filepath_FOM_base+".root";

    c1.SaveAs(filepath_FOM_png.c_str());

    TFile o_file(filepath_FOM_root.c_str(), "recreate");
    o_file.cd();
    gr_scan.Write();
    gr_best.Write();
    o_file.Close();

    emit image_path(2, QString::fromStdString(filepath_FOM_png));
  }
  // ---

  const std::string filepath_img_master_copy = output_dir+"/image_master_PatRec.png";
  assembly::cv_imwrite_png(filepath_img_master_copy, img_master_copy);

  emit image_mat(1, img_master_copy);
  emit image_mat(3, img_master_PatRec);
  emit image_mat(4, img_templa_PatRec);

  // text output file -
  const QString txt_file_path = QString::fromStdString(output_dir+"/PatRec_results.txt");

  QFile txtfile(txt_file_path);
  if(txtfile.open(QIODevice::WriteOnly | QIODevice::Text) == true)
  {
    QTextStream txts(&txtfile);

    txts << "# best_matchLoc.x best_matchLoc.y best_angle\n";
    txts << best_matchLoc.x << " " << best_matchLoc.y << " " << best_angle << "\n";
  }

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": created output file: " << txt_file_path;
  // ------------------

  // PatRec result(s)

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"reportObjectLocation(1, " << best_matchLoc.x << ", " << best_matchLoc.y << ", " << best_angle << ")\"";

  emit reportObjectLocation(1, best_matchLoc.x, best_matchLoc.y, best_angle);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_exitcode(0)\"";

  emit PatRec_exitcode(0);

//  mutex_.unlock();

//  const cv::Rect rect_result = cv::Rect(best_matchLoc, cv::Point(best_matchLoc.x + img_templa_PatRec.cols, best_matchLoc.y + img_templa_PatRec.rows));
//
//  // work out match location in field of view
//  // the origin of the FOV coordinate system is the top left corner
//  // the match loction (centre of the template) is calculated in mm
//  // this should be enough for postion correction with moverealtive()
//
//  //matchLoc_x_lab = (best_matchLoc.x +  (img_templa_PatRec.cols/2) ) * (5.0/img_master.cols); // need to add the current X pos of the lang
//  //matchLoc_y_lab = (best_matchLoc.y +  (img_templa_PatRec.rows/2) ) * (4.0/img_master.rows); // need to add the current Y pos of the lang

  return;
}

void AssemblyObjectFinderPatRec::PatRec(double& fom, cv::Point& match_loc, const cv::Mat& img_master_PatRec, const cv::Mat& img_templa_PatRec, const double angle, const int match_method, const std::string& out_dir) const
{
  // rotated master image
  cv::Mat img_master_PatRec_rot;

  const cv::Point2f src_center(img_master_PatRec.cols/2.0F, img_master_PatRec.rows/2.0F);

  const cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, angle, 1.0);

  const cv::Scalar avgPixelIntensity = cv::mean(img_master_PatRec);

  warpAffine(img_master_PatRec, img_master_PatRec_rot, rot_mat, img_master_PatRec.size(), cv::INTER_NEAREST, cv::BORDER_CONSTANT, avgPixelIntensity);

  if(out_dir != "")
  {
    const std::string filepath_img_master_PatRec_rot = out_dir+"/image_master_PatRec_Rotation_"+std::to_string(angle)+".png";

    cv::imwrite(filepath_img_master_PatRec_rot, img_master_PatRec_rot);

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "PatRec"
       << ": saved rotated input master image for PatRec to " << filepath_img_master_PatRec_rot;
  }
  // -----------

  // matrix with PatRec Figure-Of-Merit values
  cv::Mat result_mat;
  result_mat.create((img_master_PatRec.rows-img_templa_PatRec.rows+1), (img_master_PatRec.cols-img_templa_PatRec.cols+1), CV_32FC1);

  matchTemplate(img_master_PatRec_rot, img_templa_PatRec, result_mat, match_method);

  double minVal, maxVal;
  cv::Point minLoc, maxLoc;

  minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

  const bool use_minFOM = ((match_method  == CV_TM_SQDIFF) || (match_method == CV_TM_SQDIFF_NORMED));

  if(use_minFOM){ match_loc = minLoc; fom = minVal; }
  else          { match_loc = maxLoc; fom = maxVal; }
  // -----------

  // convert match-loc val of rotated  master image
  // to pixel-coordinates  in original master image
  match_loc = this->RotatePoint(src_center, match_loc, angle);

  return;
}

cv::Point2f AssemblyObjectFinderPatRec::RotatePoint(const cv::Point2f& p, const double deg) const
{
  const double rad = deg * (M_PI/180.);

  const float x = std::cos(rad) * p.x - std::sin(rad) * p.y;
  const float y = std::sin(rad) * p.x + std::cos(rad) * p.y;

  const cv::Point2f rot_p(x, y);

  return rot_p;
}

cv::Point2f AssemblyObjectFinderPatRec::RotatePoint(const cv::Point2f& cen_pt, const cv::Point2f& p, const double deg) const
{
  const cv::Point2f trans_pt = p - cen_pt;
  const cv::Point2f rot_pt   = this->RotatePoint(trans_pt, deg);
  const cv::Point2f fin_pt   = rot_pt + cen_pt;

  return fin_pt;
}

void AssemblyObjectFinderPatRec::draw_RotatedRect(cv::Mat& image, const cv::Point& orig, const double rect_size_x, const double rect_size_y, const double angle, const cv::Scalar& rect_color) const
{
  const cv::Point2f orig2f(orig.x, orig.y);

  const cv::RotatedRect rot_rect(orig2f, cv::Size2f(rect_size_x, rect_size_y), angle);

  cv::Point2f vertices_0[4];
  rot_rect.points(vertices_0);

  cv::Point2f vertices[4];
  for(unsigned int i=0; i<4; ++i)
  {
    vertices[i] = vertices_0[i] + (orig2f - vertices_0[1]);
  }

  for(unsigned int i=0; i<4; ++i)
  {
    line(image, vertices[i], vertices[(i+1)%4], rect_color, 2, 8, 0);
  }

  return;
}

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

#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyUtilities.h>

#include <iostream>
#include <fstream>

#include <QFile>
#include <QTextStream>
#include <QMutexLocker>

int AssemblyObjectFinderPatRec::exe_counter_ = -1;

AssemblyObjectFinderPatRec::AssemblyObjectFinderPatRec(AssemblyThresholder* const thresholder, const QString& output_dir_path, const QString& output_subdir_name, QObject* parent) :
  QObject(parent),

  thresholder_(thresholder),

  config_(nullptr),

  output_dir_path_   (output_dir_path   .toStdString()),
  output_subdir_name_(output_subdir_name.toStdString()),

  save_subdir_images_(true),

  updated_img_master_(false),
  updated_img_master_PatRec_(false)
{
  if(thresholder_ == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Fatal) << "initialization error"
       << ": null pointer to AssemblyThresholder object, exiting constructor";

    return;
  }

  qRegisterMetaType<AssemblyObjectFinderPatRec::Configuration>("AssemblyObjectFinderPatRec::Configuration");

  // connections
  connect(this, SIGNAL(template_matching_request(Configuration, cv::Mat, cv::Mat, cv::Mat)), this, SLOT(template_matching(Configuration, cv::Mat, cv::Mat, cv::Mat)));
  // -----------

  config_ = ApplicationConfig::instance();
  if(config_ == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Fatal) << "initialization error"
       << ": ApplicationConfig::instance() not initialized (null pointer), exiting constructor";

    return;
  }

  mm_per_pixel_row_ = config_->getValue<double>("main", "mm_per_pixel_row");
  mm_per_pixel_col_ = config_->getValue<double>("main", "mm_per_pixel_col");

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
  if(assembly::IsFile(template_filepath_) == false){ return false; }

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

void AssemblyObjectFinderPatRec::launch_PatRec(const AssemblyObjectFinderPatRec::Configuration& conf)
{
  // validate configuration parameters
  if(conf.is_valid() == false)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Fatal) << "launch_PatRec"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object, no action taken";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "launch_PatRec"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);
  }
  // ----------

  // validate master image
  if(img_master_.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Fatal) << "launch_PatRec"
       << ": input master image is empty, no action taken";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "launch_PatRec"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);
  }
  // ----------

  // update template image
  const cv::Mat img_templa = assembly::cv_imread(conf.template_filepath_, cv::IMREAD_COLOR);

  if(assembly::MatIsBlackAndWhite(img_templa) == false)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Warning) << "launch_PatRec"
       << ": updated template image is not a binary image, please check image";
  }

  // emit DBLogMessage("Starting Pattern Recognition");

  mutex_.lock();

  img_template_ = img_templa;

  mutex_.unlock();
  // ----------

  // update binary image for PatRec
  if(conf.thresholding_useThreshold_)
  {
    mutex_.lock();

    thresholder_->update_image_raw(img_master_);
    thresholder_->update_image_binary_threshold(conf.thresholding_threshold_);

    mutex_.unlock();

    this->update_image_master_PatRec(thresholder_->image_binary());
  }
  else if(conf.thresholding_useAdaptiveThreshold_)
  {
    mutex_.lock();

    thresholder_->update_image_raw(img_master_);
    thresholder_->update_image_binary_adaptiveThreshold(conf.thresholding_blocksize_);

    mutex_.unlock();
  }

  this->update_image_master_PatRec(thresholder_->image_binary());
  // ----------

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "launch_PatRec"
     << ": emitting signal \"template_matching_request\"";

  emit template_matching_request(conf, img_master_, img_master_PatRec_, img_template_);
}

void AssemblyObjectFinderPatRec::update_image_master(const cv::Mat& img)
{
  QMutexLocker ml(&mutex_);

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

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "update_image_master"
     << ": emitting signal \"updated_image_master\"";

  emit updated_image_master(img_master_);
  emit updated_image_master();
}

void AssemblyObjectFinderPatRec::delete_image_master()
{
  img_master_ = cv::Mat();

  if(updated_img_master_){ updated_img_master_ = false; }

  return;
}

void AssemblyObjectFinderPatRec::update_image_master_PatRec(const cv::Mat& img)
{
  QMutexLocker ml(&mutex_);

  if(updated_img_master_)
  {
    img_master_PatRec_ = img.clone();

    if(!updated_img_master_PatRec_){ updated_img_master_PatRec_ = true; }

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

void AssemblyObjectFinderPatRec::template_matching(const AssemblyObjectFinderPatRec::Configuration& conf, const cv::Mat& img_master, const cv::Mat& img_master_PatRec, const cv::Mat& img_templa_PatRec)
{
  // validate configuration parameters
  if(conf.is_valid() == false)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Fatal) << "template_matching"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object, no action taken";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }
  // ----------

  // validate master/template images
  if(img_master.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty master image, stopping routine";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }

  if(img_master_PatRec.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty master image for Pattern Recognition, stopping routine";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }

  if(img_templa_PatRec.empty())
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": empty template image, stopping routine";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }
  // ----------

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Master   cols = " << img_master.cols;
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Master   rows = " << img_master.rows;

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Template cols = " << img_templa_PatRec.cols;
  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching: Template rows = " << img_templa_PatRec.rows;

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_res_image_master_PatRec()\"";

  emit PatRec_res_image_master_PatRec(img_master_PatRec);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_res_image_template_PatRec()\"";

  emit PatRec_res_image_template_PatRec(img_templa_PatRec);

  // QMutex lock
  QMutexLocker ml(&mutex_);

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

  output_subdir = save_subdir_images_ ? output_dir+output_subdir_name_ : "";

  const double angle_fine_min  = -1.0 * conf.angles_finemax_;
  const double angle_fine_max  = +1.0 * conf.angles_finemax_;
  const double angle_fine_step =        conf.angles_finestep_;

  const std::vector<double>& prescan_angles = conf.angles_prescan_vec_;

  // ------------------------------------

  assembly::QDir_mkpath(output_dir);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching: created output directory: " << output_dir;

  if(output_subdir != "")
  {
    assembly::QDir_mkpath(output_subdir);

    NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching: created output directory: " << output_subdir;
  }
  // -----------

  // Input images to PatRec
  const std::string filepath_img_master        = output_dir+"/image_master_raw.png";
  const std::string filepath_img_master_PatRec = output_dir+"/image_master_PatRec.png";
  const std::string filepath_img_templa_PatRec = output_dir+"/image_template_PatRec.png";

  assembly::cv_imwrite(filepath_img_master, img_master);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved master image to " << filepath_img_master;

  assembly::cv_imwrite(filepath_img_master_PatRec, img_master_PatRec);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved PatRec-input master image to " << filepath_img_master_PatRec;

  assembly::cv_imwrite(filepath_img_templa_PatRec, img_templa_PatRec);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": saved PatRec-input template image to " << filepath_img_templa_PatRec;
  // -----------

  // --- Template Matching

  // force template image to be a greyscale image (required by matchTemplate)
  cv::Mat img_templa_PatRec_gs;
  if(img_templa_PatRec.channels() > 1)
  {
    cv::cvtColor(img_templa_PatRec, img_templa_PatRec_gs, cv::COLOR_BGR2GRAY);
  }
  else
  {
    img_templa_PatRec_gs = img_templa_PatRec.clone();
  }

  // Template-Matching method for matchTemplate() routine of OpenCV
  // For SQDIFF and SQDIFF_NORMED, the best match is the lowest value; for all the other methods, the best match is the highest value.
  // REF https://docs.opencv.org/2.4/modules/imgproc/doc/object_detection.html?highlight=matchtemplate#matchtemplate
  const int match_method = cv::TM_SQDIFF_NORMED;
  const bool use_minFOM = ((match_method  == cv::TM_SQDIFF) || (match_method == cv::TM_SQDIFF_NORMED));

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching" << ": initiated matching routine with angular scan";

  // First, get angle-prescan angle: best guess of central value for finer angular scan
  double angle_prescan(-9999.);

  if(prescan_angles.size() == 0)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
     << ": empty list of pre-scan angles, stopping Pattern Recognition";

    return;
  }

  double best_FOM(0.);

  for(unsigned int i=0; i<prescan_angles.size(); ++i)
  {
    const double i_angle = prescan_angles.at(i);

    double i_FOM(0.);
    cv::Point i_matchLoc;

    this->PatRec(i_FOM, i_matchLoc, img_master_PatRec, img_templa_PatRec_gs, i_angle, match_method);

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching" << ": pre-scan for angle " << i_angle << " yields FOM=" << i_FOM;

    const bool update = (i==0) || (use_minFOM ? (i_FOM < best_FOM) : (i_FOM > best_FOM));

    if(update){ best_FOM = i_FOM; angle_prescan = i_angle; }
  }

  if(fabs(best_FOM-1.) < std::numeric_limits<double>::epsilon()) {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
     << ": best FOM found is 1.0. No matching pattern found.";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching" << ": pre-scan estimate of best-angle yields best-angle=" << angle_prescan;
  // ----------------

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching" << ": angular scan parameters"
     << "(min="<< angle_prescan+angle_fine_min << ", max=" << angle_prescan+angle_fine_max << ", step=" << angle_fine_step << ")";

  if(angle_fine_step == 0.)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
       << ": invalid value of step-angle for fine angular scan (zero), stopping routine";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }

  const int N_rotations = (2 * int((angle_fine_max-angle_fine_min) / angle_fine_step));

  QList<QPointF>* vec_angleNfom = new QList<QPointF>();

  best_FOM = 0.;
  double    best_angle(0.);
  cv::Point best_matchLoc;

  for(double angle_fine=angle_fine_min; angle_fine<=angle_fine_max; angle_fine += angle_fine_step)
  {
    const unsigned int scan_counter = vec_angleNfom->size();

    const double i_angle = angle_prescan + angle_fine;

    double i_FOM(0.);
    cv::Point i_matchLoc;

    this->PatRec(i_FOM, i_matchLoc, img_master_PatRec, img_templa_PatRec_gs, i_angle, match_method, output_subdir);

    const bool update = (scan_counter == 0) || (use_minFOM ? (i_FOM < best_FOM) : (i_FOM > best_FOM));

    if(update)
    {
      best_FOM      = i_FOM;
      best_angle    = i_angle;
      best_matchLoc = i_matchLoc;
    }

    vec_angleNfom->append(QPointF(i_angle, i_FOM));

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": angular scan: [" << scan_counter << "] angle=" << i_angle << ", FOM=" << i_FOM;
  }

  if(fabs(best_FOM-1.) < std::numeric_limits<double>::epsilon()) {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Critical) << "template_matching"
     << ": best FOM found is 1.0. No matching pattern found.";

    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_exitcode(1)\"";

    emit PatRec_exitcode(1);

    return;
  }

  NQLog("AssemblyObjectFinderPatRec", NQLog::Message) << "template_matching"
     << ": angular scan completed: best_angle=" << best_angle;

  // copy of master image
  cv::Mat img_master_copy = img_master.clone();

  // drawings on top of master image copy ---

  // rectangle representing template in best-match position
  this->draw_RotatedRect(img_master_copy, best_matchLoc, img_templa_PatRec_gs.cols, img_templa_PatRec_gs.rows, -best_angle, cv::Scalar(0,0,255));

  // the circle of radius 4 is meant to *roughly* represent the x,y precision of the x-y motion stage
  // so that the user can see if the patrec results make sense
  // (the top left corner of the marker should be within the circle)
  circle(img_master_copy, best_matchLoc,  4, cv::Scalar(255,0,0), 1, 8, 0); // ~ 5um (motion-stage precision)
  circle(img_master_copy, best_matchLoc, 17, cv::Scalar(255,0,0), 4, 8, 0); // ~20um (only for visualization)

  // cross-lines to mark the center of the image
  line(img_master_copy, cv::Point(   img_master_copy.cols/2.0, 0), cv::Point(img_master_copy.cols/2.0, img_master_copy.rows    ), cv::Scalar(255,255,0), 2, 8, 0);
  line(img_master_copy, cv::Point(0, img_master_copy.rows/2.0   ), cv::Point(img_master_copy.cols    , img_master_copy.rows/2.0), cv::Scalar(255,255,0), 2, 8, 0);

  // label for distance scale
  line   (img_master_copy,           cv::Point(100, 125), cv::Point(100+167, 125)             , cv::Scalar(0,255,0), 2, 8, 0);
  putText(img_master_copy, "200 um", cv::Point(100, 100), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, cv::Scalar(0,255,0), 3, 8);
  // ----------------------------------------

  // FOM(angle) plot
  if(vec_angleNfom->size() > 0)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
       << ": emitting signal \"PatRec_res_image_angscan(...)\"";

    emit PatRec_res_image_angscan(*vec_angleNfom);
  }
  // ---

  const std::string filepath_img_master_copy = output_dir+"/image_master_PatRec_edited.png";
  assembly::cv_imwrite(filepath_img_master_copy, img_master_copy);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_res_image_master_edited()\"";

  emit PatRec_res_image_master_edited(img_master_copy);

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

  //
  // conversion of PatRec best-match position (pixels of master image)
  // into the (X [mm], Y [mm]) movement, in the motion-stage reference frame,
  // to move the camera on top of the PatRec best-match position
  //
  // This conversion depends on
  //   - the dimension of the image's pixel unit in mm
  //   - the angle of the XY camera frame in the XY motion stage ref-frame
  //
  // Note:
  //   the y-distance in the Camera frame has to be inverted,
  //   because the cv::Mat object in OpenCV counts
  //   columns (X) and rows (Y) starting from the top-left corner
  //   (not from the bottom-left corner as for a normal XY reference frame);
  //   in order to convert this to a normal XY ref-frame,
  //   we invert the sign of the value on the Y-axis.
  //
  const double angle_FromCameraXYtoRefFrameXY_deg = config_->getValue<double>("parameters", "AngleOfCameraFrameInRefFrame_dA");

  const double dX_0 = +1.0 * (best_matchLoc.x - (img_master_copy.cols / 2.0)) * mm_per_pixel_col_;
  const double dY_0 = -1.0 * (best_matchLoc.y - (img_master_copy.rows / 2.0)) * mm_per_pixel_row_;

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": best matching position in pixels x = " << best_matchLoc.x << ", y = " << best_matchLoc.y << "\"";

  double patrec_dX, patrec_dY;
  assembly::rotation2D_deg(patrec_dX, patrec_dY, angle_FromCameraXYtoRefFrameXY_deg, dX_0, dY_0);

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_results(" << patrec_dX << ", " << patrec_dY << ", " << best_angle << ")\"";

  const QString mess_tmp = "PatRec results:\n" +
                // QString("-- Best match X = ")     + QString::number(best_matchLoc.x) + " (px) \n" +
                // QString("-- Best match X = ")     + QString::number(best_matchLoc.x) + " (px) \n" +
                QString("-- Best match X = ")     + QString::number(patrec_dX) + " (mm) \n" +
                QString("-- Best match Y = ")     + QString::number(patrec_dY) + " (mm) \n" +
                QString("-- Best match angle = ") + QString::number(best_angle) + " (deg)";
  emit DBLogMessage(mess_tmp);

  emit PatRec_results(patrec_dX, patrec_dY, best_angle);

  // -----------------------------

  // PatRec completed successfully

  NQLog("AssemblyObjectFinderPatRec", NQLog::Spam) << "template_matching"
     << ": emitting signal \"PatRec_exitcode(0)\"";

  emit PatRec_exitcode(0);

  // -----------------------------

  return;
}

void AssemblyObjectFinderPatRec::PatRec(double& fom, cv::Point& match_loc, const cv::Mat& img_master_PatRec, const cv::Mat& img_templa_PatRec, const double angle, const int match_method, const std::string& out_dir) const
{
  // rotated master image
  cv::Mat img_master_PatRec_rot;

  const cv::Point2f src_center(img_master_PatRec.cols/2.0F, img_master_PatRec.rows/2.0F);

  //
  // angle of rotation of master image:
  //   - the angle used as function argument ("angle") corresponds to the angle of the template with respect to master image
  //   - in the PatRec routine what is actually done is that the master image is rotated and compared to the (unrotated) template image
  //   - the rotation to be applied to the master image corresponds to the opposite value of "angle"
  //
  const double angle_master = (-1.0 * angle);

  const cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, angle_master, 1.0);

  const cv::Scalar avgPixelIntensity = cv::mean(img_master_PatRec);

  warpAffine(img_master_PatRec, img_master_PatRec_rot, rot_mat, img_master_PatRec.size(), cv::INTER_NEAREST, cv::BORDER_CONSTANT, avgPixelIntensity);

  if(out_dir != "")
  {
    const std::string filepath_img_master_PatRec_rot = out_dir+"/image_master_PatRec_Rotation_"+std::to_string(angle_master)+".png";

    assembly::cv_imwrite(filepath_img_master_PatRec_rot, img_master_PatRec_rot);

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

  const bool use_minFOM = ((match_method  == cv::TM_SQDIFF) || (match_method == cv::TM_SQDIFF_NORMED));

  if(use_minFOM){ match_loc = minLoc; fom = minVal; }
  else          { match_loc = maxLoc; fom = maxVal; }
  // -----------

  // convert match-loc val of rotated  master image
  // to pixel-coordinates  in original master image
  match_loc = this->RotatePoint(src_center, match_loc, angle_master);

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

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

#include <AssemblyImageView.h>
#include <AssemblyUtilities.h>

#include <sstream>

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTimer>

using namespace std;

AssemblyImageView::AssemblyImageView(QWidget* parent) :
  QWidget(parent),

  // image
  img_ueye_(nullptr),
  img_scroll_(nullptr),
  img_load_button_(nullptr),
  img_save_button_(nullptr),
  img_celi_button_(nullptr),
  img_axes_button_(nullptr),

  // auto-focusing
  autofocus_ueye_(nullptr),
  autofocus_scroll_(nullptr),
  autofocus_result_bestZ_lineed_(nullptr),
  autofocus_exe_button_(nullptr),
  autofocus_stop_button_(nullptr),
  autofocus_param_maxDZ_lineed_(nullptr),
  autofocus_param_Nstep_lineed_(nullptr),
  autofocus_save_zscan_button_(nullptr)
{
  QGridLayout* g0 = new QGridLayout;
  this->setLayout(g0);

  //// left-hand side -----------------------------------

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  // image
  img_ueye_ = new AssemblyUEyeView(this);
  img_ueye_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  img_ueye_->setMinimumSize(500, 300);
  img_ueye_->setPalette(palette);
  img_ueye_->setBackgroundRole(QPalette::Background);
  img_ueye_->setScaledContents(true);
  img_ueye_->setAlignment(Qt::AlignCenter);

  this->setStyleSheet("QToolTip {background-color: blue; border-style: outset; border-width: 2px; border-color: beige; font: bold 15px; }");
  const ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRec", NQLog::Fatal) << "initialization error"
       << ": ApplicationConfig::instance() not initialized (null pointer), exiting constructor";

    return;
  }
  mm_per_pixel_row_ = config->getValue<double>("main", "mm_per_pixel_row");
  mm_per_pixel_col_ = config->getValue<double>("main", "mm_per_pixel_col");
  angle_FromCameraXYtoRefFrameXY_deg_ = config->getValue<double>("parameters", "AngleOfCameraFrameInRefFrame_dA");
//--------------------------------------------

  img_scroll_ = new QScrollArea(this);
  img_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  img_scroll_->setMinimumSize(500, 300);
  img_scroll_->setPalette(palette);
  img_scroll_->setBackgroundRole(QPalette::Background);
  img_scroll_->setAlignment(Qt::AlignCenter);
  img_scroll_->setWidget(img_ueye_);

  g0->addWidget(img_scroll_, 0, 0);
  // ----------

  // auto-focusing
  QVBoxLayout* autofocus_result_lay = new QVBoxLayout;

  autofocus_ueye_ = new AssemblyUEyeView(this);
  autofocus_ueye_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  autofocus_ueye_->setMinimumSize(500, 300);
  autofocus_ueye_->setPalette(palette);
  autofocus_ueye_->setBackgroundRole(QPalette::Background);
  autofocus_ueye_->setScaledContents(true);
  autofocus_ueye_->setAlignment(Qt::AlignCenter);
  autofocus_ueye_->setZoomFactor(0.75);

  autofocus_scroll_ = new QScrollArea(this);
  autofocus_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  autofocus_scroll_->setMinimumSize(500, 300);
  autofocus_scroll_->setPalette(palette);
  autofocus_scroll_->setBackgroundRole(QPalette::Background);
  autofocus_scroll_->setAlignment(Qt::AlignCenter);
  autofocus_scroll_->setWidget(autofocus_ueye_);

  autofocus_result_lay->addWidget(autofocus_scroll_);

  QHBoxLayout* autofocus_result_bestZ_lay = new QHBoxLayout;

  QLabel* autofocus_result_bestZ_label = new QLabel("Best-Focus Z-position [mm]", this);
  autofocus_result_bestZ_lineed_ = new QLineEdit("", this);
  autofocus_result_bestZ_lineed_->setReadOnly(true);

  autofocus_result_bestZ_lay->addWidget(autofocus_result_bestZ_label  , 40);
  autofocus_result_bestZ_lay->addWidget(autofocus_result_bestZ_lineed_, 60);

  autofocus_result_lay->addLayout(autofocus_result_bestZ_lay);

  g0->addLayout(autofocus_result_lay, 1, 0);
  // ----------

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  //// --------------------------------------------------

  //// right-hand side ----------------------------------

  // image
  QVBoxLayout* lImg = new QVBoxLayout;
  g0->addLayout(lImg, 0, 1);

  img_load_button_ = new QPushButton("Load Image", this);
  lImg->addWidget(img_load_button_);

  img_save_button_ = new QPushButton("Save Image", this);
  lImg->addWidget(img_save_button_);

  img_celi_button_ = new QPushButton("Show/hide Center Lines", this);
  lImg->addWidget(img_celi_button_);

  img_axes_button_ = new QPushButton("Show XY Axis Conventions", this);
  lImg->addWidget(img_axes_button_);

  connect(img_load_button_, SIGNAL(clicked()), this, SLOT(load_image()));
  connect(img_save_button_, SIGNAL(clicked()), this, SLOT(save_image()));
  connect(img_celi_button_, SIGNAL(clicked()), this, SLOT(modify_image_centerlines()));
  connect(img_axes_button_, SIGNAL(clicked()), this, SLOT(modify_image_axesConventions()));

  this->connectImageProducer_image(this, SIGNAL(image_updated(cv::Mat)));

  lImg->addStretch();
  // ----------

  // auto-focusing
  QVBoxLayout* autofocus_lay = new QVBoxLayout;
  g0->addLayout(autofocus_lay, 1, 1);

  autofocus_exe_button_ = new QPushButton("Auto-Focus Image", this);
  autofocus_lay->addWidget(autofocus_exe_button_);

  autofocus_stop_button_ = new QPushButton("Stop Auto-Focus", this);
  autofocus_lay->addWidget(autofocus_stop_button_);

  progBar_ = new QProgressBar(this);
  autofocus_lay->addWidget(progBar_, Qt::AlignCenter);
  progBar_->setValue(0); //Set initial value
  progBar_->setVisible(false); //Invisible by default
  connect(autofocus_exe_button_, SIGNAL(clicked()), this, SLOT(makeProgBarVisible()));
  connect(autofocus_stop_button_, SIGNAL(clicked()), this, SLOT(makeProgBarInvisible()));
  // -----

  autofocus_lay->addSpacing(20);

  QGroupBox* autofocus_param_box = new QGroupBox(tr("Auto-Focus Configuration"));
  autofocus_param_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  autofocus_lay->addWidget(autofocus_param_box);

  QVBoxLayout* autofocus_param_lay = new QVBoxLayout;
  autofocus_param_box->setLayout(autofocus_param_lay);

  QHBoxLayout* autofocus_param_maxDZ_lay = new QHBoxLayout;
  autofocus_param_lay->addLayout(autofocus_param_maxDZ_lay);

  QLabel* autofocus_param_maxDZ_label = new QLabel("Max delta-Z [mm]", this);
  autofocus_param_maxDZ_lineed_ = new QLineEdit("", this);

  autofocus_param_maxDZ_lay->addWidget(autofocus_param_maxDZ_label  , 40);
  autofocus_param_maxDZ_lay->addWidget(autofocus_param_maxDZ_lineed_, 60);

  QHBoxLayout* autofocus_param_Nstep_lay = new QHBoxLayout;
  autofocus_param_lay->addLayout(autofocus_param_Nstep_lay);

  QLabel* autofocus_param_Nstep_label = new QLabel("# Steps (int)", this);
  autofocus_param_Nstep_lineed_ = new QLineEdit("", this);

  autofocus_param_Nstep_lay->addWidget(autofocus_param_Nstep_label  , 40);
  autofocus_param_Nstep_lay->addWidget(autofocus_param_Nstep_lineed_, 60);
  // -----

  autofocus_lay->addSpacing(20);

  autofocus_save_zscan_button_ = new QPushButton("Save Z-Scan Image", this);
  autofocus_lay->addWidget(autofocus_save_zscan_button_);

  connect(autofocus_save_zscan_button_, SIGNAL(clicked()), this, SLOT(save_image_zscan()));

  this->connectImageProducer_autofocus(this, SIGNAL(image_zscan_updated(const cv::Mat&)));
  // -----

  autofocus_lay->addStretch();
  // ----------
}

void AssemblyImageView::update_image(const cv::Mat& img, const bool update_image_raw)
{
  if(img.channels() == 1)
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    image_ = img_color.clone();
  }
  else
  {
    image_ = img.clone();
  }

  if(update_image_raw)
  {
    image_raw_ = image_.clone();

    image_modified_ = false;
  }

  NQLog("AssemblyImageView", NQLog::Spam) << "update_image"
     << ": emitting signal \"image_updated\"";

  emit image_updated(image_);
}

void AssemblyImageView::load_image()
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  const cv::Mat img = assembly::cv_imread(filename, cv::IMREAD_COLOR);

  if(img.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "load_image"
       << ": input image is empty, no action taken";

    return;
  }

  NQLog("AssemblyImageView", NQLog::Spam) << "load_image"
     << ": emitting signal \"image_loaded\"";

  emit image_loaded(img);
}

void AssemblyImageView::save_image()
{
  if(image_.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "save_image"
       << ": input image is empty, no action taken";

    return;
  }

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  assembly::cv_imwrite(filename, image_);

  return;
}

void AssemblyImageView::save_image_zscan()
{
  if(image_zscan_.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "save_image_zscan"
       << ": input z-scan image is empty, no action taken";

    return;
  }

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  assembly::cv_imwrite(filename, image_zscan_);

  return;
}

void AssemblyImageView::modify_image_centerlines()
{
  if(image_raw_.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "modify_image_centerlines"
       << ": input raw image is empty, no action taken";

    return;
  }

  cv::Mat img = image_raw_.clone();

  if(image_modified_ == false)
  {
    line(img, cv::Point(   img.cols/2.0, 0), cv::Point(img.cols/2.0, img.rows    ), cv::Scalar(255,0,0), 2, 8, 0);
    line(img, cv::Point(0, img.rows/2.0   ), cv::Point(img.cols    , img.rows/2.0), cv::Scalar(255,0,0), 2, 8, 0);

    image_modified_ = true;

    this->update_image(img, false);
  }
  else //Remove lines (reload raw image)
  {
    this->update_image(img, true);
  }

  return;
}

//Add/remove XY axis conventions from the image (for illustration)
//(0,0) = top left corner
void AssemblyImageView::modify_image_axesConventions()
{
  if(image_.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "modify_image_axesConventions"
       << ": input raw image is empty, no action taken";

    return;
  }

  int padding = 50; //don't show axes exactly at image boundaries

  cv::Mat img = image_.clone(); //Use current image (possibly with center lines displayed), not raw image

  //See: https://docs.opencv.org/2.4/modules/core/doc/drawing_functions.html#puttext
  //cv::LINE_AA = anti-aliased linetype
  //Text position is hardcoded
  int fontFace = cv::FONT_HERSHEY_PLAIN; //FONT_HERSHEY_SIMPLEX, ...
  double fontScale = 8;
  int linethick = 3;
  putText(img, "+x", cv::Point(img.cols/2.0+padding, 2*padding), fontFace, fontScale, cv::Scalar(255,0,0), linethick, cv::LINE_AA);
  putText(img, "-x", cv::Point(img.cols/2.0+padding, img.rows-padding), fontFace, fontScale, cv::Scalar(255,0,0), linethick, cv::LINE_AA);
  putText(img, "+y", cv::Point(padding, img.rows/2.0+2*padding), fontFace, fontScale, cv::Scalar(255,0,0), linethick, cv::LINE_AA);
  putText(img, "-y", cv::Point(img.cols-5*padding, img.rows/2.0+2*padding), fontFace, fontScale, cv::Scalar(255,0,0), linethick, cv::LINE_AA);

  //Add line/label for distance scale
  line   (img, cv::Point(0, 125), cv::Point(0+167, 125), cv::Scalar(0,255,0), 2, 8, 0);
  putText(img, "200 um", cv::Point(100, 100), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, cv::Scalar(0,255,0), 3, 8);

  //Add line/label for rotation angle convention (positive angle <-> anti-clockwise)
  line   (img, cv::Point(10, img.rows-180), cv::Point(150, img.rows-80), cv::Scalar(255,0,0), 2, 8, 0);
  putText(img, "+a", cv::Point(180, img.rows-15), fontFace, fontScale, cv::Scalar(255,0,0), linethick, cv::LINE_AA);

  this->update_image(img, false);

  return;
}

void AssemblyImageView::update_text(const double z)
{
  autofocus_result_bestZ_lineed_->setText(QString::fromStdString(std::to_string(z)));

  NQLog("AssemblyImageView", NQLog::Spam) << "update_text"
     << ": displayed value of best z-position (focal point)";

  return;
}

void AssemblyImageView::update_autofocus_config(const double maxDZ, const int Nstep)
{
  std::stringstream maxDZ_strs;
  maxDZ_strs << maxDZ;

  autofocus_param_maxDZ_lineed_->setText(QString::fromStdString(maxDZ_strs.str()));

  std::stringstream Nstep_strs;
  Nstep_strs << Nstep;

  autofocus_param_Nstep_lineed_->setText(QString::fromStdString(Nstep_strs.str()));

  return;
}

void AssemblyImageView::acquire_autofocus_config()
{
  // maximum delta-Z movement
  const QString maxDZ_str = autofocus_param_maxDZ_lineed_->text();

  bool maxDZ_valid(false);
  const double maxDZ = maxDZ_str.toDouble(&maxDZ_valid);

  if(maxDZ_valid == false)
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "acquire_autofocus_config"
       << ": invalid format for maximum delta-Z movement (" << maxDZ_str << "), no action taken";

    return;
  }
  // -------------------------

  // number of steps in Z-scan
  const QString Nstep_str = autofocus_param_Nstep_lineed_->text();

  bool Nstep_valid(false);
  const double Nstep = Nstep_str.toInt(&Nstep_valid);

  if(Nstep_valid == false)
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "acquire_autofocus_config"
       << ": invalid format for number of steps in Z-scan (" << Nstep_str << "), no action taken";

    return;
  }
  // -------------------------

  NQLog("AssemblyImageView", NQLog::Spam) << "autofocus_config"
    << ": emitting signal \"autofocus_config("
    <<   "maxDZ=" << maxDZ
    << ", Nstep=" << Nstep
    << ")\"";

  emit autofocus_config(maxDZ, Nstep);
}

void AssemblyImageView::update_image_zscan(const QString& img_path)
{
  if(assembly::IsFile(img_path))
  {
    image_zscan_ = assembly::cv_imread(img_path.toStdString(), cv::IMREAD_COLOR);

    NQLog("AssemblyImageView", NQLog::Spam) << "update_image_zscan"
       << ": emitting signal \"image_zscan_updated\"";

    emit image_zscan_updated(image_zscan_);
    emit image_zscan_updated();
  }
  else
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "update_image_zscan"
       << ": invalid path to input file, no action taken (file=" << img_path << ")";

    return;
  }
}

void AssemblyImageView::connectImageProducer_image(const QObject* sender, const char* signal)
{
  NQLog("AssemblyImageView", NQLog::Debug) << "connectImageProducer_image";

  img_ueye_->connectImageProducer(sender, signal);
}

void AssemblyImageView::disconnectImageProducer_image(const QObject* sender, const char* signal)
{
  NQLog("AssemblyImageView", NQLog::Debug) << "disconnectImageProducer_image";

  img_ueye_->disconnectImageProducer(sender, signal);
}

void AssemblyImageView::connectImageProducer_autofocus(const QObject* sender, const char* signal)
{
  NQLog("AssemblyImageView", NQLog::Debug) << "connectImageProducer_autofocus";

  autofocus_ueye_->connectImageProducer(sender, signal);
}

void AssemblyImageView::disconnectImageProducer_autofocus(const QObject* sender, const char* signal)
{
  NQLog("AssemblyImageView", NQLog::Debug) << "disconnectImageProducer_autofocus";

  autofocus_ueye_->disconnectImageProducer(sender, signal);
}

void AssemblyImageView::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch (event->key())
    {
      case Qt::Key_0:
//        img_ueye_->setZoomFactor(0.25);
//        autofocus_ueye_ ->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        img_ueye_->setZoomFactor(1.00);
//        autofocus_ueye_ ->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        img_ueye_->increaseZoomFactor();
//        autofocus_ueye_ ->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        img_ueye_->decreaseZoomFactor();
//        autofocus_ueye_ ->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}

//Upon holding mouse click within image area, display mouse position w.r.t. image center, preoperly converted into a physical distance expressed in um
void AssemblyImageView::mouseMoveEvent(QMouseEvent* event)
{
    if(image_.empty()) {return;} //No action if image not yet loaded

    //Get height and width of displayed image (QT coordinates -- based on screen pixels ?)
    int img_height_QTCoord = img_ueye_->height();
    int img_width_QTCoord = img_ueye_->width();

    //For the record: alternative way to access QT coordinates of image
    // QPoint globalPosTopLeftImg = img_ueye_->mapToGlobal(img_ueye_->rect().topLeft());

    //Get nof rows and colums of displayed image (image coordinates -- based on camera pixels ?)
    int nrows = image_raw_.rows;
    int ncols = image_raw_.cols;

    //Get mouse cursor QT coordinates (in image's rest frame)
    QPoint pt = img_ueye_->mapFrom(this, event->pos());
    double x = pt.x();
    double y = pt.y();

    //If detect that mouse is outside of image's boundaries -> Ignore
    if(x > img_width_QTCoord || x < 0) {event->ignore(); return;}
    if(y > img_height_QTCoord || y < 0) {event->ignore(); return;}

    //Coordinates transformation
    //QT coord --> image pixel coord (w.r.t. image center)
    //Image pixel coord --> physical distance
    //NB: the y-distance in the Camera frame has to be inverted, because the cv::Mat object in OpenCV counts columns (X) and rows (Y) starting from the top-left corner (not from the bottom-left corner as for a normal XY reference frame); in order to convert this to a normal XY ref-frame, we invert the sign of the value on the Y-axis.
    const double dX_0 = +1.0 * (x * ((double) ncols / img_width_QTCoord) - (image_raw_.cols / 2.0)) * mm_per_pixel_col_;
    const double dY_0 = -1.0 * (y * ((double) nrows / img_height_QTCoord) - (image_raw_.rows / 2.0)) * mm_per_pixel_row_;

    //Account for angle of Camera Frame in the XY Motion Stage Ref-Frame
    double newX, newY;
    assembly::rotation2D_deg(newX, newY, angle_FromCameraXYtoRefFrameXY_deg_, dX_0, dY_0);

    //Convert mm to um //For display only
    newX*= 1000; newY*= 1000;

    //Display physical distances next to mouse cursor
    QToolTip::showText(event->globalPos(), "(x,y)[um]= " + QString::number(newX) + ", " + QString::number(newY), this, rect());

    return;
}

//Upon double-click within image area, display lines at mouse position, and prompt a message box to user for action
//Mostly duplicated from mouseMoveEvent()
void AssemblyImageView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(event->button() != Qt::LeftButton) {return;} //Only consider left-click

    // if(countNonZero(image_) < 1) {return;} //No action if image not yet loaded
    if(image_.empty()) {return;} //No action if image not yet loaded

    cv::Mat img_original = image_.clone(); //Save image before modification
    cv::Mat img_modif = image_.clone(); //Image copy to be modified

    //Get height and width of displayed image (QT coordinates -- based on screen pixels ?)
    int img_height_QTCoord = img_ueye_->height();
    int img_width_QTCoord = img_ueye_->width();

    //Get nof rows and colums of displayed image (image coordinates -- based on camera pixels ?)
    int nrows = image_raw_.rows;
    int ncols = image_raw_.cols;

    //Get mouse cursor QT coordinates (in image's rest frame)
    QPoint pt = img_ueye_->mapFrom(this, event->pos());
    double x = pt.x();
    double y = pt.y();

    //If detect that mouse is outside of image's boundaries -> Ignore
    if(x > img_width_QTCoord || x < 0) {event->ignore(); return;}
    if(y > img_height_QTCoord || y < 0) {event->ignore(); return;}

    //Coordinates transformation //QT coord --> image pixel coord (w.r.t. image center)
    double posLine_x = x * ((double) ncols / img_width_QTCoord);
    double posLine_y = y * ((double) nrows  / img_height_QTCoord);

    //Display lines
    line(img_modif, cv::Point(posLine_x, 0), cv::Point(posLine_x, img_modif.rows), cv::Scalar(0,255,0), 8, 8, 0);
    line(img_modif, cv::Point(0, posLine_y), cv::Point(img_modif.cols, posLine_y), cv::Scalar(0,255,0), 8, 8, 0);
    this->update_image(img_modif, false);

    //Coordinates transformation
    //QT coord --> image pixel coord (w.r.t. image center)
    //Image pixel coord --> physical distance
    //NB: the y-distance in the Camera frame has to be inverted, because the cv::Mat object in OpenCV counts columns (X) and rows (Y) starting from the top-left corner (not from the bottom-left corner as for a normal XY reference frame); in order to convert this to a normal XY ref-frame, we invert the sign of the value on the Y-axis.
    const double dX_0 = +1.0 * (x * ((double) ncols / img_width_QTCoord) - (image_raw_.cols / 2.0)) * mm_per_pixel_col_;
    const double dY_0 = -1.0 * (y * ((double) nrows / img_height_QTCoord) - (image_raw_.rows / 2.0)) * mm_per_pixel_row_;

    //Account for angle of Camera Frame in the XY Motion Stage Ref-Frame
    double distx, disty;
    assembly::rotation2D_deg(distx, disty, angle_FromCameraXYtoRefFrameXY_deg_, dX_0, dY_0);

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Apply relative movement"));
    msgBox.setText(QString("<p>Rel. dX=%1 mm / Rel. dY=%2 mm</p>").arg(distx).arg(disty));
    msgBox.setInformativeText("Do you want to apply this relative movement?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    this->update_image(img_original, true); //Restore original image

    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }

    emit sigRequestMoveRelative(distx, disty, 0, 0);

    NQLog("AssemblyImageView", NQLog::Spam) << "mouseDoubleClickEvent"
       << ": will apply relative movement: dx="<<distx<<" / dy="<<disty;

    //Wait for the movement to be finished (or for timeout), then automatically update the snapshot image displayed in GUI
    QEventLoop loop;
    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->start(5000); //5s
    connect(this, SIGNAL(cameraMotionIsFinished()), &loop, SLOT(quit()));
    connect(timeoutTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    loop.exec(); //blocks thread until either the signal or timeout gets 'fired'

    //Then, re-acquire new snapshot image
    emit request_image();

    delete timeoutTimer;

    return;
}

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyImageView::display_infoTab()
{
    QMessageBox::information(this, tr("Information - Image Viewer"),
            tr("<ul>"
            "<li>Click 'Snapshot' to take a camera image. An image can be loaded/saved.</li>"
            "<li>Holding the mouseclick over the picture displays the relative x/y distances to the center. Upon double-click on the image, the camera moves to the selected position.</li>"
            "<li>Clike 'Auto-focus image' to run the z-focus routine: pictures are taken at different z-values, and at the end the camera is moved to the best-focus height determined by the algorithm. Note that the result is interpolated, so the routine may be repeated to find the best average height. The delta-Z range may be adjusted to refine the measurements (min dX=5 microns).</li>"
            "</ul>"
            ));
    return;
}

void AssemblyImageView::InfoMotionFinished()
{
    emit cameraMotionIsFinished();

    return;
}

void AssemblyImageView::makeProgBarVisible()
{
    progBar_->setVisible(true);
    return;
}
void AssemblyImageView::makeProgBarInvisible()
{
    progBar_->setVisible(false);
    return;
}

//Update the value of the progress bar
void AssemblyImageView::update_progBar(int prog)
{
    progBar_->setValue(prog);
    // progBar_->setStyleSheet("QProgressBar::chunk {background: orange;}");

    // progBar_->setProperty("defaultStyleSheet", progBar_->styleSheet());
    // progBar_->setStyleSheet(progBar_->property("defaultStyleSheet").toString() +
    // " QProgressBar::chunk { background: green; }");

    return;
}

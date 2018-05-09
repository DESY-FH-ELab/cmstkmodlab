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

#include <AssemblyImageView.h>
#include <nqlogger.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

AssemblyImageView::AssemblyImageView(QWidget* parent) :
  QWidget(parent),

  img_ueye_(nullptr),
  img_scroll_(nullptr),
  img_load_button_(nullptr),
  img_save_button_(nullptr),

  AF_ueye_(nullptr),
  AF_scroll_(nullptr),
  AF_result_bestZ_lineed_(nullptr),
  AF_param_maxDZ_lineed_(nullptr),
  AF_param_Nstep_lineed_(nullptr),
  AF_exe_button_(nullptr)
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
  QVBoxLayout* AF_result_lay = new QVBoxLayout;

  AF_ueye_ = new AssemblyUEyeView(this);
  AF_ueye_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  AF_ueye_->setMinimumSize(500, 300);
  AF_ueye_->setPalette(palette);
  AF_ueye_->setBackgroundRole(QPalette::Background);
  AF_ueye_->setScaledContents(true);
  AF_ueye_->setAlignment(Qt::AlignCenter);

  AF_scroll_ = new QScrollArea(this);
  AF_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  AF_scroll_->setMinimumSize(500, 300);
  AF_scroll_->setPalette(palette);
  AF_scroll_->setBackgroundRole(QPalette::Background);
  AF_scroll_->setAlignment(Qt::AlignCenter);
  AF_scroll_->setWidget(AF_ueye_);

  AF_result_lay->addWidget(AF_scroll_);

  QFormLayout* AF_result_bestZ_lay = new QFormLayout;

  QLabel* AF_result_bestZ_label = new QLabel("Best-Focus Z-position [mm]", this);
  AF_result_bestZ_lineed_ = new QLineEdit("", this);
  AF_result_bestZ_lineed_->setReadOnly(true);

  AF_result_bestZ_lay->addRow(AF_result_bestZ_label, AF_result_bestZ_lineed_);

  AF_result_lay->addLayout(AF_result_bestZ_lay);

  g0->addLayout(AF_result_lay, 1, 0);
  // ----------

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  //// --------------------------------------------------

  //// right-hand side ----------------------------------

  // image
  QFormLayout* lImg = new QFormLayout;
  g0->addLayout(lImg, 0, 1);

  img_load_button_ = new QPushButton("Load Image", this);
  lImg->addRow(img_load_button_);

  img_save_button_ = new QPushButton("Save Image", this);
  lImg->addRow(img_save_button_);

  connect(img_load_button_, SIGNAL(clicked()), this, SLOT(load_image()));
  connect(img_save_button_, SIGNAL(clicked()), this, SLOT(save_image()));
  // ----------

  // auto-focusing
  QFormLayout* AF_lay = new QFormLayout;
  g0->addLayout(AF_lay, 1, 1);

  QGroupBox* AF_param_box = new QGroupBox(tr("Auto-Focus Configuration"));
  AF_lay->addRow(AF_param_box);

  QFormLayout* AF_param_lay = new QFormLayout;
  AF_param_box->setLayout(AF_param_lay);

  QLabel* AF_param_maxDZ_label = new QLabel("Max Z-spread [mm]", this);
  AF_param_maxDZ_lineed_ = new QLineEdit("", this);

  QLabel* AF_param_Nstep_label = new QLabel("# Steps (int)", this);
  AF_param_Nstep_lineed_ = new QLineEdit("", this);

  AF_param_lay->addRow(AF_param_maxDZ_label, AF_param_maxDZ_lineed_);
  AF_param_lay->addRow(AF_param_Nstep_label, AF_param_Nstep_lineed_);

  AF_exe_button_ = new QPushButton("Auto-Focus Image", this);
  AF_lay->addRow(AF_exe_button_);

  connect(AF_exe_button_, SIGNAL(clicked()), this, SLOT(configure_scan()));
  // ----------

  //// --------------------------------------------------
}

//!!void AssemblyImageView::save_image()
//!!{
//!!  NQLog("AssemblyImageView", NQLog::Debug) << "save_image"
//!!     << ": emitting signal \"image_request\"";
//!!
//!!  emit image_request();
//!!}
//!!
//!!void AssemblyImageView::save_image(const cv::Mat& image)
//!!{
//!!  if(image.empty() == true)
//!!  {
//!!    NQLog("AssemblyImageView", NQLog::Warning) << "save_image"
//!!       << ": input cv::Mat object is empty, no image saved";
//!!
//!!    return;
//!!  }
//!!
//!!  QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
//!!  if(filename.isNull() || filename.isEmpty()){ return; }
//!!
//!!  if(filename.endsWith(".png") == false){ filename += ".png"; }
//!!
//!!  cv::imwrite(filename.toStdString(), image);
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyImageView::updateText(const double z)
//!!{
//!!  AF_result_bestZ_lineed_->setText(QString::fromStdString(std::to_string(z)));
//!!
//!!  NQLog("AssemblyImageView", NQLog::Spam) << "updateText"
//!!     << ": displayed value of best z-position (focal point)";
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyImageView::connectImageProducer_image(const QObject* sender, const char* signal)
//!!{
//!!  NQLog("AssemblyImageView", NQLog::Debug) << "connectImageProducer_image";
//!!
//!!  img_ueye_->connectImageProducer(sender, signal);
//!!}
//!!
//!!void AssemblyImageView::disconnectImageProducer_image(const QObject* sender, const char* signal)
//!!{
//!!  NQLog("AssemblyImageView", NQLog::Debug) << "disconnectImageProducer_image";
//!!
//!!  img_ueye_->disconnectImageProducer(sender, signal);
//!!}
//!!
//!!void AssemblyImageView::connectImageProducer_autofocus(const QObject* sender, const char* signal)
//!!{
//!!  NQLog("AssemblyImageView", NQLog::Debug) << "connectImageProducer_autofocus";
//!!
//!!  AF_ueye_->connectImageProducer(sender, signal);
//!!}
//!!
//!!void AssemblyImageView::disconnectImageProducer_autofocus(const QObject* sender, const char* signal)
//!!{
//!!  NQLog("AssemblyImageView", NQLog::Debug) << "disconnectImageProducer_autofocus";
//!!
//!!  AF_ueye_->disconnectImageProducer(sender, signal);
//!!}

void AssemblyImageView::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch (event->key())
    {
      case Qt::Key_0:
//        img_ueye_->setZoomFactor(0.25);
//        AF_ueye_ ->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        img_ueye_->setZoomFactor(1.00);
//        AF_ueye_ ->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        img_ueye_->increaseZoomFactor();
//        AF_ueye_ ->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        img_ueye_->decreaseZoomFactor();
//        AF_ueye_ ->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}

//!!void AssemblyImageView::update_scan_config(const double zrange, const int points)
//!!{
//!!  const std::string str = std::to_string(zrange)+","+std::to_string(points);
//!!
//!!  std::stringstream scan_strs;
//!!  scan_strs << zrange << ", " << points;
//!!
//!!  lineed_1_->setText(QString::fromStdString(scan_strs.str()));
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyImageView::configure_scan()
//!!{
//!!  // parse lineEdit text to get target coordinates
//!!  const QString parent_string = lineed_1_->text();
//!!
//!!  const QStringList pieces = parent_string.split(",");
//!!
//!!  if(pieces.length() == 2)
//!!  {
//!!    const QString x = pieces.value(0);
//!!    const QString y = pieces.value(1);
//!!
//!!    bool valid_x_d(false);
//!!    bool valid_y_d(false);
//!!
//!!    const double x_d = x.toDouble(&valid_x_d);
//!!    const double y_d = y.toInt   (&valid_y_d);
//!!
//!!    if(!valid_x_d)
//!!    {
//!!      NQLog("AssemblyImageView", NQLog::Warning) << "configure_scan"
//!!         << ": invalid format for scan-config parameter #1 (" << x << "), no action taken";
//!!
//!!      return;
//!!    }
//!!    else if(!valid_y_d)
//!!    {
//!!      NQLog("AssemblyImageView", NQLog::Warning) << "configure_scan"
//!!         << ": invalid format for scan-config parameter #2 (" << y << "), no action taken";
//!!
//!!      return;
//!!    }
//!!
//!!    NQLog("AssemblyImageView", NQLog::Spam) << "configure_scan"
//!!       << ": emitting signal \"scan_config(" << x_d << ", " << y_d << "\")";
//!!
//!!    emit scan_config(x_d, y_d);
//!!  }
//!!  else
//!!  {
//!!    NQLog("AssemblyImageView", NQLog::Warning) << "configure_scan"
//!!       << ": invalid input string format, no action taken";
//!!
//!!    return;
//!!  }
//!!}
//!!
//!!void AssemblyImageView::read_graph(const QString& img_path)
//!!{
//!!  if(Util::IsFile(img_path))
//!!  {
//!!    const cv::Mat img = cv::imread(img_path.toStdString(), CV_LOAD_IMAGE_COLOR);
//!!
//!!    NQLog("AssemblyImageView", NQLog::Spam) << "read_graph"
//!!       << ": emitting signal \"graph_found\"";
//!!
//!!    emit graph_found(img);
//!!  }
//!!  else
//!!  {
//!!    NQLog("AssemblyImageView", NQLog::Warning) << "read_graph"
//!!       << ": invalid path to input file, no action taken (file=" << img_path << ")";
//!!
//!!    return;
//!!  }
//!!}

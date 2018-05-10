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

#include <AssemblyImageView.h>
#include <AssemblyUtilities.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

AssemblyImageView::AssemblyImageView(QWidget* parent) :
  QWidget(parent),

  // image
  img_ueye_(nullptr),
  img_scroll_(nullptr),
  img_load_button_(nullptr),
  img_save_button_(nullptr),
  img_celi_button_(nullptr),

  // auto-focusing
  AF_ueye_(nullptr),
  AF_scroll_(nullptr),
  AF_result_bestZ_lineed_(nullptr),
  AF_exe_button_(nullptr),
  AF_param_maxDZ_lineed_(nullptr),
  AF_param_Nstep_lineed_(nullptr),
  AF_save_zscan_button_(nullptr)
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
  AF_ueye_->setZoomFactor(0.75);

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
  QFormLayout*  lImg = new QFormLayout;
  g0->addLayout(lImg, 0, 1);

  img_load_button_ = new QPushButton("Load Image", this);
  lImg->addRow(img_load_button_);

  img_save_button_ = new QPushButton("Save Image", this);
  lImg->addRow(img_save_button_);

  img_celi_button_ = new QPushButton("Add/Remove Center Lines", this);
  lImg->addRow(img_celi_button_);

  connect(img_load_button_, SIGNAL(clicked()), this, SLOT(load_image()));
  connect(img_save_button_, SIGNAL(clicked()), this, SLOT(save_image()));
  connect(img_celi_button_, SIGNAL(clicked()), this, SLOT(modify_image_centerlines()));

  this->connectImageProducer_image(this, SIGNAL(image_updated(cv::Mat)));
  // ----------

  // auto-focusing
  QFormLayout* AF_lay = new QFormLayout;
  g0->addLayout(AF_lay, 1, 1);

  AF_exe_button_ = new QPushButton("Auto-Focus Image", this);
  AF_lay->addRow(AF_exe_button_);

  connect(AF_exe_button_, SIGNAL(clicked()), this, SIGNAL(autofocus_request()));

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

  AF_save_zscan_button_ = new QPushButton("Save Z-Scan Image", this);
  AF_lay->addRow(AF_save_zscan_button_);

  connect(AF_save_zscan_button_, SIGNAL(clicked()), this, SLOT(save_image_zscan()));
  // ----------

  //// --------------------------------------------------
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

  NQLog("AssemblyImageView", NQLog::Spam) << "update_image"
     << ": emitting signal \"image_updated\"";

  if(update_image_raw)
  {
    image_raw_ = image_.clone();

    image_modified_ = false;
  }

  emit image_updated(image_);
}

void AssemblyImageView::load_image()
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"), QDir::homePath(), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  const cv::Mat img = filename.endsWith(".png") ? assembly::cv_imread_png(filename.toStdString(), CV_LOAD_IMAGE_COLOR) : cv::imread(filename.toStdString(), CV_LOAD_IMAGE_COLOR);

  if(img.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "load_image"
       << ": input image is empty, no action taken";

    return;
  }

  this->update_image(img, true);

  return;
}

void AssemblyImageView::save_image()
{
  if(image_.empty())
  {
    NQLog("AssemblyImageView", NQLog::Warning) << "save_image"
       << ": input image is empty, no action taken";

    return;
  }

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::homePath(), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  if(filename.endsWith(".png"))
  {
    assembly::cv_imwrite_png(filename.toStdString(), image_);
  }
  else
  {
    cv::imwrite(filename.toStdString(), image_);
  }

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

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), QDir::homePath(), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  if(filename.endsWith(".png"))
  {
    assembly::cv_imwrite_png(filename.toStdString(), image_zscan_);
  }
  else
  {
    cv::imwrite(filename.toStdString(), image_zscan_);
  }

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
  else
  {
    this->update_image(img, true);
  }

  return;
}

void AssemblyImageView::update_text(const double z)
{
  AF_result_bestZ_lineed_->setText(QString::fromStdString(std::to_string(z)));

  NQLog("AssemblyImageView", NQLog::Spam) << "update_text"
     << ": displayed value of best z-position (focal point)";

  return;
}

void AssemblyImageView::update_autofocus_config(const double maxDZ, const int Nstep)
{
  AF_param_maxDZ_lineed_->setText(QString::fromStdString(std::to_string(maxDZ)));
  AF_param_Nstep_lineed_->setText(QString::fromStdString(std::to_string(Nstep)));

  return;
}

void AssemblyImageView::acquire_autofocus_config()
{
  // maximum delta-Z movement
  const QString maxDZ_str = AF_param_maxDZ_lineed_->text();

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
  const QString Nstep_str = AF_param_Nstep_lineed_->text();

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
    image_zscan_ = img_path.endsWith(".png") ? assembly::cv_imread_png(img_path.toStdString(), CV_LOAD_IMAGE_COLOR) : cv::imread(img_path.toStdString(), CV_LOAD_IMAGE_COLOR);

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

  AF_ueye_->connectImageProducer(sender, signal);
}

void AssemblyImageView::disconnectImageProducer_autofocus(const QObject* sender, const char* signal)
{
  NQLog("AssemblyImageView", NQLog::Debug) << "disconnectImageProducer_autofocus";

  AF_ueye_->disconnectImageProducer(sender, signal);
}

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

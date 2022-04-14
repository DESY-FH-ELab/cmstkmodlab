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

#include <AssemblyThresholderView.h>
#include <AssemblyUtilities.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

AssemblyThresholderView::AssemblyThresholderView(QWidget* parent) :
  QWidget(parent),

  // image raw
  imgraw_ueye_  (nullptr),
  imgraw_scroll_(nullptr),

  imgraw_load_button_(nullptr),
  imgraw_save_button_(nullptr),

  // image binary
  imgbin_ueye_  (nullptr),
  imgbin_scroll_(nullptr),

  imgbin_save_button_(nullptr),

  imgbin_thresh_button_(nullptr),
  imgbin_thresh_label_ (nullptr),
  imgbin_thresh_linee_ (nullptr),

  imgbin_adathr_button_(nullptr),
  imgbin_adathr_label_ (nullptr),
  imgbin_adathr_linee_ (nullptr)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  QGridLayout* l = new QGridLayout;
  this->setLayout(l);

  //// left-hand side
  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  imgraw_ueye_ = new AssemblyUEyeView(this);
  imgraw_ueye_->setMinimumSize(500, 300);
  imgraw_ueye_->setPalette(palette);
  imgraw_ueye_->setBackgroundRole(QPalette::Background);
  imgraw_ueye_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imgraw_ueye_->setScaledContents(true);
  imgraw_ueye_->setAlignment(Qt::AlignCenter);

  imgraw_scroll_ = new QScrollArea(this);
  imgraw_scroll_->setMinimumSize(500, 300);
  imgraw_scroll_->setPalette(palette);
  imgraw_scroll_->setBackgroundRole(QPalette::Background);
  imgraw_scroll_->setAlignment(Qt::AlignCenter);
  imgraw_scroll_->setWidget(imgraw_ueye_);
  imgraw_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  imgbin_ueye_ = new AssemblyUEyeView(this);
  imgbin_ueye_->setMinimumSize(500, 300);
  imgbin_ueye_->setPalette(palette);
  imgbin_ueye_->setBackgroundRole(QPalette::Background);
  imgbin_ueye_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imgbin_ueye_->setScaledContents(true);
  imgbin_ueye_->setAlignment(Qt::AlignCenter);

  imgbin_scroll_ = new QScrollArea(this);
  imgbin_scroll_->setMinimumSize(500, 300);
  imgbin_scroll_->setPalette(palette);
  imgbin_scroll_->setBackgroundRole(QPalette::Background);
  imgbin_scroll_->setAlignment(Qt::AlignCenter);
  imgbin_scroll_->setWidget(imgbin_ueye_);
  imgbin_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  l->addWidget(imgraw_scroll_, 0, 0);
  l->addWidget(imgbin_scroll_, 1, 0);

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //// ---------------

  //// right-hand side

  /// raw image
  QVBoxLayout* lRaw = new QVBoxLayout;
  l->addLayout(lRaw, 0, 1);

  imgraw_load_button_ = new QPushButton("Load Image", this);
  lRaw->addWidget(imgraw_load_button_);

  imgraw_save_button_ = new QPushButton("Save Image (before thresholding)", this);
  lRaw->addWidget(imgraw_save_button_);

  connect(imgraw_load_button_, SIGNAL(clicked()), this, SLOT(load_image_raw()));
  connect(imgraw_save_button_, SIGNAL(clicked()), this, SLOT(save_image_raw()));

  this->connectImageProducer_raw(this, SIGNAL(image_raw_updated(cv::Mat)));

  lRaw->addStretch();

  /// binary image: thresholding
  QVBoxLayout* lBin = new QVBoxLayout;
  l->addLayout(lBin, 1, 1);

  imgbin_save_button_ = new QPushButton("Save Image (after thresholding)", this);
  lBin->addWidget(imgbin_save_button_);

  connect(imgbin_save_button_, SIGNAL(clicked()), this, SLOT(save_image_binary()));

  this->connectImageProducer_binary(this, SIGNAL(image_binary_updated(cv::Mat)));

  // method-1: threshold
  lBin->addSpacing(40);

  QVBoxLayout* imgbin_thresh_layout = new QVBoxLayout;
  lBin->addLayout(imgbin_thresh_layout);

  imgbin_thresh_button_ = new QPushButton("openCV::threshold", this); //Click this button to read/apply the value in the QLineEdit field
  imgbin_thresh_layout->addWidget(imgbin_thresh_button_);
  const QSize BUTTON_SIZE = QSize(530, 35); //Set fixed button size
  imgbin_thresh_button_->setFixedSize(BUTTON_SIZE);

  connect(imgbin_thresh_button_, SIGNAL(clicked()), this, SLOT(apply_threshold()));

  QHBoxLayout* imgbin_thresh_inputcfg = new QHBoxLayout;
  imgbin_thresh_layout->addLayout(imgbin_thresh_inputcfg);

  imgbin_thresh_label_ = new QLabel(this);
  imgbin_thresh_label_->setText("Threshold (pos int)");

  imgbin_thresh_linee_ = new QLineEdit(this);
  assembly::QLineEdit_setText(imgbin_thresh_linee_, config->getDefaultValue<int>("main", "AssemblyThresholderView_threshold", 30));

  imgbin_thresh_inputcfg->addWidget(imgbin_thresh_label_, 40);
  imgbin_thresh_inputcfg->addWidget(imgbin_thresh_linee_, 60);

  //Slider widget to chose the B/W threshold value using the mouse
  threshold_slider_ = new QSlider(Qt::Horizontal, this);

  //Fancy style
  threshold_slider_->setStyleSheet("QSlider::groove:horizontal {border: 1px solid #bbb;background: white;height: 10px;border-radius: 4px;}"
  "QSlider::sub-page:horizontal {background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,stop: 0 #66e, stop: 1 #bbf); background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, stop: 0 #bbf, stop: 1 #55f); border: 1px solid #777; height: 10px; border-radius: 4px;}"
  "QSlider::add-page:horizontal {background: #fff;border: 1px solid #777;height: 10px;border-radius: 4px;}"
  "QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 #eee, stop:1 #ccc);border: 1px solid #777;width: 13px;margin-top: -2px;margin-bottom: -2px;border-radius: 4px;}"
  "QSlider::handle:horizontal:hover {background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 #fff, stop:1 #ddd);border: 1px solid #444;border-radius: 4px;}"
  "QSlider::sub-page:horizontal:disabled {background: #bbb;border-color: #999;}"
  "QSlider::add-page:horizontal:disabled {background: #eee;border-color: #999;}"
  "QSlider::handle:horizontal:disabled {background: #eee;border: 1px solid #aaa;border-radius: 4px;}"
);

  threshold_slider_->setValue(config->getDefaultValue<int>("main", "AssemblyThresholderView_threshold", 30)); //Init to default value
  threshold_slider_->setMinimum(0); threshold_slider_->setMaximum(255); //Set range
  threshold_slider_->setSingleStep(5); //Step values when using left/right keys
  threshold_slider_->setTickPosition(QSlider::TicksBothSides); //Ticks above+below //Not working with this styleSheet
  threshold_slider_->setTickInterval(50); //Step interval between ticks
  imgbin_thresh_inputcfg->addWidget(threshold_slider_, 80);
  connect(threshold_slider_, SIGNAL(valueChanged(int)), this, SLOT(set_bw_threshold_slider(int)));
  // -----

  // method-2: adaptiveThreshold
  lBin->addSpacing(40);

  QVBoxLayout* imgbin_adathr_layout = new QVBoxLayout;
  lBin->addLayout(imgbin_adathr_layout);

  imgbin_adathr_button_ = new QPushButton("openCV::adaptiveThreshold", this);
  imgbin_adathr_layout->addWidget(imgbin_adathr_button_, 0, 0);

  connect(imgbin_adathr_button_, SIGNAL(clicked()), this, SLOT(apply_adaptiveThreshold()));

  QHBoxLayout* imgbin_adathr_inputcfg = new QHBoxLayout;
  imgbin_adathr_layout->addLayout(imgbin_adathr_inputcfg);

  imgbin_adathr_label_ = new QLabel(this);
  imgbin_adathr_label_->setText("Block Size (pos odd int)");

  imgbin_adathr_linee_ = new QLineEdit(this);
  assembly::QLineEdit_setText(imgbin_adathr_linee_, config->getDefaultValue<int>("main", "AssemblyThresholderView_adaptiveThreshold", 587));

  imgbin_adathr_inputcfg->addWidget(imgbin_adathr_label_, 40);
  imgbin_adathr_inputcfg->addWidget(imgbin_adathr_linee_, 60);
  // -----

  lBin->addStretch();
  //// ---------------
}

void AssemblyThresholderView::update_image_raw(const cv::Mat& img)
{
  if(img.channels() == 1)
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    imgraw_ = img_color.clone();
  }
  else
  {
    imgraw_ = img.clone();
  }

  NQLog("AssemblyThresholderView", NQLog::Spam) << "update_image_raw"
     << ": emitting signal \"image_raw_updated\"";

  emit image_raw_updated(imgraw_);
  emit image_raw_updated();
}

void AssemblyThresholderView::update_image_binary(const cv::Mat& img)
{
  if(img.channels() == 1)
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    imgbin_ = img_color.clone();
  }
  else
  {
    imgbin_ = img.clone();
  }

  NQLog("AssemblyThresholderView", NQLog::Spam) << "update_image_binary"
     << ": emitting signal \"image_binary_updated\"";

  emit image_binary_updated(imgbin_);
  emit image_binary_updated();
}

void AssemblyThresholderView::load_image_raw()
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  const cv::Mat img = assembly::cv_imread(filename, cv::IMREAD_COLOR);

  if(img.empty())
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "load_image_raw"
       << ": input image is empty, no action taken";

    return;
  }

  NQLog("AssemblyThresholderView", NQLog::Spam) << "load_image_raw"
     << ": emitting signal \"loaded_image_raw\"";

  emit loaded_image_raw(img);
}

void AssemblyThresholderView::save_image_raw()
{
  if(imgraw_.empty())
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "save_image_raw"
       << ": input raw image is empty, no action taken";

    return;
  }

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  assembly::cv_imwrite(filename, imgraw_);

  return;
}

void AssemblyThresholderView::save_image_binary()
{
  if(imgbin_.empty())
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "save_image_binary"
       << ": input binary image is empty, no action taken";

    return;
  }

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), QString::fromStdString(Config::CMSTkModLabBasePath+"/share/assembly"), tr("PNG Files (*.png);;All Files (*)"));
  if(filename.isNull() || filename.isEmpty()){ return; }

  assembly::cv_imwrite(filename, imgbin_);

  return;
}

void AssemblyThresholderView::apply_threshold()
{
  bool valid_thr(false);

  const int thr = imgbin_thresh_linee_->text().toInt(&valid_thr);

  if(valid_thr == false)
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "apply_threshold"
       << ": invalid (non-integer) format for threshold value (" << imgbin_thresh_linee_->text() << "), no action taken";

    return;
  }

  NQLog("AssemblyThresholderView", NQLog::Debug) << "apply_threshold"
     << ": emitting signal \"threshold_value(" << thr << ")\"";

  emit threshold_request(thr);
}

void AssemblyThresholderView::apply_adaptiveThreshold()
{
  bool valid_bks(false);

  const int bks = imgbin_adathr_linee_->text().toInt(&valid_bks);

  if(valid_bks == false)
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "apply_adaptiveThreshold"
       << ": invalid (non-integer) format for block-size value (" << imgbin_adathr_linee_->text() << "), no action taken";

    return;
  }

  NQLog("AssemblyThresholderView", NQLog::Debug) << "apply_adaptiveThreshold"
     << ": emitting signal \"adaptiveThreshold_request(" << bks << ")\"";

  emit adaptiveThreshold_request(bks);
}

void AssemblyThresholderView::connectImageProducer_raw(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "connectImageProducer_raw";

  imgraw_ueye_->connectImageProducer(sender, signal);
}

void AssemblyThresholderView::disconnectImageProducer_raw(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "disconnectImageProducer_raw";

  imgraw_ueye_->disconnectImageProducer(sender, signal);
}

void AssemblyThresholderView::connectImageProducer_binary(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "connectImageProducer_binary";

  imgbin_ueye_->connectImageProducer(sender, signal);
}

void AssemblyThresholderView::disconnectImageProducer_binary(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "disconnectImageProducer_binary";

  imgbin_ueye_->disconnectImageProducer(sender, signal);
}

void AssemblyThresholderView::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch (event->key())
    {
      case Qt::Key_0:
//        imgraw_ueye_->setZoomFactor(0.25);
//        imgbin_ueye_->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        imgraw_ueye_->setZoomFactor(1.00);
//        imgbin_ueye_->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        imgraw_ueye_->increaseZoomFactor();
//        imgbin_ueye_->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        imgraw_ueye_->decreaseZoomFactor();
//        imgbin_ueye_->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyThresholderView::display_infoTab()
{
    QMessageBox::information(this, tr("Information - Convert Image to B/W"),
            tr("<p>There is no available information about the content of this tab yet.</p>"));

    return;
}

//-- Update the B/W threshold when slider value gets changed
void AssemblyThresholderView::set_bw_threshold_slider(int thr)
{
    threshold_slider_->setValue(thr); //Update the slider's value
    assembly::QLineEdit_setText(imgbin_thresh_linee_, thr); //Update the value displayed

    // NQLog("AssemblyThresholderView", NQLog::Debug) << "apply_threshold" << ": emitting signal \"threshold_value(" << thr << ")\"";

    emit threshold_request(thr); //Update the B/W threshold (real-time)

    return;
}

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

#include <AssemblyThresholderView.h>
#include <Util.h>
#include <nqlogger.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QFormLayout>

AssemblyThresholderView::AssemblyThresholderView(QWidget* parent) : QWidget(parent)
{
  QGridLayout* l = new QGridLayout;
  this->setLayout(l);

  //// left-hand side
  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  imageView_1_ = new AssemblyUEyeView(this);
  imageView_1_->setMinimumSize(500, 300);
  imageView_1_->setPalette(palette);
  imageView_1_->setBackgroundRole(QPalette::Background);
  imageView_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_1_->setScaledContents(true);
  imageView_1_->setAlignment(Qt::AlignCenter);

  scrollArea_1_ = new QScrollArea(this);
  scrollArea_1_->setMinimumSize(500, 300);
  scrollArea_1_->setPalette(palette);
  scrollArea_1_->setBackgroundRole(QPalette::Background);
  scrollArea_1_->setAlignment(Qt::AlignCenter);
  scrollArea_1_->setWidget(imageView_1_);
  scrollArea_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  imageView_2_ = new AssemblyUEyeView(this);
  imageView_2_->setMinimumSize(500, 300);
  imageView_2_->setPalette(palette);
  imageView_2_->setBackgroundRole(QPalette::Background);
  imageView_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2_->setScaledContents(true);
  imageView_2_->setAlignment(Qt::AlignCenter);

  scrollArea_2_ = new QScrollArea(this);
  scrollArea_2_->setMinimumSize(500, 300);
  scrollArea_2_->setPalette(palette);
  scrollArea_2_->setBackgroundRole(QPalette::Background);
  scrollArea_2_->setAlignment(Qt::AlignCenter);
  scrollArea_2_->setWidget(imageView_2_);
  scrollArea_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  l->addWidget(scrollArea_1_, 0, 0);
  l->addWidget(scrollArea_2_, 1, 0);

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //// ---------------

  //// right-hand side

  // raw image options
  QFormLayout* lRaw = new QFormLayout;
  l->addLayout(lRaw, 0, 1);

  imgraw_button_ = new QPushButton("Save Image (before thresholding)", this);
  lRaw->addRow(imgraw_button_);

  // image-thresholding
  QGridLayout* lTuner = new QGridLayout;
  l->addLayout(lTuner, 1, 1);

  imgbin_button_ = new QPushButton("Save Image (after thresholding)", this);
  lTuner->addWidget(imgbin_button_, 0, 0);

  // method-1: threshold
  QGridLayout* thresh_layout = new QGridLayout;
  lTuner->addLayout(thresh_layout, 1, 0);

  thresh_button_ = new QPushButton("openCV::threshold", this);
  thresh_layout->addWidget(thresh_button_, 0, 0);

  QFormLayout* thresh_inputcfg = new QFormLayout;
  thresh_layout->addLayout(thresh_inputcfg, 1, 0);

  thresh_label_ = new QLabel(this);
  thresh_label_->setText("Threshold");

  thresh_linee_ = new QLineEdit(this);
  thresh_linee_->setText("100");

  thresh_inputcfg->addRow(thresh_label_, thresh_linee_);
  // -------------------

  // method-2: adaptiveThreshold
  QGridLayout* adathr_layout = new QGridLayout;
  lTuner->addLayout(adathr_layout, 2, 0);

  adathr_button_ = new QPushButton("openCV::adaptiveThreshold", this);
  adathr_layout->addWidget(adathr_button_, 0, 0);

  QFormLayout* adathr_inputcfg = new QFormLayout;
  adathr_layout->addLayout(adathr_inputcfg, 1, 0);

  adathr_label_ = new QLabel(this);
  adathr_label_->setText("Block Size");

  adathr_linee_ = new QLineEdit(this);
  adathr_linee_->setText("587");

  adathr_inputcfg->addRow(adathr_label_, adathr_linee_);
  // -------------------

  // connection(s)
  connect(imgraw_button_, SIGNAL(clicked()), this, SLOT(save_image_raw()));
  connect(imgbin_button_, SIGNAL(clicked()), this, SLOT(save_image_binary()));
  connect(thresh_button_, SIGNAL(clicked()), this, SLOT(apply_threshold()));
  connect(adathr_button_, SIGNAL(clicked()), this, SLOT(apply_adaptiveThreshold()));
  //// ---------------
}

void AssemblyThresholderView::apply_threshold()
{
  bool valid_thr(false);

  const int thr = thresh_linee_->text().toInt(&valid_thr);

  if(valid_thr == false)
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "apply_threshold"
       << ": invalid (non-integer) format for threshold value (" << thresh_linee_->text() << "), no action taken";

    return;
  }

  NQLog("AssemblyThresholderView", NQLog::Debug) << "apply_threshold"
     << ": emitting signal \"threshold_value(" << thr << ")\"";

  emit threshold_request(thr);
}

void AssemblyThresholderView::apply_adaptiveThreshold()
{
  bool valid_bks(false);

  const int bks = adathr_linee_->text().toInt(&valid_bks);

  if(valid_bks == false)
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "apply_adaptiveThreshold"
       << ": invalid (non-integer) format for block-size value (" << adathr_linee_->text() << "), no action taken";

    return;
  }

  NQLog("AssemblyThresholderView", NQLog::Debug) << "apply_adaptiveThreshold"
     << ": emitting signal \"adaptiveThreshold_request(" << bks << ")\"";

  emit adaptiveThreshold_request(bks);
}

void AssemblyThresholderView::save_image_raw()
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "save_image_raw"
     << ": emitting signal \"image_raw_request\"";

  emit image_raw_request();
}

void AssemblyThresholderView::save_image_binary()
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "save_image_binary"
     << ": emitting signal \"image_binary_request\"";

  emit image_binary_request();
}

void AssemblyThresholderView::save_image(const cv::Mat& image)
{
  if(image.empty() == true)
  {
    NQLog("AssemblyThresholderView", NQLog::Warning) << "save_image"
       << ": input cv::Mat object is empty, no image saved";

    return;
  }

  const QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", "PNG Files (*.png);;All Files (*)");
  if(filename.isNull() || filename.isEmpty()){ return; }

  if(filename.endsWith(".png"))
  {
    Util::cv_imwrite_png(filename.toStdString(), image);
  }
  else
  {
    cv::imwrite(filename.toStdString(), image);
  }

  return;
}

void AssemblyThresholderView::connectImageProducer_1(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "connectImageProducer";

  imageView_1_->connectImageProducer(sender, signal);
}

void AssemblyThresholderView::disconnectImageProducer_1(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "disconnectImageProducer";

  imageView_1_->disconnectImageProducer(sender, signal);
}

void AssemblyThresholderView::connectImageProducer_2(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "connectImageProducer";

  imageView_2_->connectImageProducer(sender, signal);
}

void AssemblyThresholderView::disconnectImageProducer_2(const QObject* sender, const char* signal)
{
  NQLog("AssemblyThresholderView", NQLog::Debug) << "disconnectImageProducer";

  imageView_2_->disconnectImageProducer(sender, signal);
}

void AssemblyThresholderView::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch (event->key())
    {
      case Qt::Key_0:
//        imageView_1_->setZoomFactor(0.25);
//        imageView_2_->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        imageView_1_->setZoomFactor(1.00);
//        imageView_2_->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        imageView_1_->increaseZoomFactor();
//        imageView_2_->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        imageView_1_->decreaseZoomFactor();
//        imageView_2_->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}

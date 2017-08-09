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

#include <vector>

#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>

#include <nqlogger.h>

#include "AssemblyThresholdTuner.h"

AssemblyThresholdTuner::AssemblyThresholdTuner(QWidget *parent)
    : QWidget(parent)
{

  QGridLayout *l = new QGridLayout();
  setLayout(l);  

  /*button = new QPushButton("save", this);
    connect(button, SIGNAL(clicked(bool)),
            this, SLOT(snapShot()));
    
    l->addWidget(button,0,0);*/

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView1_ = new AssemblyUEyeView();
    imageView1_->setMinimumSize(500, 300);
    imageView1_->setPalette(palette);
    imageView1_->setBackgroundRole(QPalette::Background);
    imageView1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView1_->setScaledContents(true);
    imageView1_->setAlignment(Qt::AlignCenter);

    scrollArea1_ = new QScrollArea(this);
    scrollArea1_->setMinimumSize(500, 300);
    scrollArea1_->setPalette(palette);
    scrollArea1_->setBackgroundRole(QPalette::Background);
    scrollArea1_->setAlignment(Qt::AlignCenter);
    scrollArea1_->setWidget(imageView1_);
    scrollArea1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    imageView2_ = new AssemblyUEyeView();
    imageView2_->setMinimumSize(500, 300);
    imageView2_->setPalette(palette);
    imageView2_->setBackgroundRole(QPalette::Background);
    imageView2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView2_->setScaledContents(true);
    imageView2_->setAlignment(Qt::AlignCenter);

    scrollArea2_ = new QScrollArea(this);
    scrollArea2_->setMinimumSize(500, 300);
    scrollArea2_->setPalette(palette);
    scrollArea2_->setBackgroundRole(QPalette::Background);
    scrollArea2_->setAlignment(Qt::AlignCenter);
    scrollArea2_->setWidget(imageView2_);
    scrollArea2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    l->addWidget(scrollArea1_, 0, 0); 
    l->addWidget(scrollArea2_, 1, 0);   

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* Tuner = new QWidget();
    
    l -> addWidget(Tuner, 0, 1);

    QGridLayout *lTuner = new QGridLayout(this);
    Tuner -> setLayout(lTuner);

    setThresholdButton = new QPushButton("Set Threshold", this);
    lTuner->addWidget(setThresholdButton,0,0);
    
    QFormLayout *fl1 = new QFormLayout(this);
    lTuner->addLayout(fl1,2,0);

    label = new QLabel();
    lineEdit = new QLineEdit();
    fl1 -> addRow(label,lineEdit);
    
    label -> setText("Threshold");

    connect(setThresholdButton, SIGNAL(clicked()), this, SLOT(setNewThreshold()));

    
}

void AssemblyThresholdTuner::setNewThreshold()
{
  NQLog("AssemblyThresholdTuner") << "::setNewThreshold():: INFO!!!! Threshold button pressed.";
  
  setThresholdButton -> setEnabled(false);
  emit setNewThreshold((lineEdit -> text()).toInt(), image_);
      
      /*try   //Is exceptions possible in Qt?
    {
    }
    catch (const std::exception& e)
    {
      NQLog("AssemblyThresholdTuner") << " : ERROR! : incorrect threshold value.";
    }*/
}

void AssemblyThresholdTuner::enableThresholdButton()
{
  setThresholdButton -> setEnabled(true);
}

void AssemblyThresholdTuner::disableThresholdButton()
{
  setThresholdButton -> setEnabled(false);
}

void AssemblyThresholdTuner::updateThresholdLabelSlot(int value)
{ 
    NQLog("AssemblyThresholdTuner") << "::updateThresholdLabel():: INFO!! : threshold received. Value = " << value;
    
    lineEdit -> setText(QString::number(value));
    setThresholdButton -> setEnabled(true);
}

void AssemblyThresholdTuner::updateThresholdImage(QString filename)
{
  NQLog("AssemblyThresholdTuner") << "::updateThresholdImage(Qstring) " + filename;

  std::string filename_ss = filename.toUtf8().constData();

  cv::Mat img_gs = cv::imread(filename_ss, CV_LOAD_IMAGE_UNCHANGED);
  
  imageView2_ -> setImage(img_gs);
  //imageView2_ -> setImage(image_);
  //imageView2_->setZoomFactor(0.5);

}

void AssemblyThresholdTuner::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
    NQLog("AssemblyThresholdTuner") << ":connectImageProducer";

    imageView1_->connectImageProducer(sender, signal);

    connect(sender, signal,
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyThresholdTuner::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
    NQLog("AssemblyThresholdTuner") << ":disconnectImageProducer";

    imageView1_->disconnectImageProducer(sender, signal);

    disconnect(sender, signal,
               this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyThresholdTuner::snapShot()
{
    // NQLog("AssemblyThresholdTuner") << ":snapShot()";

    if (image_.rows==0) return;

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
    if (filename.isNull() || filename.isEmpty()) return;

    if (!filename.endsWith(".png")) filename += ".png";

    cv::imwrite(filename.toStdString(), image_);
}

//void AssemblyThresholdTuner::imageAcquired(const cv::Mat& newImage)
void AssemblyThresholdTuner::imageAcquired(cv::Mat newImage)
{
  NQLog("AssemblyThresholdTuner") << "::imageAcquired(const cv::Mat& newImage) : IMAGE copied!!!";
  newImage.copyTo(image_);
}

void AssemblyThresholdTuner::keyReleaseEvent(QKeyEvent * event)
{
    if (!(event->modifiers() & Qt::ShiftModifier)) {
        switch (event->key()) {
        case Qt::Key_0:
	  //            imageView1_->setZoomFactor(0.25);
          //  imageView2_->setZoomFactor(0.25);
            event->accept();
            break;
        case Qt::Key_1:
	  //            imageView1_->setZoomFactor(1.00);
          //  imageView2_->setZoomFactor(1.00);
            event->accept();
            break;
        case Qt::Key_Plus:
	  // imageView1_->increaseZoomFactor();
	  // imageView2_->increaseZoomFactor();
            event->accept();
            break;
        case Qt::Key_Minus:
	  //imageView1_->decreaseZoomFactor();
	  // imageView2_->decreaseZoomFactor();
            event->accept();
            break;
        default:
            break;
        }
    }
}

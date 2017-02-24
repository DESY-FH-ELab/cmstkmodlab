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

    QPushButton * button = new QPushButton("save", this);
    connect(button, SIGNAL(clicked(bool)),
            this, SLOT(snapShot()));
    
    l->addWidget(button,0,0);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_ = new AssemblyUEyeView();
    imageView_->setMinimumSize(800, 600);
    imageView_->setPalette(palette);
    imageView_->setBackgroundRole(QPalette::Background);
    imageView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_->setScaledContents(true);
    imageView_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setMinimumSize(800, 600);
    scrollArea_->setPalette(palette);
    scrollArea_->setBackgroundRole(QPalette::Background);
    scrollArea_->setAlignment(Qt::AlignCenter);
    scrollArea_->setWidget(imageView_);
    scrollArea_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    l->addWidget(scrollArea_, 1, 0);   

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* Tuner = new QWidget();
    
    l -> addWidget(Tuner, 1, 1);

    QGridLayout *lTuner = new QGridLayout(this);
    Tuner -> setLayout(lTuner);

    setThresholdButton = new QPushButton("Set Threshold", this);
    lTuner->addWidget(setThresholdButton,0,0);
    
    QFormLayout *fl1 = new QFormLayout(this);
    lTuner->addLayout(fl1,1,0);

    label = new QLabel();
    lineEdit = new QLineEdit();
    fl1 -> addRow(label,lineEdit);
    
    label -> setText("Threshold");

    connect(setThresholdButton, SIGNAL(clicked()), this, SLOT(setNewThreshold()));
}

void AssemblyThresholdTuner::setNewThreshold()
{
  
      emit setNewThreshold((lineEdit -> text()).toInt());
      /*try   //Is exceptions possible in Qt?
    {
    }
  catch (const std::exception& e)
    {
      NQLog("AssemblyThresholdTuner") << " : ERROR! : incorrect threshold value.";
    }*/
}

void AssemblyThresholdTuner::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
    NQLog("AssemblyThresholdTuner") << ":connectImageProducer";

    imageView_->connectImageProducer(sender, signal);

    connect(sender, signal,
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyThresholdTuner::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
    NQLog("AssemblyThresholdTuner") << ":disconnectImageProducer";

    imageView_->disconnectImageProducer(sender, signal);

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

void AssemblyThresholdTuner::imageAcquired(const cv::Mat& newImage)
{
    newImage.copyTo(image_);
}

void AssemblyThresholdTuner::keyReleaseEvent(QKeyEvent * event)
{
    if (!(event->modifiers() & Qt::ShiftModifier)) {
        switch (event->key()) {
        case Qt::Key_0:
            imageView_->setZoomFactor(0.25);
            event->accept();
            break;
        case Qt::Key_1:
            imageView_->setZoomFactor(1.00);
            event->accept();
            break;
        case Qt::Key_Plus:
            imageView_->increaseZoomFactor();
            event->accept();
            break;
        case Qt::Key_Minus:
            imageView_->decreaseZoomFactor();
            event->accept();
            break;
        default:
            break;
        }
    }
}

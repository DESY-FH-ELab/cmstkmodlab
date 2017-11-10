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

#include <AssemblyThresholdTuner.h>
#include <nqlogger.h>

#include <QFileDialog>
#include <QPushButton>

AssemblyThresholdTuner::AssemblyThresholdTuner(QWidget* parent) :
  QWidget(parent),

  imageView_1_(0),
  imageView_2_(0),

  scrollArea_1_(0),
  scrollArea_2_(0),

  thre_button_(0),
  thre_label_(0),
  thre_linee_(0)
{
    QGridLayout *l = new QGridLayout();
    this->setLayout(l);  

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_1_ = new AssemblyUEyeView();
    imageView_1_->setMinimumSize(500, 300);
    imageView_1_->setPalette(palette);
    imageView_1_->setBackgroundRole(QPalette::Background);
    imageView_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_1_->setScaledContents(true);
    imageView_1_->setAlignment(Qt::AlignCenter);

    imageView_2_ = new AssemblyUEyeView();
    imageView_2_->setMinimumSize(500, 300);
    imageView_2_->setPalette(palette);
    imageView_2_->setBackgroundRole(QPalette::Background);
    imageView_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_2_->setScaledContents(true);
    imageView_2_->setAlignment(Qt::AlignCenter);

    scrollArea_1_ = new QScrollArea(this);
    scrollArea_1_->setMinimumSize(500, 300);
    scrollArea_1_->setPalette(palette);
    scrollArea_1_->setBackgroundRole(QPalette::Background);
    scrollArea_1_->setAlignment(Qt::AlignCenter);
    scrollArea_1_->setWidget(imageView_1_);
    scrollArea_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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

    QWidget* Tuner = new QWidget();
    l->addWidget(Tuner, 0, 1);

    QGridLayout *lTuner = new QGridLayout();
    Tuner->setLayout(lTuner);

    thre_button_ = new QPushButton("Threshold Image", this);
    thre_button_->setEnabled(true);
    lTuner->addWidget(thre_button_, 0, 0);

    QFormLayout *fl1 = new QFormLayout();
    lTuner->addLayout(fl1, 2, 0);

    thre_label_ = new QLabel();
    thre_label_->setText("Threshold");

    thre_linee_ = new QLineEdit();
    thre_linee_->setText("200");

    fl1->addRow(thre_label_, thre_linee_);

    connect(thre_button_, SIGNAL(clicked()), this, SLOT(read_threshold()));
}

int AssemblyThresholdTuner::get_threshold() const
{
    return thre_linee_->text().toInt();
}

void AssemblyThresholdTuner::read_threshold()
{
    const int val = this->get_threshold();

    NQLog("AssemblyThresholdTuner", NQLog::Debug) << "update_threshold"
       << ": emitting signal \"threshold_value(" << val << ")\"";

    emit threshold_value(val);
}

void AssemblyThresholdTuner::connectImageProducer_1(const QObject* sender, const char* signal)
{
    NQLog("AssemblyThresholdTuner", NQLog::Debug) << "connectImageProducer";

    imageView_1_->connectImageProducer(sender, signal);
}

void AssemblyThresholdTuner::disconnectImageProducer_1(const QObject* sender, const char* signal)
{
    NQLog("AssemblyThresholdTuner", NQLog::Debug) << "disconnectImageProducer";

    imageView_1_->disconnectImageProducer(sender, signal);
}

void AssemblyThresholdTuner::connectImageProducer_2(const QObject* sender, const char* signal)
{
    NQLog("AssemblyThresholdTuner", NQLog::Debug) << "connectImageProducer";

    imageView_2_->connectImageProducer(sender, signal);
}

void AssemblyThresholdTuner::disconnectImageProducer_2(const QObject* sender, const char* signal)
{
    NQLog("AssemblyThresholdTuner", NQLog::Debug) << "disconnectImageProducer";

    imageView_2_->disconnectImageProducer(sender, signal);
}

void AssemblyThresholdTuner::keyReleaseEvent(QKeyEvent* event)
{
    if(!(event->modifiers() & Qt::ShiftModifier))
    {
        switch (event->key()) {
        case Qt::Key_0:
	  //            imageView_1_->setZoomFactor(0.25);
          //  imageView_2_->setZoomFactor(0.25);
            event->accept();
            break;
        case Qt::Key_1:
	  //            imageView_1_->setZoomFactor(1.00);
          //  imageView_2_->setZoomFactor(1.00);
            event->accept();
            break;
        case Qt::Key_Plus:
	  // imageView_1_->increaseZoomFactor();
	  // imageView_2_->increaseZoomFactor();
            event->accept();
            break;
        case Qt::Key_Minus:
	  //imageView_1_->decreaseZoomFactor();
	  // imageView_2_->decreaseZoomFactor();
            event->accept();
            break;
        default:
            break;
        }
    }
}

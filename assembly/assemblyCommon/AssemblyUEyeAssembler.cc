#include <vector>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>

#include <nqlogger.h>

#include "AssemblyUEyeAssembler.h"

AssemblyUEyeAssembler::AssemblyUEyeAssembler(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *l = new QFormLayout(this);
    setLayout(l);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_ = new AssemblyUEyeView();
    imageView_->setMinimumSize(400,400);
    imageView_->setPalette(palette);
    imageView_->setBackgroundRole(QPalette::Background);
    imageView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_->setScaledContents(true);
    imageView_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setMinimumSize(200, 200);
    scrollArea_->setPalette(palette);
    scrollArea_->setBackgroundRole(QPalette::Background);
    scrollArea_->setAlignment(Qt::AlignCenter);
    scrollArea_->setWidget(imageView_);
    scrollArea_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    l->addWidget(scrollArea_);
    
    
    
    AssemblyUEyeAssembleCommander * cmdr1 = new AssemblyUEyeAssembleCommander(this, "Go to pickup station (pickup position)", 100.00,100.0,100.0,100.0, 0);
    l->addWidget(cmdr1);
    AssemblyUEyeAssembleCommander * cmdr2 = new AssemblyUEyeAssembleCommander(this, "Drop and attach (drop distance)", 100.00,100.0,100.0,100.0, 0);
    l->addWidget(cmdr2);
    AssemblyUEyeAssembleCommander * cmdr3 = new AssemblyUEyeAssembleCommander(this, "Go to rotation stage (RS position)", 100.00,100.0,100.0,100.0, 0);
    l->addWidget(cmdr3);
    AssemblyUEyeAssembleCommander * cmdr4 = new AssemblyUEyeAssembleCommander(this, "Drop and detach (drop distance)", 100.00,100.0,100.0,100.0, 0);
    l->addWidget(cmdr4);
    AssemblyUEyeAssembleCommander * cmdr5 = new AssemblyUEyeAssembleCommander(this, "Align (target orientation)", 100.00,100.0,100.0,100.0, 0);
    l->addWidget(cmdr5);
    
  /*  QPushButton * button1 = new QPushButton("Go to pickup station (pickup position)", this);
    QLineEdit *lineEdit1 = new QLineEdit();
    lineEdit1->setText("100.0,100.0,100.0");
    l->addRow(button1,lineEdit1);

    QPushButton * button2 = new QPushButton("Drop and attach (drop distance)", this);
    QLineEdit *lineEdit2 = new QLineEdit();
    lineEdit2->setText("10.0");
    l->addRow(button2,lineEdit2);
    
    QPushButton * button3 = new QPushButton("Go to rotation stage (RS position)", this);
    QLineEdit *lineEdit3 = new QLineEdit();
    lineEdit3->setText("0.0,0.0,0.0");
    l->addRow(button3,lineEdit3);
    
    QPushButton * button4 = new QPushButton("Drop and detach (drop distance)", this);
    QLineEdit *lineEdit4 = new QLineEdit();
    lineEdit4->setText("10.0");
    l->addRow(button4,lineEdit4);
    
    QPushButton * button5 = new QPushButton("Align (target orientation)", this);
    QLineEdit *lineEdit5 = new QLineEdit();
    lineEdit5->setText("10.0");
    l->addRow(button5,lineEdit5);
    
    connect(button1, SIGNAL(clicked()), this, SLOT(gotoPickup()));

   */
    
    cv::Mat img_gs = cv::imread("/Users/keaveney/Desktop/calibration/im_scan___Exp10___EdgeThr145___lt110.png", CV_LOAD_IMAGE_COLOR);

    imageView_->setImage(img_gs);
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


void AssemblyUEyeAssembler::gotoPickup()
{
    NQLog("AssemblyUEyeAssembler") << ":gotoPickup()";
    //emit updateStatus("Moving to pickup area",20.0);
    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressSettings* lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    
    lStepExpressModel_->initialize();
    //emit moveAbsolute(0.0,0.0,0.0,0.0);
    
    
    

}




void AssemblyUEyeAssembler::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
    NQLog("AssemblyUEyeAssembler") << ":connectImageProducer";

    imageView_->connectImageProducer(sender, signal);

    connect(sender, signal,
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeAssembler::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
    NQLog("AssemblyUEyeAssembler") << ":disconnectImageProducer";

    imageView_->disconnectImageProducer(sender, signal);

    disconnect(sender, signal,
               this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeAssembler::snapShot()
{
    // NQLog("AssemblyUEyeAssembler") << ":snapShot()";

    if (image_.rows==0) return;

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
    if (filename.isNull() || filename.isEmpty()) return;

    if (!filename.endsWith(".png")) filename += ".png";

    cv::imwrite(filename.toStdString(), image_);
}

void AssemblyUEyeAssembler::imageAcquired(const cv::Mat& newImage)
{
    NQLog("AssemblyUEyeAssembler") << ":imageAcquired";

    newImage.copyTo(image_);
}

void AssemblyUEyeAssembler::keyReleaseEvent(QKeyEvent * event)
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



AssemblyUEyeAssembleCommander::AssemblyUEyeAssembleCommander(QWidget *parent, std::string string, double x ,double y, double z,double a,  int mode)
: QWidget(parent)
{
     QFormLayout *l = new QFormLayout(this);
     setLayout(l);

    std::ostringstream strs;
    strs.clear();
    strs << x;
    strs << ",";
    strs << y;
    strs << ",";
    strs << z;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);

    QPushButton * button1 = new QPushButton(qname, this);
    QLineEdit *lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);
 //   l->addWidget(lineEdit1);

    
    
}




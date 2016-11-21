#include <vector>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <iomanip>

#include <TGraph.h>
#include <TF1.h>
#include <TAxis.h>
#include <TCanvas.h>


#include <QApplication>

#include <iostream>
#include <sstream>
#include <nqlogger.h>

#include "AssemblyModuleAssembler.h"

//relay card
#include "../../devices/Conrad/ConradCommunication.h"
#include "../../devices/Conrad/ConradController.h"

using namespace std;
using namespace cv;


AssemblyModuleAssembler::AssemblyModuleAssembler(QWidget *parent)
    : QWidget(parent)
{
    
    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    lStepExpressModel_->initialize();

    
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);

    QGridLayout *g0 = new QGridLayout(this);
    l->addLayout(g0,0,0);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));
    
    imageView_1 = new AssemblyUEyeView();
    imageView_1->setMinimumSize(200,200);
    imageView_1->setPalette(palette);
    imageView_1->setBackgroundRole(QPalette::Background);
    imageView_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_1->setScaledContents(true);
    imageView_1->setAlignment(Qt::AlignCenter);
    QApplication::processEvents();
    
    scrollArea_1 = new QScrollArea(this);
    scrollArea_1->setMinimumSize(200, 200);
    scrollArea_1->setPalette(palette);
    scrollArea_1->setBackgroundRole(QPalette::Background);
    scrollArea_1->setAlignment(Qt::AlignCenter);
    scrollArea_1->setWidget(imageView_1);
    scrollArea_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    g0->addWidget(scrollArea_1,0,0);
    lE1 = new QLineEdit("Pickup pos. = X,X,X");
    g0->addWidget(lE1,1,0);

    
    imageView_2 = new AssemblyUEyeView();
    imageView_2->setMinimumSize(200,200);
    imageView_2->setPalette(palette);
    imageView_2->setBackgroundRole(QPalette::Background);
    imageView_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_2->setScaledContents(true);
    imageView_2->setAlignment(Qt::AlignCenter);
    
    scrollArea_2 = new QScrollArea(this);
    scrollArea_2->setMinimumSize(200, 200);
    scrollArea_2->setPalette(palette);
    scrollArea_2->setBackgroundRole(QPalette::Background);
    scrollArea_2->setAlignment(Qt::AlignCenter);
    scrollArea_2->setWidget(imageView_2);
    scrollArea_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    g0->addWidget(scrollArea_2,0,1);
    lE2 = new QLineEdit("Mntd pos. (cor. 1) = X,X,X");
    g0->addWidget(lE2,1,1);
    
    
    imageView_3 = new AssemblyUEyeView();
    imageView_3->setMinimumSize(200,200);
    imageView_3->setPalette(palette);
    imageView_3->setBackgroundRole(QPalette::Background);
    imageView_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_3->setScaledContents(true);
    imageView_3->setAlignment(Qt::AlignCenter);

    
    scrollArea_3 = new QScrollArea(this);
    scrollArea_3->setMinimumSize(200, 200);
    scrollArea_3->setPalette(palette);
    scrollArea_3->setBackgroundRole(QPalette::Background);
    scrollArea_3->setAlignment(Qt::AlignCenter);
    scrollArea_3->setWidget(imageView_3);
    scrollArea_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    
    g0->addWidget(scrollArea_3,0,2);
    lE3 = new QLineEdit("Mntd pos. (cor. 2) = X,X,X");
    g0->addWidget(lE3,1,2);
    
    
    imageView_4 = new AssemblyUEyeView();
    imageView_4->setMinimumSize(200,200);
    imageView_4->setPalette(palette);
    imageView_4->setBackgroundRole(QPalette::Background);
    imageView_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_4->setScaledContents(true);
    imageView_4->setAlignment(Qt::AlignCenter);
    
    scrollArea_4 = new QScrollArea(this);
    scrollArea_4->setMinimumSize(200, 200);
    scrollArea_4->setPalette(palette);
    scrollArea_4->setBackgroundRole(QPalette::Background);
    scrollArea_4->setAlignment(Qt::AlignCenter);
    scrollArea_4->setWidget(imageView_4);
    scrollArea_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    g0->addWidget(scrollArea_4,2,0);
    lE4 = new QLineEdit("Dtchd pos. = X,X,X");
    g0->addWidget(lE4,3,0);
    
    
    imageView_5 = new AssemblyUEyeView();
    imageView_5->setMinimumSize(200,200);
    imageView_5->setPalette(palette);
    imageView_5->setBackgroundRole(QPalette::Background);
    imageView_5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_5->setScaledContents(true);
    imageView_5->setAlignment(Qt::AlignCenter);
    
    scrollArea_5 = new QScrollArea(this);
    scrollArea_5->setMinimumSize(200, 200);
    scrollArea_5->setPalette(palette);
    scrollArea_5->setBackgroundRole(QPalette::Background);
    scrollArea_5->setAlignment(Qt::AlignCenter);
    scrollArea_5->setWidget(imageView_5);
    scrollArea_5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    g0->addWidget(scrollArea_5,2,1);
    lE5 = new QLineEdit("Mntd pos. (cor. 4) = X,X,X");
    g0->addWidget(lE5,3,1);

    
    imageView_6 = new AssemblyUEyeView();
    imageView_6->setMinimumSize(200,200);
    imageView_6->setPalette(palette);
    imageView_6->setBackgroundRole(QPalette::Background);
    imageView_6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_6->setScaledContents(true);
    imageView_6->setAlignment(Qt::AlignCenter);
    
    scrollArea_6 = new QScrollArea(this);
    scrollArea_6->setMinimumSize(200, 200);
    scrollArea_6->setPalette(palette);
    scrollArea_6->setBackgroundRole(QPalette::Background);
    scrollArea_6->setAlignment(Qt::AlignCenter);
    scrollArea_6->setWidget(imageView_6);
    scrollArea_6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
 
    g0->addWidget(scrollArea_6,2,2);
    lE6 = new QLineEdit("Mntd pos. (cor. 3) = X,X,X");
    g0->addWidget(lE6,3,2);
    
    
    QGridLayout *g1 = new QGridLayout(this);
    l->addLayout(g1,0,1);
    
    QLabel* header_label = new QLabel("Hover mouse over button for info on each step", this);
    g1->addWidget(header_label,0,0);

    AssemblyCommander * cmdr0 = new AssemblyCommander(this, "Go to origin", 0.0,0.0,0.0,0.0);
    cmdr0->setToolTip("(1) Returns x,y,z stage to origin (default = (0,0,0) using moveAbsolute(x,y,z) routine)");
    g1->addWidget(cmdr0,1,0);

    AssemblyCommander * cmdr1 = new AssemblyCommander(this, "Go to pickup", 100.0,100.0,100.0,100.0);
    cmdr1->setToolTip("(2) Moves x,y,z stage to pickup position (default = (100,100,100) using moveAbsolute(x,y,z) routine)");
    g1->addWidget(cmdr1,2,0);
    
    AssemblySensorLocator * lctr1 = new AssemblySensorLocator(this, "Locate sensor", 0.0);
    lctr1->setToolTip("(3) Acquires image from mobile camera, runs PatRec routine to deduce and report sensor (x,y,z,phi) position");
    g1->addWidget(lctr1,3,0);
    
    AssemblyZPositionIdentifier * zpid1 = new AssemblyZPositionIdentifier(this, "Identify Z-pos", 1.0, 10);
    zpid1->setToolTip("(3b) Identifies Z-position of the module using image scan by the mobile camera within the given range with a pre-defined step size, runs blur routine to report sensor (x,y,z) position");
    g1->addWidget(zpid1,3,1);
    
 
    AssemblyCommander * cmdr2 = new AssemblyCommander(this, "Correct position", 100.0,100.0,100.0,100.0);
    cmdr2->setToolTip("(4) Corrects arm position using relative displacement using (eventually) pre-calculated displacment from PatRec");
    g1->addWidget(cmdr2,4,0);
    
    
    AssemblyAttacher * attacher1 = new AssemblyAttacher("Drop/Raise", 10.0);
    g1->addWidget(attacher1,5,0);
    
    AssemblyVacuumToggler * toggle1 = new AssemblyVacuumToggler(this, "Toggle vacuum", 0.0);
    g1->addWidget(toggle1,6,0);
    
    AssemblyCommander * cmdr4 = new AssemblyCommander(this, "Go to stat. camera", 100.0,100.0,100.0,100.0);
    g1->addWidget(cmdr4, 7, 0);

    AssemblyMountChecker * cmdr5 = new AssemblyMountChecker(this, "Check mount", 100.0,100.0,100.0,100.0, 0);
    g1->addWidget(cmdr5, 8, 0);
    
    
    AssemblyCommander * cmdr6 = new AssemblyCommander(this, "Go to rotation stage", 100.0,100.0,100.0,100.0);
    g1->addWidget(cmdr6, 9, 0);
    
    
    AssemblyCommander * cmdr7 = new AssemblyCommander(this, "Drop and detach", 100.0,100.0,100.0,100.0);
    g1->addWidget(cmdr7, 10, 0);
    
    
    AssemblyAligner * cmdr8 = new AssemblyAligner(this, "Align", 0.0);
    g1->addWidget(cmdr8, 11, 0);
    
    
    //make all the neccessary connections
   
    connect(attacher1, SIGNAL(moveRelative(double,double,double,double)),
            motionManager_, SLOT(moveRelative(double,double,double,double)));
    connect(cmdr0, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(cmdr1, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(cmdr2, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveRelative(double,double,double,double)));
    connect(cmdr4, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(cmdr5, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(cmdr6, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(cmdr7, SIGNAL(moveAbsolute(double,double,double,double)),
            motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(cmdr8, SIGNAL(moveRelative(double,double,double,double)),
            motionManager_, SLOT(moveRelative(double,double,double,double)));
    connect(cmdr8, SIGNAL(locateSetdowncorner(int)), lctr1, SLOT( locateSensor(int)));
    
    connect(lctr1, SIGNAL(sendPosition(int, double,double,double)), this, SLOT(updateText(int,double,double,double)));
    
    connect(lctr1, SIGNAL(updateImage(int, std::string)), this, SLOT( updateImage(int,std::string)));
    connect(lctr1, SIGNAL(foundSensor(int)), lctr1, SLOT( foundsensor(int)));
    
    connect(zpid1, SIGNAL(moveRelative(double,double,double,double)),
            motionManager_, SLOT(moveRelative(double,double,double,double)));
    connect(zpid1, SIGNAL(updateImage(int, std::string)), this, SLOT( updateImage(int,std::string)));

    

}

void AssemblyModuleAssembler::updateText(int stage, double x, double y , double a){
    
    NQLog("AssemblyCommander::updateText");
    
    
    std::ostringstream strs;
    strs.clear();
    strs << x;
    strs << ",";
    strs << y;
    strs << ",";
    strs << a;
    std::string str = strs.str();
    QString qstr = "";
    qstr = QString::fromStdString(str);
    // QString qname = QString::fromStdString(string);
    
    if(stage == 1 ){
        qstr = "Pickup pos. = " + qstr;
        lE1->setText(qstr);
    } else if (stage==2){
        qstr = "Mntd. pos. (cor.1) = " + qstr;
        lE2->setText(qstr);
    }
    else if (stage==3){
        qstr = "Mntd. pos. (cor.2) = " + qstr;
        lE3->setText(qstr);
    }
    else if (stage==4){
        qstr = "Mntd. pos. (cor.3) = " + qstr;
        lE6->setText(qstr);
    }else if (stage==5){
        qstr = "Mntd. pos. (cor.4) = " + qstr;
        lE5->setText(qstr);
    }
    else if (stage==6){
        qstr = "Dtchd. pos. = " + qstr;
        lE4->setText(qstr);
    }
    
    
}



void AssemblyModuleAssembler::updateImage(int stage, std::string filename)
{

    NQLog("AssemblyModuleAssembler") << ":updateImage()  " + filename;

    
    //cv::Mat img_gs = cv::imread("/Users/keaveney/Desktop/calibration/PatRec_result.png", CV_LOAD_IMAGE_COLOR);
    
    cv::Mat img_gs = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
    
    if (stage == 1 ){
   imageView_1->setZoomFactor(0.2);
    imageView_1->setImage(img_gs);

    }else if (stage ==2){
        imageView_2->setImage(img_gs);
       imageView_2->setZoomFactor(0.75);
    }
    else if (stage ==3){
        imageView_3->setImage(img_gs);
    }
    else if (stage ==4){
        imageView_6->setImage(img_gs);
    }
    else if (stage ==5){
        imageView_5->setImage(img_gs);
    }
    else if (stage ==6){
        imageView_4->setImage(img_gs);
    }

    
}


void AssemblyModuleAssembler::gotoPickup()
{
    NQLog("AssemblyModuleAssembler") << ":gotoPickup()";
    connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));
}



void AssemblyModuleAssembler::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
    NQLog("AssemblyModuleAssembler") << ":connectImageProducer";

    imageView_1->connectImageProducer(sender, signal);

    connect(sender, signal,
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyModuleAssembler::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
    NQLog("AssemblyModuleAssembler") << ":disconnectImageProducer";

    imageView_1->disconnectImageProducer(sender, signal);

    disconnect(sender, signal,
               this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyModuleAssembler::snapShot()
{
    // NQLog("AssemblyModuleAssembler") << ":snapShot()";

    if (image_.rows==0) return;

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
    if (filename.isNull() || filename.isEmpty()) return;

    if (!filename.endsWith(".png")) filename += ".png";

    cv::imwrite(filename.toStdString(), image_);
}

void AssemblyModuleAssembler::imageAcquired(const cv::Mat& newImage)
{
    NQLog("AssemblyModuleAssembler") << ":imageAcquired";

    newImage.copyTo(image_);
}

void AssemblyModuleAssembler::keyReleaseEvent(QKeyEvent * event)
{
    if (!(event->modifiers() & Qt::ShiftModifier)) {
        switch (event->key()) {
        case Qt::Key_0:
            imageView_1->setZoomFactor(0.25);
            event->accept();
            break;
        case Qt::Key_1:
            imageView_1->setZoomFactor(1.00);
            event->accept();
            break;
        case Qt::Key_Plus:
            imageView_1->increaseZoomFactor();
            event->accept();
            break;
        case Qt::Key_Minus:
            imageView_1->decreaseZoomFactor();
            event->accept();
            break;
        default:
            break;
        }
    }
}



AssemblyVacuumToggler::AssemblyVacuumToggler(QWidget *parent, std::string string, double a)
: QWidget(parent),local_a(a)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    const char* deviceName = "/dev/ttyUSB0";
    cnrd1 = new ConradController(deviceName);
    cnrd1->initialize();

    std::ostringstream strs;
    strs.clear();
    strs << a;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    state = false;
    
    button1 = new QPushButton(qname, this);
    l->addWidget(button1,0,0);
    
    ql = new QLabel("", this);
    l->addWidget(ql,0,1);
    
    QPixmap pixmap(100,100);
    pixmap.fill(QColor("transparent"));
    
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(0, 0, 30, 30);
    
    ql->setPixmap(pixmap);
    ql->setText("VACUUM OFF");
    ql->setStyleSheet("QLabel { background-color : green; color : black; }");
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(toggleVacuum()));
    
}


void AssemblyVacuumToggler::toggleVacuum()
{
    
    NQLog("AssemblyVacuumToggler") << ": toggling vacuum voltage";
    
    if (!state){
    cnrd1->setChannel(1, true);

    ql->setText("VACUUM ON");
    ql->setStyleSheet("QLabel { background-color : red; color : black; }");
    state = true;
    }else if (state){
    cnrd1->setChannel(1, false);

    ql->setText("VACUUM OFF");
    ql->setStyleSheet("QLabel { background-color : green; color : black; }");
    state = false;
    }        
}


AssemblyAttacher::AssemblyAttacher(std::string string, double drop)
: local_drop(drop)
{

    
    QFormLayout *ll = new QFormLayout(this);
    setLayout(ll);
    

    std::ostringstream strs;
    strs.clear();
    strs << drop;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);

    
    button1 = new QPushButton(qname, this);
    
    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    ll->addRow(button1,lineEdit1);
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(dropAttach()));

}



void AssemblyAttacher::dropAttach(){
    
    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lineEdit1->text();

    double d_d = parent_string.toDouble();
    
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<" requesting move... ";
    
    emit moveRelative(0.0,0.0,-d_d, 0.0);
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<"move requested ";
    
    
}


AssemblyMountChecker::AssemblyMountChecker(QWidget *parent, std::string string, double x ,double y, double z,double a,  int mode)
: QWidget(parent), local_x(x), local_y(y),local_z(z),local_a(a)
{
    QFormLayout *l = new QFormLayout(this);
    setLayout(l);
    
    NQLog("AssemblyMountChecker") << ": in mode" << mode;
    
    
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
    
    this->local_x = x;
    this->local_y = y;
    this->local_z = z;
    this->local_a = a;
    
    button1 = new QPushButton(qname, this);
    
    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(checkMount()));
    
}


void AssemblyMountChecker::checkMount(){
    

    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lineEdit1->text();
    
    QStringList pieces = parent_string.split( "," );
    QString x = pieces.value( pieces.length() - 3);
    QString y = pieces.value( pieces.length() - 2);
    QString z = pieces.value( pieces.length() - 1);
    QString a = pieces.value( pieces.length() - 1);
    
    double x_d = x.toDouble();
    double y_d = y.toDouble();
    double z_d = z.toDouble();
    double a_d = a.toDouble();
    
    
    NQLog("AssemblyMountChecker") << ": going to target (parsed) "<< x_d<<" "<< y_d<<"  "<< z_d;
    
    emit moveAbsolute(x_d, y_d, z_d, a_d);// this should bring you to the first corner
    emit locateCorner(2);
    emit reportCornerLocation(1);
    emit moveRelative(0.0, 10.0, 0.0, 0.0);//move to next corner
    emit locateCorner(3);
    emit reportCornerLocation(2);
    emit moveRelative(5.0, 0.0, 0.0, 0.0);//move to next corner
    emit locateCorner(4);
    emit reportCornerLocation(3);
    emit moveRelative(0.0, -10.0, 0.0, 0.0);//move to next corner
    emit locateCorner(5);
    emit reportCornerLocation(4);
    
}




AssemblyCommander::AssemblyCommander(QWidget *parent, std::string string, double x ,double y, double z,double a)
: QWidget(parent), local_x(x), local_y(y),local_z(z),local_a(a)
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
    
    this->local_x = x;
    this->local_y = y;
    this->local_z = z;
    this->local_a = a;
    
    button1 = new QPushButton(qname, this);

    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);

    connect(button1, SIGNAL(clicked()),
            this, SLOT(goToTarget()));
    
    
}



void AssemblyCommander::goToTarget(){
    
    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lineEdit1->text();
    
    QStringList pieces = parent_string.split( "," );
    QString x = pieces.value( pieces.length() - 3);
    QString y = pieces.value( pieces.length() - 2);
    QString z = pieces.value( pieces.length() - 1);
    QString a = pieces.value( pieces.length() - 1);
    
    double x_d = x.toDouble();
    double y_d = y.toDouble();
    double z_d = z.toDouble();
    double a_d = a.toDouble();

    
    NQLog("AssemblyCommander:goToTarget") << ": going to target (parsed) "<< x_d<<" "<< y_d<<"  "<< z_d;
    
    NQLog("AssemblyCommander:goToTarget") <<" requesting move...";
    
    emit moveAbsolute(x_d, y_d, z_d, a_d);
    
    NQLog("AssemblyCommander:goToTarget") <<"move requested...";
    
}


AssemblyAligner::AssemblyAligner(QWidget *parent, std::string string, double a)
: QWidget(parent),local_a(a)
{
    QFormLayout *l = new QFormLayout(this);
    setLayout(l);
    
    std::ostringstream strs;
    strs.clear();
    strs << a;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    button1 = new QPushButton(qname, this);
    
    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);
    
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(setDown()));

}


void AssemblyAligner::setDown(){
    
    emit locateSetdowncorner(6);
    
}



void AssemblyAligner::align(){
    
//     emit locate(6);
    
    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lineEdit1->text();
    
  //  QStringList pieces = parent_string.split( "," );
  //  QString x = pieces.value( pieces.length() - 3);
  //  QString y = pieces.value( pieces.length() - 2);
  //  QString z = pieces.value( pieces.length() - 1);
  //  QString a = pieces.value( pieces.length() - 1);
    
   // double x_d = x.toDouble();
   // double y_d = y.toDouble();
   // double z_d = z.toDouble();
    double a_d = parent_string.toDouble();
    
    
    NQLog("AssemblyAligner:align") << ": going to target alignment (parsed) "<< a_d<<" ";
    
    emit moveRelative(0.0, 0.0, 0.0, a_d);
    
    
}




AssemblySensorLocator::AssemblySensorLocator(QWidget *parent, std::string string, double a)
: QWidget(parent),local_a(a)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    std::ostringstream strs;
    strs.clear();
    strs << a;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    button1 = new QPushButton(qname, this);
    
    l->addWidget(button1,0,0);
    
    groupBox = new QGroupBox(tr("Pattern Recognition Method"));
    
    radio1 = new QRadioButton(tr("&Circle Seed Algorithm"));
    radio2 = new QRadioButton(tr("T&emplate Matching"));
    radio2->setChecked(true);
    
    vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    
    l->addWidget(groupBox,1,0);


    ql = new QLabel("", this);
    l->addWidget(ql,0,1);
    
    QPixmap pixmap(100,100);
    pixmap.fill(QColor("transparent"));
    
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(0, 0, 30, 30);
    
    ql->setPixmap(pixmap);
    ql->setText("WAITING");
    ql->setStyleSheet("QLabel { background-color : orange; color : black; }");
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(locatePickup()));
    connect(this, SIGNAL(locatePickupCorner_circleSeed(int)),
            this, SLOT(locateSensor_circleSeed(int)));
    connect(this, SIGNAL(locatePickupCorner_templateMatching(int)),
            this, SLOT(locateSensor_templateMatching(int)));

}

void AssemblySensorLocator::locatePickup(){

    if (radio1->isChecked()) {
        NQLog("AssemblySensorLocator") << "Circle seed selected" ;
        emit locatePickupCorner_circleSeed(1);

    }else if (radio2->isChecked() ){
        NQLog("AssemblySensorLocator") << "Template matching selected" ;

        emit locatePickupCorner_templateMatching(1);

    }
    
}

void AssemblySensorLocator::locateSensor_templateMatching(int stage){

    NQLog("AssemblySensorLocator") << "Finding Marker (Template Matching)" ;
    cv::Mat img, img_clip_A, img_clip_B, result_1, result_2, dst;
    int match_method;
    
    img = cv::imread("/Users/keaveney/Desktop/calibration/RawSensor_4.png", CV_LOAD_IMAGE_COLOR);
    img_clip_A = cv::imread("/Users/keaveney/Desktop/calibration/RawSensor_3_clipA.png", CV_LOAD_IMAGE_COLOR);
    img_clip_B = cv::imread("/Users/keaveney/Desktop/calibration/RawSensor_3_clipB.png", CV_LOAD_IMAGE_COLOR);

    Point matchLoc_1, matchLoc_2, matchLoc_final;
   
    Mat img_copy = img.clone();
    
    
    //Greyscale images
    Mat img_copy_gs(img_copy.size(), img_copy.type());
    Mat img_clip_A_gs(img_clip_A.size(), img_clip_A.type());
    Mat img_clip_B_gs(img_clip_B.size(), img_clip_B.type());

    //convert color to GS
    cvtColor( img_copy,   img_copy_gs,   CV_BGR2GRAY );
    cvtColor( img_clip_A, img_clip_A_gs, CV_BGR2GRAY );
    cvtColor( img_clip_B, img_clip_B_gs, CV_BGR2GRAY );

    
    //Binary images
    Mat img_copy_bin(img_copy_gs.size(), img_copy_gs.type());
    Mat img_clip_A_bin(img_clip_A_gs.size(), img_clip_A_gs.type());
    Mat img_clip_B_bin(img_clip_B_gs.size(), img_clip_B_gs.type());

    
    
    //Apply thresholding
    cv::threshold(img_copy_gs, img_copy_bin, 60, 255, cv::THRESH_BINARY);
    cv::threshold(img_clip_A_gs, img_clip_A_bin, 90, 255, cv::THRESH_BINARY);
    cv::threshold(img_clip_B_gs, img_clip_B_bin, 90, 255, cv::THRESH_BINARY);
    
    
   // img_copy_bin = img_copy_gs.clone();
   // img_clip_A_bin = img_clip_A_gs.clone();
   // img_clip_B_bin = img_clip_B_gs.clone();
    
    
    std::string filename_img_bin = "/Users/keaveney/Desktop/calibration/Sensor_bin.png";
    std::string filename_clip_A_bin = "/Users/keaveney/Desktop/calibration/clip_A_bin.png";
    std::string filename_clip_B_bin = "/Users/keaveney/Desktop/calibration/clip_B_bin.png";

    cv::imwrite(filename_img_bin, img_copy_bin);
    cv::imwrite(filename_clip_A_bin, img_clip_A_bin);
    cv::imwrite(filename_clip_B_bin, img_clip_B_bin);
    
    emit updateImage(4, filename_img_bin);
    emit updateImage(5, filename_clip_A_bin);
    emit updateImage(6, filename_clip_B_bin);

    
    
    //GaussianBlur( img_gs_copy, img_gs_copy, Size( 51, 51 ), 0, 0 );
    //GaussianBlur( img_clip_gs, img_clip_gs, Size( 51, 51 ), 0, 0 );
    //GaussianBlur( img_clip_gs_2, img_clip_gs_2, Size( 51, 51 ), 0, 0 );

    
    /// Localizing the best match with minMaxLoc
    double FOM, FOM_inc = 1000.0, minVal, maxVal; Point minLoc; Point maxLoc;
    double thetas[40], FOMs[40];
    
    //first find the (x,y) location of the circle within the corner marker
    
    //create result matrix to hold correlation values
    int result_cols =  img_copy_bin.cols - img_clip_A.cols + 1;
    int result_rows = img_copy_bin.rows - img_clip_A.rows + 1;
    
    result_1.create( result_rows, result_cols, CV_32FC1 );
    
    match_method =1;
    matchTemplate( img_copy_bin, img_clip_A_bin, result_1, match_method);
    
    minMaxLoc( result_1, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    {
        matchLoc_1 = minLoc;
        FOM = minVal;
    }
    else
    {
        matchLoc_1 = maxLoc;
        FOM = maxVal;
      
  
    }
    
    //circle( img_gs, Point( matchLoc_1.x + (img_clip_gs.cols/2.0) , matchLoc_1.y + (img_clip_gs.rows/2.0) ), 10, Scalar(255,0,0) );
    
    circle( img, Point( matchLoc_1.x  , matchLoc_1.y ), 30, Scalar(255,0,0) );
    rectangle( img, matchLoc_1, Point( matchLoc_1.x + img_clip_A_bin.cols , matchLoc_1.y + img_clip_A_bin.rows ), Scalar(255,0,0), 2, 8, 0 );
    
    NQLog("AssemblySensorLocator") <<  matchLoc_1.x<< "   "<< matchLoc_1.y;

    
    int i = 0;
    int color = 200;

    //Optional - currently not preferred.
    //Precisiely find the circle within the marker

    /*
    // define location of sub matrices in image
    Rect roi1( matchLoc_1.x  , matchLoc_1.y, img_clip_A_bin.cols, img_clip_A_bin.rows );

    
    // define sub matrices in main matrix
    Mat img_copy_2 = img_copy_gs.clone();
    Mat sub1( img_copy_2, roi1 );
    
    GaussianBlur( sub1, sub1, Size( 9, 9 ), 4,4 );
    
    vector<Vec3f> circles;
    HoughCircles(sub1, circles, CV_HOUGH_GRADIENT, 2, sub1.rows/2, 20, 40 );
    
    NQLog("AssemblySensorLocator  ") <<  circles.size() << "  circles detected ";

    
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]) + matchLoc_1.x , cvRound(circles[i][1]) + matchLoc_1.y);
        int radius = cvRound(circles[i][2]);
        // draw the circle center
        circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    
    */
    


   for (float theta = -6.0; theta < 6.0;  theta = theta + 0.3){
  //    for (float theta = -180.0; theta < 180.0;  theta = theta + 9.0){

   // Point2f src_center(img_gs_copy.cols/2.0F, img_gs_copy.rows/2.0F);
       Point2f src_center( matchLoc_1.x + (img_clip_A_bin.cols/2) , matchLoc_1.y + (img_clip_A_bin.rows/2) );
     
    Mat rot_mat = getRotationMatrix2D(matchLoc_1, theta, 1.0);
    cv::Scalar avgPixelIntensity = cv::mean( img_copy_bin );
    warpAffine(img_copy_bin, dst, rot_mat, img_copy_bin.size(), cv::INTER_CUBIC,
                   cv::BORDER_CONSTANT, avgPixelIntensity);

    std::string filename_rotated_base = "/Users/keaveney/Desktop/calibration/RotatedImages/Rotation_result_";
    std::ostringstream ss;
    ss << theta;
    std::string theta_str = ss.str();
    std::string filename_rotated = filename_rotated_base + theta_str + ".png";
    cv::imwrite(filename_rotated, dst);
    
    //create result matrix to hold correlation values
    result_cols =  img_copy_bin.cols - img_clip_B_bin.cols + 1;
    result_rows = img_copy_bin.rows - img_clip_B_bin.rows + 1;
        
    result_2.create( result_rows, result_cols, CV_32FC1 );
    matchTemplate( dst, img_clip_B_bin, result_2, match_method );
        
   // normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    minMaxLoc( result_2, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        
   /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    {
        matchLoc_2 = minLoc;
        FOM = minVal;
    }
    else
    {
        matchLoc_2 = maxLoc;
        FOM = maxVal;
    }
        
    std::cout << std::setprecision(10);
//    cout<< theta<<"  ,  " << FOM  <<endl;
     
    thetas[i] = theta;
    FOMs[i] = FOM;
    i++;
       
       if (FOM < FOM_inc ) {
       
           FOM_inc = FOM;
           matchLoc_final = matchLoc_2;
       }
       
       
    color = 50 + 200*theta;
        
       
}
 
    
    rectangle( img, matchLoc_final, Point( matchLoc_final.x + img_clip_B_bin.cols , matchLoc_final.y + img_clip_B_bin.rows ), Scalar(color,color-50,color+50), 2, 8, 0 );
   // rectangle( result_2, matchLoc_2, Point( matchLoc_2.x + img_clip_B_bin.cols , matchLoc_2.y + img_clip_B_bin.rows ), Scalar(color,color-50,color+50), 2, 8, 0 );

  
    TCanvas *c1 = new TCanvas("c1","Rotation extraction",200,10,700,500);


    if (thetas[0] && FOMs[0]){
    TGraph *gr = new TGraph(40, thetas, FOMs);
    gr->Draw("AC*");
    const char * filename_canvas = "/Users/keaveney/Desktop/calibration/RotationExtraction.png";
    c1->SaveAs(filename_canvas);
   
    emit updateImage(2, filename_canvas);
    }
 
    
    
    std::string filename = "/Users/keaveney/Desktop/calibration/PatRec_TM_result.png";
    cv::imwrite(filename, img);
    
    emit updateImage(stage, filename);
    
    
    emit foundSensor(1);


}


void AssemblySensorLocator::locateSensor_circleSeed(int stage){

    NQLog("AssemblySensorLocator") << "Finding Marker (Circle Seed Algorithm)" ;

    cv::Mat img_gs, img_rgb, img_edges;
    
    img_gs = cv::imread("/Users/keaveney/Desktop/calibration/im_scan___Exp10___EdgeThr145___lt110.png", CV_LOAD_IMAGE_COLOR);

    
    cvtColor(img_gs, img_gs , CV_RGB2GRAY);

    
    int radius;
    int  expectedCircleRadius_ = 89;
    double circleCenterDetectionThreshold_  = 35.0;
    double circleEdgeDetectionThreshold_  = 145.0;
    double linesCannyEdgeDetectionThreshold1_ =50;
    double linesCannyEdgeDetectionThreshold2_ =200;
    double linesCannyEdgeDetectionApertureSize_ =3;
    double linesCannyEdgeDetectionL2Gradient_ =1;
    double linesHoughDistanceResolution_ = 1.0;
    //    double linesHoughAngleResolution_*CV_PI/180,
    double linesHoughAngleResolution_ = 0.1;
    double linesHoughThreshold_ = 110.0;
    double linesHoughMinLineLength_=  150.0;
    double linesHoughMaxLineGap_ =25.0;
    bool  dist1_, dist2_, dist3_, doca_ ;
    double slope_final = 0.0, ang_final = 0.0;
    double distance,doca,x0,x1,x2, y0,y1,y2, x,y;
    
    
    std::vector<cv::Vec3f> circles;
    cv::Point2f circleCenter_;

    std::vector<cv::Vec4i> tempLines;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > mergedLines_;
    
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
    std::vector<cv::Point2f> finalIntersections_;
    std::vector<cv::Point2f> finalIntersectionsUp_;
    std::vector<cv::Point2f> finalIntersectionsDown_;
    
    lines_.clear();
    goodLines_.clear();
    intersections_.clear();
    goodIntersections_.clear();
    finalIntersections_.clear();
    
    //detect circles
    cv::HoughCircles(img_gs, circles, CV_HOUGH_GRADIENT, 1, 20,
                     circleEdgeDetectionThreshold_, circleCenterDetectionThreshold_,
                     expectedCircleRadius_ - 15., expectedCircleRadius_ + 15.);
    
    //get centre of first circle...problematic if this is not the right one.
    if (circles.size() != 0){
        circleCenter_.x = circles[0][0];
        circleCenter_.y = circles[0][1];
    }
    

    cvtColor(img_gs, img_rgb , CV_GRAY2RGB);

    /// Draw the circles detected in blue
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        x = cvRound(circles[i][0]);
        y = cvRound(circles[i][1]);
        radius = cvRound(circles[i][2]);
        // circle center
        circle( img_rgb, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
        //circle outline;
        circle( img_rgb, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    }
    
    //detect edges
    cv::Canny(img_gs, img_edges,
              linesCannyEdgeDetectionThreshold1_,
              linesCannyEdgeDetectionThreshold2_,
              linesCannyEdgeDetectionApertureSize_,
              linesCannyEdgeDetectionL2Gradient_);
    
    
    NQLog("AssemblyVUEyeCamera") << "  running hough lines";
    
    
    //detect lines
    cv::HoughLinesP(img_edges, tempLines,
                    linesHoughDistanceResolution_,
                    linesHoughAngleResolution_*CV_PI/180,
                    linesHoughThreshold_,
                    linesHoughMinLineLength_,
                    linesHoughMaxLineGap_);
    
    
    for (size_t i = 0; i < tempLines.size(); i++ ) {
        lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]), cv::Point2f(tempLines[i][2],tempLines[i][3])));
    }
    
    
    //draw the initial collection of lines in red (handy for debugging, monitoring of PatRec)
    for (size_t i = 0; i < lines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(lines_[i].first.x), cvRound(lines_[i].first.y)),
                 cv::Point(cvRound(lines_[i].second.x), cvRound(lines_[i].second.y)),
                 cv::Scalar(255, 0, 0), 2, CV_AA);
    }
    
    
    //select only the lines which have starting/ending points close to the circle centre and DOCA consistent with the L marker outline
    for (size_t i = 0; i < lines_.size(); i++ ) {
        dist1_  = dist2_ = dist3_ = doca_ = false;
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].first.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].first.y, 2.0));
        if (distance < expectedCircleRadius_*6.0) dist1_ = true;
        
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].second.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].second.y, 2.0));
        //  if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);
        if (distance < expectedCircleRadius_*6.0)  dist2_ = true  ;
        
        x0 = circleCenter_.x;
        y0 = circleCenter_.y;
        x1 =  lines_[i].first.x;
        y1 =  lines_[i].first.y;
        x2 =  lines_[i].second.x;
        y2 =  lines_[i].second.y;
        
        doca =  std::fabs( x0*(y2-y1)  -  y0*(x2-x1) + x2*y1 -y2*x1)/ std::sqrt(   std::pow(y2 - y1,2.0)   +    std::pow(x2 - x1,2.0)      );
        
        
        if ((doca <expectedCircleRadius_*5.8  && doca >expectedCircleRadius_*5.4)  || (doca <expectedCircleRadius_*1.3  && doca >expectedCircleRadius_*0.7)   ) doca_ = true;
        
        if (dist1_ && dist2_ && doca_ ) goodLines_.push_back(lines_[i]);
    }
    
    //draw the initial collection of "good" lines in pink (handy for debugging, monitoring of PatRec)

    for (size_t i = 0; i < goodLines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(goodLines_[i].first.x), cvRound(goodLines_[i].first.y)),
                 cv::Point(cvRound(goodLines_[i].second.x), cvRound(goodLines_[i].second.y)),
                 cv::Scalar(255, 0, 255), 4, CV_AA);
    }

    
    //find all intersections between goodlines that intersect @ ~ 90 degrees
    for (size_t i = 0; i < goodLines_.size(); i++) {
        for (size_t j = 0; j < goodLines_.size(); j++) {
            
            if (i == j ) continue;
            cv::Point2f theIntersection;
            cv::Point2f o1,o2,p1,p2,r;
            double distance;
            o1 = goodLines_[i].first;
            p1 = goodLines_[i].second;
            o2 = goodLines_[j].first;
            p2 = goodLines_[j].second;
            
            cv::Point2f x = o2 - o1;
            cv::Point2f d1 = p1 - o1;
            cv::Point2f d2 = p2 - o2;
            
            double cross = d1.x*d2.y - d1.y*d2.x;
            double slope1 = (p1.y - o1.y) / (p1.x - o1.x);
            double slope2 = (p2.y - o2.y) / (p2.x - o2.x);
            double ang1 = atan (slope1) * 180 / 3.14;
            double ang2 = atan (slope2) * 180 / 3.14;
            
            double ang = fabs(ang2) - fabs(ang1);
            

            if ( fabs(ang) > 80.0  && fabs(ang) < 100.0   &&   abs(cross) > /*EPS*/1e-7){
                double t1 = (x.x * d2.y - x.y * d2.x)/cross;
                r = o1 + d1 * t1;
                intersections_.push_back(r);
                
                
                distance = std::sqrt(std::pow(circleCenter_.x - r.x, 2.0) +
                                     std::pow(circleCenter_.y - r.y, 2.0));
                //if (distance > 6.0*expectedCircleRadius_)continue;
            }
        }
    }
    
   
    cout <<"n intersections  = = "<< intersections_.size()<< endl;

    
    // Now select intersections that occur near the start or end point of the intersecting lines. This removes random intersections from sprurious "noise" lines.
    
    for(unsigned int  intsec = 0; intsec < intersections_.size(); intsec++ ){
        bool goodIntersection = false;
        for(unsigned int  gl = 0; gl < goodLines_.size(); gl++ ){
            
            double d_start = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].first.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].first.y, 2.0));
            double d_end = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].second.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].second.y, 2.0));
            if(d_start< 5.0 || d_end < 5.0) goodIntersection = true;
        }
        if(goodIntersection == true) goodIntersections_.push_back(intersections_[intsec]);
    }
    
    for (unsigned int goodint =0; goodint < goodIntersections_.size(); goodint++){
        
        
        // Select the two intersections that are adjacent to the circle.
        if (std::sqrt(std::pow(circleCenter_.x - goodIntersections_[goodint].x, 2.0) + std::pow(circleCenter_.y - goodIntersections_[goodint].y, 2.0)) < expectedCircleRadius_*1.6) finalIntersections_.push_back(goodIntersections_[goodint]);

    }
    

    
    //one typically ends up with multiple intersections at each corner due extra, spurious lines
    cout <<"n final intersection  = = "<< finalIntersections_.size()<< endl;

    // I divide the collection of final intersections into those abpve the circle's centre "up"
    // and below "down". I use the average point of these collections to define to define the interesting intersections.
    double running_total_x  =0;
    double running_total_y  =0;
    double av_x_up;
    double av_x_down;
    double av_y_up;
    double av_y_down;
    
    finalIntersectionsDown_.clear();
    finalIntersectionsUp_.clear();
    
    if (finalIntersections_.size() >0){
        for (unsigned int finalint =0; finalint < finalIntersections_.size(); finalint++) {
            if ( y > finalIntersections_[finalint].y){
                finalIntersectionsDown_.push_back(finalIntersections_[finalint]);
            }
            else if ( y < finalIntersections_[finalint].y){
                finalIntersectionsUp_.push_back(finalIntersections_[finalint]);
            }
            
        }
        
        
        for (unsigned int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
            running_total_x = running_total_x + finalIntersectionsUp_[finalintup].x;
            running_total_y = running_total_y + finalIntersectionsUp_[finalintup].y;
        }
        
        av_x_up = running_total_x/finalIntersectionsUp_.size();
        av_y_up = running_total_y/finalIntersectionsUp_.size();
        
        running_total_x  =0;
        running_total_y  =0;
        
        for (unsigned int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
            running_total_x = running_total_x + finalIntersectionsDown_[finalintdown].x;
            running_total_y = running_total_y + finalIntersectionsDown_[finalintdown].y;
        }
        
        av_x_down = running_total_x/finalIntersectionsDown_.size();
        av_y_down = running_total_y/finalIntersectionsDown_.size();
        
        for (unsigned int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
            circle( img_rgb, finalIntersectionsUp_[finalintup], 10, cv::Scalar(0,255,255), 3, 8, 0 );
        }
        
        for (unsigned int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
            circle( img_rgb, finalIntersectionsDown_[finalintdown], 10, cv::Scalar(0,255,0), 3, 8, 0 );
        }
        
        cv::line(img_rgb,
                 cv::Point(cvRound(av_x_down), cvRound(av_y_down)),
                 cv::Point(cvRound(av_x_up), cvRound(av_y_up)),
                 cv::Scalar(0, 255, 255), 4, CV_AA);
        
        
        circle( img_rgb, cv::Point(x,y), 10, cv::Scalar(255,255,0), 3, 8, 0 );
        
        
        slope_final = (av_y_up -  av_y_down) / (av_x_up -  av_x_down);
        ang_final = atan (slope_final) * 180 / 3.14;
        
        
    }
    
    
   std::ostringstream convert;   // stream used for the conversion
   convert.str(" ");
   convert.clear();
   convert << ang_final;
   std::string str_ang_final = convert.str();
   convert.str(" ");
   convert.clear();
   convert << circleCenter_.x ;
   std::string str_x = convert.str();
   convert.str(" ");
   convert.clear();
   convert << circleCenter_.y;
   std::string str_y = convert.str();
    
    std::string text_result = "Marker is at = (" +  str_x  +  ", " + str_y + "), with orientation = " + str_ang_final  + " degrees";
    
    
    putText(img_rgb, text_result, cvPoint(30,100),
            cv::FONT_HERSHEY_COMPLEX_SMALL, 2.8, cvScalar(200,200,250), 1, CV_AA);
    std::string filename = "/Users/keaveney/Desktop/calibration/PatRec_result.png";
    cv::imwrite(filename, img_rgb);
    
    emit updateImage(stage, filename);
    emit foundSensor(1);
    emit sendPosition(stage, circleCenter_.x,circleCenter_.y, ang_final);
    NQLog("AssemblyVUEyeCamera") << "  found marker";
}


void AssemblySensorLocator::foundsensor(int state){

    NQLog("AssemblySensorLocator::foundsensor");

    
    if (state == 0){
        ql->setText("WAITING");
        NQLog("AssemblySensorLocator::foundsensor")<< " 0  ";

        ql->setStyleSheet("QLabel { background-color : orange; color : black; }");
    
    } else if (state == 1){
        
        NQLog("AssemblySensorLocator::foundsensor")<< " 1 ";

        ql->setText("FOUND MARKER");
        ql->setStyleSheet("QLabel { background-color : green; color : black; }");
    
    } else if (state == 2){
        NQLog("AssemblySensorLocator::foundsensor")<< " 2 ";
        ql->setText("ERROR");
        ql->setStyleSheet("QLabel { background-color : red; color : black; }");

    }
    
    


}


AssemblyZPositionIdentifier::AssemblyZPositionIdentifier(QWidget *parent, std::string string, double scan_step ,int scan_n_iter)
: QWidget(parent), local_scan_step(scan_step), local_scan_n_iter(scan_n_iter)
{
    QFormLayout *l = new QFormLayout(this);
    setLayout(l);
    
    std::ostringstream strs;
    strs.clear();
    strs << scan_step;
    strs << ",";
    strs << scan_n_iter;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    this->local_scan_step = scan_step;
    this->local_scan_n_iter = scan_n_iter;
    
    button1 = new QPushButton(qname, this);

    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);

    connect(button1, SIGNAL(clicked()),
            this, SLOT(performZScan()));
               
}



void AssemblyZPositionIdentifier::performZScan() {
    
    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lineEdit1->text();
    
    QStringList pieces = parent_string.split( "," );
    QString str_step = pieces.value(0);
    QString str_n_iter = pieces.value(1);
    
    double scan_step = str_step.toDouble();
    int scan_n_iter = str_n_iter.toInt();
    
    std::cout<<"Scan step = "<<scan_step<<"; Scan iterations = "<<scan_n_iter<<std::endl;
    
    if (scan_step == 0.0) {
        std::cout<< "Warning! Scan step (double) should differ from '0'. Exiting ... " << std::endl;
        exit(319);
    }
    
    if (scan_n_iter <= 0) {
        std::cout<< "Warning! Scan range (integer) should be positive and be different from '0'. Exiting ... " << std::endl;
        exit(320);
    }     
    
    double scan_distance = scan_step*scan_n_iter;
    std::cout<<"Scanning "<<scan_distance<<" mm in Z-direction with "<<scan_n_iter<<" iterations..."<<std::endl;   
    
    //~~~~~~~~~~~~~~~
    // take first photo..."0" idx and then move --- map properly idx and distances !!!!
    for (int img_idx = 0; img_idx <= scan_n_iter; ++img_idx) {   
        emit moveRelative(0.0, 0.0, scan_step, 0.0);
    
    
    //Put here routine to acquire images from camera
    //emit moveAbsolute(x_d, y_d, z_d, a_d);            
    }            
    
    // go back sequence
    //~~~~~~~~~~~~~~~~~        

    const int img_scan_points = scan_n_iter + 1;//include photo at position "0.0"
    double variances[img_scan_points];
    double z_relative_coordinates[img_scan_points];
    double max_variance = 0.0;
    int max_variance_idx = 0;
    
    for (int img_idx = 0; img_idx < img_scan_points; ++img_idx) {      
        cv::Mat img_temp;
        img_temp = this->imageFromScanInColor(img_idx);

        double img_variance = this->imageVariance(img_temp);
        if(img_variance > max_variance) {// potentially unreliable -> switch to derivative method if needed
            max_variance = img_variance;
            max_variance_idx = img_idx;
        }
        //std::cout<<"Laplace variance: "<<img_variance<<std::endl; // DEBUG INFO
        //std::cout<<"Maximal variance: "<<max_variance<<std::endl; //DEBUG INFO
        
        z_relative_coordinates[img_idx] = img_idx*scan_step;
        variances[img_idx] = img_variance;
        std::cout<<"Relative position in Z (mm) = "<<z_relative_coordinates[img_idx]<<" Laplace variance = "<<img_variance<<std::endl; // DEBUG INFO
        //std::cout<<variances[img_idx]<<std::endl; //DEBUG INFO
    }
    
    TCanvas *c1 = new TCanvas("c1","Z-scan",200,10,700,500);    

    if (img_scan_points > 0) {
        TGraph *gr = new TGraph( img_scan_points, z_relative_coordinates, variances);
        gr->SetTitle("Z-scan output");
        gr->GetXaxis()->SetTitle("Relative Z-position, mm");
        gr->GetYaxis()->SetTitle("Variance");
        gr->Draw("AC*");
        gr->Fit("gaus");
        TF1 *ff = gr->GetFunction("gr");
        //std::cout<< ff->GetParameter(0) <<" "<< ff->GetParameter(1) <<" "<< ff->GetParameter(2) << std::endl;
        
        
        const char * filename_canvas = "/home/savitsky/Software/TrkModLab/BlurImage/ZScan.png";
        c1->SaveAs(filename_canvas);   
        emit updateImage(2, filename_canvas);
    }
    
}


cv::Mat AssemblyZPositionIdentifier::imageFromScanInColor(int idx) {

    bool blur_test = 0;
    
    std::ostringstream s_stream;
    if (blur_test == 0) {
        s_stream << "/home/savitsky/Software/TrkModLab/BlurImage/gal_cen_hubble_" << idx << ".png";//img_
    } else {
        s_stream << "/home/savitsky/Software/TrkModLab/BlurImage/gal_cen_hubble.png";
    } 
    std::string img_name = s_stream.str();    
    if( !(access( img_name.c_str(), F_OK ) != -1 )) { 
        std::cout<< "Warning! No access to input image for Z-scan with index '"<< idx << "'. Exiting ... " << std::endl;
        exit(322);
    }
    
    //std::cout<<"Reading image: "<<img_name<<std::endl;
    
    cv::Mat img;
    img = cv::imread(img_name, CV_LOAD_IMAGE_COLOR);  
    
    
    if (blur_test == 1) {
        blur(img, img, Size(2*idx+1, 2*idx+1));
        int idx_write = 9 - idx; // idx + 10
        std::ostringstream s_stream_write;
        s_stream_write << "/home/savitsky/Software/TrkModLab/BlurImage/gal_cen_hubble_" << idx_write << ".png";
        cv::imwrite(s_stream_write.str(), img);
    }    
    
    return img;   
  
}  


double AssemblyZPositionIdentifier::imageVariance(cv::Mat img_input) {

    Mat img_copy = img_input.clone();    
    
    //Convert color image to greyscale (GS)
    Mat img_copy_gs(img_copy.size(), img_copy.type());
    cvtColor(img_copy, img_copy_gs, CV_BGR2GRAY);
    
    //Apply laplacian function to GS image
    Mat img_laplace;
    Laplacian(img_copy_gs, img_laplace, CV_64F);
    
    //Calculate standard deviation of laplace image
    cv::Scalar mean, std_dev;
    meanStdDev(img_laplace, mean, std_dev, cv::Mat());
    
    //std::cout<<"Variance: "<<std_dev.val[0]*std_dev.val[0]<<std::endl;
    
    return std_dev.val[0]*std_dev.val[0];   
  
}






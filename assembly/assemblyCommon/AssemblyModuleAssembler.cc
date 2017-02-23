#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>

#include <TGraph.h>
#include <TCanvas.h>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "AssemblyModuleAssembler.h"

#include "LStepExpressWidget.h"

using namespace std;
using namespace cv;

AssemblyModuleAssembler::AssemblyModuleAssembler(AssemblyVUEyeModel *uEyeModel_, AssemblySensorMarkerFinder * finder_,
                                                 LStepExpressModel* lStepExpressModel_,
                                                 QWidget *parent)
  : QWidget(parent)
{

  LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);


  camera_ = uEyeModel_->getCameraByID(10);
    
    if (camera_){
  NQLog("AssemblyModuleAssembler::constructed  camera");
    }else{
    
        NQLog("AssemblyModuleAssembler::  cant  construct camera");

    }

  QGridLayout *l = new QGridLayout();
  setLayout(l);

  QGridLayout *g0 = new QGridLayout();
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
    
  imageView_1->connectImageProducer(finder_, SIGNAL(markerFound(const cv::Mat&)));

  //  connect(uEyeModel_, SIGNAL(imageAcquired(const cv::Mat&)),
  //         finder_, SLOT(findMarker(const cv::Mat&)));

  scrollArea_1 = new QScrollArea(this);
  scrollArea_1->setMinimumSize(200, 200);
  scrollArea_1->setPalette(palette);
  scrollArea_1->setBackgroundRole(QPalette::Background);
  scrollArea_1->setAlignment(Qt::AlignCenter);
  scrollArea_1->setWidget(imageView_1);
  scrollArea_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_1,0,0);
  lE1 = new QLineEdit("Object location. = X,Z");
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
  lE2 = new QLineEdit("Orientation. = alpha");
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

  g0->addWidget(scrollArea_3,2,0);
  lE3 = new QLineEdit("Mntd pos. (cor. 2) = lE3,X");
 // g0->addWidget(lE3,1,2);

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

  g0->addWidget(scrollArea_4,2,1);
  lE4 = new QLineEdit("Dtchd pos. = ,X,X");
 // g0->addWidget(lE4,3,0);

    QGridLayout *gl = new QGridLayout();
    l->addLayout(gl,1,0);
    
    // Add all the axes displays
  for (unsigned int i=0;i<4;++i) {
    gl->addWidget(new LStepExpressAxisWidget(lStepExpressModel_, i, this), 0, i);
    }

  //LStepExpressPositionWidget *lStepPosition = new LStepExpressPositionWidget(motionManager_, lStepExpressModel_, this);
  //g0->addWidget(lStepPosition,3,0);
    
    
  QGridLayout *g1 = new QGridLayout();
  l->addLayout(g1,0,1);

  QLabel* header_label = new QLabel("Hover mouse over button for info on each step", this);
  g1->addWidget(header_label,0,0);

//  QPushButton* connect_button = new QPushButton("Connect Devcies", this);
//  g1->addWidget(connect_button,1,0);


  AssemblyCommander * cmdr0 = new AssemblyCommander(this, "Move absolute", 0.0,0.0,0.0,0.0);
  cmdr0->setToolTip("(1) Returns x,y,z stage to origin (default = (0,0,0) using moveAbsolute(x,y,z) routine)");
  g1->addWidget(cmdr0,2,0);

  AssemblyAttacher * cmdr1 = new AssemblyAttacher("Move relative", 0.0,0.0,0.0,0.0);
  cmdr1->setToolTip("(2) Moves x,y,z stage realtive to current position using moveAbsolute(x,y,z) routine)");
  g1->addWidget(cmdr1,3,0);
    
  //AssemblyCommander * cmdr1 = new AssemblyCommander(this, "Move relative", 0.0,0.0,0.0,0.0);
  //cmdr1->setToolTip("(2) Moves x,y,z stage to pickup position (default = (100,100,100) using moveAbsolute(x,y,z) routine)");
  //g1->addWidget(cmdr1,4,0);

  AssemblySensorLocator * lctr1 = new AssemblySensorLocator(this, "Locate object", 0.0, finder_);
  lctr1->setToolTip("(3) Acquires image from mobile camera, runs PatRec routine to deduce and report sensor (x,y,z,phi) postion");
  g1->addWidget(lctr1,5,0);

 // AssemblyCommander * cmdr2 = new AssemblyCommander(this, "Correct position", 100.0,100.0,100.0,100.0);
 // cmdr2->setToolTip("(4) Corrects arm position using relative displacement using (eventually) pre-calculated displacment from PatRec");
 // g1->addWidget(cmdr2,6,0);

  AssemblyAttacher * attacher1 = new AssemblyAttacher("Drop/Raise", 10.0);
 // g1->addWidget(attacher1,7,0);

  //AssemblyVacuumToggler * toggle1 = new AssemblyVacuumToggler(this, "Toggle vacuum");
  toggle1 = new AssemblyVacuumToggler(this, "Toggle vacuum");
  g1->addWidget(toggle1,8,0);
    
  AssemblyPrecisionEstimator * precision1 = new AssemblyPrecisionEstimator(this, "Estimate Assembly Precision", "-200.0,0.0,0.0", "0.0,0.0,0.0", 1 );
  g1->addWidget(precision1 ,9,0);

  //AssemblyCommander * cmdr4 = new AssemblyCommander(this, "Go to stat. camera", 100.0,100.0,100.0,100.0);
  //g1->addWidget(cmdr4, 9, 0);

 // AssemblyMountChecker * cmdr5 = new AssemblyMountChecker(this, "Check mount", 100.0,100.0,100.0,100.0, 0);
 // g1->addWidget(cmdr5, 10, 0);

  //  AssemblyCommander * cmdr6 = new AssemblyCommander(this, "Go to rotation stage", 100.0,100.0,100.0,100.0);
  //g1->addWidget(cmdr6, 11, 0);

  //AssemblyCommander * cmdr7 = new AssemblyCommander(this, "Drop and detach", 100.0,100.0,100.0,100.0);
  //g1->addWidget(cmdr7, 12, 0);

  //AssemblyAligner * cmdr8 = new AssemblyAligner(this, "Align", 0.0);
  //g1->addWidget(cmdr8, 13, 0);

  //make all the neccessary connections
  connect(attacher1, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
  connect(cmdr0, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr1, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
    
  connect(precision1, SIGNAL(launchPrecisionEstimation(double, double, double, double, double, double, int)),
            this, SLOT(startMacro(double, double, double, double, double, double, int)));
    
    
  //connect(cmdr2, SIGNAL(moveAbsolute(double,double,double,double)),
  //        motionManager_, SLOT(moveRelative(double,double,double,double)));
 // connect(cmdr4, SIGNAL(moveAbsolute(double,double,double,double)),
  //        motionManager_, SLOT(moveAbsolute(double,double,double,double)));
 // connect(cmdr5, SIGNAL(moveAbsolute(double,double,double,double)),
  //        motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  //connect(cmdr6, SIGNAL(moveAbsolute(double,double,double,double)),
  //        motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  //connect(cmdr7, SIGNAL(moveAbsolute(double,double,double,double)),
  //       motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  //connect(cmdr8, SIGNAL(moveRelative(double,double,double,double)),
  //        motionManager_, SLOT(moveRelative(double,double,double,double)));
  //connect(cmdr8, SIGNAL(locateSetdowncorner(int)), lctr1, SLOT( locateSensor(int)));

    
    
  connect(lStepExpressModel_, SIGNAL(acquireImage()), camera_, SLOT(acquireImage()));
    
  connect(lctr1, SIGNAL(acquireImage()), camera_, SLOT(acquireImage()));

  connect(camera_, SIGNAL(imageAcquired(cv::Mat)), lctr1, SLOT(locatePickup(cv::Mat)));

    
  connect(lctr1, SIGNAL(sendPosition(int, double,double,double)), this, SLOT(updateText(int,double,double,double)));
  connect(finder_, SIGNAL(updateImage(int, QString)), this, SLOT( updateImage(int,QString)));
  connect(finder_, SIGNAL(foundSensor(int)), lctr1, SLOT( foundsensor(int)));
  connect(finder_, SIGNAL(reportObjectLocation(int, double, double, double)), this, SLOT( updateText(int, double, double, double )));

}

void AssemblyModuleAssembler::updateText(int stage, double x, double y, double a){

  NQLog("AssemblyCommander::updateText");

  std::ostringstream strs_position;
  std::ostringstream strs_orientation;

  strs_position.clear();
  strs_orientation.clear();

  strs_position << x;
  strs_position << ",";
  strs_position << y;
  std::string str_position = strs_position.str();
  QString qstr_position = "";
  qstr_position = QString::fromStdString(str_position);
    
  strs_orientation << a;
  std::string str_orientation = strs_orientation.str();
  QString qstr_orientation = "";
  qstr_orientation = QString::fromStdString(str_orientation);
    
  if(stage == 1 ){
    qstr_position = "Object location = " + qstr_position + " mm, mm (lab frame)";
    lE1->setText(qstr_position);
      
    qstr_orientation = "Object orientation = " + qstr_orientation + " degrees";
    lE2->setText(qstr_orientation);
  }

}

void AssemblyModuleAssembler::startMacro(double x_meas, double y_meas, double z_meas, double x_pickup, double y_pickup, double z_pickup, int iterations)
{
    NQLog("AssemblyModuleAssembler::startMacro");
    
    emit launchPrecisionEstimation(x_meas,  y_meas,  z_meas,  x_pickup,  y_pickup, z_pickup, iterations);
    
}


void AssemblyModuleAssembler::updateImage(int stage, QString filename)
{
  NQLog("AssemblyModuleAssembler") << ":updateImage()  " + filename;

  std::string filename_ss = filename.toUtf8().constData();

  cv::Mat img_gs = cv::imread(filename_ss, CV_LOAD_IMAGE_UNCHANGED);

  if (stage == 1 ){
    imageView_1->setZoomFactor(0.5);
    imageView_1->setImage(img_gs);
    
  }else if (stage ==2){
    imageView_2->setImage(img_gs);
    imageView_2->setZoomFactor(0.5);
  }
  else if (stage ==3){
    imageView_4->setImage(img_gs);
    imageView_4->setZoomFactor(0.5);

  }
  else if (stage ==4){
    imageView_3->setImage(img_gs);
    imageView_3->setZoomFactor(0.5);

  }

}

void AssemblyModuleAssembler::gotoPickup()
{
  NQLog("AssemblyModuleAssembler") << ":gotoPickup()";
  //connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));
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
   NQLog("AssemblyModuleAssembler") << ":snapShot()";

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


AssemblyPrecisionEstimator::AssemblyPrecisionEstimator(QWidget *parent, string text, string measurement_position, string pickup_position, int iterations )
: QWidget(parent)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    QString q_pu_pos = QString::fromStdString(pickup_position);
    QString q_meas_pos = QString::fromStdString(measurement_position);
    QString qname = QString::fromStdString(text);

    button1 = new QPushButton(qname, this);
    l->addWidget(button1,0,0);
    
    QFormLayout *fl1 = new QFormLayout(this);
    l->addLayout(fl1,1,0);

    label1 = new QLabel();
    lineEdit1 = new QLineEdit();
    fl1->addRow(label1,lineEdit1);

    label2 = new QLabel();
    lineEdit2 = new QLineEdit();
    fl1->addRow(label2,lineEdit2);
    
    label3 = new QLabel();
    lineEdit3 = new QLineEdit();
    fl1->addRow(label3,lineEdit3);
    

    label1->setText("Measurement position (x,y,z)");
    label2->setText("Pickup position (x,y,z)");
    label3->setText("N iterations");
    

    
    lineEdit1->setText("54.8327,-19.9927,-76.3294");
    lineEdit2->setText("-34.1604,-60.9907,-107.3152");
    lineEdit3->setText("3");
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(run()));
}


void AssemblyPrecisionEstimator::recordPosition(double x, double y, double theta)
{
 
    NQLog("AssemblyPrecisionEstimator") << ":recordPosition";

}


void AssemblyPrecisionEstimator::run()
{
    
    NQLog("AssemblyPrecisionEstimator") << ":run";
    
    
    
    //parse lineEdit text to get target coordinates
    QString  parent_string_meas = this->lineEdit1->text();
    
    QStringList pieces_meas = parent_string_meas.split( "," );
    QString x_meas_s = pieces_meas.value( pieces_meas.length() - 3);
    QString y_meas_s = pieces_meas.value( pieces_meas.length() - 2);
    QString z_meas_s = pieces_meas.value( pieces_meas.length() -1);
    
    double x_meas_d = x_meas_s.toDouble();
    double y_meas_d = y_meas_s.toDouble();
    double z_meas_d = z_meas_s.toDouble();
    
    
    QString  parent_string_pickup = this->lineEdit2->text();
    
    QStringList pieces_pickup = parent_string_pickup.split( "," );
    QString x_pickup_s = pieces_pickup.value( pieces_pickup.length() - 3);
    QString y_pickup_s = pieces_pickup.value( pieces_pickup.length() - 2);
    QString z_pickup_s = pieces_pickup.value( pieces_pickup.length() - 1);
    
    double x_pickup_d = x_pickup_s.toDouble();
    double y_pickup_d = y_pickup_s.toDouble();
    double z_pickup_d = z_pickup_s.toDouble();
    
    
    QString  parent_string_iterations = this->lineEdit3->text();
    
    int iterations_i = parent_string_iterations.toInt();
    
    NQLog("AssemblyPrecisionEstimator::run")<<  " x_m = " << x_meas_d <<  " y_m = " << y_meas_d << " z_m = " << z_meas_d << " x_p = " << x_pickup_d  << " y_p = " << y_pickup_d  <<" z_p = " << z_pickup_d <<  " iterations = " << iterations_i;


    emit launchPrecisionEstimation(x_meas_d, y_meas_d, z_meas_d, x_pickup_d, y_pickup_d, z_pickup_d, iterations_i );
    
}



AssemblyVacuumToggler::AssemblyVacuumToggler(QWidget *parent, std::string string)
: QWidget(parent)
{
  QGridLayout *l = new QGridLayout(this);
  setLayout(l);

  state = false;
    
  //cnrd1 = new ConradModel(parent);

  QString qname = QString::fromStdString(string);

  state = false;//???

  button1 = new QPushButton(qname, this);
  l->addWidget(button1,0,0);
    

  radio1 = new QRadioButton(tr("&Channel 1"));
  radio2 = new QRadioButton(tr("&Channel 2"));
  radio3 = new QRadioButton(tr("&Channel 3"));
    
    
  valves.push_back(radio1);
  valves.push_back(radio2);
  valves.push_back(radio3);
    

  l->addWidget(radio1,1,0);
  l->addWidget(radio2,3,0);
  l->addWidget(radio3,5,0);
    

  QPixmap pixmap(100,100);
  pixmap.fill(QColor("transparent"));
    
  QPainter painter(&pixmap);
  painter.setBrush(QBrush(Qt::red));
  painter.drawEllipse(0, 0, 30, 30);
    
  ql1 = new QLabel("", this);
  l->addWidget(ql1,1,1);

  ql1->setPixmap(pixmap);
  ql1->setText("VACUUM OFF");
  ql1->setStyleSheet("QLabel { background-color : green; color : black; }");
    
  ql2 = new QLabel("", this);
  l->addWidget(ql2,3,1);
    
  ql2->setPixmap(pixmap);
  ql2->setText("VACUUM OFF");
  ql2->setStyleSheet("QLabel { background-color : green; color : black; }");
   
  ql3 = new QLabel("", this);
  l->addWidget(ql3,5,1);
    
  ql3->setPixmap(pixmap);
  ql3->setText("VACUUM OFF");
  ql3->setStyleSheet("QLabel { background-color : green; color : black; }");
    
  labels.push_back(ql1);
  labels.push_back(ql2);
  labels.push_back(ql3);

  connect(button1, SIGNAL(clicked()),
          this, SLOT(toggleVacuum()));

}


//possibly incorrect to have this methid defined here
// TODO: move it to a controller class

void AssemblyVacuumToggler::toggleVacuum()
{
    NQLog("AssemblyVacuumToggler") << ": toggling vacuum voltage ";

    for (int i = 0; i < 3 ; i ++){
    
        if (valves[i]->isChecked()){
            
	  NQLog("AssemblyVacuumToggler") << ": emit signal to channel " << (i + 1);
	  button1 -> setEnabled(false);
	  emit toggleVacuum(i + 1);
	  return;
        }
    }
    NQLog("AssemblyVacuumToggler") << ": None channel selected! Vacuum is not toggled.";
    
}

void AssemblyVacuumToggler::enableVacuumButton()
{
  button1 -> setEnabled(true);
}

void AssemblyVacuumToggler::disableVacuumButton()
{
  button1 -> setEnabled(false);
}

void AssemblyVacuumToggler::updateVacuumChannelState(int channelNumber, bool channelState)
{
  emit enableVacuumButton();

  if (channelState){
    labels[channelNumber]->setText("VACUUM ON");
    labels[channelNumber]->setStyleSheet("QLabel { background-color : red; color : black; }");
  }else {
    labels[channelNumber]->setText("VACUUM OFF");
    labels[channelNumber]->setStyleSheet("QLabel { background-color : green; color : black; }");    
  }
}



AssemblyAttacher::AssemblyAttacher(std::string string, double x ,double y, double z,double a)
:local_x(x), local_y(y),local_z(z),local_a(a)
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
          this, SLOT(moveRelative()));
}

void AssemblyAttacher::moveRelative(){
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
    
    NQLog("AssemblyAttacher::moveRelative") << ": moving relative (parsed) "<< x_d<<" "<< y_d<<" "<< z_d;
    
    NQLog("AssemblyAttacher::moveRelative") <<" requesting move...";
    
    emit moveRelative(x_d, y_d, z_d, a_d);
    
    NQLog("AssemblyAttacher::moveRelative") <<"move requested...";
}

AssemblyMountChecker::AssemblyMountChecker(QWidget *parent, std::string string,
                                           double x ,double y, double z,
                                           double a, int mode)
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

void AssemblyMountChecker::checkMount()
{
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

AssemblyCommander::AssemblyCommander(QWidget *parent, std::string string,
                                     double x ,double y, double z,
                                     double a)
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

void AssemblyCommander::goToTarget()
{
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

  NQLog("AssemblyCommander:goToTarget") << ": going to target (parsed) "<< x_d<<" "<< y_d<<" "<< z_d;

  NQLog("AssemblyCommander:goToTarget") <<" requesting move...";

  emit moveAbsolute(x_d, y_d, z_d, a_d);

  NQLog("AssemblyCommander:goToTarget") <<"move requested...";
}


AssemblyAligner::AssemblyAligner(QWidget *parent, std::string string,
                                 double a)
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

void AssemblyAligner::setDown()
{
  emit locateSetdowncorner(6);

}

void AssemblyAligner::align()
{
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

AssemblySensorLocator::AssemblySensorLocator(QWidget *parent, std::string string,
                                             double a, AssemblySensorMarkerFinder * finder_ )
 : QWidget(parent), local_a(a)
{
    
  std::ostringstream strs;
  strs.clear();
  strs << a;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString cachedirTemp = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString cachedirTemp = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  cachedirTemp += "/assembly/calibration";
  QDir dir1(cachedirTemp);
  if (!dir1.exists()) dir1.mkpath(".");
  cacheDirectory1_ = cachedirTemp.toStdString();
  cachedirTemp += "/RotatedImages";
  QDir dir2(cachedirTemp);
  if (!dir2.exists()) dir2.mkpath(".");
  cacheDirectory2_ = cachedirTemp.toStdString();

  QGridLayout *l = new QGridLayout(this);
  setLayout(l);

  button1 = new QPushButton(qname, this);

  l->addWidget(button1,0,0);
    
  groupBox1 = new QGroupBox(tr("Object sought"));
  groupBox2 = new QGroupBox(tr("Mode"));
    
    
  radio1 = new QRadioButton(tr("&Fiducial marker"));
  radio2 = new QRadioButton(tr("&Positioning pin"));
  radio3 = new QRadioButton(tr("&Sensor corner"));
    
  radio1->setChecked(true);

  vbox1 = new QVBoxLayout;
  vbox1->addWidget(radio1);
  vbox1->addWidget(radio2);
  vbox1->addWidget(radio3);

  vbox1->addStretch(1);
  groupBox1->setLayout(vbox1);
    
  radio4 = new QRadioButton(tr("&Demo"));
  radio5 = new QRadioButton(tr("&Lab"));
  radio4->setChecked(true);
    
  vbox2 = new QVBoxLayout;
  vbox2->addWidget(radio4);
  vbox2->addWidget(radio5);
  vbox2->addStretch(1);
  groupBox2->setLayout(vbox2);

  l->addWidget(groupBox1,1,0);
  l->addWidget(groupBox2,1,1);

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

    
  connect(button1, SIGNAL(clicked()), this, SLOT(detectPatRecMode()));
    
  connect(this, SIGNAL(runObjectDetection(int, int )), finder_, SLOT(runObjectDetection(int, int)));

    
 // connect(this, SIGNAL(locatePickupCorner_circleSeed(int)),
  //        this, SLOT(locateSensor_circleSeed(int)));

  connect(this, SIGNAL(locatePickupCorner_templateMatching(cv::Mat,cv::Mat)),
             finder_, SLOT(findMarker_templateMatching(cv::Mat,cv::Mat)));
    
  //connect(this, SIGNAL(locatePickupCorner_circleSeed(cv::Mat,cv::Mat)),
  //          finder_, SLOT(findMarker_circleSeed(cv::Mat,cv::Mat)));

}


void AssemblySensorLocator::detectPatRecMode()
{
    objectmode = 0;
    labmode = 0;
    
    if (radio1->isChecked()){
        objectmode =0;
    } else if (radio2->isChecked()){
        objectmode =1;
        
    }else if (radio3->isChecked()){
        
        objectmode =2;
    }
    
    if (radio4->isChecked()){
        labmode = 0;
        
    } else if(radio5->isChecked())
    {
        labmode = 1;
    }
    
    emit runObjectDetection(labmode, objectmode);

}






void AssemblySensorLocator::foundsensor(int state)
{
  NQLog("AssemblySensorLocator::foundsensor");

  if (state == 0) {
    ql->setText("WAITING");
    NQLog("AssemblySensorLocator::foundsensor")<< " 0  ";

    ql->setStyleSheet("QLabel { background-color : orange; color : black; }");
    
  } else if (state == 1) {
    
    NQLog("AssemblySensorLocator::foundsensor")<< " 1 ";
    
    ql->setText("FOUND MARKER");
    ql->setStyleSheet("QLabel { background-color : green; color : black; }");
    
  } else if (state == 2) {
    NQLog("AssemblySensorLocator::foundsensor")<< " 2 ";
    ql->setText("ERROR");
    ql->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
}

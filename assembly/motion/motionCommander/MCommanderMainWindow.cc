#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "MCommanderMainWindow.h"

MCommanderMainWindow::MCommanderMainWindow(QWidget *parent)
: QMainWindow(parent)
{
  ApplicationConfig* config = ApplicationConfig::instance();
  //  std::cout<<"pointer config "<<config<<" memory address "<<&config<<std::endl;
  
  connect(QApplication::instance(), SIGNAL(aboutToQuit()),
          this, SLOT(quit()));
  
  lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),
                                             1000, 100);
  //  std::cout<<"pointer lStepExpressModel_ "<<lStepExpressModel_<<" memory = "<<&lStepExpressModel_<<std::endl;
  //lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
  //std::cout<<"pointer lStepExpressSettings_ "<<lStepExpressSettings_<<" memory = "<<&lStepExpressSettings_<<std::endl;
  motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
  //std::cout<<"pointer lStepExpressMotionManager_ "<<motionManager_<<" memory = "<<&motionManager_<<std::endl;
  motionThread_ = new LStepExpressMotionThread(this);
  //std::cout<<"pointer lStepExpressMotionThread_ "<<motionThread_<<" memory = "<<&motionThread_<<std::endl;
  motionThread_->start();
  //lStepExpressSettings_->moveToThread(motionThread_);
  motionManager_->myMoveToThread(motionThread_);
  
  laserModel_ = new LaserModel(config->getValue<std::string>("KeyenceDevice").c_str());
  //std::cout<<"pointer laserModel_ "<<laserModel_<<" memory = "<<&laserModel_<<std::endl;
  laserThread_ = new LaserThread(this);
  //std::cout<<"pointer laserThread_ "<<laserThread_<<" memory = "<<&laserThread_<<std::endl;
  laserModel_->moveToThread(laserThread_);
  laserThread_->start();
  
  tabWidget_ = new QTabWidget(this);
  //std::cout<<"pointer tabWidget_ "<<tabWidget_<<std::endl; 
 
  QWidget * widget;
  
  widget= new QWidget(tabWidget_);
  
  tabWidget_->addTab(widget, "Motion Manager");
  
  // widget = new QWidget(tabWidget_);
  
  QHBoxLayout * layout = new QHBoxLayout(widget);
  widget->setLayout(layout);
  
  QVBoxLayout * layoutv = new QVBoxLayout(widget);
  
  LStepExpressWidget *lStepExpressWidget = new LStepExpressWidget(lStepExpressModel_, widget);
  //std::cout<<"pointer lStepExpressWidget_ "<<lStepExpressWidget<<" memory = "<<&lStepExpressWidget<<std::endl;
  layoutv->addWidget(lStepExpressWidget);
  
  LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(lStepExpressModel_, widget);
  //std::cout<<"pointer lStepJoystick_ "<<lStepJoystick<<" memory = "<<&lStepJoystick<<std::endl;
  layoutv->addWidget(lStepJoystick);
  
  layout->addLayout(layoutv);

  QVBoxLayout * layoutv2 = new QVBoxLayout(widget);
  
  //LStepExpressStatusWindow *lStepStatusWindow = new LStepExpressStatusWindow(lStepExpressModel_, widget);
  //layoutv2->addWidget(lStepStatusWindow);
  
  LStepExpressPositionWidget *lStepPosition = new LStepExpressPositionWidget(motionManager_, lStepExpressModel_, widget);
  //std::cout<<"pointer lStepExpressPositionWidget_ "<<lStepPosition<<" memory = "<<&lStepPosition<<std::endl;
  layoutv2->addWidget(lStepPosition);
  
  layout->addLayout(layoutv2);
  
  tabWidget_->addTab(widget, "LStep Express");
  
  //  LStepExpressSettingsWidget *lStepExpressSettingsWidget = new LStepExpressSettingsWidget(lStepExpressSettings_, tabWidget_);
  //std::cout<<"pointer lStepExpressSettingsWidget_ "<<lStepExpressSettingsWidget<<" memory = "<<&lStepExpressSettingsWidget<<std::endl;
  //tabWidget_->addTab(lStepExpressSettingsWidget, "LStep Express Settings");
  
  setCentralWidget(tabWidget_);
  
  /*
    connect(this, SIGNAL(moveAbsolute(double,double,double,double)),
    motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(this, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
  */
  
  //QTimer::singleShot(1000, lStepExpressModel_, SLOT(setDeviceEnabled()));
  
  //QTimer::singleShot(2000, this, SLOT(testManager()));
  
  //LStepExpressMeasurementWidget *lStepExpressMeasurementWidget = new LStepExpressMeasurementWidget(lStepExpressModel_, motionManager_, laserModel_, widget);
  //tabWidget_->addTab(lStepExpressMeasurementWidget, "Measurements");
  


  measurementTable_ = new LStepExpressMeasurementTable();
  //std::cout<<"pointer measurementTable_ "<<measurementTable_<<" memory = "<<&measurementTable_<<std::endl;
  measurement_ = new LStepExpressMeasurement(lStepExpressModel_, motionManager_, laserModel_, measurementTable_);
  //std::cout<<"pointer measurement_ "<<measurement_<<" memory = "<<&measurement_<<std::endl;
  measurement_->moveToThread(motionThread_);

  LStepExpressMeasurementWidget_v2 *lStepExpressMeasurementWidget = new LStepExpressMeasurementWidget_v2(lStepExpressModel_, laserModel_, measurement_, measurementTable_, widget);
  //std::cout<<"pointer lStepExpressMeasurementWidget_ "<<lStepExpressMeasurementWidget<<" memory = "<<&lStepExpressMeasurementWidget<<std::endl;
    tabWidget_->addTab(lStepExpressMeasurementWidget, "Measurements");

}

void MCommanderMainWindow::quit()
{
  std::cout<<"MCommanderMainWindow::quit1"<<std::endl;    
  if (laserThread_) {
    laserThread_->quit();
    laserThread_->wait();
  }
  std::cout<<"MCommanderMainWindow::quit2"<<std::endl;
  if (motionThread_) {
    motionThread_->quit();
    motionThread_->wait();
  }    
  std::cout<<"MCommanderMainWindow::quit3"<<std::endl;
  if(lStepExpressModel_){ lStepExpressModel_->setDeviceEnabled(false); lStepExpressModel_->deleteLater(); }
  std::cout<<"MCommanderMainWindow::quit4"<<std::endl;
    if(laserModel_){ laserModel_->setDeviceEnabled(false); laserModel_->deleteLater(); }    
  std::cout<<"MCommanderMainWindow::quit5"<<std::endl;
    if(measurement_){ measurement_->deleteLater();}
  std::cout<<"MCommanderMainWindow::quit6"<<std::endl;
    if(measurementTable_){ measurementTable_->deleteLater();}
  std::cout<<"MCommanderMainWindow::quit7"<<std::endl;
    if(motionManager_){motionManager_->deleteLater();}
  std::cout<<"MCommanderMainWindow::quit8"<<std::endl;
    if(tabWidget_){tabWidget_->deleteLater();}
  std::cout<<"MCommanderMainWindow::quit9"<<std::endl;    
}

/*
  void MCommanderMainWindow::testManager()
  {
  //emit moveAbsolute(20, 10, 5, 2);
  //emit moveRelative(3, 5, 6, -2);
    //emit moveRelative(0, 40, -40, 80);
    }
*/

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

  connect(QApplication::instance(), SIGNAL(aboutToQuit()),
          this, SLOT(quit()));

  lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),
                                             1000, 100);
  //  lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);

  motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);

  motionThread_ = new LStepExpressMotionThread(this);
  motionThread_->start();
  //lStepExpressSettings_->moveToThread(motionThread_);
  motionManager_->myMoveToThread(motionThread_);

  laserModel_ = new LaserModel(config->getValue<std::string>("KeyenceDevice").c_str());
  laserThread_ = new LaserThread(this);
  laserModel_->moveToThread(laserThread_);
  laserThread_->start();

  tabWidget_ = new QTabWidget(this);

  QWidget * widget;

  widget= new QWidget(tabWidget_);

  tabWidget_->addTab(widget, "Motion Manager");

  widget = new QWidget(tabWidget_);

  QHBoxLayout * layout = new QHBoxLayout(widget);
  widget->setLayout(layout);

  QVBoxLayout * layoutv = new QVBoxLayout(widget);
  //  widget->setLayout(layout);

  LStepExpressWidget *lStepExpressWidget = new LStepExpressWidget(lStepExpressModel_, widget);
  layoutv->addWidget(lStepExpressWidget);

  LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(lStepExpressModel_, widget);
  layoutv->addWidget(lStepJoystick);
  
  layout->addLayout(layoutv);

  //LStepExpressPositionWidget *lStepPosition = new LStepExpressPositionWidget(lStepExpressModel_, widget);
  //layout->addWidget(lStepPosition);

  tabWidget_->addTab(widget, "LStep Express");

  //  LStepExpressSettingsWidget *lStepExpressSettingsWidget = new LStepExpressSettingsWidget(lStepExpressSettings_, tabWidget_);
// tabWidget_->addTab(lStepExpressSettingsWidget, "LStep Express Settings");

  setCentralWidget(tabWidget_);

  /*
  connect(this, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(this, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
  */

  //QTimer::singleShot(1000, lStepExpressModel_, SLOT(setDeviceEnabled()));

  //QTimer::singleShot(2000, this, SLOT(testManager()));

  // LStepExpressMeasurementWidget *lStepExpressMeasurementWidget = new LStepExpressMeasurementWidget(lStepExpressModel_, motionManager_, laserModel_, widget);
  //tabWidget_->addTab(lStepExpressMeasurementWidget, "Measurements");

  std::cout<<"MCommanderMainWindow " << "main window constructed"<<std::endl;
}

void MCommanderMainWindow::quit()
{
    std::cout<<"MCommanderMainWindow " << "quit"<<std::endl;
    
    std::cout<<"MCommanderMainWindow " << "Disable LStepController"<<std::endl;
    if(lStepExpressModel_){ lStepExpressModel_->setDeviceEnabled(false); lStepExpressModel_->deleteLater(); }//delete lStepExpressModel_; lStepExpressModel_ = NULL;}
    
    std::cout<<"MCommanderMainWindow " << "Disable Keyence Laser"<<std::endl;
    if(laserModel_){ laserModel_->setDeviceEnabled(false); laserModel_->deleteLater(); }//delete laserModel_; laserModel_ = NULL;}
    
    motionManager_->deleteLater();
     tabWidget_->deleteLater();
     
     if (motionThread_) {
         motionThread_->quit();
         motionThread_->wait();
     }
     
     if (laserThread_) {
         laserThread_->quit();
         laserThread_->wait();
     }

}

/*
void MCommanderMainWindow::testManager()
{
    //emit moveAbsolute(20, 10, 5, 2);
    //emit moveRelative(3, 5, 6, -2);
    //emit moveRelative(0, 40, -40, 80);
}
*/

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

#include <MCommanderMainWindow.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>

#include <string>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

MCommanderMainWindow::MCommanderMainWindow(QWidget *parent)
: QMainWindow(parent)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(quit()));

  lStepExpressModel_ = new LStepExpressModel(
    config->getValue<std::string>("main", "LStepExpressDevice"),
    config->getValue<std::string>("main", "LStepExpressDevice_ver"),
    config->getValue<std::string>("main", "LStepExpressDevice_iver"),
    1000,
    1000
  );

  //lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
  motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
  motionThread_ = new LStepExpressMotionThread(motionManager_, this);
  //lStepExpressSettings_->moveToThread(motionThread_);
  motionThread_->start();

  laserModel_ = new LaserModel(config->getValue<std::string>("main", "KeyenceDevice").c_str());
  laserThread_ = new LaserThread(this);
  laserModel_->moveToThread(laserThread_);
  laserThread_->start();

  tabWidget_ = new QTabWidget(this);

  QWidget * widget;

  widget= new QWidget(tabWidget_);

  tabWidget_->addTab(widget, "Motion Manager");

  QHBoxLayout * layout = new QHBoxLayout(widget);
  widget->setLayout(layout);

  QVBoxLayout * layoutv1 = new QVBoxLayout();
  
  LStepExpressWidget *lStepExpressWidget = new LStepExpressWidget(lStepExpressModel_, widget);
  layoutv1->addWidget(lStepExpressWidget);
  
  LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(lStepExpressModel_, widget);
  layoutv1->addWidget(lStepJoystick);
  
  layout->addLayout(layoutv1);

  QVBoxLayout * layoutv2 = new QVBoxLayout();
  
  //LStepExpressStatusWindow *lStepStatusWindow = new LStepExpressStatusWindow(lStepExpressModel_, widget);
  //layoutv2->addWidget(lStepStatusWindow);
  
  LStepExpressPositionWidget *lStepPosition = new LStepExpressPositionWidget(motionManager_, lStepExpressModel_, widget);
  layoutv2->addWidget(lStepPosition);
  
  layout->addLayout(layoutv2);
  LaserWidget *laserWidget_ = new LaserWidget(laserModel_);
  laserWidget_->setMinimumWidth(400);
  layout->addWidget(laserWidget_);
  
  tabWidget_->addTab(widget, "LStep Express");

  LaserControlsWidget *laserControlsWidget = new LaserControlsWidget(laserModel_, widget);
  tabWidget_->addTab(laserControlsWidget, "Laser Control");
  
  //  LStepExpressSettingsWidget *lStepExpressSettingsWidget = new LStepExpressSettingsWidget(lStepExpressSettings_, tabWidget_);
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
  
  measurementTable_ = new LStepExpressMeasurementTable();
  measurement_ = new LStepExpressMeasurement(lStepExpressModel_, motionManager_, laserModel_, measurementTable_);
  measurement_->moveToThread(motionThread_);

  LStepExpressMeasurementWidget *lStepExpressMeasurementWidget = new LStepExpressMeasurementWidget(lStepExpressModel_, laserModel_, measurement_, measurementTable_, widget);
    tabWidget_->addTab(lStepExpressMeasurementWidget, "Measurements");

}

void MCommanderMainWindow::quit()
{
  if (laserThread_) {
    laserThread_->quit();
    laserThread_->wait();
  }

  if (motionThread_) {
    motionThread_->quit();
    motionThread_->wait();
  }    

  if(lStepExpressModel_){ lStepExpressModel_->setDeviceEnabled(false); lStepExpressModel_->deleteLater(); }
  if(laserModel_){ laserModel_->setDeviceEnabled(false); laserModel_->deleteLater(); }    
  if(measurement_){ measurement_->deleteLater();}
  if(measurementTable_){ measurementTable_->deleteLater();}
  if(motionManager_){motionManager_->deleteLater();}
  if(tabWidget_){tabWidget_->deleteLater();}
}

/*
  void MCommanderMainWindow::testManager()
  {
  //emit moveAbsolute(20, 10, 5, 2);
  //emit moveRelative(3, 5, 6, -2);
    //emit moveRelative(0, 40, -40, 80);
    }
*/

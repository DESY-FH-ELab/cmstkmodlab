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
                                             1000, 100, this);
  lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_, this);

  motionManager_ = new LStepExpressMotionManager(lStepExpressModel_, this);

  motionThread_ = new LStepExpressMotionThread(this);
  motionThread_->start();
  lStepExpressSettings_->moveToThread(motionThread_);
  motionManager_->myMoveToThread(motionThread_);

  tabWidget_ = new QTabWidget(this);

  QWidget * widget;

  widget= new QWidget(tabWidget_);

  tabWidget_->addTab(widget, "Motion Manager");

  widget = new QWidget(tabWidget_);

  QVBoxLayout * layout = new QVBoxLayout(widget);
  widget->setLayout(layout);

  LStepExpressWidget *lStepExpressWidget = new LStepExpressWidget(lStepExpressModel_, widget);
  layout->addWidget(lStepExpressWidget);

  LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(lStepExpressModel_, widget);
  layout->addWidget(lStepJoystick);

  tabWidget_->addTab(widget, "LStep Express");

  setCentralWidget(tabWidget_);

  connect(this, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(this, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));

  QTimer::singleShot(100, lStepExpressModel_, SLOT(setDeviceEnabled()));

  QTimer::singleShot(2000, this, SLOT(testManager()));

  NQLog("MCommanderMainWindow") << "main window constructed";
}

void MCommanderMainWindow::quit()
{
  NQLog("MCommanderMainWindow") << "quit";

  if (motionThread_) {
      motionThread_->quit();
      motionThread_->wait();
  }

  lStepExpressModel_->setDeviceEnabled(false);
}

void MCommanderMainWindow::testManager()
{
    /*
    emit moveAbsolute(20, 10, 5, 2);
    emit moveRelative(3, 5, 6, -2);
    emit moveRelative(0, 40, -40, 80);
    */
}

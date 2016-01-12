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
                                             5000, 1000, this);

  motionManager_ = new LStepExpressMotionManager(lStepExpressModel_, this);

  motionThread_ = new LStepExpressMotionThread(this);
  motionThread_->start();

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

  QTimer::singleShot(100, lStepExpressModel_, SLOT(setDeviceEnabled()));

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

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

  QWidget * widget = new QWidget(this);

  QVBoxLayout * layout = new QVBoxLayout(widget);
  widget->setLayout(layout);

  LStepExpressWidget *lStepExpressWidget = new LStepExpressWidget(lStepExpressModel_, widget);
  layout->addWidget(lStepExpressWidget);

  LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(lStepExpressModel_, widget);
  layout->addWidget(lStepJoystick);

  setCentralWidget(widget);

  QTimer::singleShot(100, lStepExpressModel_, SLOT(setDeviceEnabled()));

  NQLog("MCommanderMainWindow") << "main window constructed";
}

void MCommanderMainWindow::quit()
{
  NQLog("MCommanderMainWindow") << "quit";

  lStepExpressModel_->setDeviceEnabled(false);
}

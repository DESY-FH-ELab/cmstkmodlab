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
                                             5, this);

  QTimer::singleShot(2000, lStepExpressModel_, SLOT(setDeviceEnabled()));

  NQLog("MCommanderMainWindow") << "main window constructed";
}

void MCommanderMainWindow::quit()
{
  NQLog("MCommanderMainWindow") << "quit";

  lStepExpressModel_->setDeviceEnabled(false);
}

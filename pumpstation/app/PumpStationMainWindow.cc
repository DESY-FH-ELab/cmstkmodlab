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

#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "PumpStationMainWindow.h"

PumpStationMainWindow::PumpStationMainWindow(double updateInterval,
                                             QWidget *parent)
  : QMainWindow(parent),
    updateInterval_(updateInterval)
{
  ApplicationConfig* config = ApplicationConfig::instance();

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  view_ = new QWebView(parent);
  view_->setMinimumWidth(860);
  view_->setMinimumHeight(540);
  view_->load(QUrl(config->getValue("URL").c_str()));
  view_->show();
#else
  view_ = new QWebEngineView(parent);
  view_->setMinimumWidth(860);
  view_->setMinimumHeight(540);
  view_->load(QUrl(config->getValue("URL").c_str()));
  view_->show();
#endif

  setCentralWidget(view_);
  updateGeometry();

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect(timer_, SIGNAL(timeout()), view_, SLOT(reload()));

  timer_->start();

  NQLog("PumpStationMainWindow") << "main window constructed";
}

void PumpStationMainWindow::quit()
{
  NQLog("PumpStationMainWindow") << "quit";
}

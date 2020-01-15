/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
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

#include <QWidget>
#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>

#include "ApplicationConfig.h"

#include "HuberUnistat525wWidget.h"
#include "RohdeSchwarzNGE103BWidget.h"
#include "KeithleyDAQ6510Widget.h"

#include "Thermo2DAQWidget.h"
#include "Thermo2ScriptWidget.h"

#include "Thermo2MainWindow.h"

Thermo2MainWindow::Thermo2MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  QWidget * widget;
  QBoxLayout * wlayout;

#ifdef USE_FAKEIO
  huberModel_ = new HuberUnistat525wModel(config->getValue<std::string>("HuberUnistatDevice").c_str(),
                                          5, this);
#else
  huberModel_ = new HuberUnistat525wModel(config->getValue<std::string>("HuberUnistatDevice").c_str(),
                                          20, this);
#endif

#ifdef USE_FAKEIO
  nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("RohdeSchwarzNGE103B").c_str(),
                                               5, this);
#else
  nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("RohdeSchwarzNGE103B").c_str(),
                                               10, this);
#endif

#ifdef USE_FAKEIO
  keithleyModel_ = new KeithleyDAQ6510Model(config->getValue<std::string>("KeithleyDAQ6510").c_str(),
                                            5, this);
#else
  keithleyModel_ = new KeithleyDAQ6510Model(config->getValue<std::string>("KeithleyDAQ6510").c_str(),
                                            30, this);
#endif

  daqModel_ = new Thermo2DAQModel(huberModel_,
                                  nge103BModel_,
                                  keithleyModel_,
                                  this);

  // SCRIPT MODEL
  scriptModel_ = new Thermo2ScriptModel(daqModel_,
                                        huberModel_,
                                        nge103BModel_,
                                        keithleyModel_,
                                        this);

  daqStreamer_ = new Thermo2DAQStreamer(daqModel_, this);

//  QString webuser(config->getValue<std::string>("Webuser").c_str());
//  if (webuser==getenv("USER")) {
//    daqWebInfo_ = new ThermoDAQWebInfo(daqModel_, this);
//  }

  daqServer_ = new Thermo2DAQServer(daqModel_, this);
  daqServer_->listen(QHostAddress::LocalHost, config->getValue<unsigned int>("ServerPort"));

  daqThread_ = new Thermo2DAQThread(daqModel_, this);
  connect(QApplication::instance(), SIGNAL(aboutToQuit()),
          this, SLOT(quit()));
  daqThread_->start();
  daqModel_->myMoveToThread(daqThread_);

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);


  widget = new QWidget();
  wlayout = new QVBoxLayout();
  widget->setLayout(wlayout);

  // HUBER MODEL
  HuberUnistat525wWidget* huberWidget = new HuberUnistat525wWidget(huberModel_, widget);
  huberWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  wlayout->addWidget(huberWidget);

//  // PFEIFFER MODEL
//  //PfeifferWidget* pfeifferWidget = new PfeifferWidget(pfeifferModel_, widget);
//  ThermoPfeifferWidget* pfeifferWidget = new ThermoPfeifferWidget(pfeifferModel_, widget);
//  pfeifferWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  wlayout->addWidget(pfeifferWidget);

  wlayout->addWidget(new QWidget());

  tabWidget_->addTab(widget, "Chiller");

  // Rohde & Schwarz NGE130B Widget
  RohdeSchwarzNGE103BWidget* nge103BWidget = new RohdeSchwarzNGE103BWidget(nge103BModel_);
  //nge103BWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(nge103BWidget, "Power Supply");

  // KEITHLEY MODEL
  KeithleyDAQ6510Widget* keithleyWidget = new KeithleyDAQ6510Widget(keithleyModel_);
  keithleyWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(keithleyWidget, "Multimeter");

  widget = new QWidget();
  widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  wlayout = new QVBoxLayout();
  widget->setLayout(wlayout);

  // DAQ Widget
  Thermo2DAQWidget* daqWidget = new Thermo2DAQWidget(daqModel_, widget);
  daqWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  wlayout->addWidget(daqWidget);

  // Script Widget
  Thermo2ScriptWidget* scriptWidget = new Thermo2ScriptWidget(scriptModel_, widget);
  scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  wlayout->addWidget(scriptWidget);

  tabWidget_->addTab(widget, "DAQ + Scripting");

  setCentralWidget(tabWidget_);
  updateGeometry();
}

void Thermo2MainWindow::quit()
{
  if (daqThread_) {
    daqThread_->quit();
    daqThread_->wait();
  }
}

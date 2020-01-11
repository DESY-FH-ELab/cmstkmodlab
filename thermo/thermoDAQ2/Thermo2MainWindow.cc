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

#include "RohdeSchwarzNGE103BWidget.h"
#include "KeithleyDAQ6510Widget.h"

#include "Thermo2DAQWidget.h"
#include "Thermo2ScriptWidget.h"

#include "Thermo2MainWindow.h"

Thermo2MainWindow::Thermo2MainWindow(QWidget *parent)
  : QMainWindow(parent)
//    daqThread_(0)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  QWidget * widget;
  QBoxLayout * wlayout;

  // Rohde & Schwarz NGE130B Model
  nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("RohdeSchwarzNGE103B").c_str(),
                                               10, this);

//  // HUBER MODEL
//  huberModel_ = new HuberPetiteFleurModel(config->getValue<std::string>("HuberPetiteFleurDevice").c_str(),
//                                          5, this);

  // KEITHLEY MODEL
  keithleyModel_ = new KeithleyDAQ6510Model(config->getValue<std::string>("KeithleyDAQ6510").c_str(),
                                            30, this);

//  // PFEIFFER MODEL
//  pfeifferModel_ = new PfeifferModel(config->getValue<std::string>("PfeifferDevice").c_str(),
//                                     10, this);
//
//  // IOTA MODEL
//  iotaModel_ = new IotaModel(config->getValue<std::string>("IotaDevice").c_str(),
//                             5, this);
//  iotaModel_->setDeviceEnabled(false);
//
//  // ARDUINO PRES MODEL
//  arduinoPresModel_ = new ArduinoPresModel(config->getValue<std::string>("ArduinoPresDevice").c_str(),
//                                           1, this);
//  arduinoPresModel_->setDeviceEnabled(false);

  daqModel_ = new Thermo2DAQModel(nge103BModel_,
                                  keithleyModel_,
                                  this);

  // SCRIPT MODEL
  scriptModel_ = new Thermo2ScriptModel(daqModel_,
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


//  widget = new QWidget();
//  wlayout = new QVBoxLayout();
//  widget->setLayout(wlayout);
//
//  // HUBER MODEL
//  HuberPetiteFleurWidget* huberWidget = new HuberPetiteFleurWidget(huberModel_, widget);
//  huberWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  wlayout->addWidget(huberWidget);
//
//  // PFEIFFER MODEL
//  //PfeifferWidget* pfeifferWidget = new PfeifferWidget(pfeifferModel_, widget);
//  ThermoPfeifferWidget* pfeifferWidget = new ThermoPfeifferWidget(pfeifferModel_, widget);
//  pfeifferWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  wlayout->addWidget(pfeifferWidget);
//
//  tabWidget_->addTab(widget, "Chiller + Vacuum");

  // Rohde & Schwarz NGE130B Widget
  RohdeSchwarzNGE103BWidget* nge103BWidget = new RohdeSchwarzNGE103BWidget(nge103BModel_);
  //nge103BWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(nge103BWidget, "Power Supply");

  // KEITHLEY MODEL
  KeithleyDAQ6510Widget* keithleyWidget = new KeithleyDAQ6510Widget(keithleyModel_);
  keithleyWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(keithleyWidget, "Multimeter");

//  widget = new QWidget();
//  wlayout = new QHBoxLayout();
//  widget->setLayout(wlayout);
//
//  // IOTA MODEL
//  IotaWidget* iotaWidget = new IotaWidget(iotaModel_, widget);
//  iotaWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  wlayout->addWidget(iotaWidget);
//
//  // ARDUINO PRES MODEL
//  ArduinoPresWidget* arduinoPresWidget = new ArduinoPresWidget(arduinoPresModel_, widget);
//  arduinoPresWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  wlayout->addWidget(arduinoPresWidget);
//
//  tabWidget_->addTab(widget, "Microchannel");

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

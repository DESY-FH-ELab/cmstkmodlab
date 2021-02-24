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
#include "MartaWidget.h"
#include "AgilentTwisTorr304Widget.h"
#include "LeyboldGraphixOneWidget.h"
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
  QGroupBox * gbox;
  QBoxLayout * vlayout;
  QBoxLayout * vlayout2;
  QBoxLayout * hlayout;

#ifdef USE_FAKEIO
  huberModel_ = new HuberUnistat525wModel(config->getValue<std::string>("HuberUnistatDevice").c_str(),
                                          5, this);
#else
  huberModel_ = new HuberUnistat525wModel(config->getValue<std::string>("HuberUnistatDevice").c_str(),
                                          20, this);
#endif

#ifdef USE_FAKEIO
  martaModel_ = new MartaModel(config->getValue<std::string>("MartaIPAddress").c_str(),
			       5, this);
#else
  martaModel_ = new MartaModel(config->getValue<std::string>("MartaIPAddress").c_str(),
			       10, this);
#endif

#ifdef USE_FAKEIO
  agilentModel_ = new AgilentTwisTorr304Model(config->getValue<std::string>("AgilentTwisTorr304Device").c_str(),
                                              5, this);
#else
  agilentModel_ = new AgilentTwisTorr304Model(config->getValue<std::string>("AgilentTwisTorr304Device").c_str(),
                                              20, this);
#endif

#ifdef USE_FAKEIO
  leyboldModel_ = new LeyboldGraphixOneModel(config->getValue<std::string>("LeyboldGraphixOneDevice").c_str(),
                                             5, this);
#else
  leyboldModel_ = new LeyboldGraphixOneModel(config->getValue<std::string>("LeyboldGraphixOneDevice").c_str(),
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
																	martaModel_,
                                  agilentModel_,
                                  leyboldModel_,
                                  nge103BModel_,
                                  keithleyModel_,
                                  this);

  // SCRIPT MODEL
  scriptModel_ = new Thermo2ScriptModel(daqModel_,
                                        huberModel_,
																				martaModel_,
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
  vlayout = new QVBoxLayout();
  widget->setLayout(vlayout);

  hlayout = new QHBoxLayout();
  vlayout->addLayout(hlayout);

  gbox = new QGroupBox("Chiller");
  vlayout2 = new QVBoxLayout();
  gbox->setLayout(vlayout2);

  // HUBER MODEL
  HuberUnistat525wWidget* huberWidget = new HuberUnistat525wWidget(huberModel_, widget);
  huberWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vlayout2->addWidget(huberWidget);

  hlayout->addWidget(gbox);

  gbox = new QGroupBox("Vacuum");
  vlayout2 = new QVBoxLayout();
  gbox->setLayout(vlayout2);

  // AGILET MODEL
  AgilentTwisTorr304Widget* agilentWidget = new AgilentTwisTorr304Widget(agilentModel_, widget);
  agilentWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vlayout2->addWidget(agilentWidget);

  // LEYBOLD MODEL
  LeyboldGraphixOneWidget* leyboldWidget = new LeyboldGraphixOneWidget(leyboldModel_, widget);
  leyboldWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vlayout2->addWidget(leyboldWidget);

  vlayout2->addWidget(new QWidget());

  hlayout->addWidget(gbox);

  vlayout->addWidget(new QWidget());

  tabWidget_->addTab(widget, "Chiller && Vacuum");

  // MARTA MODEL
  MartaWidget* martaWidget = new MartaWidget(martaModel_);
  martaWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(martaWidget, "Marta");

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
  vlayout = new QVBoxLayout();
  vlayout->setSpacing(16*(vlayout->spacing()+1));
  widget->setLayout(vlayout);

  // DAQ Widget
  Thermo2DAQWidget* daqWidget = new Thermo2DAQWidget("DAQ",
                                                     daqModel_, widget);
  daqWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vlayout->addWidget(daqWidget);

  // Script Widget
  Thermo2ScriptWidget* scriptWidget = new Thermo2ScriptWidget("Scripting",
                                                              scriptModel_, widget);
  scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  vlayout->addWidget(scriptWidget);

  tabWidget_->addTab(widget, "DAQ && Scripting");

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

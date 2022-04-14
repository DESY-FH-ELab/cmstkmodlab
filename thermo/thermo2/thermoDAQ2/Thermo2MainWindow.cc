/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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

#include <nqlogger.h>

#include "MartaWidget.h"

#include "Thermo2DAQWidget.h"
#include "Thermo2ScriptWidget.h"

#include "Thermo2ThroughPlaneWidget.h"

#include "Thermo2MainWindow.h"

Thermo2MainWindow::Thermo2MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  NQLog("Thermo2MainWindow", NQLog::Debug) << "Thermo2MainWindow(QWidget *parent)";

  ApplicationConfig* config = ApplicationConfig::instance();

  QWidget * widget;
  QGroupBox * gbox;
  QBoxLayout * vlayout;
  QBoxLayout * vlayout2;
  QBoxLayout * hlayout;

  chillerAndVacuumActive_ = config->getValue<int>("main", "ChillerAndVacuumActive");
  martaActive_ = config->getValue<int>("main", "MartaActive");
  throughPlaneActive_ = config->getValue<int>("main", "ThroughPlaneSetupActive");

  huberModel_ = 0;
  if (chillerAndVacuumActive_) {
#ifdef USE_FAKEIO
  	huberModel_ = new HuberUnistat525wModel(config->getValue<std::string>("main", "HuberUnistatDevice").c_str(),
  			5, this);
#else
  	huberModel_ = new HuberUnistat525wModel(config->getValue<std::string>("main", "HuberUnistatDevice").c_str(),
  			10, this);
#endif
  }

  martaModel_ = 0;
  if (martaActive_) {
#ifdef USE_FAKEIO
  	martaModel_ = new MartaModel(config->getValue<std::string>("main", "MartaIPAddress").c_str(),
  			5, this);
#else
  	martaModel_ = new MartaModel(config->getValue<std::string>("main", "MartaIPAddress").c_str(),
  			10, this);
#endif
  }

  agilentModel_ = 0;
  if (chillerAndVacuumActive_) {
#ifdef USE_FAKEIO
  	agilentModel_ = new AgilentTwisTorr304Model(config->getValue<std::string>("main", "AgilentTwisTorr304Device").c_str(),
  			5, this);
#else
  	agilentModel_ = new AgilentTwisTorr304Model(config->getValue<std::string>("main", "AgilentTwisTorr304Device").c_str(),
  			20, this);
#endif
  }

  leyboldModel_ = 0;
  if (chillerAndVacuumActive_) {
#ifdef USE_FAKEIO
  	leyboldModel_ = new LeyboldGraphixOneModel(config->getValue<std::string>("main", "LeyboldGraphixOneDevice").c_str(),
  			5, this);
#else
  	leyboldModel_ = new LeyboldGraphixOneModel(config->getValue<std::string>("main", "LeyboldGraphixOneDevice").c_str(),
  			20, this);
#endif
  }

#ifdef USE_FAKEIO
  nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("main", "RohdeSchwarzNGE103BDevice").c_str(),
                                               5, this);
#else
  nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("main", "RohdeSchwarzNGE103BDevice").c_str(),
                                               10, this);
#endif

#ifdef USE_FAKEIO
  keithleyModel_ = new KeithleyDAQ6510Model(config->getValue<std::string>("main", "KeithleyDAQ6510Device").c_str(),
                                            5, this);
#else
  keithleyModel_ = new KeithleyDAQ6510Model(config->getValue<std::string>("main", "KeithleyDAQ6510Device").c_str(),
                                            30, this);
#endif

  throughPlaneModel_ = 0;
  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    throughPlaneModel_ = new Thermo2ThroughPlaneModel(huberModel_,
        nge103BModel_,
        keithleyModel_,
        this);
  }

  daqModel_ = new Thermo2DAQModel(huberModel_,
				  martaModel_,
				  agilentModel_,
				  leyboldModel_,
				  nge103BModel_,
				  keithleyModel_,
				  this);

  // SCRIPT MODEL
  scriptModel_ = new Thermo2ScriptModel(daqModel_,
					leyboldModel_,
					huberModel_,
					martaModel_,
					nge103BModel_,
					keithleyModel_,
					throughPlaneModel_,
					this);

  daqStreamer_ = new Thermo2DAQStreamer(daqModel_, this);

//  QString webuser(config->getValue<std::string>("Webuser").c_str());
//  if (webuser==getenv("USER")) {
//    daqWebInfo_ = new ThermoDAQWebInfo(daqModel_, this);
//  }

  daqServer_ = new Thermo2DAQServer(daqModel_, this);
  daqServer_->listen(QHostAddress::LocalHost, config->getValue<unsigned int>("main", "ServerPort"));

  daqThread_ = new Thermo2DAQThread(daqModel_, this);
  connect(QApplication::instance(), SIGNAL(aboutToQuit()),
          this, SLOT(quit()));
  daqThread_->start();
  daqModel_->myMoveToThread(daqThread_);

  commServer_ = new Thermo2CommunicationServer(daqModel_,
      huberModel_,
      martaModel_,
      agilentModel_,
      leyboldModel_,
      nge103BModel_,
      keithleyModel_,
      this);

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  widget = new QWidget();
  vlayout = new QVBoxLayout();
  widget->setLayout(vlayout);

  hlayout = new QHBoxLayout();
  vlayout->addLayout(hlayout);

  if (chillerAndVacuumActive_) {
  	gbox = new QGroupBox("Chiller");
  	vlayout2 = new QVBoxLayout();
  	gbox->setLayout(vlayout2);

  	// HUBER MODEL
  	huberWidget_ = new HuberUnistat525wWidget(huberModel_, widget);
  	huberWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  	vlayout2->addWidget(huberWidget_);

  	hlayout->addWidget(gbox);

  	gbox = new QGroupBox("Vacuum");
  	vlayout2 = new QVBoxLayout();
  	gbox->setLayout(vlayout2);

  	// AGILET MODEL
  	agilentWidget_ = new AgilentTwisTorr304Widget(agilentModel_, widget);
  	agilentWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  	vlayout2->addWidget(agilentWidget_);

  	// LEYBOLD MODEL
  	leyboldWidget_ = new LeyboldGraphixOneWidget(leyboldModel_, widget);
  	leyboldWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  	vlayout2->addWidget(leyboldWidget_);

  	vlayout2->addWidget(new QWidget());

  	hlayout->addWidget(gbox);

  	vlayout->addWidget(new QWidget());

  	tabWidget_->addTab(widget, "Chiller && Vacuum");
  }

  if (martaActive_) {
  	// MARTA MODEL
  	martaWidget_ = new MartaWidget(martaModel_);
  	martaWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  	tabWidget_->addTab(martaWidget_, "Marta");
  }

  // Rohde & Schwarz NGE130B Widget
  nge103BWidget_ = new RohdeSchwarzNGE103BWidget(nge103BModel_);
  //nge103BWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(nge103BWidget_, "Power Supply");

  // KEITHLEY MODEL
  keithleyWidget_ = new KeithleyDAQ6510Widget(keithleyModel_);
  keithleyWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(keithleyWidget_, "Multimeter");

  if (chillerAndVacuumActive_ && throughPlaneActive_) {
  	// THROUGH PLANE MODEL
  	Thermo2ThroughPlaneWidget* throughPlaneWidget = new Thermo2ThroughPlaneWidget(throughPlaneModel_);
  	throughPlaneWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  	tabWidget_->addTab(throughPlaneWidget, "Through Plane");
  }

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

  configViewer_ = new ApplicationConfigViewer(false, this);
  tabWidget_->addTab(configViewer_, "Configuration");

  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
          this, SLOT(controlStateChanged(bool)));

  setCentralWidget(tabWidget_);
  updateGeometry();
}

void Thermo2MainWindow::quit()
{
  NQLog("Thermo2MainWindow", NQLog::Debug) << "quit()";

  if (daqThread_) {
    daqThread_->quit();
    daqThread_->wait();
  }
}

void Thermo2MainWindow::controlStateChanged(bool state)
{
  huberWidget_->controlStateChanged(state);
  if (martaModel_) martaWidget_->controlStateChanged(state);  
  agilentWidget_->controlStateChanged(state);  
  leyboldWidget_->controlStateChanged(state);  
  nge103BWidget_->controlStateChanged(state);  
  keithleyWidget_->controlStateChanged(state);  
}

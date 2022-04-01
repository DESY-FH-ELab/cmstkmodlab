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

#include <iostream>
#include <string>
#include <cmath>

#include <QApplication>
#include <QGroupBox>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>
#include <QVBoxLayout>

#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "ThermoDisplay2MainWindow.h"

ThermoDisplay2MainWindow::ThermoDisplay2MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  chillerAndVacuumActive_ = config->getValue<int>("main", "ChillerAndVacuumActive");
  martaActive_ = config->getValue<int>("main", "MartaActive");
  throughPlaneActive_ = config->getValue<int>("main", "ThroughPlaneSetupActive");

  resistance_ = config->getValue<double>("main", "ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("main", "ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("main", "ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("main", "ThroughPlaneABlock");
  nge103BChannel_ = config->getValue<unsigned int>("main", "ThroughPlaneNGE103BChannel");
  keithleyTopSensors_ = config->getValueArray<unsigned int,6>("main", "ThroughPlaneKeithleyTopSensors");
  keithleyTopPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopPositions");
  keithleyTopCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor0");
  keithleyTopCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor1");
  keithleyTopCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor2");
  keithleyTopCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor3");
  keithleyBottomSensors_ = config->getValueArray<unsigned int,6>("main", "ThroughPlaneKeithleyBottomSensors");
  keithleyBottomPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomPositions");
  keithleyBottomCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor0");
  keithleyBottomCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor1");
  keithleyBottomCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor2");
  keithleyBottomCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor3");
  keithleyAmbientSensor_ = config->getValue<double>("main", "KeithleyAmbientSensor");

  unsigned int sensor;
  unsigned int card;
  unsigned int channel;
  for (unsigned int c=0;c<6;++c) {
    sensor = keithleyTopSensors_[c];
    channel = sensor%100;
    card = (sensor-channel)/100;

    keithleyTopCards_[c] = card-1;
    keithleyTopChannels_[c] = channel-1;

    sensor = keithleyBottomSensors_[c];
    channel = sensor%100;
    card = (sensor-channel)/100;

    keithleyBottomCards_[c] = card-1;
    keithleyBottomChannels_[c] = channel-1;
  }
  if (keithleyAmbientSensor_!=0) {
    sensor = keithleyAmbientSensor_;
    channel = sensor%100;
    card = (sensor-channel)/100;

    keithleyAmbientCard_ = card-1;
    keithleyAmbientChannel_ = channel-1;
  } else {
    keithleyAmbientCard_ = 0;
    keithleyAmbientChannel_ = 0;
  }

  tabWidget_ = new QTabWidget(this);

  QToolBar *toolBar = new QToolBar(this);

  QToolButton * btnClear = new QToolButton(toolBar);
  btnClear->setText("Clear");
  btnClear->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
  connect(btnClear, SIGNAL(clicked()), this, SLOT(clearData()));
  toolBar->addWidget(btnClear);

  QToolButton * btnSavePlots = new QToolButton(toolBar);
  btnSavePlots->setText("Save Plots");
  btnSavePlots->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
  connect(btnSavePlots, SIGNAL(clicked()), this, SLOT(savePlots()));
  toolBar->addWidget(btnSavePlots);

  addToolBar(toolBar);

  QWidget *w;
  QVBoxLayout *layout;

  if (chillerAndVacuumActive_) {
    w = new QWidget(tabWidget_);
    layout = new QVBoxLayout(w);

    ChillerTSChart_ = new ThermoDisplay2TemperatureStateChart();

    ChillerTInternalSeries_ = new ThermoDisplay2LineSeries();
    ChillerTInternalSeries_->setName("Internal");
    ChillerTSChart_->addSeries(ChillerTInternalSeries_);

    ChillerTProcessSeries_ = new ThermoDisplay2LineSeries();
    ChillerTProcessSeries_->setName("Process");
    ChillerTSChart_->addSeries(ChillerTProcessSeries_);

    ChillerTReturnSeries_ = new ThermoDisplay2LineSeries();
    ChillerTReturnSeries_->setName("Return");
    ChillerTSChart_->addSeries(ChillerTReturnSeries_);

    ChillerTCWISeries_ = new ThermoDisplay2LineSeries();
    ChillerTCWISeries_->setName("CW Inlet");
    ChillerTSChart_->addSeries(ChillerTCWISeries_);

    ChillerTCWOSeries_ = new ThermoDisplay2LineSeries();
    ChillerTCWOSeries_->setName("CW Outlet");
    ChillerTSChart_->addSeries(ChillerTCWOSeries_);

    ChillerSTCSeries_ = new ThermoDisplay2LineSeries();
    ChillerSTCSeries_->setName("T Control");
    ChillerTSChart_->addStateSeries(ChillerSTCSeries_);

    ChillerSCSeries_ = new ThermoDisplay2LineSeries();
    ChillerSCSeries_->setName("Circulator");
    ChillerTSChart_->addStateSeries(ChillerSCSeries_);

    ChillerTSChartView_ = new ThermoDisplay2TemperatureStateChartView(ChillerTSChart_);
    ChillerTSChartView_->setRenderHint(QPainter::Antialiasing);
    ChillerTSChartView_->setMinimumSize(800, 300);
    layout->addWidget(ChillerTSChartView_);

    ChillerTSChart_->connectMarkers();
    ChillerTSChart_->updateLegend();
    ChillerTSChart_->refreshAxes();

    ChillerPPChart_ = new ThermoDisplay2PowerPressureChart();

    ChillerPowerSeries_ = new ThermoDisplay2LineSeries();
    ChillerPowerSeries_->setName("Power");
    ChillerPPChart_->addPowerSeries(ChillerPowerSeries_);

    ChillerPressureSeries_ = new ThermoDisplay2LineSeries();
    ChillerPressureSeries_->setName("Pressure");
    ChillerPPChart_->addPressureSeries(ChillerPressureSeries_);

    ChillerPPChartView_ = new ThermoDisplay2PowerPressureChartView(ChillerPPChart_);
    ChillerPPChartView_->setRenderHint(QPainter::Antialiasing);
    ChillerPPChartView_->setMinimumSize(800, 300);
    layout->addWidget(ChillerPPChartView_);

    ChillerPPChart_->connectMarkers();
    ChillerPPChart_->updateLegend();

    tabWidget_->addTab(w, "Chiller");
  }

  if (chillerAndVacuumActive_) {
    w = new QWidget(tabWidget_);
    layout = new QVBoxLayout(w);

    VacuumPressureSeries_ = new ThermoDisplay2LineSeries();
    VacuumPressureSeries_->setName("pressure");

    VacuumPressureChart_ = new ThermoDisplay2LogPressureChart();
    VacuumPressureChart_->addSeries(VacuumPressureSeries_);

    VacuumChartView_ = new ThermoDisplay2PressureChartView(VacuumPressureChart_);
    VacuumChartView_->setRenderHint(QPainter::Antialiasing);
    VacuumChartView_->setMinimumSize(800, 300);
    layout->addWidget(VacuumChartView_);

    VacuumPressureChart_->connectMarkers();
    VacuumPressureChart_->updateLegend();

    tabWidget_->addTab(w, "Vacuum");
  }

  w = new QWidget(tabWidget_);
  layout = new QVBoxLayout(w);

  U1Series_ = new ThermoDisplay2LineSeries();
  U1Series_->setName("U1");
  U2Series_ = new ThermoDisplay2LineSeries();
  U2Series_->setName("U2");
  U3Series_ = new ThermoDisplay2LineSeries();
  U3Series_->setName("U3");

  UChart_ = new ThermoDisplay2VoltageChart();
  UChart_->addSeries(U1Series_);
  UChart_->addSeries(U2Series_);
  UChart_->addSeries(U3Series_);

  UChartView_ = new ThermoDisplay2VoltageChartView(UChart_);
  UChartView_->setRenderHint(QPainter::Antialiasing);
  UChartView_->setMinimumSize(800, 300);
  layout->addWidget(UChartView_);
  UChart_->connectMarkers();
  UChart_->updateLegend();

  I1Series_ = new ThermoDisplay2LineSeries();
  I1Series_->setName("I1");
  I2Series_ = new ThermoDisplay2LineSeries();
  I2Series_->setName("I2");
  I3Series_ = new ThermoDisplay2LineSeries();
  I3Series_->setName("I3");

  IChart_ = new ThermoDisplay2CurrentChart();
  IChart_->addSeries(I1Series_);
  IChart_->addSeries(I2Series_);
  IChart_->addSeries(I3Series_);

  IChartView_ = new ThermoDisplay2CurrentChartView(IChart_);
  IChartView_->setRenderHint(QPainter::Antialiasing);
  IChartView_->setMinimumSize(800, 300);
  layout->addWidget(IChartView_);
  IChart_->connectMarkers();
  IChart_->updateLegend();

  tabWidget_->addTab(w, "Power");

  w = new QWidget(tabWidget_);
  layout = new QVBoxLayout(w);

  for (unsigned int card = 0;card<2;++card) {

    TChart_[card] = new ThermoDisplay2TemperatureChart();

    for (unsigned int channel = 0;channel<10;++channel) {
      unsigned int sensor = (card+1)*100 + channel + 1;
      TSeries_[card][channel] = new ThermoDisplay2LineSeries();
      TSeries_[card][channel]->setName(QString("T%1").arg(sensor));

      TChart_[card]->addSeries(TSeries_[card][channel]);
    }

    TChartView_[card] = new ThermoDisplay2TemperatureChartView(TChart_[card]);
    TChartView_[card]->setRenderHint(QPainter::Antialiasing);
    TChartView_[card]->setMinimumSize(800, 300);
    layout->addWidget(TChartView_[card]);
    TChart_[card]->connectMarkers();
    TChart_[card]->updateLegend();
  }

  tabWidget_->addTab(w, "Temperature");

  if (martaActive_) {
    w = new QWidget(tabWidget_);
    layout = new QVBoxLayout(w);

    MartaPT03Series_ = new ThermoDisplay2LineSeries();
    MartaPT03Series_->setName("PT03R507");

    MartaPT05Series_ = new ThermoDisplay2LineSeries();
    MartaPT05Series_->setName("PT05R507");

    MartaPT01CO2Series_ = new ThermoDisplay2LineSeries();
    MartaPT01CO2Series_->setName("PT01CO2");

    MartaPT02CO2Series_ = new ThermoDisplay2LineSeries();
    MartaPT02CO2Series_->setName("PT02CO2");

    MartaPT03CO2Series_ = new ThermoDisplay2LineSeries();
    MartaPT03CO2Series_->setName("PT03CO2");

    MartaPT04CO2Series_ = new ThermoDisplay2LineSeries();
    MartaPT04CO2Series_->setName("PT04CO2");

    /*
      MartaPT05CO2Series_ = new ThermoDisplay2LineSeries();
      MartaPT05CO2Series_->setName("PT05CO2");
    */

    MartaPT06CO2Series_ = new ThermoDisplay2LineSeries();
    MartaPT06CO2Series_->setName("PT06CO2");

    MartaDP01CO2Series_ = new ThermoDisplay2LineSeries();
    MartaDP01CO2Series_->setName("DP01CO2");

    MartaDP02CO2Series_ = new ThermoDisplay2LineSeries();
    MartaDP02CO2Series_->setName("DP02CO2");

    MartaDP03CO2Series_ = new ThermoDisplay2LineSeries();
    MartaDP03CO2Series_->setName("DP03CO2");

    MartaDP04CO2Series_ = new ThermoDisplay2LineSeries();
    MartaDP04CO2Series_->setName("DP04CO2");

    MartaPressureChart_ = new ThermoDisplay2LinPressureDeltaPressureChart();
    MartaPressureChart_->addPressureSeries(MartaPT03Series_);
    MartaPressureChart_->addPressureSeries(MartaPT05Series_);
    MartaPressureChart_->addPressureSeries(MartaPT01CO2Series_);
    MartaPressureChart_->addPressureSeries(MartaPT02CO2Series_);
    MartaPressureChart_->addPressureSeries(MartaPT03CO2Series_);
    MartaPressureChart_->addPressureSeries(MartaPT04CO2Series_);
    //MartaPressureChart_->addPressureSeries(MartaPT05CO2Series_);
    MartaPressureChart_->addPressureSeries(MartaPT06CO2Series_);
    MartaPressureChart_->addDeltaPressureSeries(MartaDP01CO2Series_);
    MartaPressureChart_->addDeltaPressureSeries(MartaDP02CO2Series_);
    MartaPressureChart_->addDeltaPressureSeries(MartaDP03CO2Series_);
    MartaPressureChart_->addDeltaPressureSeries(MartaDP04CO2Series_);

    MartaPressureChartView_ = new ThermoDisplay2PressureDeltaPressureChartView(MartaPressureChart_);
    MartaPressureChartView_->setRenderHint(QPainter::Antialiasing);
    MartaPressureChartView_->setMinimumSize(800, 300);
    layout->addWidget(MartaPressureChartView_);

    MartaPressureChart_->connectMarkers();
    MartaPressureChart_->updateLegend();

    MartaTSetPointSeries_= new ThermoDisplay2LineSeries();
    MartaTSetPointSeries_->setName("Set Point");

    MartaTT02Series_ = new ThermoDisplay2LineSeries();
    MartaTT02Series_->setName("TT02R507");

    MartaTT01CO2Series_ = new ThermoDisplay2LineSeries();
    MartaTT01CO2Series_->setName("TT01CO2");

    MartaTT02CO2Series_ = new ThermoDisplay2LineSeries();
    MartaTT02CO2Series_->setName("TT02CO2");

    MartaTT03CO2Series_ = new ThermoDisplay2LineSeries();
    MartaTT03CO2Series_->setName("TT03CO2");

    MartaTT04CO2Series_ = new ThermoDisplay2LineSeries();
    MartaTT04CO2Series_->setName("TT04CO2");

    /*
      MartaTT05CO2Series_ = new ThermoDisplay2LineSeries();
      MartaTT05CO2Series_->setName("TT05CO2");
    */

    MartaTT06CO2Series_ = new ThermoDisplay2LineSeries();
    MartaTT06CO2Series_->setName("TT06CO2");

    MartaTT07CO2Series_ = new ThermoDisplay2LineSeries();
    MartaTT07CO2Series_->setName("TT07CO2");

    MartaST01CO2Series_ = new ThermoDisplay2LineSeries();
    MartaST01CO2Series_->setName("ST01CO2");

    MartaST02CO2Series_ = new ThermoDisplay2LineSeries();
    MartaST02CO2Series_->setName("ST02CO2");

    MartaST03CO2Series_ = new ThermoDisplay2LineSeries();
    MartaST03CO2Series_->setName("ST03CO2");

    MartaST04CO2Series_ = new ThermoDisplay2LineSeries();
    MartaST04CO2Series_->setName("ST04CO2");

    MartaDT02CO2Series_ = new ThermoDisplay2LineSeries();
    MartaDT02CO2Series_->setName("DT02CO2");

    MartaDT03CO2Series_ = new ThermoDisplay2LineSeries();
    MartaDT03CO2Series_->setName("DT03CO2");

    MartaTemperatureChart_ = new ThermoDisplay2TemperatureDeltaTemperatureChart();
    MartaTemperatureChart_->addTemperatureSeries(MartaTSetPointSeries_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT02Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT01CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT02CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT03CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT04CO2Series_);
    //MartaTemperatureChart_->addTemperatureSeries(MartaTT05CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT06CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaTT07CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaST01CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaST02CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaST03CO2Series_);
    MartaTemperatureChart_->addTemperatureSeries(MartaST04CO2Series_);
    MartaTemperatureChart_->addDeltaTemperatureSeries(MartaDT02CO2Series_);
    MartaTemperatureChart_->addDeltaTemperatureSeries(MartaDT03CO2Series_);

    MartaTemperatureChartView_ = new ThermoDisplay2TemperatureDeltaTemperatureChartView(MartaTemperatureChart_);
    MartaTemperatureChartView_->setRenderHint(QPainter::Antialiasing);
    MartaTemperatureChartView_->setMinimumSize(800, 300);
    layout->addWidget(MartaTemperatureChartView_);

    MartaTemperatureChart_->connectMarkers();
    MartaTemperatureChart_->updateLegend();

    tabWidget_->addTab(w, "Marta");
  }

  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    w = new QWidget(tabWidget_);
    layout = new QVBoxLayout(w);

    ThroughPlaneTChart_ = new ThermoDisplay2TemperatureChart();

    for (unsigned int c = 0;c<6;++c) {
      ThroughPlaneTopTSeries_[c] = new ThermoDisplay2LineSeries();
      ThroughPlaneTopTSeries_[c]->setName(QString("Top%1 (%2)").arg(c+1).arg(keithleyTopSensors_[c]));
      ThroughPlaneTChart_->addSeries(ThroughPlaneTopTSeries_[c]);
    }

    for (unsigned int c = 0;c<6;++c) {
      ThroughPlaneBottomTSeries_[c] = new ThermoDisplay2LineSeries();
      ThroughPlaneBottomTSeries_[c]->setName(QString("Bottom%1 (%2)").arg(c+1).arg(keithleyBottomSensors_[c]));
      ThroughPlaneTChart_->addSeries(ThroughPlaneBottomTSeries_[c]);
    }

    ThroughPlaneTSampleTop_ = new ThermoDisplay2LineSeries();
    ThroughPlaneTSampleTop_->setName(QString("SampleTop"));
    ThroughPlaneTChart_->addSeries(ThroughPlaneTSampleTop_);

    ThroughPlaneTSampleBottom_ = new ThermoDisplay2LineSeries();
    ThroughPlaneTSampleBottom_->setName(QString("SampleBottom"));
    ThroughPlaneTChart_->addSeries(ThroughPlaneTSampleBottom_);

    ThroughPlaneTSampleMiddle_ = new ThermoDisplay2LineSeries();
    ThroughPlaneTSampleMiddle_->setName(QString("SampleMiddle"));
    ThroughPlaneTChart_->addSeries(ThroughPlaneTSampleMiddle_);

    if (keithleyAmbientSensor_!=0) {
      ThroughPlaneTAmbient_ = new ThermoDisplay2LineSeries();
      ThroughPlaneTAmbient_->setName(QString("Ambient"));
      ThroughPlaneTChart_->addSeries(ThroughPlaneTAmbient_);
    }

    ThroughPlaneTSink_ = new ThermoDisplay2LineSeries();
    ThroughPlaneTSink_->setName(QString("Sink"));
    ThroughPlaneTChart_->addSeries(ThroughPlaneTSink_);

    ThroughPlaneTChartView_ = new ThermoDisplay2TemperatureChartView(ThroughPlaneTChart_);
    ThroughPlaneTChartView_->setRenderHint(QPainter::Antialiasing);
    ThroughPlaneTChartView_->setMinimumSize(800, 300);
    layout->addWidget(ThroughPlaneTChartView_);
    ThroughPlaneTChart_->connectMarkers();
    ThroughPlaneTChart_->updateLegend();

    ThroughPlanePChart_ = new ThermoDisplay2PowerChart();

    ThroughPlanePSource_ = new ThermoDisplay2LineSeries();
    ThroughPlanePSource_->setName(QString("Source"));
    ThroughPlanePChart_->addSeries(ThroughPlanePSource_);

    ThroughPlanePTop_ = new ThermoDisplay2LineSeries();
    ThroughPlanePTop_->setName(QString("Top"));
    ThroughPlanePChart_->addSeries(ThroughPlanePTop_);

    ThroughPlanePBottom_ = new ThermoDisplay2LineSeries();
    ThroughPlanePBottom_->setName(QString("Bottom"));
    ThroughPlanePChart_->addSeries(ThroughPlanePBottom_);

    ThroughPlanePChartView_ = new ThermoDisplay2PowerChartView(ThroughPlanePChart_);
    ThroughPlanePChartView_->setRenderHint(QPainter::Antialiasing);
    ThroughPlanePChartView_->setMinimumSize(800, 300);
    layout->addWidget(ThroughPlanePChartView_);
    ThroughPlanePChart_->connectMarkers();
    ThroughPlanePChart_->updateLegend();

    tabWidget_->addTab(w, "Through-Plane Setup");
  }

  configViewer_ = new ApplicationConfigViewer(false, this);
  tabWidget_->addTab(configViewer_, "Configuration");

  client_ = new ThermoDAQ2Client(config->getValue<unsigned int>("main", "ServerPort"));
  reader_ = new ThermoDAQ2NetworkReader(this);

  QObject::connect(client_, SIGNAL(handleMessage(QString&)),
                   reader_, SLOT(run(QString&)));
  QObject::connect(reader_, SIGNAL(finished()),
                   this, SLOT(updateInfo()));

  setCentralWidget(tabWidget_);

  /*
    QPalette pal = palette();
    QColor color = pal.color(QPalette::Window);
    if (color.lightnessF()<0.4) {
    ChillerTSChart_->setTheme(QChart::ChartThemeDark);
    UChart_->setTheme(QChart::ChartThemeDark);
    IChart_->setTheme(QChart::ChartThemeDark);
    TChart_[0]->setTheme(QChart::ChartThemeDark);
    TChart_[1]->setTheme(QChart::ChartThemeDark);
    }
  */

  requestData();

  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()),
          this, SLOT(requestData()));
  timer_->setSingleShot(false);
  timer_->start(5000);

  connect(config, SIGNAL(valueChanged()),
          this, SLOT(configurationChanged()));
}

void ThermoDisplay2MainWindow::configurationChanged()
{
  ApplicationConfig* config = ApplicationConfig::instance();

  resistance_ = config->getValue<double>("main", "ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("main", "ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("main", "ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("main", "ThroughPlaneABlock");
  keithleyTopPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopPositions");
  keithleyTopCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor0");
  keithleyTopCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor1");
  keithleyTopCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor2");
  keithleyTopCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor3");
  keithleyBottomPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomPositions");
  keithleyBottomCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor0");
  keithleyBottomCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor1");
  keithleyBottomCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor2");
  keithleyBottomCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor3");
}

void ThermoDisplay2MainWindow::clearData()
{
  if (chillerAndVacuumActive_) {
    ChillerTSChart_->clearData();
    ChillerPPChart_->clearData();
    VacuumPressureChart_->clearData();
  }
  UChart_->clearData();
  IChart_->clearData();
  TChart_[0]->clearData();
  TChart_[1]->clearData();
  if (martaActive_) {
    MartaPressureChart_->clearData();
    MartaTemperatureChart_->clearData();
  }
  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    ThroughPlaneTChart_->clearData();
    ThroughPlanePChart_->clearData();
  }
}

void ThermoDisplay2MainWindow::savePlots()
{
  QDateTime dt = QDateTime::currentDateTime();

  ApplicationConfig* config = ApplicationConfig::instance();

  QString startDir = config->getValue<std::string>("main", "PlotSaveDirectory").c_str();

  QString dir = QFileDialog::getExistingDirectory(this, "Save Directory",
                                                  startDir,
                                                  QFileDialog::ShowDirsOnly);

  if (dir.isEmpty()) return;

  int currentTab = tabWidget_->currentIndex();
  for (int idx=0;idx<4;++idx) {
    if (idx!=currentTab) tabWidget_->setCurrentIndex(idx);
  }
  tabWidget_->setCurrentIndex(currentTab);

  config->setValue("main", "PlotSaveDirectory", dir);
  config->save();

  QApplication::processEvents();

  if (chillerAndVacuumActive_) {
    auto dpr = 2.0*ChillerTSChartView_->devicePixelRatioF();
    QPixmap buffer(ChillerTSChartView_->width() * dpr,
		   ChillerTSChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    ChillerTSChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_chillerTS.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  QApplication::processEvents();

  if (chillerAndVacuumActive_) {
    auto dpr = 2.0*ChillerPPChartView_->devicePixelRatioF();
    QPixmap buffer(ChillerPPChartView_->width() * dpr,
		   ChillerPPChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    ChillerPPChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_chillerPP.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  QApplication::processEvents();

  if (chillerAndVacuumActive_) {
    auto dpr = 2.0*VacuumChartView_->devicePixelRatioF();
    QPixmap buffer(VacuumChartView_->width() * dpr,
		   VacuumChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    VacuumChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_vacuum.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  QApplication::processEvents();

  {
    auto dpr = 2.0*UChartView_->devicePixelRatioF();
    QPixmap buffer(UChartView_->width() * dpr,
		   UChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    UChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_voltage.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  QApplication::processEvents();

  {
    auto dpr = 2.0*IChartView_->devicePixelRatioF();
    QPixmap buffer(IChartView_->width() * dpr,
		   IChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    IChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_current.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  QApplication::processEvents();

  {
    auto dpr = 2.0*TChartView_[0]->devicePixelRatioF();
    QPixmap buffer(TChartView_[0]->width() * dpr,
		   TChartView_[0]->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    TChartView_[0]->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_temperatures1.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  QApplication::processEvents();

  {
    auto dpr = 2.0*TChartView_[1]->devicePixelRatioF();
    QPixmap buffer(TChartView_[1]->width() * dpr,
		   TChartView_[1]->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    TChartView_[1]->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_temperatures2.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  if (martaActive_) {
    auto dpr = 2.0*MartaPressureChartView_->devicePixelRatioF();
    QPixmap buffer(MartaPressureChartView_->width() * dpr,
		   MartaPressureChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    MartaPressureChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_marta_pressures.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  if (martaActive_) {
    auto dpr = 2.0*MartaTemperatureChartView_->devicePixelRatioF();
    QPixmap buffer(MartaTemperatureChartView_->width() * dpr,
		   MartaTemperatureChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    MartaTemperatureChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_marta_temperatures.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    auto dpr = 2.0*ThroughPlaneTChartView_->devicePixelRatioF();
    QPixmap buffer(ThroughPlaneTChartView_->width() * dpr,
		   ThroughPlaneTChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    ThroughPlaneTChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_throughplane1.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    auto dpr = 2.0*ThroughPlanePChartView_->devicePixelRatioF();
    QPixmap buffer(ThroughPlanePChartView_->width() * dpr,
		   ThroughPlanePChartView_->height() * dpr);
    buffer.fill(Qt::transparent);

    QPainter *painter = new QPainter(&buffer);
    painter->setPen(*(new QColor(255,34,255,255)));
    ThroughPlanePChartView_->render(painter);

    QFile file(dir + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "_thermo2_throughplane2.png");
    file.open(QIODevice::WriteOnly);
    buffer.save(&file, "PNG");
  }

}

void ThermoDisplay2MainWindow::requestData()
{
  NQLogDebug("ThermoDisplay2MainWindow") << "requestData()";
  client_->readDAQStatus();
}

void ThermoDisplay2MainWindow::updateInfo()
{
  NQLogSpam("ThermoDisplay2MainWindow") << "updateInfo()";

  const Measurement_t& m = reader_->getMeasurement();

  if (chillerAndVacuumActive_) {
    bool updateLegend = false;

    if (ChillerTInternalSeries_->isEnabled()!=m.u525wState_) updateLegend = true;
    ChillerTInternalSeries_->setEnabled(m.u525wState_);
    ChillerTProcessSeries_->setEnabled(m.u525wState_);
    ChillerTReturnSeries_->setEnabled(m.u525wState_);
    ChillerTCWISeries_->setEnabled(m.u525wState_);
    ChillerTCWOSeries_->setEnabled(m.u525wState_);
    ChillerPowerSeries_->setEnabled(m.u525wState_);
    ChillerPressureSeries_->setEnabled(m.u525wState_);
    ChillerSTCSeries_->setEnabled(m.u525wState_);
    ChillerSCSeries_->setEnabled(m.u525wState_);
    ChillerTInternalSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wInternalTemperature_);
    ChillerTProcessSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wProcessTemperature_);
    ChillerTReturnSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wReturnTemperature_);
    ChillerTCWISeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wCWInletTemperature_);
    ChillerTCWOSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wCWOutletTemperature_);
    ChillerPowerSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wPower_/1000.);
    ChillerPressureSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wPumpPressure_);
    ChillerSTCSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wTemperatureControlEnabled_);
    ChillerSCSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wCirculatorEnabled_);

    if (updateLegend) {
      ChillerTSChart_->updateLegend();
      ChillerPPChart_->updateLegend();
    }
  }

  if (chillerAndVacuumActive_) {
    bool updateLegend = false;

    if (VacuumPressureSeries_->isEnabled()!=m.leyboldState_) updateLegend = true;
    VacuumPressureSeries_->setEnabled(m.leyboldState_);
    VacuumPressureSeries_->append(m.dt.toMSecsSinceEpoch(), m.leyboldPressure_);

    if (updateLegend) {
      VacuumPressureChart_->updateLegend();
    }
  }

  {
    bool updateLegend = false;

    if (U1Series_->isEnabled()!=m.nge103BState[0]) updateLegend = true;
    U1Series_->setEnabled(m.nge103BState[0]);
    I1Series_->setEnabled(m.nge103BState[0]);
    U1Series_->append(m.dt.toMSecsSinceEpoch(), m.nge103BVoltage[0]);
    I1Series_->append(m.dt.toMSecsSinceEpoch(), m.nge103BCurrent[0]);

    if (U2Series_->isEnabled()!=m.nge103BState[1]) updateLegend = true;
    U2Series_->setEnabled(m.nge103BState[1]);
    I2Series_->setEnabled(m.nge103BState[1]);
    U2Series_->append(m.dt.toMSecsSinceEpoch(), m.nge103BVoltage[1]);
    I2Series_->append(m.dt.toMSecsSinceEpoch(), m.nge103BCurrent[1]);

    if (U3Series_->isEnabled()!=m.nge103BState[2]) updateLegend = true;
    U3Series_->setEnabled(m.nge103BState[2]);
    I3Series_->setEnabled(m.nge103BState[2]);
    U3Series_->append(m.dt.toMSecsSinceEpoch(), m.nge103BVoltage[2]);
    I3Series_->append(m.dt.toMSecsSinceEpoch(), m.nge103BCurrent[2]);

    if (updateLegend) {
      UChart_->updateLegend();
      IChart_->updateLegend();
    }
  }

  {
    for (unsigned int card = 0;card<2;++card) {
      bool updateLegend = false;
      for (unsigned int channel = 0;channel<10;++channel) {
        if (TSeries_[card][channel]->isEnabled()!=m.keithleyState[card][channel]) updateLegend = true;
        TSeries_[card][channel]->setEnabled(m.keithleyState[card][channel]);
        TSeries_[card][channel]->append(m.dt.toMSecsSinceEpoch(), m.keithleyTemperature[card][channel]);
      }
      if (updateLegend) TChart_[card]->updateLegend();
    }
  }


  if (martaActive_) {
    bool updateLegend = false;

    if (MartaPT01CO2Series_->isEnabled()!=m.martaState_) updateLegend = true;

    MartaPT03Series_->setEnabled(m.martaState_);
    MartaPT05Series_->setEnabled(m.martaState_);
    MartaPT01CO2Series_->setEnabled(m.martaState_);
    MartaPT02CO2Series_->setEnabled(m.martaState_);
    MartaPT03CO2Series_->setEnabled(m.martaState_);
    MartaPT04CO2Series_->setEnabled(m.martaState_);
    //MartaPT05CO2Series_->setEnabled(m.martaState_);
    MartaPT06CO2Series_->setEnabled(m.martaState_);

    MartaDP01CO2Series_->setEnabled(m.martaState_);
    MartaDP02CO2Series_->setEnabled(m.martaState_);
    MartaDP03CO2Series_->setEnabled(m.martaState_);
    MartaDP04CO2Series_->setEnabled(m.martaState_);

    MartaTSetPointSeries_->setEnabled(m.martaState_);
    MartaTT02Series_->setEnabled(m.martaState_);
    MartaTT01CO2Series_->setEnabled(m.martaState_);
    MartaTT02CO2Series_->setEnabled(m.martaState_);
    MartaTT03CO2Series_->setEnabled(m.martaState_);
    MartaTT04CO2Series_->setEnabled(m.martaState_);
    //MartaTT05CO2Series_->setEnabled(m.martaState_);
    MartaTT06CO2Series_->setEnabled(m.martaState_);
    MartaTT07CO2Series_->setEnabled(m.martaState_);

    MartaST01CO2Series_->setEnabled(m.martaState_);
    MartaST02CO2Series_->setEnabled(m.martaState_);
    MartaST03CO2Series_->setEnabled(m.martaState_);
    MartaST04CO2Series_->setEnabled(m.martaState_);

    MartaDT02CO2Series_->setEnabled(m.martaState_);
    MartaDT03CO2Series_->setEnabled(m.martaState_);

    MartaPT03Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT03_);
    MartaPT05Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT05_);
    MartaPT01CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT01CO2_);
    MartaPT02CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT02CO2_);
    MartaPT03CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT03CO2_);
    MartaPT04CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT04CO2_);
    //MartaPT05CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT05CO2_);
    MartaPT06CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaPT06CO2_);

    MartaDP01CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaDP01CO2_);
    MartaDP02CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaDP02CO2_);
    MartaDP03CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaDP03CO2_);
    MartaDP04CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaDP04CO2_);

    MartaTSetPointSeries_->append(m.dt.toMSecsSinceEpoch(), m.martaTemperatureSetpoint_);
    MartaTT02Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT02_);
    MartaTT01CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT01CO2_);
    MartaTT02CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT02CO2_);
    MartaTT03CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT03CO2_);
    MartaTT04CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT04CO2_);
    //MartaTT05CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT05CO2_);
    MartaTT06CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT06CO2_);
    MartaTT07CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaTT07CO2_);

    MartaST01CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaST01CO2_);
    MartaST02CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaST02CO2_);
    MartaST03CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaST03CO2_);
    MartaST04CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaST04CO2_);

    MartaDT02CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaDT02CO2_);
    MartaDT03CO2Series_->append(m.dt.toMSecsSinceEpoch(), m.martaDT03CO2_);

    if (updateLegend) {
      MartaPressureChart_->updateLegend();
      MartaTemperatureChart_->updateLegend();
    }
  }

  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    bool updateLegend = false;

    if (ThroughPlaneTSink_->isEnabled()!=m.u525wState_) updateLegend = true;
    ThroughPlaneTSink_->setEnabled(m.u525wState_);
    ThroughPlaneTSink_->append(m.dt.toMSecsSinceEpoch(), m.u525wInternalTemperature_);

    unsigned int card, channel;
    unsigned int countTop = 0;
    unsigned int countBottom = 0;
    double temp, tcor;

    for (unsigned int c = 0;c<6;++c) {

      card = keithleyTopCards_[c];
      channel = keithleyTopChannels_[c];

      if (ThroughPlaneTopTSeries_[c]->isEnabled()!=m.keithleyState[card][channel]) updateLegend = true;
      if (m.keithleyState[card][channel]) countTop++;
      ThroughPlaneTopTSeries_[c]->setEnabled(m.keithleyState[card][channel]);
      temp = m.keithleyTemperature[card][channel];
      tcor =  keithleyTopCor0_[c];
      tcor += keithleyTopCor1_[c] * temp;
      tcor += keithleyTopCor2_[c] * temp * temp;
      tcor += keithleyTopCor3_[c] * temp * temp * temp;
      ThroughPlaneTopTSeries_[c]->append(m.dt.toMSecsSinceEpoch(), tcor);

      card = keithleyBottomCards_[c];
      channel = keithleyBottomChannels_[c];

      if (ThroughPlaneBottomTSeries_[c]->isEnabled()!=m.keithleyState[card][channel]) updateLegend = true;
      if (m.keithleyState[card][channel]) countBottom++;
      ThroughPlaneBottomTSeries_[c]->setEnabled(m.keithleyState[card][channel]);
      temp = m.keithleyTemperature[card][channel];
      tcor =  keithleyBottomCor0_[c];
      tcor += keithleyBottomCor1_[c] * temp;
      tcor += keithleyBottomCor2_[c] * temp * temp;
      tcor += keithleyBottomCor3_[c] * temp * temp * temp;
      ThroughPlaneBottomTSeries_[c]->append(m.dt.toMSecsSinceEpoch(),tcor);
    }

    if (keithleyAmbientSensor_!=0) {
      card = keithleyAmbientCard_;
      channel = keithleyAmbientChannel_;

      if (ThroughPlaneTAmbient_->isEnabled()!=m.keithleyState[card][channel]) updateLegend = true;
      ThroughPlaneTAmbient_->setEnabled(m.keithleyState[card][channel]);
      ThroughPlaneTAmbient_->append(m.dt.toMSecsSinceEpoch(), m.keithleyTemperature[card][channel]);
    }

    if (m.nge103BState[nge103BChannel_-1]) {
      double sourcePower = resistance_ * std::pow(m.nge103BCurrent[nge103BChannel_-1], 2);
      ThroughPlanePSource_->setEnabled(true);
      ThroughPlanePSource_->append(m.dt.toMSecsSinceEpoch(), sourcePower);
    } else {
      ThroughPlanePSource_->setEnabled(true);
      ThroughPlanePSource_->append(m.dt.toMSecsSinceEpoch(), 0.0);
    }

    if (countTop>=2 && countBottom>=2) {

      ThroughPlaneTSampleTop_->setEnabled(true);
      ThroughPlaneTSampleMiddle_->setEnabled(true);
      ThroughPlaneTSampleBottom_->setEnabled(true);
      ThroughPlanePTop_->setEnabled(true);
      ThroughPlanePBottom_->setEnabled(true);
      
      std::vector<std::pair<double,double> > values;
      double pos;
      double p0, p1;

      values.clear();
      for (unsigned int i=0;i<6;i++) {
        card = keithleyTopCards_[i];
        channel = keithleyTopChannels_[i];

        if (m.keithleyState[card][channel]) {
          temp = m.keithleyTemperature[card][channel];
          tcor =  keithleyTopCor0_[i];
          tcor += keithleyTopCor1_[i] * temp;
          tcor += keithleyTopCor2_[i] * temp * temp;
          tcor += keithleyTopCor3_[i] * temp * temp * temp;
          values.push_back(std::pair<double,double>(keithleyTopPositions_[i], tcor));
        }
      }
      fitter_.fit(values, 2, p0, p1);

      double sampleTTop = p0; // [degC]
      double gradientTop = -1.0 * p1 * 1000.; // [degC/m]
      double powerTop = -1.0 * gradientTop * kBlock_ * ABlock_ * 1e-6; // [W]

      values.clear();
      for (unsigned int i=0;i<6;i++) {
        card = keithleyBottomCards_[i];
        channel = keithleyBottomChannels_[i];

        if (m.keithleyState[card][channel]) {
          temp = m.keithleyTemperature[card][channel];
          tcor =  keithleyBottomCor0_[i];
          tcor += keithleyBottomCor1_[i] * temp;
          tcor += keithleyBottomCor2_[i] * temp * temp;
          tcor += keithleyBottomCor3_[i] * temp * temp * temp;
          values.push_back(std::pair<double,double>(keithleyBottomPositions_[i], tcor));
        }
      }
      fitter_.fit(values, 2, p0, p1);

      double sampleTBottom = p0;
      double gradientBottom = 1.0 * p1 * 1000.; // [degC/m]
      double powerBottom = -1.0 * gradientBottom * kBlock_ * ABlock_ * 1e-6; // [W]

      ThroughPlaneTSampleTop_->append(m.dt.toMSecsSinceEpoch(), sampleTTop);
      ThroughPlaneTSampleMiddle_->append(m.dt.toMSecsSinceEpoch(), 0.5*(sampleTTop+sampleTBottom));
      ThroughPlaneTSampleBottom_->append(m.dt.toMSecsSinceEpoch(), sampleTBottom);

      ThroughPlanePTop_->append(m.dt.toMSecsSinceEpoch(), powerTop);
      ThroughPlanePBottom_->append(m.dt.toMSecsSinceEpoch(), powerBottom);
    }

    if (updateLegend) {
      ThroughPlaneTChart_->updateLegend();
      ThroughPlanePChart_->updateLegend();
    }
  }

  if (chillerAndVacuumActive_) {
    ChillerTSChartView_->refreshAxes();
    ChillerPPChartView_->refreshAxes();
    VacuumChartView_->refreshAxes();
  }
  UChartView_->refreshAxes();
  IChartView_->refreshAxes();
  TChartView_[0]->refreshAxes();
  TChartView_[1]->refreshAxes();
  if (martaActive_) {
    MartaPressureChartView_->refreshAxes();
    MartaTemperatureChartView_->refreshAxes();
  }
  if (chillerAndVacuumActive_ && throughPlaneActive_) {
    ThroughPlaneTChartView_->refreshAxes();
    ThroughPlanePChartView_->refreshAxes();
  }
}

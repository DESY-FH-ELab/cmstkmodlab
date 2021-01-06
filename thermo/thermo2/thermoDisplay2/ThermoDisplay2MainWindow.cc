/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
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

  w = new QWidget(tabWidget_);
  layout = new QVBoxLayout(w);

  ChillerTSChart_ = new ThermoDisplay2TemperatureStateChart();

  ChillerTBathSeries_ = new ThermoDisplay2LineSeries();
  ChillerTBathSeries_->setName("Bath");
  ChillerTSChart_->addSeries(ChillerTBathSeries_);

  /*
  {
    ChillerTBathSeries_->setEnabled(true);
    QDateTime dtEnd = QDateTime::currentDateTime();
    QDateTime dtStart = dtEnd.addSecs(-2 *  24 * 60 * 60);
    while (dtStart<=dtEnd) {
      ChillerTBathSeries_->append(dtStart.toMSecsSinceEpoch(), 1.23);
      dtStart = dtStart.addSecs(60);
    }
  }
  */
  
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

  client_ = new ThermoDAQ2Client(config->getValue<unsigned int>("ServerPort"));
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
}

void ThermoDisplay2MainWindow::clearData()
{
  ChillerTSChart_->clearData();
  ChillerPPChart_->clearData();
  VacuumPressureChart_->clearData();
  UChart_->clearData();
  IChart_->clearData();
  TChart_[0]->clearData();
  TChart_[1]->clearData();
  MartaPressureChart_->clearData();
  MartaTemperatureChart_->clearData();
}

void ThermoDisplay2MainWindow::savePlots()
{
  QDateTime dt = QDateTime::currentDateTime();

  ApplicationConfig* config = ApplicationConfig::instance();

  QString startDir = config->getValue<std::string>("PlotSaveDirectory").c_str();

  QString dir = QFileDialog::getExistingDirectory(this, "Save Directory",
                                                  startDir,
                                                  QFileDialog::ShowDirsOnly);

  if (dir.isEmpty()) return;

  int currentTab = tabWidget_->currentIndex();
  for (int idx=0;idx<4;++idx) {
    if (idx!=currentTab) tabWidget_->setCurrentIndex(idx);
  }
  tabWidget_->setCurrentIndex(currentTab);

  config->setValue("PlotSaveDirectory", dir);
  config->safe(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermo2.cfg");

  QApplication::processEvents();

  {
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

  {
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

  {
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

  {
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

  {
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
}

void ThermoDisplay2MainWindow::requestData()
{
  NQLogDebug("ThermoDisplay2MainWindow") << "requestData()";
  client_->readDAQStatus();
}

void ThermoDisplay2MainWindow::updateInfo()
{
  NQLogDebug("ThermoDisplay2MainWindow") << "updateInfo()";

  const Measurement_t& m = reader_->getMeasurement();

  {
    bool updateLegend = false;

    if (ChillerTBathSeries_->isEnabled()!=m.u525wState_) updateLegend = true;
    ChillerTBathSeries_->setEnabled(m.u525wState_);
    ChillerTReturnSeries_->setEnabled(m.u525wState_);
    ChillerTCWISeries_->setEnabled(m.u525wState_);
    ChillerTCWOSeries_->setEnabled(m.u525wState_);
    ChillerPowerSeries_->setEnabled(m.u525wState_);
    ChillerPressureSeries_->setEnabled(m.u525wState_);
    ChillerSTCSeries_->setEnabled(m.u525wState_);
    ChillerSCSeries_->setEnabled(m.u525wState_);
    ChillerTBathSeries_->append(m.dt.toMSecsSinceEpoch(), m.u525wBathTemperature_);
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

  {
    bool updateLegend = false;

    NQLogDebug("ThermoDisplay2MainWindow") << "updateInfo() " << m.leyboldState_;
    NQLogDebug("ThermoDisplay2MainWindow") << "updateInfo() " << m.leyboldPressure_;

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


  {
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

  ChillerTSChartView_->refreshAxes();
  ChillerPPChartView_->refreshAxes();
  VacuumChartView_->refreshAxes();
  UChartView_->refreshAxes();
  IChartView_->refreshAxes();
  TChartView_[0]->refreshAxes();
  TChartView_[1]->refreshAxes();
  MartaPressureChartView_->refreshAxes();
  MartaTemperatureChartView_->refreshAxes();
}

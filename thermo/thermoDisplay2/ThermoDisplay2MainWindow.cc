/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
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

  addToolBar(toolBar);

  QWidget *w;
  QVBoxLayout *layout;

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

  client_ = new ThermoDAQ2Client(config->getValue<unsigned int>("ServerPort"));
  reader_ = new ThermoDAQ2NetworkReader(this);

  QObject::connect(client_, SIGNAL(handleMessage(QString&)),
                   reader_, SLOT(run(QString&)));
  QObject::connect(reader_, SIGNAL(finished()),
                   this, SLOT(updateInfo()));

  setCentralWidget(tabWidget_);

  QPalette pal = palette();
  QColor color = pal.color(QPalette::Window);
  if (color.lightnessF()<0.4) {
    UChart_->setTheme(QChart::ChartThemeDark);
    IChart_->setTheme(QChart::ChartThemeDark);
    TChart_[0]->setTheme(QChart::ChartThemeDark);
    TChart_[1]->setTheme(QChart::ChartThemeDark);
  }

  requestData();

  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()),
          this, SLOT(requestData()));
  timer_->setSingleShot(false);
  timer_->start(5000);
}

void ThermoDisplay2MainWindow::clearData()
{
  UChart_->clearData();
  IChart_->clearData();
  TChart_[0]->clearData();
  TChart_[1]->clearData();
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

  UChartView_->refreshAxes();
  IChartView_->refreshAxes();
  TChartView_[0]->refreshAxes();
  TChartView_[1]->refreshAxes();
}

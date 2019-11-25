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

#include <qwt_symbol.h>

#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "ThermoDisplay2MainWindow.h"

ThermoDisplay2MainWindow::ThermoDisplay2MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  tabWidget_ = new QTabWidget(this);

  QToolBar *toolBar = new QToolBar(this);

  QToolButton * btnExport = new QToolButton(toolBar);
  btnExport->setText("Export");
  btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
  connect(btnExport, SIGNAL(clicked()), this, SLOT(exportPlot()));
  toolBar->addWidget(btnExport);

  QToolButton * btnClear = new QToolButton(toolBar);
  btnClear->setText("Clear");
  btnClear->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
  connect(btnClear, SIGNAL(clicked()), this, SLOT(clearData()));
  toolBar->addWidget(btnClear);

  addToolBar(toolBar);

  QWidget *w = new QWidget(tabWidget_);
  QVBoxLayout *layout = new QVBoxLayout(w);

  voltageDisplay_ = new ThermoDAQ2VoltageDisplayWidget(this);
  voltageDisplay_->setMinimumWidth(600);
  voltageDisplay_->setMinimumHeight(200);
  layout->addWidget(voltageDisplay_);

  currentDisplay_ = new ThermoDAQ2CurrentDisplayWidget(this);
  currentDisplay_->setMinimumWidth(600);
  currentDisplay_->setMinimumHeight(200);
  layout->addWidget(currentDisplay_);

  tabWidget_->addTab(w, "Power");

  U1Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("U1"), &U1_);
  U1Plot_->setPen(Qt::green, 2);
  U1Plot_->setStyle(QwtPlotCurve::Lines);
  U1Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                   QPen(Qt::green), QSize(5,5)));
  U1Plot_->setYAxis(QwtPlot::yLeft);
  U1Plot_->attachToPlot(voltageDisplay_);

  U2Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("U2"), &U2_);
  U2Plot_->setPen(Qt::blue, 2);
  U2Plot_->setStyle(QwtPlotCurve::Lines);
  U2Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                   QPen(Qt::blue), QSize(5,5)));
  U2Plot_->setYAxis(QwtPlot::yLeft);
  U2Plot_->attachToPlot(voltageDisplay_);

  U3Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("U3"), &U3_);
  U3Plot_->setPen(Qt::red, 2);
  U3Plot_->setStyle(QwtPlotCurve::Lines);
  U3Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                   QPen(Qt::red), QSize(5,5)));
  U3Plot_->setYAxis(QwtPlot::yLeft);
  U3Plot_->attachToPlot(voltageDisplay_);

  I1Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("I1"), &I1_);
  I1Plot_->setPen(Qt::green, 2);
  I1Plot_->setStyle(QwtPlotCurve::Lines);
  I1Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                   QPen(Qt::green), QSize(5,5)));
  I1Plot_->setYAxis(QwtPlot::yLeft);
  I1Plot_->attachToPlot(currentDisplay_);

  I2Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("I2"), &I2_);
  I2Plot_->setPen(Qt::blue, 2);
  I2Plot_->setStyle(QwtPlotCurve::Lines);
  I2Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                   QPen(Qt::blue), QSize(5,5)));
  I2Plot_->setYAxis(QwtPlot::yLeft);
  I2Plot_->attachToPlot(currentDisplay_);

  I3Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("I3"), &I3_);
  I3Plot_->setPen(Qt::red, 2);
  I3Plot_->setStyle(QwtPlotCurve::Lines);
  I3Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                   QPen(Qt::red), QSize(5,5)));
  I3Plot_->setYAxis(QwtPlot::yLeft);
  I3Plot_->attachToPlot(currentDisplay_);

  tempDisplay_ = new ThermoDAQ2TemperatureDisplayWidget(tabWidget_);
  tempDisplay_->setMinimumWidth(600);
  tempDisplay_->setMinimumHeight(400);
  tabWidget_->addTab(tempDisplay_, "Temperature");

//  bathTemperaturePlot_ = new ThermoDAQ2DisplayPlotItem(QwtText("Bath"), &bathTemperature_);
//  bathTemperaturePlot_->setPen(Qt::green, 2);
//  bathTemperaturePlot_->setStyle(QwtPlotCurve::Lines);
//  bathTemperaturePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
//                                                QPen(Qt::green), QSize(5,5)));
//  bathTemperaturePlot_->attachToPlot(tempDisplay_);
//
//  workingTemperaturePlot_ = new ThermoDAQ2DisplayPlotItem(QwtText("Work"), &workingTemperature_);
//  workingTemperaturePlot_->setPen(Qt::cyan, 2);
//  workingTemperaturePlot_->setStyle(QwtPlotCurve::Lines);
//  workingTemperaturePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
//                                                   QPen(Qt::cyan), QSize(5,5)));
//  workingTemperaturePlot_->attachToPlot(tempDisplay_);

  const char * colors[] = {
                           "LightSalmon",
                           "SteelBlue",
                           "Yellow",
                           "Fuchsia",
                           "PaleGreen",
                           "PaleTurquoise",
                           "Cornsilk",
                           "HotPink",
                           "Peru",
                           "Maroon"
  };

//  for (int i=0;i<10;++i) {
//    temperaturePlot_[i] = new ThermoDAQ2DisplayPlotItem(QwtText(QString("T%1").arg(i)),
//                                                        &temperature_[i]);
//    QColor color(colors[i]);
//    temperaturePlot_[i]->setPen(color, 2);
//    temperaturePlot_[i]->setStyle(QwtPlotCurve::Lines);
//    temperaturePlot_[i]->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
//                                                 QPen(color), QSize(5,5)));
//    temperaturePlot_[i]->setEnabled(false);
//    temperaturePlot_[i]->setVisible(false);
//    temperaturePlot_[i]->attachToPlot(tempDisplay_);
//  }

//  pDisplay_ = new ThermoDAQ2PressureDisplayWidget(tabWidget_);
//  pDisplay_->setMinimumWidth(600);
//  pDisplay_->setMinimumHeight(400);
//  tabWidget_->addTab(pDisplay_, "Pressure");
//
//  pressure1Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("p1"), &pressure1_);
//  pressure1Plot_->setPen(Qt::green, 2);
//  pressure1Plot_->setStyle(QwtPlotCurve::Lines);
//  pressure1Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
//                                          QPen(Qt::green), QSize(5,5)));
//  pressure1Plot_->attachToPlot(pDisplay_);
//
//  pressure2Plot_ = new ThermoDAQ2DisplayPlotItem(QwtText("p2"), &pressure2_);
//  pressure2Plot_->setPen(Qt::cyan, 2);
//  pressure2Plot_->setStyle(QwtPlotCurve::Lines);
//  pressure2Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
//                                          QPen(Qt::cyan), QSize(5,5)));
//  pressure2Plot_->attachToPlot(pDisplay_);

  client_ = new ThermoDAQ2Client(config->getValue<unsigned int>("ServerPort"));
  reader_ = new ThermoDAQ2NetworkReader(this);

  QObject::connect(client_, SIGNAL(handleMessage(QString&)),
                   reader_, SLOT(run(QString&)));
  QObject::connect(reader_, SIGNAL(finished()),
                   this, SLOT(updateInfo()));

  setCentralWidget(tabWidget_);

  requestData();

  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()),
          this, SLOT(requestData()));
  timer_->setSingleShot(false);
  timer_->start(5000);
}

void ThermoDisplay2MainWindow::exportPlot()
{
  int tabIndex = tabWidget_->currentIndex();

  if (tabIndex==0) voltageDisplay_->exportPlot();
  if (tabIndex==1) tempDisplay_->exportPlot();
//  if (tabIndex==1) pDisplay_->exportPlot();
//  if (tabIndex==2) microPressureDisplay_->exportPlot();
}

void ThermoDisplay2MainWindow::clearData()
{
  U1_.clear();
  U2_.clear();
  U3_.clear();

  I1_.clear();
  I2_.clear();
  I3_.clear();

  //  bathTemperature_.clearData();
//  workingTemperature_.clearData();
//
//  for (int i=0;i<10;++i) {
//    temperature_[i].clearData();
//  }
//  pressure1_.clearData();
//  pressure2_.clearData();
//
//  arduinoA_.clearData();
//  arduinoB_.clearData();
//  iotaActPressure_.clearData();
//  iotaSetPressure_.clearData();
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

  bool updateLegend = false;

  if (U1_.push(m.dt, m.nge103BVoltage[0])) U1Plot_->refresh();
  if (U2_.push(m.dt, m.nge103BVoltage[1])) U2Plot_->refresh();
  if (U3_.push(m.dt, m.nge103BVoltage[2])) U3Plot_->refresh();

  if (I1_.push(m.dt, m.nge103BCurrent[0])) I1Plot_->refresh();
  if (I2_.push(m.dt, m.nge103BCurrent[1])) I2Plot_->refresh();
  if (I3_.push(m.dt, m.nge103BCurrent[2])) I3Plot_->refresh();

//  if (bathTemperature_.push(m.dt, m.bathTemperature)) bathTemperaturePlot_->refresh();
//  if (workingTemperature_.push(m.dt, m.workingTemperature)) workingTemperaturePlot_->refresh();
//  for (int i=0;i<10;++i) {
//    if (m.channelActive[i]) {
//      if (temperaturePlot_[i]->isEnabled()==false) updateLegend = true;
//      temperaturePlot_[i]->setEnabled(true);
//      // if (!temperaturePlot_[i]->isVisible()) temperaturePlot_[i]->setVisible(true);
//    }
//    if (!m.channelActive[i]) {
//      if (temperaturePlot_[i]->isEnabled()==true) updateLegend = true;
//      temperaturePlot_[i]->setEnabled(false);
//      // if (temperaturePlot_[i]->isVisible()) temperaturePlot_[i]->setVisible(false);
//    }
//    if (m.channelActive[i]) {
//      if (temperature_[i].push(m.dt, m.temperature[i])) temperaturePlot_[i]->refresh();
//    }
//  }

  if (updateLegend) {
    voltageDisplay_->updateLegend();
    currentDisplay_->updateLegend();
  }
  voltageDisplay_->replot();
  currentDisplay_->replot();

//  if (pressure1_.push(m.dt, m.gaugePressure1)) pressure1Plot_->refresh();
//  if (pressure2_.push(m.dt, m.gaugePressure2)) pressure2Plot_->refresh();
//  pDisplay_->replot();
}

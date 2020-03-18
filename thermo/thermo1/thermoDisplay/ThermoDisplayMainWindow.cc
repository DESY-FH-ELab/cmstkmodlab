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

#include <iostream>
#include <string>

#include <QGroupBox>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>

#include <qwt_symbol.h>

#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "ThermoDisplayMainWindow.h"

ThermoDisplayMainWindow::ThermoDisplayMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
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

    tempDisplay_ = new ThermoDAQTemperatureDisplayWidget(tabWidget_);
    tempDisplay_->setMinimumWidth(600);
    tempDisplay_->setMinimumHeight(400);
    tabWidget_->addTab(tempDisplay_, "Temperature");

    bathTemperaturePlot_ = new ThermoDAQDisplayPlotItem(QwtText("Bath"), &bathTemperature_);
    bathTemperaturePlot_->setPen(Qt::green, 2);
    bathTemperaturePlot_->setStyle(QwtPlotCurve::Lines);
    bathTemperaturePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                                  QPen(Qt::green), QSize(5,5)));
    bathTemperaturePlot_->attachToPlot(tempDisplay_);
    workingTemperaturePlot_ = new ThermoDAQDisplayPlotItem(QwtText("Work"), &workingTemperature_);
    workingTemperaturePlot_->setPen(Qt::cyan, 2);
    workingTemperaturePlot_->setStyle(QwtPlotCurve::Lines);
    workingTemperaturePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                                     QPen(Qt::cyan), QSize(5,5)));
    workingTemperaturePlot_->attachToPlot(tempDisplay_);

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

    for (int i=0;i<10;++i) {
        temperaturePlot_[i] = new ThermoDAQDisplayPlotItem(QwtText(QString("T%1").arg(i)),
                                                                   &temperature_[i]);
        QColor color(colors[i]);
        temperaturePlot_[i]->setPen(color, 2);
        temperaturePlot_[i]->setStyle(QwtPlotCurve::Lines);
        temperaturePlot_[i]->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                                     QPen(color), QSize(5,5)));
        temperaturePlot_[i]->setEnabled(false);
        temperaturePlot_[i]->setVisible(false);
        temperaturePlot_[i]->attachToPlot(tempDisplay_);
    }

    pDisplay_ = new ThermoDAQPressureDisplayWidget(tabWidget_);
    pDisplay_->setMinimumWidth(600);
    pDisplay_->setMinimumHeight(400);
    tabWidget_->addTab(pDisplay_, "Pressure");

    pressure1Plot_ = new ThermoDAQDisplayPlotItem(QwtText("p1"), &pressure1_);
    pressure1Plot_->setPen(Qt::green, 2);
    pressure1Plot_->setStyle(QwtPlotCurve::Lines);
    pressure1Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                            QPen(Qt::green), QSize(5,5)));
    pressure1Plot_->attachToPlot(pDisplay_);
    pressure2Plot_ = new ThermoDAQDisplayPlotItem(QwtText("p2"), &pressure2_);
    pressure2Plot_->setPen(Qt::cyan, 2);
    pressure2Plot_->setStyle(QwtPlotCurve::Lines);
    pressure2Plot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                            QPen(Qt::cyan), QSize(5,5)));
    pressure2Plot_->attachToPlot(pDisplay_);

    microPressureDisplay_ = new ThermoDAQMicroPressureDisplayWidget(tabWidget_);
    microPressureDisplay_->setMinimumWidth(600);
    microPressureDisplay_->setMinimumHeight(400);
    tabWidget_->addTab(microPressureDisplay_, "Microchannel Pressure");

    arduinoAPlot_ = new ThermoDAQDisplayPlotItem(QwtText("arduino A"), &arduinoA_);
    arduinoAPlot_->setPen(Qt::green, 2);
    arduinoAPlot_->setStyle(QwtPlotCurve::Lines);
    arduinoAPlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                            QPen(Qt::green), QSize(5,5)));
    arduinoAPlot_->attachToPlot(microPressureDisplay_);
    arduinoBPlot_ = new ThermoDAQDisplayPlotItem(QwtText("arduino B"), &arduinoB_);
    arduinoBPlot_->setPen(Qt::cyan, 2);
    arduinoBPlot_->setStyle(QwtPlotCurve::Lines);
    arduinoBPlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
					   QPen(Qt::cyan), QSize(5,5)));
    arduinoBPlot_->attachToPlot(microPressureDisplay_);
    iotaActPressurePlot_ = new ThermoDAQDisplayPlotItem(QwtText("iota measured"), &iotaActPressure_);
    iotaActPressurePlot_->setPen(Qt::red, 2);
    iotaActPressurePlot_->setStyle(QwtPlotCurve::Lines);
    iotaActPressurePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
						  QPen(Qt::red), QSize(5,5)));
    iotaActPressurePlot_->attachToPlot(microPressureDisplay_);
    iotaSetPressurePlot_ = new ThermoDAQDisplayPlotItem(QwtText("iota set"), &iotaSetPressure_);
    iotaSetPressurePlot_->setPen(Qt::blue, 2);
    iotaSetPressurePlot_->setStyle(QwtPlotCurve::Lines);
    iotaSetPressurePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
						  QPen(Qt::blue), QSize(5,5)));
    iotaSetPressurePlot_->attachToPlot(microPressureDisplay_);

    client_ = new ThermoDAQClient(55555);
    reader_ = new ThermoDAQNetworkReader(this);

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

void ThermoDisplayMainWindow::exportPlot()
{
    int tabIndex = tabWidget_->currentIndex();

    if (tabIndex==0) tempDisplay_->exportPlot();
    if (tabIndex==1) pDisplay_->exportPlot();
    if (tabIndex==2) microPressureDisplay_->exportPlot();
}

void ThermoDisplayMainWindow::clearData()
{
    bathTemperature_.clearData();
    workingTemperature_.clearData();

    for (int i=0;i<10;++i) {
        temperature_[i].clearData();
    }
    pressure1_.clearData();
    pressure2_.clearData();

    arduinoA_.clearData();
    arduinoB_.clearData();
    iotaActPressure_.clearData();
    iotaSetPressure_.clearData();
}

void ThermoDisplayMainWindow::requestData()
{
    NQLog("ThermoDisplayMainWindow", NQLog::Debug) << "requestData()";
    client_->readDAQStatus();
}

void ThermoDisplayMainWindow::updateInfo()
{
    NQLog("ThermoDisplayMainWindow", NQLog::Debug) << "updateInfo()";

    const Measurement_t& m = reader_->getMeasurement();

    bool updateLegend = false;

    if (bathTemperature_.push(m.dt, m.bathTemperature)) bathTemperaturePlot_->refresh();
    if (workingTemperature_.push(m.dt, m.workingTemperature)) workingTemperaturePlot_->refresh();
    for (int i=0;i<10;++i) {
        if (m.channelActive[i]) {
            if (temperaturePlot_[i]->isEnabled()==false) updateLegend = true;
            temperaturePlot_[i]->setEnabled(true);
            // if (!temperaturePlot_[i]->isVisible()) temperaturePlot_[i]->setVisible(true);
        }
        if (!m.channelActive[i]) {
            if (temperaturePlot_[i]->isEnabled()==true) updateLegend = true;
            temperaturePlot_[i]->setEnabled(false);
            // if (temperaturePlot_[i]->isVisible()) temperaturePlot_[i]->setVisible(false);
        }
        if (m.channelActive[i]) {
            if (temperature_[i].push(m.dt, m.temperature[i])) temperaturePlot_[i]->refresh();
        }
    }

    if (updateLegend) tempDisplay_->updateLegend();
    tempDisplay_->replot();

    if (pressure1_.push(m.dt, m.gaugePressure1)) pressure1Plot_->refresh();
    if (pressure2_.push(m.dt, m.gaugePressure2)) pressure2Plot_->refresh();
    pDisplay_->replot();

    if (arduinoA_.push(m.dt, m.arduinoPressureA)) arduinoAPlot_->refresh();
    if (arduinoB_.push(m.dt, m.arduinoPressureB)) arduinoBPlot_->refresh();
    if (iotaActPressure_.push(m.dt, m.iotaActPressure)) iotaActPressurePlot_->refresh();
    if (iotaSetPressure_.push(m.dt, m.iotaSetPressure)) iotaSetPressurePlot_->refresh();
    microPressureDisplay_->replot();
}

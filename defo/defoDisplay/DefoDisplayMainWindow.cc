#include <iostream>
#include <string>

#include <QGroupBox>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>

#include <qwt_symbol.h>

#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "DefoDisplayMainWindow.h"

DefoDisplayMainWindow::DefoDisplayMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
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

    tempDisplay_ = new DefoDAQTemperatureDisplayWidget(this);
    tempDisplay_->setMinimumWidth(600);
    tempDisplay_->setMinimumHeight(400);
    
    bathTemperaturePlot_ = new DefoDAQDisplayPlotItem(QwtText("Bath"), &bathTemperature_);
    bathTemperaturePlot_->setPen(Qt::green, 2);
    bathTemperaturePlot_->setStyle(QwtPlotCurve::Lines);
    bathTemperaturePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                                  QPen(Qt::green), QSize(5,5)));
    bathTemperaturePlot_->attachToPlot(tempDisplay_);
    workingTemperaturePlot_ = new DefoDAQDisplayPlotItem(QwtText("Work"), &workingTemperature_);
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
        temperaturePlot_[i] = new DefoDAQDisplayPlotItem(QwtText(QString("T%1").arg(i)),
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

    client_ = new DefoDAQClient(55556);
    reader_ = new DefoDAQNetworkReader(this);

    QObject::connect(client_, SIGNAL(handleMessage(QString&)),
                     reader_, SLOT(run(QString&)));
    QObject::connect(reader_, SIGNAL(finished()),
                     this, SLOT(updateInfo()));

    setCentralWidget(tempDisplay_);

    requestData();

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()),
            this, SLOT(requestData()));
    timer_->setSingleShot(false);
    timer_->start(5000);
}

void DefoDisplayMainWindow::exportPlot()
{
  tempDisplay_->exportPlot();
}

void DefoDisplayMainWindow::clearData()
{
    bathTemperature_.clearData();
    workingTemperature_.clearData();

    for (int i=0;i<10;++i) {
        temperature_[i].clearData();
    }
}

void DefoDisplayMainWindow::requestData()
{
    NQLog("DefoDisplayMainWindow", NQLog::Debug) << "requestData()";
    client_->readDAQStatus();
}

void DefoDisplayMainWindow::updateInfo()
{
    NQLog("DefoDisplayMainWindow", NQLog::Debug) << "updateInfo()";

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
}

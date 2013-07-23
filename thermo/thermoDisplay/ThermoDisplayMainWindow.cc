#include <iostream>
#include <string>

#include <QGroupBox>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>

#include <qwt_symbol.h>

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
}

void ThermoDisplayMainWindow::clearData()
{
    bathTemperature_.clearData();
}

void ThermoDisplayMainWindow::requestData()
{
    // std::cout << "void ThermoDisplayMainWindow::requestData()" << std::endl;

    client_->readDAQStatus();
}

void ThermoDisplayMainWindow::updateInfo()
{
    // std::cout << "void ThermoDisplayMainWindow::updateInfo()" << std::endl;

    const Measurement_t& m = reader_->getMeasurement();

    bool updateLegend = false;

    if (bathTemperature_.push(m.dt, m.bathTemperature)) bathTemperaturePlot_->refresh();
    if (workingTemperature_.push(m.dt, m.workingTemperature)) workingTemperaturePlot_->refresh();
    for (int i=0;i<10;++i) {
        if (m.channelActive[i]) {
            if (temperaturePlot_[i]->isEnabled()==false) updateLegend = true;
            temperaturePlot_[i]->setEnabled(true);
            if (!temperaturePlot_[i]->isVisible())
                temperaturePlot_[i]->setVisible(true);
        }
        if (!m.channelActive[i]) {
            if (temperaturePlot_[i]->isEnabled()==true) updateLegend = true;
            temperaturePlot_[i]->setEnabled(false);
            if (temperaturePlot_[i]->isVisible())
                temperaturePlot_[i]->setVisible(false);
        }
        if (m.channelActive[i]) {
            if (temperature_[i].push(m.dt, m.temperature[i])) temperaturePlot_[i]->refresh();
        }
    }

    if (updateLegend) tempDisplay_->updateLegend();
    tempDisplay_->replot();
    //tempDisplay_->updateZoomBase();

    if (pressure1_.push(m.dt, m.gaugePressure1)) pressure1Plot_->refresh();
    if (pressure2_.push(m.dt, m.gaugePressure2)) pressure2Plot_->refresh();

    pDisplay_->replot();
    //pDisplay_->updateZoomBase();
}

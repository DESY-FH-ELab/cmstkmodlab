#include <iostream>
#include <string>

#include <QGroupBox>
#include <QFileDialog>

#include "ApplicationConfig.h"

#include "ThermoDisplayMainWindow.h"

ThermoDisplayMainWindow::ThermoDisplayMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    tabWidget_ = new QTabWidget(this);

    tempDisplay_ = new ThermoDAQTemperatureDisplayWidget(tabWidget_);
    tempDisplay_->setMinimumWidth(600);
    tempDisplay_->setMinimumHeight(400);
    tabWidget_->addTab(tempDisplay_, "Temperature");

    bathTemperaturePlot_ = new ThermoDAQDisplayPlotItem<double>(QwtText("Bath"), &bathTemperature_);
    bathTemperaturePlot_->setPen(Qt::green);
    bathTemperaturePlot_->attachToPlot(tempDisplay_);
    workingTemperaturePlot_ = new ThermoDAQDisplayPlotItem<double>(QwtText("Working"), &workingTemperature_);
    workingTemperaturePlot_->setPen(Qt::cyan);
    workingTemperaturePlot_->attachToPlot(tempDisplay_);
    for (int i=0;i<10;++i) {
        temperaturePlot_[i] = new ThermoDAQDisplayPlotItem<double>(QwtText(QString("T%1").arg(i)),
                                                                   &temperature_[i]);
        temperaturePlot_[i]->setPen(Qt::red);
    }

    pDisplay_ = new ThermoDAQPressureDisplayWidget(tabWidget_);
    pDisplay_->setMinimumWidth(600);
    pDisplay_->setMinimumHeight(400);
    tabWidget_->addTab(pDisplay_, "Pressure");

    pressure1Plot_ = new ThermoDAQDisplayPlotItem<double>(QwtText("p1"), &pressure1_);
    pressure1Plot_->setPen(Qt::green);
    pressure1Plot_->attachToPlot(pDisplay_);
    pressure2Plot_ = new ThermoDAQDisplayPlotItem<double>(QwtText("p2"), &pressure2_);
    pressure2Plot_->setPen(Qt::cyan);
    pressure2Plot_->attachToPlot(pDisplay_);

    client_ = new ThermoDAQClient(55555);
    reader_ = new ThermoDAQNetworkReader(this);

    QObject::connect(client_, SIGNAL(handleMessage(QString&)),
                     reader_, SLOT(run(QString&)));
    QObject::connect(reader_, SIGNAL(finished()),
                     this, SLOT(updateInfo()));

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()),
            this, SLOT(requestData()));
    timer_->setSingleShot(true);
    timer_->start(1000);

    setCentralWidget(tabWidget_);
}

void ThermoDisplayMainWindow::requestData()
{
    std::cout << "void ThermoDisplayMainWindow::requestData()" << std::endl;

    client_->readDAQStatus();
}

void ThermoDisplayMainWindow::updateInfo()
{
    std::cout << "void ThermoDisplayMainWindow::updateInfo()" << std::endl;

    const Measurement_t& m = reader_->getMeasurement();

    if (bathTemperature_.push(m.dt, m.bathTemperature)) bathTemperaturePlot_->refresh();
    if (workingTemperature_.push(m.dt, m.workingTemperature)) workingTemperaturePlot_->refresh();
    for (int i=0;i<10;++i) {
        if (m.channelActive[i] && !temperaturePlot_[i]->isVisible())
            temperaturePlot_[i]->setVisible(true);
        if (!m.channelActive[i] && temperaturePlot_[i]->isVisible())
            temperaturePlot_[i]->setVisible(false);

        if (m.channelActive[i]) {
            if (temperature_[i].push(m.dt, m.temperature[i])) temperaturePlot_[i]->refresh();
        }
    }

    tempDisplay_->replot();
    //tempDisplay_->updateZoomBase();

    if (pressure1_.push(m.dt, m.gaugePressure1)) pressure1Plot_->refresh();
    if (pressure2_.push(m.dt, m.gaugePressure2)) pressure2Plot_->refresh();

    pDisplay_->replot();
    //pDisplay_->updateZoomBase();

    timer_->start(5000);
}

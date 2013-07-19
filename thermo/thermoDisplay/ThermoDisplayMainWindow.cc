#include <iostream>
#include <string>

#include <QGroupBox>
#include <QFileDialog>

#include "ApplicationConfig.h"

#include "ThermoDisplayMainWindow.h"

ThermoDisplayMainWindow::ThermoDisplayMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
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

    bath_.pushIfChanged(m.dt, m.bathTemperature);

    timer_->start(10000);
}

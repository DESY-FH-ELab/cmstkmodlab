#include <iostream>
#include <string>

#include <QGroupBox>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>

#include <qwt_symbol.h>

#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "MicroDisplayMainWindow.h"

MicroDisplayMainWindow::MicroDisplayMainWindow(QWidget *parent) :
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

    microPressureDisplay_ = new MicroDAQMicroPressureDisplayWidget(tabWidget_);
    microPressureDisplay_->setMinimumWidth(600);
    microPressureDisplay_->setMinimumHeight(400);
    tabWidget_->addTab(microPressureDisplay_, "Microchannel Pressure");

    arduinoAPlot_ = new MicroDAQDisplayPlotItem(QwtText("arduino A"), &arduinoA_);
    arduinoAPlot_->setPen(Qt::green, 2);
    arduinoAPlot_->setStyle(QwtPlotCurve::Lines);
    arduinoAPlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                            QPen(Qt::green), QSize(5,5)));
    arduinoAPlot_->attachToPlot(microPressureDisplay_);
    arduinoBPlot_ = new MicroDAQDisplayPlotItem(QwtText("arduino B"), &arduinoB_);
    arduinoBPlot_->setPen(Qt::cyan, 2);
    arduinoBPlot_->setStyle(QwtPlotCurve::Lines);
    arduinoBPlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
					   QPen(Qt::cyan), QSize(5,5)));
    arduinoBPlot_->attachToPlot(microPressureDisplay_);
    iotaActPressurePlot_ = new MicroDAQDisplayPlotItem(QwtText("iota measured"), &iotaActPressure_);
    iotaActPressurePlot_->setPen(Qt::red, 2);
    iotaActPressurePlot_->setStyle(QwtPlotCurve::Lines);
    iotaActPressurePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
						  QPen(Qt::red), QSize(5,5)));
    iotaActPressurePlot_->attachToPlot(microPressureDisplay_);
    iotaSetPressurePlot_ = new MicroDAQDisplayPlotItem(QwtText("iota set"), &iotaSetPressure_);
    iotaSetPressurePlot_->setPen(Qt::blue, 2);
    iotaSetPressurePlot_->setStyle(QwtPlotCurve::Lines);
    iotaSetPressurePlot_->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
						  QPen(Qt::blue), QSize(5,5)));
    iotaSetPressurePlot_->attachToPlot(microPressureDisplay_);

    client_ = new MicroDAQClient(55556);
    reader_ = new MicroDAQNetworkReader(this);

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

void MicroDisplayMainWindow::exportPlot()
{
    int tabIndex = tabWidget_->currentIndex();

    if (tabIndex==0) microPressureDisplay_->exportPlot();
}

void MicroDisplayMainWindow::clearData()
{
    arduinoA_.clearData();
    arduinoB_.clearData();
    iotaActPressure_.clearData();
    iotaSetPressure_.clearData();
}

void MicroDisplayMainWindow::requestData()
{
    NQLog("MicroDisplayMainWindow", NQLog::Debug) << "requestData()";
    client_->readDAQStatus();
}

void MicroDisplayMainWindow::updateInfo()
{
    NQLog("MicroDisplayMainWindow", NQLog::Debug) << "updateInfo()";

    const Measurement_t& m = reader_->getMeasurement();

    if (arduinoA_.push(m.dt, m.arduinoPressureA)) arduinoAPlot_->refresh();
    if (arduinoB_.push(m.dt, m.arduinoPressureB)) arduinoBPlot_->refresh();
    if (iotaActPressure_.push(m.dt, m.iotaActPressure)) iotaActPressurePlot_->refresh();
    if (iotaSetPressure_.push(m.dt, m.iotaSetPressure)) iotaSetPressurePlot_->refresh();
    microPressureDisplay_->replot();
}

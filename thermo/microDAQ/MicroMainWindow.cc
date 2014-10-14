#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>

#include "ApplicationConfig.h"
#include "MicroDAQWidget.h"
#include "MicroScriptWidget.h"

#include "MicroMainWindow.h"

MicroMainWindow::MicroMainWindow(QWidget *parent) :
    QMainWindow(parent),
    daqThread_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    // IOTA MODEL
    iotaModel_ = new IotaModel(config->getValue<std::string>("IotaDevice").c_str(),
                               2, this);

    // ARDUINO PRES MODEL
    arduinoPresModel_ = new ArduinoPresModel(config->getValue<std::string>("ArduinoPresDevice").c_str(),
                                             2, this);
    
    daqModel_ = new MicroDAQModel(iotaModel_,
                                  arduinoPresModel_,
                                  this);

    // SCRIPT MODEL
    scriptModel_ = new MicroScriptModel(daqModel_,
                                        iotaModel_,
                                        arduinoPresModel_,
                                        this);

    daqStreamer_ = new MicroDAQStreamer(daqModel_, this);

    daqServer_ = new MicroDAQServer(daqModel_, this);
    daqServer_->listen(QHostAddress::LocalHost, 55556);

    daqThread_ = new MicroDAQThread(daqModel_, this);
    connect(QApplication::instance(), SIGNAL(aboutToQuit()),
            this, SLOT(quit()));
    daqThread_->start();
    daqModel_->myMoveToThread(daqThread_);

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QWidget * widget = new QWidget();
    QBoxLayout * wlayout = new QVBoxLayout();
    widget->setLayout(wlayout);

    widget = new QWidget();
    wlayout = new QHBoxLayout();
    widget->setLayout(wlayout);

    // IOTA MODEL
    IotaWidget* iotaWidget = new IotaWidget(iotaModel_, widget);
    iotaWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(iotaWidget);

    // ARDUINO PRES MODEL
    ArduinoPresWidget* arduinoPresWidget = new ArduinoPresWidget(arduinoPresModel_, widget);
    arduinoPresWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(arduinoPresWidget);

    tabWidget_->addTab(widget, "Microchannel");

    widget = new QWidget();
    wlayout = new QVBoxLayout();
    widget->setLayout(wlayout);

    // DAQ MODEL
    MicroDAQWidget* daqWidget = new MicroDAQWidget(daqModel_, widget);
    daqWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(daqWidget);

    // SCRIPT MODEL
    MicroScriptWidget* scriptWidget = new MicroScriptWidget(scriptModel_, widget);
    scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(scriptWidget);

    tabWidget_->addTab(widget, "DAQ + Scripting");

    setCentralWidget(tabWidget_);
    updateGeometry();
}

void MicroMainWindow::quit()
{
    if (daqThread_) {
        daqThread_->quit();
        daqThread_->wait();
    }
}

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>

#include "ApplicationConfig.h"
#include "ThermoDAQWidget.h"
#include "ThermoScriptWidget.h"

#include "ThermoMainWindow.h"

ThermoMainWindow::ThermoMainWindow(QWidget *parent) :
    QMainWindow(parent),
    daqThread_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    // HUBER MODEL
    huberModel_ = new HuberPetiteFleurModel(config->getValue<std::string>("HuberPetiteFleurDevice").c_str(),
                                            5, this);

    // KEITHLEY MODEL
    keithleyModel_ = new KeithleyModel(config->getValue<std::string>("KeithleyDevice").c_str(),
                                       20, this);

    // HAMEG MODEL
    hamegModel_ = new HamegModel(config->getValue<std::string>("HamegDevice").c_str(),
                                 10, this);

    // PFEIFFER MODEL
    pfeifferModel_ = new PfeifferModel(config->getValue<std::string>("PfeifferDevice").c_str(),
                                       10, this);

    daqModel_ = new ThermoDAQModel(huberModel_,
                                   keithleyModel_,
                                   hamegModel_,
                                   pfeifferModel_,
                                   this);

    // SCRIPT MODEL
    scriptModel_ = new ThermoScriptModel(daqModel_,
                                         huberModel_,
                                         keithleyModel_,
                                         hamegModel_,
                                         pfeifferModel_,
                                         this);

    daqStreamer_ = new ThermoDAQStreamer(daqModel_, this);

    daqServer_ = new ThermoDAQServer(daqModel_, this);
    daqServer_->listen(QHostAddress::LocalHost, 55555);
    //std::cout << daqServer_->serverPort() << std::endl;

    daqThread_ = new ThermoDAQThread(daqModel_, this);
    connect(QApplication::instance(), SIGNAL(aboutToQuit()),
            this, SLOT(quit()));
    daqThread_->start();
    daqModel_->myMoveToThread(daqThread_);

    //  connect(scriptModel_, SIGNAL(prepareNewMeasurement()),
    //	  this, SLOT(prepareNewMeasurement()));
    //  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
    //	  conradModel_, SLOT(setControlsEnabled(bool)));
    //  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
    //	  cameraModel_, SLOT(setControlsEnabled(bool)));
    //  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
    //	  julaboModel_, SLOT(setControlsEnabled(bool)));
    //  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
    //	  keithleyModel_, SLOT(setControlsEnabled(bool)));
    //  connect(scriptModel_, SIGNAL(setControlsEnabled(bool)),
    //	  pointModel_, SLOT(setControlsEnabled(bool)));

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QWidget * widget = new QWidget();
    QBoxLayout * wlayout = new QVBoxLayout();
    widget->setLayout(wlayout);

    // HUBER MODEL
    HuberPetiteFleurWidget* huberWidget = new HuberPetiteFleurWidget(huberModel_, widget);
    huberWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(huberWidget);

    // PFEIFFER MODEL
    //PfeifferWidget* pfeifferWidget = new PfeifferWidget(pfeifferModel_, widget);
    ThermoPfeifferWidget* pfeifferWidget = new ThermoPfeifferWidget(pfeifferModel_, widget);
    pfeifferWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(pfeifferWidget);

    tabWidget_->addTab(widget, "Chiller + Vacuum");

    // HAMEG MODEL
    HamegWidget* hamegWidget = new HamegWidget(hamegModel_);
    hamegWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(hamegWidget, "Power Supply");

    // KEITHLEY MODEL
    KeithleyWidget* keithleyWidget = new KeithleyWidget(keithleyModel_);
    keithleyWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(keithleyWidget, "Multimeter");

    widget = new QWidget();
    wlayout = new QVBoxLayout();
    widget->setLayout(wlayout);

    // DAQ MODEL
    ThermoDAQWidget* daqWidget = new ThermoDAQWidget(daqModel_, widget);
    daqWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(daqWidget);

    // SCRIPT MODEL
    ThermoScriptWidget* scriptWidget = new ThermoScriptWidget(scriptModel_, widget);
    scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    wlayout->addWidget(scriptWidget);

    tabWidget_->addTab(widget, "DAQ + Scripting");

    setCentralWidget(tabWidget_);
    updateGeometry();
}

void ThermoMainWindow::quit()
{
    std::cout << "void ThermoMainWindow::quit()" << std::endl;
    if (daqThread_) {
      daqThread_->quit();
      daqThread_->wait();
    }
}

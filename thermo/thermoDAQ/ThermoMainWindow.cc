#include <string>

#include <QGroupBox>
#include <QFileDialog>

#include "ApplicationConfig.h"
#include "ThermoDAQWidget.h"
#include "ThermoScriptWidget.h"

#include "ThermoMainWindow.h"

ThermoMainWindow::ThermoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // HUBER MODEL
    huberModel_ = new HuberPetiteFleurModel(15, this);

    // KEITHLEY MODEL
    keithleyModel_ = new KeithleyModel("/dev/ttyS5", 30, this);

    // HAMEG MODEL
    hamegModel_ = new HamegModel(10, this);

    // PFEIFFER MODEL
    pfeifferModel_ = new PfeifferModel(10, this);

    // SCRIPT MODEL
    scriptModel_ = new ThermoScriptModel(huberModel_,
                                         keithleyModel_,
                                         hamegModel_,
                                         pfeifferModel_,
                                         this);

    daqModel_ = new ThermoDAQModel(huberModel_,
                                   keithleyModel_,
                                   hamegModel_,
                                   pfeifferModel_,
                                   this);

    daqStreamer_ = new ThermoDAQStreamer(daqModel_, this);

    daqServer_ = new ThermoDAQServer(daqModel_, this);
    daqServer_->listen(QHostAddress::LocalHost, 55555);
    //std::cout << daqServer_->serverPort() << std::endl;

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
    PfeifferWidget* pfeifferWidget = new PfeifferWidget(pfeifferModel_, widget);
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
}

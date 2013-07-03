#include <string>

#include <QGroupBox>
#include <QFileDialog>

#include "ApplicationConfig.h"
#include "ThermoDAQWidget.h"
#include "ThermoDAQDisplayWidget.h"
#include "ThermoScriptWidget.h"

#include "ThermoMainWindow.h"

ThermoMainWindow::ThermoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // JULABO MODEL
    julaboModel_ = new JulaboModel(5, this);

    // KEITHLEY MODEL
    keithleyModel_ = new KeithleyModel(1, this);

    // HAMEG MODEL
    hamegModel_ = new HamegModel(5, this);

    // PFEIFFER MODEL
    pfeifferModel_ = new PfeifferModel(5, this);

    // SCRIPT MODEL
    scriptModel_ = new ThermoScriptModel(julaboModel_,
                                         keithleyModel_,
                                         hamegModel_,
                                         pfeifferModel_,
                                         this);

    daqModel_ = new ThermoDAQModel(julaboModel_,
                                   keithleyModel_,
                                   hamegModel_,
                                   pfeifferModel_,
                                   this);
    daqStreamer_ = new ThermoDAQStreamer(daqModel_, this);

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

    // JULABO MODEL
    JulaboWidget* julaboWidget = new JulaboWidget(julaboModel_);
    julaboWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(julaboWidget, "Chiller");

    // HAMEG MODEL
    HamegWidget* hamegWidget = new HamegWidget(hamegModel_);
    hamegWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(hamegWidget, "Power Supply");

    // KEITHLEY MODEL
    KeithleyWidget* keithleyWidget = new KeithleyWidget(keithleyModel_);
    keithleyWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(keithleyWidget, "Multimeter");

    // PFEIFFER MODEL
    PfeifferWidget* pfeifferWidget = new PfeifferWidget(pfeifferModel_);
    pfeifferWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(pfeifferWidget, "Vacuum");

    // DAQ MODEL
    ThermoDAQWidget* daqWidget = new ThermoDAQWidget(daqModel_);
    daqWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(daqWidget, "DAQ");

    // SCRIPT MODEL
    ThermoScriptWidget* scriptWidget = new ThermoScriptWidget(scriptModel_);
    scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(scriptWidget, "Scripting");

    // DISPLAY
    ThermoDAQDisplayWidget* displayWidget = new ThermoDAQDisplayWidget(daqModel_);
    displayWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    tabWidget_->addTab(displayWidget, "Display");

    setCentralWidget(tabWidget_);
}

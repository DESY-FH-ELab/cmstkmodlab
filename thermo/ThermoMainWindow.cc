#include <string>

#include <QGroupBox>
#include <QFileDialog>

#include "ApplicationConfig.h"
#include "ThermoScriptWidget.h"

#include "ThermoMainWindow.h"

ThermoMainWindow::ThermoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  // JULABO MODEL
  julaboModel_ = new JulaboModel(5);

  // KEITHLEY MODEL
  keithleyModel_ = new KeithleyModel(15);

  // HAMEG MODEL
  hamegModel_ = new HamegModel(5);

  // PFEIFFER MODEL
  pfeifferModel_ = new PfeifferModel(5);

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

//  prepareNewMeasurement();

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

//  QBoxLayout *layout = new QHBoxLayout();
//  QWidget *chillerAndControlWidget = new QWidget(tabWidget_);
//  chillerAndControlWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  chillerAndControlWidget->setLayout(layout);

//  QBoxLayout * vlayout = new QVBoxLayout();
//  QWidget *widget = new QWidget(chillerAndControlWidget);
//  widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  widget->setLayout(vlayout);

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

  // SCRIPT MODEL
  ThermoScriptWidget* scriptWidget = new ThermoScriptWidget(scriptModel_);
  scriptWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  tabWidget_->addTab(scriptWidget, "Scripting");

//  vbox = new QVBoxLayout;
//  vbox->addWidget(scriptWidget);
//  vbox->addStretch(1);
//  groupbox->setLayout(vbox);

//  vlayout->addWidget(groupbox);

//  layout->addWidget(widget);

//  tabWidget_->addTab(chillerAndControlWidget, "Chiller and Control");

//  layout = new QHBoxLayout();
//  QWidget *measurementWidget = new QWidget(tabWidget_);
//  measurementWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  measurementWidget->setLayout(layout);

//  layout->addWidget(new ThermoCameraWidget(
//					 cameraModel_
//                   , conradModel_
//                   , listModel_
//                   , selectionModel_
//                   , this
//					 ));

//  if (cameraModel_->getDeviceState() == READY) {
//    cameraModel_->setOptionSelection(ThermoCameraModel::APERTURE, 6);
//    cameraModel_->setOptionSelection(ThermoCameraModel::ISO, 1);
//    cameraModel_->setOptionSelection(ThermoCameraModel::SHUTTER_SPEED, 26);
//  }

//  vlayout = new QVBoxLayout();
//  widget = new QWidget(measurementWidget);
//  //widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  widget->setLayout(vlayout);

//  // Camera buttons
//  QWidget* buttons = new QWidget(widget);
//  // buttons->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  vlayout->addWidget(buttons);

//  QGridLayout *buttonLayout = new QGridLayout();
//  buttons->setLayout(buttonLayout);

//  QPushButton *exportMeasurementButton = new QPushButton("&Export Measurement", buttons);
//  connect(exportMeasurementButton, SIGNAL(clicked()), this, SLOT(exportMeasurement()));
//  buttonLayout->addWidget(exportMeasurementButton, 0, 0);

//  QPushButton *newMeasurementButton = new QPushButton("&New Measurement", buttons);
//  connect(newMeasurementButton, SIGNAL(clicked()), this, SLOT(prepareNewMeasurement()));
//  buttonLayout->addWidget(newMeasurementButton, 0, 1);

//  // read default settings
//  pointModel_->setThresholdValue(
//        ThermoPointRecognitionModel::THRESHOLD_1
//        , ThermoConfig::instance()->getValue<int>( "STEP1_THRESHOLD" )
//        );
//  pointModel_->setThresholdValue(
//        ThermoPointRecognitionModel::THRESHOLD_2
//        , ThermoConfig::instance()->getValue<int>( "STEP2_THRESHOLD" )
//        );
//  pointModel_->setThresholdValue(
//        ThermoPointRecognitionModel::THRESHOLD_3
//        , ThermoConfig::instance()->getValue<int>( "STEP3_THRESHOLD" )
//        );
//  pointModel_->setHalfSquareWidth(
//        ThermoConfig::instance()->getValue<int>( "HALF_SQUARE_WIDTH" )
//        );

//  ThermoPointRecognitionWidget * pointWidget =
//      new ThermoPointRecognitionWidget(listModel_,
//                                     selectionModel_,
//                                     pointModel_,
//                                     widget);
//  vlayout->addWidget(pointWidget);

//  layout->addWidget(widget);

//  tabWidget_->addTab(measurementWidget, "Measurement");

//  layout = new QHBoxLayout();
//  QWidget *temperatureWidget = new QWidget(tabWidget_);
//  temperatureWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//  temperatureWidget->setLayout(layout);

//  // KEITHLEY MODEL
//  ThermoKeithleyWidget *keithleyWidget = new ThermoKeithleyWidget(keithleyModel_);
//  layout->addWidget( keithleyWidget );

//  tabWidget_->addTab(temperatureWidget, "Temperature");

    setCentralWidget(tabWidget_);
}

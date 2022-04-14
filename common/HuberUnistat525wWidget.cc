/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "HuberUnistat525wWidget.h"

HuberUnistat525wTemperatureControlModeBox::HuberUnistat525wTemperatureControlModeBox(HuberUnistat525wModel* model,
                                                                                     QWidget *parent)
 : QComboBox(parent),
   model_(model)
{
  addItem("internal", 0);
  addItem("process", 1);

  int idx = findData(model_->getTemperatureControlMode());
  if (idx!=-1) setCurrentIndex(idx);

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInformation()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(currentItemChanged(int)));

  connect(this, SIGNAL(valueChanged(bool)),
          model_, SLOT(setTemperatureControlMode(bool)));
}

void HuberUnistat525wTemperatureControlModeBox::currentItemChanged(int idx)
{
  emit(valueChanged(itemData(idx).toBool()));
}

void HuberUnistat525wTemperatureControlModeBox::updateInformation()
{
  if (hasFocus()) return;
  
  int idx = findData(model_->getTemperatureControlMode());
  if (idx!=-1) setCurrentIndex(idx);
}

HuberUnistat525wWidget::HuberUnistat525wWidget(HuberUnistat525wModel* model,
                                               QWidget *parent)
 : QWidget(parent),
   model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  huberUnistat525wCheckBox_ = new QCheckBox("Enable Chiller");
  layout->addWidget(huberUnistat525wCheckBox_);

  operationPanel_ = new QWidget(this);
  QFormLayout* operationLayout = new QFormLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);
  layout->addWidget(operationPanel_);

  temperatureSetPointSpinner_ = new QDoubleSpinBox(operationPanel_);
  temperatureSetPointSpinner_->setSingleStep(1.0);
  temperatureSetPointSpinner_->setKeyboardTracking(false);
  temperatureSetPointSpinner_->setMinimum(-40);
  temperatureSetPointSpinner_->setMaximum(+30);
  temperatureSetPointSpinner_->setDecimals(2);
  temperatureSetPointSpinner_->setAlignment(Qt::AlignRight);

  operationLayout->addRow(QString::fromUtf8("Temperature Setpoint [°C]"),
                          temperatureSetPointSpinner_);

  autoPIDCheckBox_ = new QCheckBox("enabled");
  operationLayout->addRow("auto PID",
                          autoPIDCheckBox_);

  temperatureControlModeBox_ = new HuberUnistat525wTemperatureControlModeBox(model_);
  operationLayout->addRow(QString::fromUtf8("Temperature Control Mode"),
                          temperatureControlModeBox_);

  kpSpinBox_ = new QSpinBox();
  kpSpinBox_->setSingleStep(1);
  kpSpinBox_->setKeyboardTracking(false);
  kpSpinBox_->setMinimum(20);
  kpSpinBox_->setMaximum(1000);
  kpSpinBox_->setAlignment(Qt::AlignRight);
  operationLayout->addRow(QString::fromUtf8("Kp"),
                          kpSpinBox_);

  tnSpinBox_ = new QDoubleSpinBox();
  tnSpinBox_->setSingleStep(1);
  tnSpinBox_->setKeyboardTracking(false);
  tnSpinBox_->setMinimum(20);
  tnSpinBox_->setMaximum(1000);
  tnSpinBox_->setDecimals(1);
  tnSpinBox_->setAlignment(Qt::AlignRight);
  operationLayout->addRow(QString::fromUtf8("Tn"),
                          tnSpinBox_);

  tvSpinBox_ = new QDoubleSpinBox();
  tvSpinBox_->setSingleStep(1);
  tvSpinBox_->setKeyboardTracking(false);
  tvSpinBox_->setMinimum(0);
  tvSpinBox_->setMaximum(1000);
  tvSpinBox_->setDecimals(1);
  tvSpinBox_->setAlignment(Qt::AlignRight);
  operationLayout->addRow(QString::fromUtf8("Tv"),
                          tvSpinBox_);

  temperatureControlCheckBox_ = new QCheckBox("enabled");
  operationLayout->addRow("Temperature Control",
                          temperatureControlCheckBox_);

  circulatorCheckBox_ = new QCheckBox("enabled");
  operationLayout->addRow("Circulator",
                          circulatorCheckBox_);

  internalTempLCD_ = new QLCDNumber(LCD_SIZE);
  internalTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  internalTempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Internal Temperature [°C]"),
                          internalTempLCD_);

  processTempLCD_ = new QLCDNumber(LCD_SIZE);
  processTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  processTempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Process Temperature [°C]"),
                          processTempLCD_);

  returnTempLCD_ = new QLCDNumber(LCD_SIZE);
  returnTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  returnTempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Return Temperature [°C]"),
                          returnTempLCD_);

  pumpPressureLCD_ = new QLCDNumber(LCD_SIZE);
  pumpPressureLCD_->setSegmentStyle(QLCDNumber::Flat);
  pumpPressureLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Pump Pressure [bar]"),
                          pumpPressureLCD_);

  powerLCD_ = new QLCDNumber(LCD_SIZE);
  powerLCD_->setSegmentStyle(QLCDNumber::Flat);
  powerLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Power [W]"),
                          powerLCD_);

  cwInletTempLCD_ = new QLCDNumber(LCD_SIZE);
  cwInletTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  cwInletTempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Cooling Water Inlet Temperature [°C]"),
                          cwInletTempLCD_);

  cwOutletTempLCD_ = new QLCDNumber(LCD_SIZE);
  cwOutletTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  cwOutletTempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Cooling Water Outlet Temperature [°C]"),
                          cwOutletTempLCD_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(huberUnistat525wCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  connect(temperatureSetPointSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTemperatureSetPoint(double)));

  connect(autoPIDCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setAutoPID(bool)));

  connect(temperatureControlCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setTemperatureControlEnabled(bool)));

  connect(kpSpinBox_, SIGNAL(valueChanged(int)),
          model_, SLOT(setKp(int)));

  connect(tnSpinBox_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTn(double)));

  connect(tvSpinBox_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTv(double)));

  connect(circulatorCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setCirculatorEnabled(bool)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
   Updates the GUI according to the new state of the chiller chiller.
*/
void HuberUnistat525wWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY || newState == INITIALIZING);
  huberUnistat525wCheckBox_->setChecked(ready);
  operationPanel_->setEnabled(ready);

  bool autoPID = model_->getAutoPID();
  if (autoPID) {
    temperatureControlModeBox_->setEnabled(false);
    kpSpinBox_->setEnabled(false);
    tnSpinBox_->setEnabled(false);
    tvSpinBox_->setEnabled(false);

  } else {
    temperatureControlModeBox_->setEnabled(true);
    kpSpinBox_->setEnabled(true);
    tnSpinBox_->setEnabled(true);
    tvSpinBox_->setEnabled(true);
  }
}

void HuberUnistat525wWidget::controlStateChanged(bool enabled)
{
  huberUnistat525wCheckBox_->setEnabled(enabled);
  operationPanel_->setEnabled(enabled);
}

/**
   Sets the values of all the subelements (except the global enablement)
   according to the model.
*/
void HuberUnistat525wWidget::updateInfo()
{
  if (!temperatureSetPointSpinner_->hasFocus())
    temperatureSetPointSpinner_->setValue(model_->getTemperatureSetPoint());

  bool autoPID = model_->getAutoPID();
  
  autoPIDCheckBox_->setChecked(autoPID);

  if (autoPID) {
    temperatureControlModeBox_->setEnabled(false);
    kpSpinBox_->setEnabled(false);
    tnSpinBox_->setEnabled(false);
    tvSpinBox_->setEnabled(false);

  } else {
    temperatureControlModeBox_->setEnabled(true);
    kpSpinBox_->setEnabled(true);
    tnSpinBox_->setEnabled(true);
    tvSpinBox_->setEnabled(true);
    
    if (!kpSpinBox_->hasFocus())
      kpSpinBox_->setValue(model_->getKp());
    
    if (!tnSpinBox_->hasFocus())
      tnSpinBox_->setValue(model_->getTn());
    
    if (!tvSpinBox_->hasFocus())
      tvSpinBox_->setValue(model_->getTv());
  }
  
  temperatureControlCheckBox_->setChecked(model_->getTemperatureControlEnabled());
  circulatorCheckBox_->setChecked(model_->getCirculatorEnabled());

  internalTempLCD_->display(QString::number(model_->getInternalTemperature(), 'f', 2));
  processTempLCD_->display(QString::number(model_->getProcessTemperature(), 'f', 2));
  returnTempLCD_->display(QString::number(model_->getReturnTemperature(), 'f', 2));

  pumpPressureLCD_->display(QString::number(model_->getPumpPressure(), 'f', 3));
  powerLCD_->display(QString::number(model_->getPower()));

  cwInletTempLCD_->display(QString::number(model_->getCoolingWaterInletTemperature(), 'f', 2));
  cwOutletTempLCD_->display(QString::number(model_->getCoolingWaterOutletTemperature(), 'f', 2));
}

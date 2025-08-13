/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "JulaboWidget.h"

/**
  \brief Creates a new panel with all the controls and read-outs for the Julabo
  chiller.
  */
JulaboWidget::JulaboWidget(JulaboModel* model, QWidget *parent)
: QWidget(parent),
  model_(model)
{
  // Create all the nescessary widgets
  chillerCheckBox_ = new QCheckBox("Enable chiller", this);

  operationPanel_ = new QWidget(this);

  DeviceParameterFloat prop = model_->getProportionalParameter();
  proportionalSpinner_ = new QDoubleSpinBox(operationPanel_);
  proportionalSpinner_->setDecimals(prop.getPrecision());
  proportionalSpinner_->setMinimum(prop.getMinimum());
  proportionalSpinner_->setMaximum(prop.getMaximum());
  proportionalSpinner_->setKeyboardTracking(false);

  DeviceParameterUInt integral = model_->getIntegralParameter();
  integralSpinner_ = new QSpinBox(operationPanel_);
  integralSpinner_->setMinimum(integral.getMinimum());
  integralSpinner_->setMaximum(integral.getMaximum());
  integralSpinner_->setKeyboardTracking(false);

  DeviceParameterUInt differential = model_->getDifferentialParameter();
  differentialSpinner_ = new QSpinBox(operationPanel_);
  differentialSpinner_->setMinimum(differential.getMinimum());
  differentialSpinner_->setMaximum(differential.getMaximum());
  differentialSpinner_->setKeyboardTracking(false);

  circulatorCheckBox_ = new QCheckBox("Enable circulator", operationPanel_);

  DeviceParameterUInt pump = model_->getPumpPressureParameter();
  pumpSpinner_ = new QSpinBox(operationPanel_);
  pumpSpinner_->setMinimum(pump.getMinimum());
  pumpSpinner_->setMaximum(pump.getMaximum());
  pumpSpinner_->setKeyboardTracking(false);

  bathTempLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  bathTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  bathTempLCD_->setSmallDecimalPoint(true);

  DeviceParameterFloat working = model_->getWorkingTemperatureParameter();
  workingTempSpinner_ = new QDoubleSpinBox(operationPanel_);
  workingTempSpinner_->setDecimals(working.getPrecision());
  workingTempSpinner_->setMinimum(working.getMinimum());
  workingTempSpinner_->setMaximum(working.getMaximum());
  workingTempSpinner_->setKeyboardTracking(false);

  powerLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  powerLCD_->setSegmentStyle(QLCDNumber::Flat);

  // Put everything in place
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  layout->addWidget(chillerCheckBox_);
  layout->addWidget(operationPanel_);

  QHBoxLayout* operationLayout = new QHBoxLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);

  QWidget* parameterPanel = new QWidget(operationPanel_);
  operationLayout->addWidget(parameterPanel);
  QFormLayout* parameterLayout = new QFormLayout(parameterPanel);
  parameterPanel->setLayout(parameterLayout);

  parameterLayout->addRow("Proportional", proportionalSpinner_);
  parameterLayout->addRow("Integral", integralSpinner_);
  parameterLayout->addRow("Differential", differentialSpinner_);

  QWidget* tempPanel = new QWidget(operationPanel_);
  operationLayout->addWidget(tempPanel);
  QFormLayout* tempLayout = new QFormLayout(tempPanel);
  tempPanel->setLayout(tempLayout);

  tempLayout->addRow(circulatorCheckBox_);
  tempLayout->addRow("Pump pressure", pumpSpinner_);
  tempLayout->addRow(QString::fromUtf8("Bath temperature (°C)"), bathTempLCD_);
  tempLayout->addRow(QString::fromUtf8("Working temperature (°C)"), workingTempSpinner_);
  tempLayout->addRow("Power (%)", powerLCD_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateChillerInfo()));

  connect(chillerCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setDeviceEnabled(bool)));

  connect(proportionalSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setProportionalValue(double)));

  connect(differentialSpinner_, SIGNAL(valueChanged(int)),
          model_, SLOT(setDifferentialValue(int)));

  connect(circulatorCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setCirculatorEnabled(bool)));

  connect(pumpSpinner_, SIGNAL(valueChanged(int)),
          model_, SLOT(setPumpPressureValue(int)));

  connect(workingTempSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setWorkingTemperatureValue(double)));

  // Set GUI according to the current chiller state
  updateDeviceState( model_->getDeviceState() );
  updateChillerInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
 */
void JulaboWidget::updateDeviceState(State newState)
{
  chillerCheckBox_->setChecked( newState == INITIALIZING || newState == READY );
  operationPanel_->setEnabled( newState == READY );
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void JulaboWidget::controlStateChanged(bool enabled)
{
  if (enabled) {
    chillerCheckBox_->setEnabled(true);
    updateDeviceState(model_->getDeviceState());
  } else {
    chillerCheckBox_->setEnabled(false);
    operationPanel_->setEnabled(false);
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void JulaboWidget::updateChillerInfo()
{
  if (!proportionalSpinner_->hasFocus())
    proportionalSpinner_->setValue(model_->getProportionalParameter().getValue());

  if (!integralSpinner_->hasFocus())
    integralSpinner_->setValue(model_->getIntegralParameter().getValue());

  if (!differentialSpinner_->hasFocus())
    differentialSpinner_->setValue(model_->getDifferentialParameter().getValue());

  circulatorCheckBox_->setChecked(model_->isCirculatorEnabled());

  if (!pumpSpinner_->hasFocus())
    pumpSpinner_->setValue(model_->getPumpPressureParameter().getValue());

  bathTempLCD_->display(model_->getBathTemperature());

  if (!workingTempSpinner_->hasFocus())
    workingTempSpinner_->setValue(model_->getWorkingTemperatureParameter().getValue());

  powerLCD_->display( static_cast<int>(model_->getPower()) );
}

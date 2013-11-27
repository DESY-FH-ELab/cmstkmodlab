#include "HuberPetiteFleurWidget.h"

/**
  \brief Creates a new panel with all the controls and read-outs for the HuberPetiteFleur
  chiller.
  */
HuberPetiteFleurWidget::HuberPetiteFleurWidget(HuberPetiteFleurModel* model, QWidget *parent) :
    QWidget(parent), model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  huberPetiteFleurCheckBox_ = new QCheckBox("Enable chiller", this);
  layout->addWidget(huberPetiteFleurCheckBox_);

  operationPanel_ = new QWidget(this);
  QFormLayout* operationLayout = new QFormLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);
  layout->addWidget(operationPanel_);

  circulatorCheckBox_ = new QCheckBox("Enable circulator", operationPanel_);
  operationLayout->addRow(circulatorCheckBox_);

  bathTempLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  bathTempLCD_->setSegmentStyle(QLCDNumber::Flat);
  bathTempLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("Bath temperature (°C)"),
                             bathTempLCD_);

  DeviceParameterFloat working = model_->getWorkingTemperatureParameter();
  workingTempSpinner_ = new QDoubleSpinBox(operationPanel_);
  workingTempSpinner_->setSingleStep(0.1);
  workingTempSpinner_->setKeyboardTracking(false);
  workingTempSpinner_->setButtonSymbols(QAbstractSpinBox::NoButtons);
  workingTempSpinner_->setDecimals(working.getPrecision());
  workingTempSpinner_->setMinimum(working.getMinimum());
  workingTempSpinner_->setMaximum(working.getMaximum());
  operationLayout->addRow(QString::fromUtf8("Working temperature (°C)"),
                          workingTempSpinner_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(huberPetiteFleurCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setDeviceEnabled(bool)));

  connect(circulatorCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setCirculatorEnabled(bool)));

  connect(workingTempSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setWorkingTemperatureValue(double)));

  // Set GUI according to the current chiller state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
  */
void HuberPetiteFleurWidget::updateDeviceState(State newState) {

  bool ready = (newState == READY);
  huberPetiteFleurCheckBox_->setChecked( ready );
  operationPanel_->setEnabled( ready );
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void HuberPetiteFleurWidget::controlStateChanged(bool enabled) {
  
  huberPetiteFleurCheckBox_->setEnabled(enabled);
  if (enabled) {
    State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
  */
void HuberPetiteFleurWidget::updateInfo() {

 circulatorCheckBox_->setChecked(model_->isCirculatorEnabled());

 char buffer[10];
 sprintf(buffer, "%.02f", model_->getBathTemperature());
 bathTempLCD_->display(buffer);

 workingTempSpinner_->setValue(model_->getWorkingTemperatureParameter().getValue());
}

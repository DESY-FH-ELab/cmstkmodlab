#include "IotaWidget.h"

/**
  \brief Creates a new panel with all the controls and read-outs for the Iota
  pump.
  */
IotaWidget::IotaWidget(IotaModel* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  iotaCheckBox_ = new QCheckBox("Enable pump", this);
  layout->addWidget(iotaCheckBox_);

  operationPanel_ = new QWidget(this);
  QFormLayout* operationLayout = new QFormLayout(operationPanel_);
  operationPanel_->setLayout(operationLayout);
  layout->addWidget(operationPanel_);

  pumpCheckBox_ = new QCheckBox("Start run", operationPanel_);
  operationLayout->addRow(pumpCheckBox_);

  DeviceParameterFloat sPressure = model_->getSetPressureParameter();
  sPressureSpinner_ = new QDoubleSpinBox(operationPanel_);
  sPressureSpinner_->setSingleStep(1);
  sPressureSpinner_->setKeyboardTracking(false);
  sPressureSpinner_->setButtonSymbols(QAbstractSpinBox::NoButtons);
  sPressureSpinner_->setDecimals(sPressure.getPrecision());
  sPressureSpinner_->setMinimum(sPressure.getMinimum());
  sPressureSpinner_->setMaximum(sPressure.getMaximum());
  operationLayout->addRow(QString::fromUtf8("pressure set (bar)"),
                          sPressureSpinner_);

  aPressureLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  aPressureLCD_->setSegmentStyle(QLCDNumber::Flat);
  aPressureLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("act pressure (bar)"),
                          aPressureLCD_);

  DeviceParameterFloat sFlow = model_->getSetFlowParameter();
  sFlowSpinner_ = new QDoubleSpinBox(operationPanel_);
  sFlowSpinner_->setSingleStep(1);
  sFlowSpinner_->setKeyboardTracking(false);
  sFlowSpinner_->setButtonSymbols(QAbstractSpinBox::NoButtons);
  sFlowSpinner_->setDecimals(sFlow.getPrecision());
  sFlowSpinner_->setMinimum(sFlow.getMinimum());
  sFlowSpinner_->setMaximum(sFlow.getMaximum());
  operationLayout->addRow(QString::fromUtf8("flow set (ml/min)"),
                          sFlowSpinner_);

  aFlowLCD_ = new QLCDNumber(LCD_SIZE, operationPanel_);
  aFlowLCD_->setSegmentStyle(QLCDNumber::Flat);
  aFlowLCD_->setSmallDecimalPoint(true);
  operationLayout->addRow(QString::fromUtf8("act flow (ml/min)"),
                          aFlowLCD_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(iotaCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setDeviceEnabled(bool)));

  connect(pumpCheckBox_, SIGNAL(toggled(bool)),
          model, SLOT(setPumpEnabled(bool)));

  connect(sPressureSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setPressureValue(double)));

  connect(sFlowSpinner_, SIGNAL(valueChanged(double)),
          model_, SLOT(setFlowValue(double)));

  // Set GUI according to the current pump state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the pump.
 */
void IotaWidget::updateDeviceState(State newState) {

  bool ready = (newState == READY);
  iotaCheckBox_->setChecked( ready );
  operationPanel_->setEnabled( ready );
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void IotaWidget::controlStateChanged(bool enabled) {
  
  iotaCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void IotaWidget::updateInfo() {

  pumpCheckBox_->setChecked(model_->isPumpEnabled());

  char bufferP[10];
  sprintf(bufferP, "%.02f", model_->getActPressure());
  aPressureLCD_->display(bufferP);

  char bufferF[10];
  sprintf(bufferF, "%.02f", model_->getActFlow());
  aFlowLCD_->display(bufferF);

  if (!sPressureSpinner_->hasFocus())
    sPressureSpinner_->setValue(model_->getSetPressureParameter().getValue());

  if (!sFlowSpinner_->hasFocus())
    sFlowSpinner_->setValue(model_->getSetFlowParameter().getValue());
}

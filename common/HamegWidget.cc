#include <iostream>

#include <QGridLayout>

#include <nqlogger.h>

#include "HamegWidget.h"

HamegChannelWidget::HamegChannelWidget(HamegModel* model, int channel,
                                       QWidget *parent)
: QGroupBox(parent),
  model_(model),
  channel_(channel)
{
  setTitle(QString("channel %1").arg(channel));

  QGridLayout* layout = new QGridLayout();
  setLayout(layout);

  QButtonGroup* modeGroup = new QButtonGroup(this);
  cvModeButton_ = new QRadioButton("Constant Voltage Mode", this);
  cvModeButton_->setEnabled(false);
  modeGroup->addButton(cvModeButton_);
  layout->addWidget(cvModeButton_, 0, 0);
  ccModeButton_ = new QRadioButton("Constant Current Mode", this);
  ccModeButton_->setEnabled(false);
  modeGroup->addButton(ccModeButton_);
  layout->addWidget(ccModeButton_, 1, 0);

  QHBoxLayout* hlayout;

  hlayout= new QHBoxLayout();
  QGroupBox* voltageGroup = new QGroupBox("Voltage", this);
  voltageGroup->setLayout(hlayout);

  voltageDisplay_ = new QLCDNumber(LCD_SIZE, voltageGroup);
  voltageDisplay_->setSegmentStyle(QLCDNumber::Flat);
  voltageDisplay_->setSmallDecimalPoint(true);
  voltageDisplay_->setDigitCount(6);
  hlayout->addWidget(voltageDisplay_);

  voltageSpinner_ = new QDoubleSpinBox(voltageGroup);
  voltageSpinner_->setMinimum(0.0);
  voltageSpinner_->setMaximum(30.0);
  voltageSpinner_->setSingleStep(0.01);
  voltageSpinner_->setDecimals(2);
  voltageSpinner_->setKeyboardTracking(false);
  hlayout->addWidget(voltageSpinner_);

  layout->addWidget(voltageGroup, 0, 1);

  hlayout= new QHBoxLayout();
  QGroupBox* currentGroup = new QGroupBox("Current", this);
  currentGroup->setLayout(hlayout);

  currentDisplay_ = new QLCDNumber(LCD_SIZE, currentGroup);
  currentDisplay_->setSegmentStyle(QLCDNumber::Flat);
  currentDisplay_->setSmallDecimalPoint(true);
  currentDisplay_->setDigitCount(8);
  hlayout->addWidget(currentDisplay_);

  currentSpinner_ = new QDoubleSpinBox(voltageGroup);
  currentSpinner_->setMinimum(0.0);
  currentSpinner_->setMaximum(0.31);
  currentSpinner_->setSingleStep(0.001);
  currentSpinner_->setDecimals(3);
  currentSpinner_->setKeyboardTracking(false);
  hlayout->addWidget(currentSpinner_);

  layout->addWidget(currentGroup, 1, 1);

  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(modeGroup, SIGNAL(buttonClicked(int)),
          this, SLOT(modeChanged(int)));

  connect(voltageSpinner_, SIGNAL(valueChanged(double)),
          this, SLOT(voltageSpinnerChanged(double)));
  connect(currentSpinner_, SIGNAL(valueChanged(double)),
          this, SLOT(currentSpinnerChanged(double)));

  // Set GUI according to the current chiller state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void HamegChannelWidget::modeChanged(int button)
{
  if (button==0) {
    model_->setVoltage(channel_, voltageSpinner_->value());
  } else {
    model_->setCurrent(channel_, currentSpinner_->value());
  }
}

void HamegChannelWidget::voltageSpinnerChanged(double voltage)
{
  model_->setVoltage(channel_, voltage);
}

void HamegChannelWidget::currentSpinnerChanged(double voltage)
{
  model_->setCurrent(channel_, voltage);
}

void HamegChannelWidget::updateDeviceState(State /*state*/)
{

}

void HamegChannelWidget::controlStateChanged(bool /*state*/)
{

}

void HamegChannelWidget::updateInfo()
{
  NQLog("HamegChannelWidget", NQLog::Debug) << "updateInfo()";

  unsigned int status = model_->getStatus();

  if (channel_==1) {

    if (status&VHameg8143::hmCV1) {
      cvModeButton_->setChecked(true);
    } else if (status&VHameg8143::hmCC1) {
      ccModeButton_->setChecked(true);
    }
  } else {
    if (status&VHameg8143::hmCV2) {
      cvModeButton_->setChecked(true);
    } else if (status&VHameg8143::hmCC2) {
      ccModeButton_->setChecked(true);
    }
  }

  char dummy[10];

  if (!voltageSpinner_->hasFocus()) {
    float setVoltage = model_->getVoltageParameter(channel_).getValue();
    voltageSpinner_->setValue(setVoltage);
  }
  float voltage = model_->getVoltage(channel_);
  sprintf(dummy, "%.02f", voltage);
  voltageDisplay_->display(dummy);

  if (!currentSpinner_->hasFocus()) {
    float setCurrent = model_->getCurrentParameter(channel_).getValue();
    currentSpinner_->setValue(setCurrent);
  }
  float current = model_->getCurrent(channel_);
  sprintf(dummy, "%.03f", current);
  currentDisplay_->display(dummy);
}

/**
  \brief Creates a new panel with all the controls and read-outs for the Hameg
  chiller.
 */
HamegWidget::HamegWidget(HamegModel* model, QWidget *parent)
: QWidget(parent),
  model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);

  hamegCheckBox_ = new QCheckBox("Enable power supply", this);
  layout->addWidget(hamegCheckBox_);

  operationPanel_ = new QWidget(this);
  QVBoxLayout* layout2 = new QVBoxLayout();
  operationPanel_->setLayout(layout2);
  layout->addWidget(operationPanel_);

  QHBoxLayout* hlayout = new QHBoxLayout();
  QWidget* buttonPanel = new QWidget(operationPanel_);
  buttonPanel->setLayout(hlayout);
  layout2->addWidget(buttonPanel);

  hamegRemoteBox_ = new QCheckBox("Remote mode", buttonPanel);
  hlayout->addWidget(hamegRemoteBox_);

  hamegOutputBox_= new QCheckBox("Outputs enabled", buttonPanel);
  hlayout->addWidget(hamegOutputBox_);

  // Create all the nescessary widgets
  channel1_ = new HamegChannelWidget(model_, 1, operationPanel_);
  layout2->addWidget(channel1_);
  channel2_ = new HamegChannelWidget(model_, 2, operationPanel_);
  layout2->addWidget(channel2_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(hamegCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  connect(hamegRemoteBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setRemoteMode(bool)));
  connect(hamegOutputBox_, SIGNAL(toggled(bool)),
          model_, SLOT(switchOutputOn(bool)));

  // Set GUI according to the current chiller state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
 */
void HamegWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);
  hamegCheckBox_->setChecked(ready);
  operationPanel_->setEnabled(ready);
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void HamegWidget::controlStateChanged(bool enabled)
{
  hamegCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void HamegWidget::updateInfo()
{
  NQLog("HamegWidget", NQLog::Debug) << "updateInfo()";

  unsigned int status = model_->getStatus();

  if (status&VHameg8143::hmRM0) {
    hamegRemoteBox_->setChecked(false);
    channel1_->setEnabled(false);
    channel2_->setEnabled(false);
    hamegOutputBox_->setEnabled(false);
  } else if (status&VHameg8143::hmRM1) {
    hamegRemoteBox_->setChecked(true);
    channel1_->setEnabled(true);
    channel2_->setEnabled(true);
    hamegOutputBox_->setEnabled(true);
  }

  if (status&VHameg8143::hmOP0)
    hamegOutputBox_->setChecked(false);
  else if (status&VHameg8143::hmOP1)
    hamegOutputBox_->setChecked(true);
}

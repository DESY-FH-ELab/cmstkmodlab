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

#include <iostream>

#include <QGridLayout>

#include <nqlogger.h>

#include "RohdeSchwarzNGE103BWidget.h"

RohdeSchwarzNGE103BChannelWidget::RohdeSchwarzNGE103BChannelWidget(RohdeSchwarzNGE103BModel* model,
                                                                   int channel,
                                                                   QWidget *parent)
  : QGroupBox(parent),
    model_(model),
    channel_(channel)
{
  setTitle(QString("Channel %1").arg(channel));

  QGridLayout* layout = new QGridLayout();
  setLayout(layout);

  QVBoxLayout* vlayout;

  vlayout= new QVBoxLayout();
  QGroupBox* voltageGroup = new QGroupBox("Voltage", this);
  voltageGroup->setLayout(vlayout);

  voltageDisplay_ = new QLCDNumber(LCD_SIZE, voltageGroup);
  voltageDisplay_->setSegmentStyle(QLCDNumber::Flat);
  voltageDisplay_->setSmallDecimalPoint(true);
  voltageDisplay_->setDigitCount(6);
  vlayout->addWidget(voltageDisplay_);

  lcdOFFPalette_ = voltageDisplay_->palette();
  lcdCVPalette_ = lcdOFFPalette_;
  lcdCVPalette_.setColor(QPalette::Dark, Qt::green);
  lcdCVPalette_.setColor(QPalette::Light, Qt::green);
  lcdCCPalette_ = lcdOFFPalette_;
  lcdCCPalette_.setColor(QPalette::Dark, Qt::red);
  lcdCCPalette_.setColor(QPalette::Light, Qt::red);

  voltageSpinner_ = new QDoubleSpinBox(voltageGroup);
  voltageSpinner_->setMinimum(VRohdeSchwarzNGE103B::MinVoltage);
  voltageSpinner_->setMaximum(VRohdeSchwarzNGE103B::MaxVoltage);
  voltageSpinner_->setSingleStep(0.1);
  voltageSpinner_->setDecimals(2);
  voltageSpinner_->setKeyboardTracking(false);
  voltageSpinner_->setSuffix(" V");
  voltageSpinner_->setAlignment(Qt::AlignRight);
  vlayout->addWidget(voltageSpinner_);

  layout->addWidget(voltageGroup, 0, 0);

  vlayout= new QVBoxLayout();
  QGroupBox* currentGroup = new QGroupBox("Current", this);
  currentGroup->setLayout(vlayout);

  currentDisplay_ = new QLCDNumber(LCD_SIZE, currentGroup);
  currentDisplay_->setSegmentStyle(QLCDNumber::Flat);
  currentDisplay_->setSmallDecimalPoint(true);
  currentDisplay_->setDigitCount(6);
  vlayout->addWidget(currentDisplay_);

  currentSpinner_ = new QDoubleSpinBox(voltageGroup);
  currentSpinner_->setMinimum(VRohdeSchwarzNGE103B::MinCurrent);
  currentSpinner_->setMaximum(VRohdeSchwarzNGE103B::MaxCurrent);
  currentSpinner_->setSingleStep(0.01);
  currentSpinner_->setDecimals(3);
  currentSpinner_->setKeyboardTracking(false);
  currentSpinner_->setSuffix(" A");
  currentSpinner_->setAlignment(Qt::AlignRight);
  vlayout->addWidget(currentSpinner_);

  layout->addWidget(currentGroup, 0, 1);

  vlayout= new QVBoxLayout();
  QGroupBox* easyRampGroup = new QGroupBox("Easy Ramp", this);
  easyRampGroup->setLayout(vlayout);

  easyRampStateBox_ = new QCheckBox("Enabled", this);
  vlayout->addWidget(easyRampStateBox_);

  easyRampDurationSpinner_ = new QDoubleSpinBox(easyRampGroup);
  easyRampDurationSpinner_->setMinimum(VRohdeSchwarzNGE103B::MinEasyRampDuration);
  easyRampDurationSpinner_->setMaximum(VRohdeSchwarzNGE103B::MaxEasyRampDuration);
  easyRampDurationSpinner_->setSingleStep(0.1);
  easyRampDurationSpinner_->setDecimals(2);
  easyRampDurationSpinner_->setKeyboardTracking(false);
  easyRampDurationSpinner_->setSuffix(" s");
  easyRampDurationSpinner_->setAlignment(Qt::AlignRight);
  vlayout->addWidget(easyRampDurationSpinner_);

  layout->addWidget(easyRampGroup, 0, 2);

  outputStateBox_ = new QCheckBox("Output Enabled", this);
  layout->addWidget(outputStateBox_, 1, 0, 1, 3);

  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(voltageSpinner_, SIGNAL(valueChanged(double)),
          this, SLOT(voltageSpinnerChanged(double)));
  connect(currentSpinner_, SIGNAL(valueChanged(double)),
          this, SLOT(currentSpinnerChanged(double)));
  connect(easyRampDurationSpinner_, SIGNAL(valueChanged(double)),
          this, SLOT(easyRampDurationSpinnerChanged(double)));

  connect(easyRampStateBox_, SIGNAL(toggled(bool)),
          this, SLOT(easyRampStateChanged(bool)));

  connect(outputStateBox_, SIGNAL(toggled(bool)),
          this, SLOT(setOutputStateChanged(bool)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void RohdeSchwarzNGE103BChannelWidget::voltageSpinnerChanged(double voltage)
{
  model_->setVoltage(channel_, voltage);
}

void RohdeSchwarzNGE103BChannelWidget::currentSpinnerChanged(double current)
{
  model_->setCurrent(channel_, current);
}

void RohdeSchwarzNGE103BChannelWidget::easyRampDurationSpinnerChanged(double duration)
{
  model_->setEasyRampDuration(channel_, duration);
}

void RohdeSchwarzNGE103BChannelWidget::easyRampStateChanged(bool state)
{
  model_->setEasyRampState(channel_, state);
  if (state) {
    easyRampDurationSpinner_->setEnabled(false);
  } else {
    easyRampDurationSpinner_->setEnabled(true);
  }
}

void RohdeSchwarzNGE103BChannelWidget::setOutputStateChanged(bool state)
{
  model_->setOutputState(channel_, state);
}

void RohdeSchwarzNGE103BChannelWidget::updateDeviceState(State /*state*/)
{

}

void RohdeSchwarzNGE103BChannelWidget::controlStateChanged(bool /*state*/)
{

}

void RohdeSchwarzNGE103BChannelWidget::updateInfo()
{
  NQLogDebug("RohdeSchwarzNGE103BChannelWidget") << "updateInfo() channel(" << channel_ << ")";

  unsigned int outputMode = model_->getOutputMode(channel_);

  switch (outputMode) {
  case VRohdeSchwarzNGE103B::RohdeSchwarzNGE103B_OFF: {
    voltageDisplay_->setPalette(lcdOFFPalette_);
    currentDisplay_->setPalette(lcdOFFPalette_);
    break;
  }
  case VRohdeSchwarzNGE103B::RohdeSchwarzNGE103B_CV: {
    voltageDisplay_->setPalette(lcdCVPalette_);
    currentDisplay_->setPalette(lcdCVPalette_);
    break;
  }
  case VRohdeSchwarzNGE103B::RohdeSchwarzNGE103B_CC: {
    voltageDisplay_->setPalette(lcdCCPalette_);
    currentDisplay_->setPalette(lcdCCPalette_);
    break;
  }
  default: {
    voltageDisplay_->setPalette(lcdOFFPalette_);
    currentDisplay_->setPalette(lcdOFFPalette_);
  }
  }

  if (!voltageSpinner_->hasFocus()) {
    float setVoltage = model_->getVoltage(channel_);
    voltageSpinner_->setValue(setVoltage);
  }
  voltageDisplay_->display(QString::number(model_->getMeasuredVoltage(channel_), 'f', 2));

  if (!currentSpinner_->hasFocus()) {
    float setCurrent = model_->getCurrent(channel_);
    currentSpinner_->setValue(setCurrent);
  }
  currentDisplay_->display(QString::number(model_->getMeasuredCurrent(channel_), 'f', 3));

  if (!easyRampDurationSpinner_->hasFocus()) {
    float setEasyRampDuration = model_->getEasyRampDuration(channel_);
    easyRampDurationSpinner_->setValue(setEasyRampDuration);
  }

  if (model_->getEasyRampState(channel_) != easyRampStateBox_->isChecked()) {
    easyRampStateBox_->setChecked(model_->getEasyRampState(channel_));
  }

  if (model_->getOutputState(channel_) != outputStateBox_->isChecked()) {
    outputStateBox_->setChecked(model_->getOutputState(channel_));
  }
}

/**
  \brief Creates a new panel with all the controls and read-outs for the RohdeSchwarzNGE103B
  chiller.
 */
RohdeSchwarzNGE103BWidget::RohdeSchwarzNGE103BWidget(RohdeSchwarzNGE103BModel* model,
                                                     QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);

  deviceCheckBox_ = new QCheckBox("Enable Power Supply", this);
  layout->addWidget(deviceCheckBox_);

  operationPanel_ = new QWidget(this);
  QVBoxLayout* layout2 = new QVBoxLayout();
  operationPanel_->setLayout(layout2);
  layout->addWidget(operationPanel_);

  // Create all the nescessary widgets
  channel1_ = new RohdeSchwarzNGE103BChannelWidget(model_, 1, operationPanel_);
  layout2->addWidget(channel1_);
  channel2_ = new RohdeSchwarzNGE103BChannelWidget(model_, 2, operationPanel_);
  layout2->addWidget(channel2_);
  channel3_ = new RohdeSchwarzNGE103BChannelWidget(model_, 3, operationPanel_);
  layout2->addWidget(channel3_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(deviceCheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  // Set GUI according to the current power supply state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
 */
void RohdeSchwarzNGE103BWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY || newState == INITIALIZING);
  deviceCheckBox_->setChecked(ready);
  controlStateChanged(ready);
}

/// Updates the GUI when the power supply is enabled/disabled.
void RohdeSchwarzNGE103BWidget::controlStateChanged(bool enabled)
{
  deviceCheckBox_->setEnabled(enabled);
  operationPanel_->setEnabled(enabled);
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void RohdeSchwarzNGE103BWidget::updateInfo()
{

}

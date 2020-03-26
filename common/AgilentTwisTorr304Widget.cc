/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QMessageBox>

#include "AgilentTwisTorr304Widget.h"

AgilentTwisTorr304Widget::AgilentTwisTorr304Widget(AgilentTwisTorr304Model* model,
                                                   QWidget *parent)
 : QWidget(parent),
   model_(model)
{
  // Create all the nescessary widgets
  agilentCheckBox_ = new QCheckBox("Enable vacuum controller", this);

  operationPanel_ = new QWidget(this);

  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  layout->addWidget(agilentCheckBox_);

  QVBoxLayout* vlayout = new QVBoxLayout();
  operationPanel_->setLayout(vlayout);

  statusLabel_ = new QLabel(model_->getPumpStatusText().c_str());
  vlayout->addWidget(statusLabel_);

  errorCodeLabel_ = new QLabel("0");
  vlayout->addWidget(errorCodeLabel_);

  QHBoxLayout* hlayout = new QHBoxLayout();

  switchPumpOnButton_ = new QPushButton("On", operationPanel_);
  connect(switchPumpOnButton_, SIGNAL(clicked()),
          this, SLOT(switchPumpOnRequest()));
  hlayout->addWidget(switchPumpOnButton_);

  switchPumpOffButton_ = new QPushButton("Off", operationPanel_);
  connect(switchPumpOffButton_, SIGNAL(clicked()),
          this, SLOT(switchPumpOffRequest()));
  hlayout->addWidget(switchPumpOffButton_);

  vlayout->addLayout(hlayout);

  layout->addWidget(operationPanel_);

  // Connect all the signals
  connect(model_,
          SIGNAL(deviceStateChanged(State)),
          this,
          SLOT(updateDeviceState(State)));

  connect(model_,
          SIGNAL(controlStateChanged(bool)),
          this,
          SLOT(controlStateChanged(bool)));

  connect(model_,
          SIGNAL(informationChanged()),
          this,
          SLOT(updateInfo()));

  connect(agilentCheckBox_,
          SIGNAL(toggled(bool)),
          model_,
          SLOT(setDeviceEnabled(bool)));

  // Set GUI according to the current chiller state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void AgilentTwisTorr304Widget::switchPumpOnRequest()
{
  int ret = QMessageBox::question(this,
                                  "Switch Pump On",
                                  "Switch Pump On?");

  if (ret==QMessageBox::Yes) {
    model_->switchPumpOn();
    switchPumpOnButton_->setEnabled(false);
    switchPumpOffButton_->setEnabled(true);
  }
}

void AgilentTwisTorr304Widget::switchPumpOffRequest()
{
  int ret = QMessageBox::question(this,
                                  "Switch Pump Off",
                                  "Switch Pump Off?");

  if (ret==QMessageBox::Yes) {
    model_->switchPumpOff();
    switchPumpOnButton_->setEnabled(true);
    switchPumpOffButton_->setEnabled(false);
  }
}

/**
  Updates the GUI according to the new state of the chiller chiller.
  */
void AgilentTwisTorr304Widget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  agilentCheckBox_->setChecked(ready);
  operationPanel_->setEnabled(ready);

  if (model_->getPumpState()) {
    switchPumpOnButton_->setEnabled(false);
    switchPumpOffButton_->setEnabled(true);
  } else {
    switchPumpOnButton_->setEnabled(true);
    switchPumpOffButton_->setEnabled(false);
  }
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void AgilentTwisTorr304Widget::controlStateChanged(bool enabled)
{
  agilentCheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
  */
void AgilentTwisTorr304Widget::updateInfo()
{
  statusLabel_->setText(model_->getPumpStatusText().c_str());
  errorCodeLabel_->setText(QString::number(model_->getErrorCode()));

  if (model_->getPumpState()) {
    switchPumpOnButton_->setEnabled(false);
    switchPumpOffButton_->setEnabled(true);
  } else {
    switchPumpOnButton_->setEnabled(true);
    switchPumpOffButton_->setEnabled(false);
  }
}

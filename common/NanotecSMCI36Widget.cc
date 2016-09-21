#include <iostream>

#include <QGridLayout>

#include <nqlogger.h>

#include "NanotecSMCI36Widget.h"

NanotecSMCI36Widget::NanotecSMCI36Widget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);

  smci36CheckBox_ = new QCheckBox("Enable controller", this);
  layout->addWidget(smci36CheckBox_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(smci36CheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  // Set GUI according to the current chiller state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
 */
void NanotecSMCI36Widget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);
  smci36CheckBox_->setChecked(ready);
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void NanotecSMCI36Widget::controlStateChanged(bool enabled)
{
  smci36CheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void NanotecSMCI36Widget::updateInfo()
{
  NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";

  unsigned int status = model_->getStatus();

}

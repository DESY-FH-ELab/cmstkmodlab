#include "DefoConradButton.h"

DefoConradButton::DefoConradButton(DefoConradModel *model,
				   const QString &text,
				   QWidget *parent)
  : QPushButton(text, parent),
    model_(model)
{
  setCheckable(true);
}

/// Creates a new action associated with a certain device relais.
DefoConradSwitchButton::DefoConradSwitchButton(DefoConradModel* model,
					       const DefoConradModel::DeviceSwitch& device,
					       const QString& text,
					       QWidget *parent)
  : DefoConradButton(model, text, parent),
    device_(device)
{
  connect(model, SIGNAL(switchStateChanged(DefoConradModel::DeviceSwitch, State)),
	  this, SLOT(switchStateChanged(DefoConradModel::DeviceSwitch,  State)));

  connect(model, SIGNAL(deviceStateChanged(State)),
	  this, SLOT(deviceStateChanged(State)));

  connect(model_, SIGNAL(controlStateChanged(bool)),
	  this, SLOT(controlStateChanged(bool)));

  connect(this, SIGNAL(toggled(bool)),
	  this, SLOT(buttonToggled(bool)));

  deviceStateChanged(model->getDeviceState());
  switchStateChanged(device_, model->getSwitchState(device));
}

/// Method to notify the DefoConradAction upon model change.
void DefoConradSwitchButton::switchStateChanged(DefoConradModel::DeviceSwitch device,
						State newState)
 {
  // Check if the updated device is ours
  if ( device_ == device ) {
    setChecked( newState == READY ||  newState == INITIALIZING );
    setEnabled(model_->getDeviceState() == READY &&
	       newState != INITIALIZING &&
	       newState != CLOSING);
  }
}

/// Method to set the model when the checkbox is toggled.
void DefoConradSwitchButton::buttonToggled(bool checked)
{
  model_->setSwitchEnabled(device_, checked);
}

/// Sets whether it makes sense to be enabled according to switch state.
void DefoConradSwitchButton::deviceStateChanged(State newState)
{
  setEnabled(newState == READY);
}

void DefoConradSwitchButton::controlStateChanged(bool enabled)
{
  if (enabled) {
    setEnabled(model_->getDeviceState() == READY);
  } else {
    setEnabled(false);
  }
}

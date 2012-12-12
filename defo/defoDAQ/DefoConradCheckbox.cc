#include "DefoConradCheckbox.h"


/*--- DefoConradCheckbox ---*/

DefoConradCheckbox::DefoConradCheckbox(
    DefoConradModel *model
  , const QString &text
  , QWidget *parent
) :
    QCheckBox(text, parent), model_(model)
{}



/*--- DefoConradDeviceCheckbox ---*/

/// Creates a new checkbox associated with the Conrad relais switch.
DefoConradDeviceCheckbox::DefoConradDeviceCheckbox(
    DefoConradModel* model
  , const QString& text
  , QWidget *parent
) :
    DefoConradCheckbox(model, text, parent)
{

  connect(
        model_
      , SIGNAL( deviceStateChanged(State) )
      , this
      , SLOT( deviceStateChanged(State) )
  );

  connect(
        model_
      , SIGNAL( controlStateChanged(bool) )
      , this
      , SLOT( controlStateChanged(bool) )
  );

  connect( this, SIGNAL(toggled(bool)), model_, SLOT(setDeviceEnabled(bool)) );

//  setChecked( model->getDeviceState() == READY );
  deviceStateChanged( model_->getDeviceState() );

}

///// Method to set the model when the DefoConradAction is toggled.
//void DefoConradDeviceCheckbox::setChecked(bool checked) {

//  DefoConradCheckbox::setChecked( checked );
//  model_->setDeviceEnabled( checked );

//}

/// Sets whether it makes sense to be enabled according to switch state.
void DefoConradDeviceCheckbox::deviceStateChanged(
    State newState
) {

  setChecked(
         newState == READY
      || newState == INITIALIZING
  );

  setEnabled(
         newState != INITIALIZING
      && newState != CLOSING
  );

}

void DefoConradDeviceCheckbox::controlStateChanged(bool enabled) {
  if (enabled) {
    State state = model_->getDeviceState();
    setEnabled( state != INITIALIZING
	     && state != CLOSING );
  } else {
    setEnabled( false );
  }
}


/*--- DefoConradSwitchCheckbox ---*/

/// Creates a new action associated with a certain device relais.
DefoConradSwitchCheckbox::DefoConradSwitchCheckbox(
    DefoConradModel* model
  , const DefoConradModel::DeviceSwitch& device
  , const QString& text
  , QWidget *parent
) :
    DefoConradCheckbox(model, text, parent), device_(device)
{

  connect(
        model
      , SIGNAL(
          switchStateChanged(
              DefoConradModel::DeviceSwitch
            , State
          )
        )
      , this
      , SLOT(
          switchStateChanged(
              DefoConradModel::DeviceSwitch
            ,  State
          )
        )
  );

  connect(
        model
      , SIGNAL( deviceStateChanged(State) )
      , this
      , SLOT( deviceStateChanged(State) )
  );

  connect(
        model_
      , SIGNAL( controlStateChanged(bool) )
      , this
      , SLOT( controlStateChanged(bool) )
  );

  connect( this, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)) );

  deviceStateChanged( model->getDeviceState() );
  switchStateChanged( device_, model->getSwitchState(device) );

}

/// Method to notify the DefoConradAction upon model change.
void DefoConradSwitchCheckbox::switchStateChanged(
    DefoConradModel::DeviceSwitch device
  , State newState
) {

  // Check if the updated device is ours
  if ( device_ == device ) {
    setChecked( newState == READY ||  newState == INITIALIZING );
    setEnabled(
         model_->getDeviceState() == READY
      && newState != INITIALIZING
      && newState != CLOSING
    );
  }

}

/// Method to set the model when the checkbox is toggled.
void DefoConradSwitchCheckbox::buttonToggled(bool checked) {
  model_->setSwitchEnabled( device_, checked );
}

/// Sets whether it makes sense to be enabled according to switch state.
void DefoConradSwitchCheckbox::deviceStateChanged(State newState) {
  setEnabled( newState == READY );
}

void DefoConradSwitchCheckbox::controlStateChanged(bool enabled) {
  if (enabled) {
    setEnabled( model_->getDeviceState() == READY );
  } else {
    setEnabled( false );
  }
}

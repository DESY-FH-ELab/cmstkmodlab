#include "DefoConradCheckbox.h"


/*--- DefoConradAction ---*/

DefoConradCheckbox::DefoConradCheckbox(
    DefoConradModel *model
  , const QString &text
  , QWidget *parent
) :
    QCheckBox(text, parent), model_(model)
{

//  setCheckable( true );

}



/*--- DefoConradDeviceCheckbox ---*/

/// Creates a new action associated with the Conrad relais switch.
DefoConradDeviceCheckbox::DefoConradDeviceCheckbox(
    DefoConradModel* model
  , const QString& text
  , QWidget *parent
) :
    DefoConradCheckbox(model, text, parent)
{

  setChecked( model->getDeviceState() == READY );

  connect(
        model
      , SIGNAL( deviceStateChanged(State) )
      , this
      , SLOT( deviceStateChanged(State) )
  );

  connect( this, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)) );

}

/// Method to set the model when the DefoConradAction is toggled.
void DefoConradDeviceCheckbox::setChecked(bool checked) {

  DefoConradCheckbox::setChecked( checked );
  model_->setDeviceEnabled( checked );

}

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

  setEnabled( model->getDeviceState() == READY );
  setChecked( model->getSwitchState(device) == READY );

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

  connect( this, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)) );

}

/// Method to notify the DefoConradAction upon model change.
void DefoConradSwitchCheckbox::switchStateChanged(
    DefoConradModel::DeviceSwitch device
  , State newState
) {

  // Check if the updated device is ours
  if ( device_ == device ) {
    setChecked(
          newState == READY
//      ||  newState == INITIALIZING
    );
  }

}

/// Method to set the model when the DefoConradAction is toggled.
void DefoConradSwitchCheckbox::setChecked(bool checked) {

  DefoConradCheckbox::setChecked( checked );
  model_->setSwitchEnabled( device_, checked );

}

/// Sets whether it makes sense to be enabled according to switch state.
void DefoConradSwitchCheckbox::deviceStateChanged(
    State newState
) {

  setEnabled( newState == READY );

}

#include "HamegModel.h"

/*
  HamegModel implementation
  */
const QString HamegModel::Hameg_PORT = QString("/dev/ttyS5");

HamegModel::HamegModel(QObject *parent) :
    QObject(parent)
//  , state_(OFF) // Initialize all fields to prevent random values
//  , controller_(NULL)
  , AbstractDeviceModel()
{
  setDeviceEnabled(true);
}

/**
  Sets up the communication with the Hameg FP50 chiller and retrieves the
  settings and read-outs.
  */
void HamegModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(Hameg_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );

  if ( enabled ) {
    setDeviceState(READY);
    updateInformation();
  }
  else {
    setDeviceState( OFF );
    delete controller_;
    controller_ = NULL;
  }
}

/// \see AbstractDeviceModel::setDeviceState
void HamegModel::setDeviceState( State state ) {

  if ( state_ != state ) {
    state_ = state;

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the Hameg chiller and signals any
  changes.
  */
void HamegModel::updateInformation() {

  if ( state_ == READY ) {
    emit informationChanged();
  }
}

/// Attempts to enable/disable the (communication with) the Hameg FP50 chiller.
void HamegModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel::setDeviceEnabled(enabled);
}

void HamegModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

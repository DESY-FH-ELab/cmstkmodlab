#include "HuberPetiteFleurModel.h"

/*
  HuberPetiteFleurModel implementation
  */
const QString HuberPetiteFleurModel::HuberPetiteFleur_PORT = QString("/dev/ttyS5");

HuberPetiteFleurModel::HuberPetiteFleurModel(float updateInterval, QObject *parent) :
    QObject(parent)
//  , state_(OFF) // Initialize all fields to prevent random values
//  , controller_(NULL)
  , AbstractDeviceModel<HuberPetiteFleur_t>()
  , updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

    setDeviceEnabled(true);
    setControlsEnabled(true);
}

/**
  Sets up the communication with the Huber Petite Fleur chiller and retrieves the
  settings and read-outs.
  */
void HuberPetiteFleurModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(HuberPetiteFleur_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->IsCommunication() );

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
void HuberPetiteFleurModel::setDeviceState( State state ) {

  if ( state_ != state ) {
    state_ = state;

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the HuberPetiteFleur chiller and signals any
  changes.
  */
void HuberPetiteFleurModel::updateInformation() {

  if ( state_ == READY ) {
    emit informationChanged();
  }
}

/// Attempts to enable/disable the (communication with) the HuberPetiteFleur FP50 chiller.
void HuberPetiteFleurModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<HuberPetiteFleur_t>::setDeviceEnabled(enabled);
}

void HuberPetiteFleurModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

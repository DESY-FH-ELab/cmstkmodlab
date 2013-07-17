#include "HuberPetiteFleurModel.h"

/*
  HuberPetiteFleurModel implementation
  */
const QString HuberPetiteFleurModel::HuberPetiteFleur_PORT = QString("/dev/ttyACM0");

HuberPetiteFleurModel::HuberPetiteFleurModel(float updateInterval, QObject *parent) :
    QObject(parent)
//  , state_(OFF) // Initialize all fields to prevent random values
//  , controller_(NULL)
  , AbstractDeviceModel<HuberPetiteFleur_t>()
  , updateInterval_(updateInterval)
  , circulatorEnabled_(false)
  , workingTemperature_(-40, 40, 2)
  , bathTemperature_(0)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

    setDeviceEnabled(true);
    setControlsEnabled(true);
}

bool HuberPetiteFleurModel::isCirculatorEnabled() const {
  return circulatorEnabled_;
}

const DeviceParameterFloat& HuberPetiteFleurModel::getWorkingTemperatureParameter() const {
  return workingTemperature_;
}

float HuberPetiteFleurModel::getBathTemperature() const {
  return bathTemperature_;
}

/**
  Sets up the communication with the Huber Petite Fleur chiller and retrieves the
  settings and read-outs.
  */
void HuberPetiteFleurModel::initialize() {


    std::cout << "void HuberPetiteFleurModel::initialize()" << std::endl;

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

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the HuberPetiteFleur chiller and signals any
  changes.
  */
void HuberPetiteFleurModel::updateInformation() {

    std::cout << "HuberPetiteFleurModel::updateInformation()" << std::endl;

  if ( state_ == READY ) {

      float newBathTemp = controller_->GetBathTemperature();
      float newWorkingTemp = controller_->GetWorkingTemperature();
      bool newCirculatorStatus = controller_->GetCirculatorStatus();

      if (   newBathTemp != bathTemperature_
          || newWorkingTemp != workingTemperature_.getValue()
          || newCirculatorStatus != circulatorEnabled_
      ) {

        bathTemperature_ = newBathTemp;
        workingTemperature_.setValue(newWorkingTemp);
        circulatorEnabled_ = newCirculatorStatus;

        emit informationChanged();
      }

      emit informationChanged();
  }
}

/**
  Tries to update the given parameter by setting the value on the chiller.
  Will signal upon succes and failure, such that GUI induced request can be
  denied.
  */
template <class T> void HuberPetiteFleurModel::updateParameterCache(
    DeviceParameter<T>& parameter
  , const T& value
) {

  if ( parameter.getValue() != value ) {
    // Store old value
    T oldValue = parameter.getValue();

    // Emit signal to notify (reverted) changes
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

/// Attempts to enable or disable the circulator.
void HuberPetiteFleurModel::setCirculatorEnabled(bool enabled) {

  if ( circulatorEnabled_ != enabled ) {

    // Store new value
    circulatorEnabled_ = enabled;

    // Attempt remote setting
    bool success = enabled
        ? controller_->SetCirculatorOn()
        : controller_->SetCirculatorOff();

    // Revert upon failure
    if ( !success )
      circulatorEnabled_ = !enabled;

    emit informationChanged();
  }
}

void HuberPetiteFleurModel::setWorkingTemperatureValue(double value) {

  if ( state_ == READY ) {

    if ( workingTemperature_.getValue() != value ) {

      float oldValue = workingTemperature_.getValue();

      if (   workingTemperature_.setValue(static_cast<float>(value))
          && !controller_->SetWorkingTemperature(value)
          )
        workingTemperature_.setValue(oldValue);

      emit informationChanged();

    }
  }
}

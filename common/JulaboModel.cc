/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>

#include "JulaboModel.h"

/*
  JulaboModel implementation
  */

const QString JulaboModel::JULABO_PORT = QString("/dev/ttyS5");

/**
  \brief Creates a new JulaboModel for command and control of the chiller.
  The JulaboModel periodically checks for updated read-outs with
  the Julabo FP50 chiller used to cool the experimental setup. The default
  update interval is 5 seconds. The allowed ranges for the chiller parameters
  are hardcoded in the Parameter-object fields using the values provided by the
  Julabo manual.
  \arg updateInterval Time between read-out cache refreshes; in seconds.
  \arg parent Pointer to the parent QObject.
  */
JulaboModel::JulaboModel(float updateInterval, QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<JulaboFP50_t>(),
    updateInterval_(updateInterval),
    proportional_(0.1, 99.9, 1),
    integral_(3, 9999, 0),
    differential_(0, 999, 0),
    circulatorEnabled_(false),
    pumpPressure_(1, 4, 0),
    workingTemperature_(-94.99, 300.00, 2),
    bathTemperature_(0),
    safetySensorTemperature_(0),
    power_(0)
{

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

  setDeviceEnabled(true);
}

// Getter functions for (cached) Julabo state information

const DeviceParameterFloat& JulaboModel::getProportionalParameter() const {
  return proportional_;
}

const DeviceParameterUInt& JulaboModel::getIntegralParameter() const {
  return integral_;
}

const DeviceParameterUInt& JulaboModel::getDifferentialParameter() const {
  return differential_;
}

bool JulaboModel::isCirculatorEnabled() const {
  return circulatorEnabled_;
}

const DeviceParameterUInt& JulaboModel::getPumpPressureParameter() const {
  return pumpPressure_;
}

const DeviceParameterFloat& JulaboModel::getWorkingTemperatureParameter() const {
  return workingTemperature_;
}

float JulaboModel::getBathTemperature() const {
  return bathTemperature_;
}

float JulaboModel::getSafetySensorTemperature() const {
  return safetySensorTemperature_;
}

unsigned int JulaboModel::getPower() const {
  return power_;
}

/**
  Sets up the communication with the Julabo FP50 chiller and retrieves the
  settings and read-outs.
  */
void JulaboModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(JULABO_PORT);

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
void JulaboModel::setDeviceState( State state ) {

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
  Updates the cached information about the Julabo chiller and signals any
  changes.
  */
void JulaboModel::updateInformation() {

  NQLog("HuberPetiteFleurModel", NQLog::Debug) << "updateInformation()";

  // NOTE Julabo status messages in manual §12.4
  if ( state_ == READY ) {
    float newBathTemp = controller_->GetBathTemperature();
//    float newSafetySensorTemp = controller_->GetSafetySensorTemperature();
    float newWorkingTemp = controller_->GetWorkingTemperature();
    unsigned int newHeatingPower = controller_->GetHeatingPower();
    unsigned int newPumpPressure = controller_->GetPumpPressure();
    bool newCirculatorStatus = controller_->GetCirculatorStatus();

    // NOTE virtual std::pair<int,std::string> GetStatus( void ) const = 0;
//    std::pair<int,std::string> status = controller_->GetStatus();

    if (newBathTemp != bathTemperature_ ||
        newWorkingTemp != workingTemperature_.getValue() ||
        newHeatingPower != power_ ||
        newPumpPressure != pumpPressure_.getValue() ||
        newCirculatorStatus != circulatorEnabled_) {

      bathTemperature_ = newBathTemp;
      workingTemperature_.setValue(newWorkingTemp);
      power_ = newHeatingPower;
      pumpPressure_.setValue(newPumpPressure);
      circulatorEnabled_ = newCirculatorStatus;

      NQLog("JulaboModel", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }

}

/* CONTROL FUNCTIONS */

/**
  Tries to update the given parameter by setting the value on the chiller.
  Will signal upon succes and failure, such that GUI induced request can be
  denied.
  */
template <class T> void JulaboModel::updateParameterCache(DeviceParameter<T>& parameter,
                                                          const T& value) {

  if ( parameter.getValue() != value ) {
    // Store old value
    T oldValue = parameter.getValue();

    // Attempt to set new value (in cache and on device), revert on failure
    if (parameter.setValue(value) &&
        !controller_->SetControlParameters(proportional_.getValue(),
                                           integral_.getValue(),
                                           differential_.getValue()))
      parameter.setValue(oldValue);

    // Emit signal to notify (reverted) changes
    emit informationChanged();
  }
}

// Device control

/// Attempts to enable/disable the (communication with) the Julabo FP50 chiller.
void JulaboModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<JulaboFP50_t>::setDeviceEnabled(enabled);
}

void JulaboModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

// Cooling parameters

/// Attempts to set the proportional parameter (Xp) to the given value.
void JulaboModel::setProportionalValue(double value) {
  updateParameterCache( proportional_, static_cast<float>(value) );
}

/// Attempts to set the integral parameter (Tn) to the given value.
void JulaboModel::setIntegralValue(int value) {
  updateParameterCache( integral_, static_cast<unsigned int>(value) );
}

/// Attempts to set the differential parameter (Tv) to the given value.
void JulaboModel::setDifferentialValue(int value) {
  updateParameterCache( differential_, static_cast<unsigned int>(value) );
}


/// Attempts to enable or disable the circulator.
void JulaboModel::setCirculatorEnabled(bool enabled) {

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

/// Attempts to set the pump pressure stage.
void JulaboModel::setPumpPressureValue(int pressure) {

  if ( (int)pumpPressure_.getValue() != pressure ) {

    unsigned int oldValue = pumpPressure_.getValue();

    if (pumpPressure_.setValue(static_cast<unsigned int>(pressure)) &&
        !controller_->SetPumpPressure(pressure))
      pumpPressure_.setValue(oldValue);

    emit informationChanged();
  }
}

void JulaboModel::setWorkingTemperatureValue(double value) {

  if ( state_ == READY ) {

    if ( workingTemperature_.getValue() != value ) {

      float oldValue = workingTemperature_.getValue();

      if (workingTemperature_.setValue(static_cast<float>(value)) &&
          !controller_->SetWorkingTemperature(value))
        workingTemperature_.setValue(oldValue);

      emit informationChanged();

    }
  }
}

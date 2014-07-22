#include <QApplication>
#include <iostream>
#include "IotaModel.h"

/*
  IotaModel implementation
  */
IotaModel::IotaModel(const char* port,
                     float updateInterval,
                     QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<Iota_t>(),
    Iota_PORT(port),
    // state_(OFF), // Initialize all fields to prevent random values
    // controller_(NULL),
    updateInterval_(updateInterval),
    pumpEnabled_(false),
    actPressure_(0),
    setPressure_(0, 50, 1),
    actFlow_(0),
    setFlow_(0, 200, 1)
    {
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);
  setControlsEnabled(true);
    }


bool IotaModel::isPumpEnabled() const {
  return pumpEnabled_;
}


float IotaModel::getActPressure() const {
  return actPressure_;
}

const DeviceParameterFloat& IotaModel::getSetPressureParameter() const {
  return setPressure_;
}

float IotaModel::getActFlow() const {
  return actFlow_;
}

const DeviceParameterFloat& IotaModel::getSetFlowParameter() const {
  return setFlow_;
}

/**
  Sets up the communication with the Iota pump and retrieves the
  settings and read-outs.
 */
void IotaModel::initialize() {

  std::cout << "void IotaModel::initialize()" << std::endl;

  setDeviceState(INITIALIZING);

  renewController(Iota_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->IsCommunication() );

  if ( enabled ) {
    setDeviceState(READY);
    updateInformation();
  } else {
    setDeviceState( OFF );
    delete controller_;
    controller_ = NULL;
  }
}

/// \see AbstractDeviceModel::setDeviceState
void IotaModel::setDeviceState( State state ) {

  if ( state_ != state ) {
    state_ = state;

    // No need to run the timer if the pump is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the Iota pump and signals any
  changes.
 */
void IotaModel::updateInformation() {

  std::cout << "IotaModel::updateInformation()";
  if (thread()==QApplication::instance()->thread()) {
    std::cout << " running in main application thread" << std::endl;
  } else {
    std::cout << " running in dedicated DAQ thread" << std::endl;
  }

  if ( state_ == READY ) {

    float newGetSetFlow = controller_->GetSetFlow();
    float newGetActFlow = controller_->GetActFlow();
    float newGetSetPressure = controller_->GetSetPressure();
    float newGetActPressure = controller_->GetActPressure();
    bool newPumpStatus = controller_->GetStatus();

    if (newPumpStatus != pumpEnabled_ ||
        newGetSetFlow != setFlow_.getValue() ||
        newGetActFlow != actFlow_ ||
        newGetSetPressure != setPressure_.getValue() ||
        newGetActPressure != actPressure_) {

      pumpEnabled_ = newPumpStatus;
      setFlow_.setValue(newGetSetFlow);
      actFlow_ = newGetActFlow;
      setPressure_.setValue(newGetSetPressure);
      actPressure_ = newGetActPressure;

      emit informationChanged();
    }
  }
}

/**
  Tries to update the given parameter by setting the value on the pump.
  Will signal upon succes and failure, such that GUI induced request can be
  denied.
 */
template <class T> void IotaModel::updateParameterCache(DeviceParameter<T>& parameter,
                                                        const T& value) {

  if ( parameter.getValue() != value ) {
    // Store old value
    T oldValue = parameter.getValue();

    // Emit signal to notify (reverted) changes
    emit informationChanged();
  }
}

/// Attempts to enable/disable the (communication with) the Iota pump.
void IotaModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<Iota_t>::setDeviceEnabled(enabled);
}

void IotaModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

/// Attempts to enable or disable the circulator.
void IotaModel::setPumpEnabled(bool enabled) {

  if ( pumpEnabled_ != enabled ) {

    // Store new value
    pumpEnabled_ = enabled;

    // Attempt remote setting
    bool success = enabled ? controller_->SetPumpOn() : controller_->SetPumpOff();

    // Revert upon failure
    if ( !success )
      pumpEnabled_ = !enabled;

    emit informationChanged();
  }
}

void IotaModel::setFlowValue(double value) {

  if ( state_ == READY ) {

    if ( setFlow_.getValue() != value ) {

      int oldValue = setFlow_.getValue();

      if (setFlow_.setValue(static_cast<float>(value)) &&
          !controller_->SetFlow(value))
        setFlow_.setValue(oldValue);

      emit informationChanged();
    }
  }
}

void IotaModel::setPressureValue(double value) {

  if ( state_ == READY ) {

    if ( setPressure_.getValue() != value ) {

      int oldValue = setPressure_.getValue();

      if (setPressure_.setValue(static_cast<float>(value)) &&
          !controller_->SetPressure(value))
        setPressure_.setValue(oldValue);

      emit informationChanged();
    }
  }
}

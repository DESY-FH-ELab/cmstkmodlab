#include <QApplication>
#include <iostream>
#include "CoriFlowModel.h"

/*
  CoriFlowModel implementation
  */
CoriFlowModel::CoriFlowModel(const char* port,
                     float updateInterval,
                     QObject * /*parent*/) :
    QObject(),
    AbstractDeviceModel<CoriFlow_t>(),
    CoriFlow_PORT(port),
    // state_(OFF), // Initialize all fields to prevent random values
    // controller_(NULL),
    updateInterval_(updateInterval),
    Temp_(0),
    Pres_(0),
    Measure_(0)
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);
  setControlsEnabled(true);
}


float CoriFlowModel::getTemp() const {
  return Temp_;
}

float CoriFlowModel::getPres() const {
  return Pres_;
}

float CoriFlowModel::getMeasure() const {
  return Measure_;
}


/**
  Sets up the communication with the cori flow and retrieves the
  settings and read-outs.
 */
void CoriFlowModel::initialize() {

  std::cout << "void CoriFlowModel::initialize()" << std::endl;

  setDeviceState(INITIALIZING);

  renewController(CoriFlow_PORT);

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
void CoriFlowModel::setDeviceState( State state ) {

  if ( state_ != state ) {
    state_ = state;

    // No need to run the timer if the flow meter is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the cori flow meter and signals any
  changes.
 */
void CoriFlowModel::updateInformation() {

  std::cout << "CoriFlowModel::updateInformation()";
  if (thread()==QApplication::instance()->thread()) {
    std::cout << " running in main application thread" << std::endl;
  } else {
    std::cout << " running in dedicated DAQ thread" << std::endl;
  }

  if ( state_ == READY ) {

    float newGetTemp = controller_->getTemp();
    float newGetPres = controller_->getPres();
    float newGetMeasure = controller_->getMeasure();

    if (newGetTemp != Temp_ ||
        newGetPres != Pres_ ||
        newGetMeasure != Measure_) {

      Temp_ = newGetTemp;
      Pres_ = newGetPres;
      Measure_ = newGetMeasure;

      emit informationChanged();
    }
  }
}

/**
  Tries to update the given parameter by setting the value on the pump.
  Will signal upon succes and failure, such that GUI induced request can be
  denied.
 */
template <class T> void CoriFlowModel::updateParameterCache(DeviceParameter<T>& parameter,
                                                        const T& value) {

  if ( parameter.getValue() != value ) {
    // Store old value
    T oldValue = parameter.getValue();

    // Emit signal to notify (reverted) changes
    emit informationChanged();
  }
}

/// Attempts to enable/disable the (communication with) the CoriFlow Flow.
void CoriFlowModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<CoriFlow_t>::setDeviceEnabled(enabled);
}

void CoriFlowModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

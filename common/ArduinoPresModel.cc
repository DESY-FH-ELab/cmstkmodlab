#include <QApplication>
#include <iostream>
#include "ArduinoPresModel.h"

/*
  ArduinoPresModel implementation
  */
ArduinoPresModel::ArduinoPresModel(const char* port,
					     float updateInterval,
					     QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<ArduinoPres_t>(),
    ArduinoPres_PORT(port),
    // state_(OFF), // Initialize all fields to prevent random values
    // controller_(NULL),
    updateInterval_(updateInterval),
    PressureA_(0),
    PressureB_(0)

{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

    setDeviceEnabled(true);
    setControlsEnabled(true);
}




float ArduinoPresModel::getPressureA() const {
  return PressureA_;
}

float ArduinoPresModel::getPressureB() const {
  return PressureB_;
}

/**
  Sets up the communication with the ArduinoPres pump and retrieves the
  settings and read-outs.
  */
void ArduinoPresModel::initialize() {

    std::cout << "void ArduinoPresModel::initialize()" << std::endl;

  setDeviceState(INITIALIZING);

  renewController(ArduinoPres_PORT);

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
void ArduinoPresModel::setDeviceState( State state ) {

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
  Updates the cached information about the ArduinoPres and signals any
  changes.
  */
void ArduinoPresModel::updateInformation() {

    std::cout << "ArduinoPresModel::updateInformation()";
    if (thread()==QApplication::instance()->thread()) {
        std::cout << " running in main application thread" << std::endl;
    } else {
        std::cout << " running in dedicated DAQ thread" << std::endl;
    }

    if ( state_ == READY ) {

        float newGetPressureA = controller_->GetPressureA();
        float newGetPressureB = controller_->GetPressureB();

        if (newGetPressureA != PressureA_ ||
            newGetPressureB != PressureB_) {

            PressureA_ = newGetPressureA;
            PressureB_ = newGetPressureB;

            emit informationChanged();
        }
    }
}

/**
  Tries to update the given parameter by setting the value on the pump.
  Will signal upon succes and failure, such that GUI induced request can be
  denied.
  */
template <class T> void ArduinoPresModel::updateParameterCache(
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


/// Attempts to enable/disable the (communication with) the ArduinoPres.
void ArduinoPresModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<ArduinoPres_t>::setDeviceEnabled(enabled);
}

void ArduinoPresModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

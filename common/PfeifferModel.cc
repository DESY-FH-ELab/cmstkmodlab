#include "PfeifferModel.h"

/*
  PfeifferModel implementation
  */
const QString PfeifferModel::Pfeiffer_PORT = QString("/dev/ttyS5");

PfeifferModel::PfeifferModel(float updateInterval, QObject *parent) :
    QObject(parent),
    AbstractDeviceModel<PfeifferTPG262_t>(),
    pressure1_(1013),
    pressure2_(1013),
    updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    setDeviceEnabled(true);
}

double PfeifferModel::getPressure1() const
{
    return 0.3;
    return pressure1_;
}

double PfeifferModel::getPressure2() const
{
    return 2.0;
    return pressure2_;
}

/**
  Sets up the communication with the Pfeiffer TPG262 gauge and retrieves the
  settings and read-outs.
  */
void PfeifferModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(Pfeiffer_PORT);

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
void PfeifferModel::setDeviceState( State state ) {

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
  Updates the cached information about the Pfeiffer chiller and signals any
  changes.
  */
void PfeifferModel::updateInformation() {

    std::cout << "-----> PfeifferModel::updateInformation()" << std::endl;

    if ( state_ == READY ) {

      bool changed = false;
      double temp;

      temp = controller_->GetPressure1();
      if (pressure1_!=temp) {
        changed = true;
        pressure1_ = temp;
      }

      temp = controller_->GetPressure2();
      if (pressure2_!=temp) {
        changed = true;
        pressure2_ = temp;
      }

      std::cout << "##### PfeifferModel::updateInformation()" << std::endl;

      emit informationChanged();
  }
}

/// Attempts to enable/disable the (communication with) the Pfeiffer TPG262 gauge.
void PfeifferModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<PfeifferTPG262_t>::setDeviceEnabled(enabled);
}

void PfeifferModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

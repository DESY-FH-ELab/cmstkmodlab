#include "PfeifferModel.h"

/*
  PfeifferModel implementation
  */
const QString PfeifferModel::Pfeiffer_PORT = QString("/dev/ttyS4");

PfeifferModel::PfeifferModel(float updateInterval, QObject * /*parent*/) :
    QObject(),
    AbstractDeviceModel<PfeifferTPG262_t>(),
    status1_(VPfeifferTPG262::tpg262GaugeInvalidStatus),
    pressure1_(1013),
    status2_(VPfeifferTPG262::tpg262GaugeInvalidStatus),
    pressure2_(1013),
    updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    setDeviceEnabled(true);
}

VPfeifferTPG262::GaugeStatus PfeifferModel::getStatus1() const
{
    return status1_;
}

double PfeifferModel::getPressure1() const
{
    return pressure1_;
}

VPfeifferTPG262::GaugeStatus PfeifferModel::getStatus2() const
{
    return status2_;
}

double PfeifferModel::getPressure2() const
{
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

        VPfeifferTPG262::reading_t reading1, reading2;
        bool ok = controller_->GetPressures(reading1, reading2);

        if (ok) {
            if ( status1_ != reading1.first ||
                 pressure1_ != reading1.second ||
                 status2_ != reading2.first ||
                 pressure2_!= reading2.second ) {

                status1_ = reading1.first;
                pressure1_ = reading1.second;
                status2_ = reading2.first;
                pressure2_ = reading2.second;

                std::cout << "##### PfeifferModel::updateInformation()" << std::endl;

                emit informationChanged();
            }
        }
    }
}

/// Attempts to enable/disable the (communication with) the Pfeiffer TPG262 gauge.
void PfeifferModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<PfeifferTPG262_t>::setDeviceEnabled(enabled);
}

void PfeifferModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

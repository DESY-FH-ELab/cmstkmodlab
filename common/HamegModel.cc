#include <iostream>

#include "HamegModel.h"

/*
  HamegModel implementation
  */
const QString HamegModel::Hameg_PORT = QString("/dev/ttyUSB0");

HamegModel::HamegModel(float updateInterval, QObject *parent) :
    QObject(parent),
    AbstractDeviceModel<Hameg8143_t>(),
    updateInterval_(updateInterval),
    forceRemoteMode_(false),
    voltage1Parameter_(0, 30.000, 2),
    current1Parameter_(0, 0.250, 3),
    voltage2Parameter_(0, 30.000, 2),
    current2Parameter_(0, 0.250, 3)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

    setDeviceEnabled(true);
}

/**
  Sets up the communication with the Hameg power supply and retrieves the
  settings and read-outs.
  */
void HamegModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(Hameg_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );

  std::cout << "init " << (int)enabled << std::endl;

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

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the Hameg chiller and signals any
  changes.
  */
void HamegModel::updateInformation() {

  if ( state_ == READY ) {

	  unsigned int newStatus = controller_->GetStatus();

      float newSetVoltage1 = controller_->GetSetVoltage(1);
      float newSetCurrent1 = controller_->GetSetCurrent(1);
      float newSetVoltage2 = controller_->GetSetVoltage(2);
      float newSetCurrent2 = controller_->GetSetCurrent(2);

      float newVoltage1 = controller_->GetVoltage(1);
      float newCurrent1 = controller_->GetCurrent(1);
      float newVoltage2 = controller_->GetVoltage(2);
      float newCurrent2 = controller_->GetCurrent(2);

      if (!forceRemoteMode_) controller_->SetRemoteMode(false);

      if (newStatus != status_ ||
          newSetVoltage1 != voltage1Parameter_.getValue() ||
          newSetCurrent1 != current1Parameter_.getValue() ||
          newSetVoltage2 != voltage2Parameter_.getValue() ||
          newSetCurrent2 != current2Parameter_.getValue() ||
          newVoltage1 != voltage_[0] || newCurrent1 != current_[0] ||
          newVoltage2 != voltage_[1] || newCurrent2 != current_[1] ) {

          status_ = newStatus;

          voltage1Parameter_.setValue(newSetVoltage1);
          current1Parameter_.setValue(newSetCurrent1);
          voltage2Parameter_.setValue(newSetVoltage2);
          current2Parameter_.setValue(newSetCurrent2);

          voltage_[0] = newVoltage1;
          current_[0] = newCurrent1;
          voltage_[1] = newVoltage2;
          current_[1] = newCurrent2;

          std::cout << "emit informationChanged()" << std::endl;

          emit informationChanged();
      }
   }
}

/// Attempts to enable/disable the (communication with) the Hameg FP50 chiller.
void HamegModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<Hameg8143_t>::setDeviceEnabled(enabled);
}

void HamegModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

bool HamegModel::isConstantVoltageMode(int channel) const
{
    if (channel==1)
        return getStatus()&VHameg8143::hmCV1;
    else
        return getStatus()&VHameg8143::hmCV2;
}

const DeviceParameterFloat& HamegModel::getVoltageParameter(int channel) const
{
    if (channel==1)
        return voltage1Parameter_;
    else
        return voltage2Parameter_;
}

const DeviceParameterFloat& HamegModel::getCurrentParameter(int channel) const
{
    if (channel==1)
        return current1Parameter_;
    else
        return current2Parameter_;
}

float HamegModel::getVoltage(int channel) const
{
    return voltage_[channel-1];
}

float HamegModel::getCurrent(int channel) const
{
    return current_[channel-1];
}

void HamegModel::setRemoteMode(bool remote)
{
    if (state_!=READY) return;
    forceRemoteMode_ = remote;
    controller_->SetRemoteMode(remote);
}

void HamegModel::switchOutputOn(bool on)
{
    if (state_!=READY) return;
    if (on)
        controller_->SwitchOutputOn();
    else
        controller_->SwitchOutputOff();
}

void HamegModel::setVoltage(int channel, float voltage)
{
    if (state_!=READY) return;

    float oldVoltage;
    if (channel==1) {
        if (voltage1Parameter_.getValue()==voltage) return;
        oldVoltage = voltage1Parameter_.getValue();
        voltage1Parameter_.setValue(voltage);
    } else {
        if (voltage2Parameter_.getValue()==voltage) return;
        oldVoltage = voltage2Parameter_.getValue();
        voltage2Parameter_.setValue(voltage);
    }

    if (!controller_->SetVoltage(channel, voltage)) {
        if (channel==1) {
            voltage1Parameter_.setValue(oldVoltage);
        } else {
            voltage2Parameter_.setValue(oldVoltage);
        }
    }
    if (!forceRemoteMode_) controller_->SetRemoteMode(false);
}

void HamegModel::setCurrent(int channel, float current)
{
    if (state_!=READY) return;

    float oldCurrent;
    if (channel==1) {
        if (current1Parameter_.getValue()==current) return;
        oldCurrent = current1Parameter_.getValue();
        current1Parameter_.setValue(current);
    } else {
        if (current2Parameter_.getValue()==current) return;
        oldCurrent = current2Parameter_.getValue();
        current2Parameter_.setValue(current);
    }

    if (!controller_->SetCurrent(channel, current)) {
        if (channel==1) {
            current1Parameter_.setValue(oldCurrent);
        } else {
            current2Parameter_.setValue(oldCurrent);
        }
    }
    if (!forceRemoteMode_) controller_->SetRemoteMode(false);
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QApplication>

#include <nqlogger.h>

#include "RohdeSchwarzNGE103BModel.h"

/*
  RohdeSchwarzNGE103BModel implementation
 */
RohdeSchwarzNGE103BModel::RohdeSchwarzNGE103BModel(const char* port,
                                                   float updateInterval,
                                                   QObject * /*parent*/)
 :QObject(),
  AbstractDeviceModel<RohdeSchwarzNGE103B_t>(),
  RohdeSchwarzNGE103B_PORT(port),
  updateInterval_(updateInterval),
  voltageParameter_(VRohdeSchwarzNGE103B::MinVoltage,
                    VRohdeSchwarzNGE103B::MaxVoltage,
                    2),
  currentParameter_(VRohdeSchwarzNGE103B::MinCurrent,
                    VRohdeSchwarzNGE103B::MaxCurrent,
                    3),
  easyRampDurationParameter_(VRohdeSchwarzNGE103B::MinEasyRampDuration,
                             VRohdeSchwarzNGE103B::MaxEasyRampDuration,
                             2)
{
  voltage_ = { 0., 0., 0. };
  measuredVoltage_ = { 0., 0., 0. };
  current_ = { 0., 0., 0. };
  measuredCurrent_ = { 0., 0., 0. };

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);
}

bool RohdeSchwarzNGE103BModel::getOutputState(int channel) const
{
  if (channel>=1 && channel<=3) return outputState_[channel-1];
  return false;
}

void RohdeSchwarzNGE103BModel::setOutputState(int channel, bool state)
{
  NQLogDebug("RohdeSchwarzNGE103BModel") << "setOutputState(int channel, bool state) "
      << channel << " " << state;

  if (state_!=READY) return;

  if (channel<1 || channel>3) return;

  if (state==outputState_[channel-1]) return;

  controller_->SelectChannel(channel);
  controller_->SetOutputState(state);

  outputState_[channel-1] = state;
  
  if (easyRampState_[channel-1]) {
    QTimer::singleShot(1000*easyRampDuration_[channel-1], this,
                       SLOT(updateInformation()));
  } else {
    QTimer::singleShot(100, this,
                       SLOT(updateInformation()));
  }
}

unsigned int RohdeSchwarzNGE103BModel::getOutputMode(int channel) const
{
  if (channel>=1 && channel<=3) return outputMode_[channel-1];
  return 0;
}

float RohdeSchwarzNGE103BModel::getVoltage(int channel) const
{
  if (channel>=1 && channel<=3) return voltage_[channel-1];
  return 0;
}

void RohdeSchwarzNGE103BModel::setVoltage(int channel, float voltage)
{
  NQLogDebug("RohdeSchwarzNGE103BModel") << "setVoltage(int channel, float voltage) "
      << channel << " " << voltage;

  if (state_!=READY) return;

  if (channel<1 || channel>3) return;

  if (voltage<voltageParameter_.getMinimum() ||
      voltage>voltageParameter_.getMaximum()) return;

  if (voltage==voltage_[channel-1]) return;

  controller_->SelectChannel(channel);
  controller_->SetVoltage(voltage);

  voltage_[channel-1] = voltage;

  emit informationChanged();
}

float RohdeSchwarzNGE103BModel::getMeasuredVoltage(int channel) const
{
  if (channel>=1 && channel<=3) return measuredVoltage_[channel-1];
  return 0;
}

float RohdeSchwarzNGE103BModel::getCurrent(int channel) const
{
  if (channel>=1 && channel<=3) return current_[channel-1];
  return 0;
}

void RohdeSchwarzNGE103BModel::setCurrent(int channel, float current)
{
  NQLogDebug("RohdeSchwarzNGE103BModel") << "setCurrent(int channel, float current) "
      << channel << " " << current;

  if (state_!=READY) return;

  if (channel<1 || channel>3) return;

  if (current<currentParameter_.getMinimum() ||
      current>currentParameter_.getMaximum()) return;

  if (current==current_[channel-1]) return;

  controller_->SelectChannel(channel);
  controller_->SetCurrent(current);

  current_[channel-1] = current;

  emit informationChanged();
}

float RohdeSchwarzNGE103BModel::getMeasuredCurrent(int channel) const
{
  if (channel>=1 && channel<=3) return measuredCurrent_[channel-1];
  return 0;
}

float RohdeSchwarzNGE103BModel::getEasyRampDuration(int channel) const
{
  if (channel>=1 && channel<=3) return easyRampDuration_[channel-1];
  return 0;
}

void RohdeSchwarzNGE103BModel::setEasyRampDuration(int channel, float duration)
{
  NQLogDebug("RohdeSchwarzNGE103BModel") << "setEasyRampDuration(int channel, float duration) "
      << channel << " " << duration;

  if (state_!=READY) return;

  if (channel<1 || channel>3) return;

  if (duration<easyRampDurationParameter_.getMinimum() ||
      duration>easyRampDurationParameter_.getMaximum()) return;

  if (duration==easyRampDuration_[channel-1]) return;

  controller_->SelectChannel(channel);
  controller_->SetEasyRampDuration(duration);

  easyRampDuration_[channel-1] = duration;

  emit informationChanged();
}

bool RohdeSchwarzNGE103BModel::getEasyRampState(int channel) const
{
  if (channel>=1 && channel<=3) return easyRampState_[channel-1];
  return false;
}

void RohdeSchwarzNGE103BModel::setEasyRampState(int channel, bool state)
{
  NQLogDebug("RohdeSchwarzNGE103BModel") << "setEasyRampState(int channel, bool state) "
      << channel << " " << state;

  if (state_!=READY) return;

  if (channel<1 || channel>3) return;

  if (state==easyRampState_[channel-1]) return;

  controller_->SelectChannel(channel);
  controller_->SetEasyRampState(state);

  easyRampState_[channel-1] = state;

  emit informationChanged();
}

/**
  Sets up the communication with the RohdeSchwarzNGE103B power supply and retrieves the
  settings and read-outs.
  */
void RohdeSchwarzNGE103BModel::initialize()
{
  setDeviceState(INITIALIZING);

  renewController(RohdeSchwarzNGE103B_PORT);

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
void RohdeSchwarzNGE103BModel::setDeviceState( State state )
{
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
  Updates the cached information about the RohdeSchwarzNGE103B power supply and signals any
  changes.
  */
void RohdeSchwarzNGE103BModel::updateInformation()
{
  NQLogDebug("RohdeSchwarzNGE103BModel") << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("RohdeSchwarzNGE103BModel") << "running in main application thread";
  } else {
    NQLogDebug("RohdeSchwarzNGE103BModel") << "running in dedicated DAQ thread";
  }

  if (state_ == READY) {

    std::array<bool,3> newOutputState;
    std::array<unsigned int,3> newOutputMode;
    std::array<float,3> newVoltage;
    std::array<float,3> newMeasuredVoltage;
    std::array<float,3> newCurrent;
    std::array<float,3> newMeasuredCurrent;
    std::array<float,3> newEasyRampDuration;
    std::array<bool,3> newEasyRampState;

    for (unsigned int c=0;c<3;++c) {
      controller_->SelectChannel(c+1);

      newOutputState[c] = controller_->GetOutputState();
      newOutputMode[c] = controller_->GetOutputMode();
      newVoltage[c] = controller_->GetVoltage();
      newMeasuredVoltage[c] = controller_->MeasureVoltage();
      newCurrent[c] = controller_->GetCurrent();
      newMeasuredCurrent[c] = controller_->MeasureCurrent();
      newEasyRampDuration[c] = controller_->GetEasyRampDuration();
      newEasyRampState[c] = controller_->GetEasyRampState();
    }

    if (newOutputState!=outputState_ ||
        newOutputMode!=outputMode_ ||
        newVoltage!=voltage_ ||
        newMeasuredVoltage!=measuredVoltage_ ||
        newCurrent!=current_ ||
        newMeasuredCurrent!=measuredCurrent_ ||
        newEasyRampDuration!=easyRampDuration_ ||
        newEasyRampState!=easyRampState_) {

      outputState_ = newOutputState;
      outputMode_ = newOutputMode;
      voltage_ = newVoltage;
      measuredVoltage_ = newMeasuredVoltage;
      current_ = newCurrent;
      measuredCurrent_ = newMeasuredCurrent;
      easyRampDuration_ = newEasyRampDuration;
      easyRampState_ = newEasyRampState;

      NQLogDebug("RohdeSchwarzNGE103BModel") << "information changed";

      emit informationChanged();
    }
  }
}

/// Attempts to enable/disable the (communication with) the RohdeSchwarzNGE103B power supply.
void RohdeSchwarzNGE103BModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<RohdeSchwarzNGE103B_t>::setDeviceEnabled(enabled);
}

void RohdeSchwarzNGE103BModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

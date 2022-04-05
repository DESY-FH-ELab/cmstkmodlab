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

#include "KeithleyDAQ6510Model.h"

KeithleyDAQ6510Model::KeithleyDAQ6510Model(const char* port,
                                           int updateInterval,
                                           QObject * /*parent*/)
: QObject(),
  AbstractDeviceModel<KeithleyDAQ6510_t>(),
  port_(port),
  updateInterval_(updateInterval)
{
  for (int card=0;card<2;++card) {
    for (int channel=0;channel<10;++channel) {
      sensorStates_[card][channel] = OFF;
      temperatures_[card][channel] = 0.0;
    }
  }

  scanState_ = false;
  
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(scanTemperatures()) );

  setDeviceEnabled(true);
  setControlsEnabled(true);
}

/* DEVICE CONTROL */

/// Sets up the communication with the multimeter.
void KeithleyDAQ6510Model::initialize()
{
  setDeviceState(INITIALIZING);

  try {

    renewController(port_);

    // Give the device some time to process befor spamming it with upcoming
    // commands
    // sleep(1);

    setDeviceState(READY);

    // Set empty string to disable all channels
    // controller_->SetActiveChannels("0-9");

    // ... then reenable previously enabled sensors.
    for (int card=0;card<2;++card) {
      for (int channel=0;channel<10;++channel) {
        if (sensorStates_[card][channel] == READY)
          controller_->ActivateChannel(card+1, channel+1, true);
      }
    }

    // scanTemperatures();
  }
  catch (int e) {
    // TODO log failure
    setDeviceState(OFF);
  }
}

void KeithleyDAQ6510Model::setDeviceEnabled(bool enabled)
{
  // Trivial reimplementation as slot.
  AbstractDeviceModel<KeithleyDAQ6510_t>::setDeviceEnabled(enabled);

  // scanTemperatures();
}

void KeithleyDAQ6510Model::setScanEnabled(bool enabled)
{
  if ( state_ == READY ) {
    if (scanState_==enabled) return;

    if (enabled) {
      timer_->start();
    } else {
      timer_->stop();
    }
    
    scanState_ = enabled;

    emit scanStateChanged(enabled);

    if (scanState_) scanTemperatures();
  }
}

void KeithleyDAQ6510Model::setDeviceState(State state)
{
  if (state_ != state) {
    state_ = state;

    if ( state != READY && scanState_) {
      timer_->stop();
      scanState_ = false;
      emit scanStateChanged(scanState_);
    }

    emit deviceStateChanged(state);
  }
}

/* SENSOR CONTROL */

/// Caches the new state for a sensor and emits a signal if needed
void KeithleyDAQ6510Model::setSensorState(unsigned int sensor, State state)
{
  NQLogDebug("KeithleyDAQ6510Model") << "setSensorState(int sensor, State state) "
      << sensor << " " << state;

  unsigned int card = sensor / 100 - 1;
  unsigned int channel = sensor % 100 - 1;

  if (sensorStates_[card][channel] != state) {
    sensorStates_[card][channel] = state;
    emit sensorStateChanged(sensor, state);
  }
}

/// Attempts to enable a sensor.
void KeithleyDAQ6510Model::setSensorEnabled(unsigned int sensor, bool enabled)
{
  unsigned int card = sensor / 100 - 1;
  unsigned int channel = sensor % 100 - 1;

  if (enabled && sensorStates_[card][channel] == OFF) {
    setSensorState(sensor, INITIALIZING);
    controller_->ActivateChannel(card+1, channel+1, true);
    setSensorState(sensor, READY);
    scanDuration_ = controller_->GetScanDuration();
  }
  else if (!enabled && sensorStates_[card][channel] == READY) {
    setSensorState(sensor, CLOSING);
    controller_->ActivateChannel(card+1, channel+1, false);
    setSensorState(sensor, OFF);
    scanDuration_ = controller_->GetScanDuration();
  }
  emit activeSensorCountChanged(controller_->GetActiveChannelCount());
}

void KeithleyDAQ6510Model::setSensorMode(unsigned int sensor, KeithleyDAQ6510_t::ChannelMode_t mode)
{
  unsigned int card = sensor / 100;
  unsigned int channel = sensor % 100;

  if (controller_->GetChannelMode(card, channel)==mode) return;

  // if (sensorStates_[card][channel] == READY) {
    controller_->SetChannelMode(card, channel, mode);
    emit sensorModeChanged(sensor, mode);
  //}
}

void KeithleyDAQ6510Model::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

void KeithleyDAQ6510Model::setUpdateInterval(int updateInterval)
{
  if (updateInterval<10) return;
  updateInterval_ = updateInterval;
  timer_->setInterval(updateInterval_ * 1000);
}

/// Returns the current cached state of the requested sensor.
const State & KeithleyDAQ6510Model::getSensorState(unsigned int sensor) const
{
  unsigned int card = sensor / 100 - 1;
  unsigned int channel = sensor % 100 - 1;

  return sensorStates_[card][channel];
}

VKeithleyDAQ6510::ChannelMode_t KeithleyDAQ6510Model::getSensorMode(unsigned int sensor) const
{
  unsigned int card = sensor / 100;
  unsigned int channel = sensor % 100;

  return controller_->GetChannelMode(card, channel);
}

const std::map<VKeithleyDAQ6510::ChannelMode_t,std::string>& KeithleyDAQ6510Model::getSensorModeNames() const
{
  return controller_->GetChannelModeNames();
}

/// Returns the current cached temperature of the requested sensor.
double KeithleyDAQ6510Model::getTemperature(unsigned int sensor) const
{
  unsigned int card = sensor / 100 - 1;
  unsigned int channel = sensor % 100 - 1;

  return temperatures_[card][channel];
}

/**
  Calls the Keithley controller to get the current reading on the temperatures.
  These are then cached and signals are emitted upon changes.
  */
void KeithleyDAQ6510Model::scanTemperatures()
{
  if (controller_->GetActiveChannelCount()==0) return;
    
  NQLogDebug("KeithleyDAQ6510Model") << "scanTemperatures()";

  controller_->Scan();

  QTimer::singleShot(500 + scanDuration_*1000, this,
                     SLOT(scanComplete()));
}

void KeithleyDAQ6510Model::scanComplete()
{
  NQLogDebug("KeithleyDAQ6510Model") << "scanComplete()";

  VKeithleyDAQ6510::reading_t data;
  controller_->GetScanData(data);

  NQLogDebug("KeithleyDAQ6510Model") << "data size: " << data.size();

  bool changed = false;

  for (VKeithleyDAQ6510::reading_t::iterator it=data.begin();it!=data.end();++it) {
    unsigned int sensor;
    double temperature;
    double relativeTime;
    std::tie(sensor, temperature, relativeTime) = *it;

    unsigned int card = sensor / 100 - 1;
    unsigned int channel = sensor % 100 - 1;

    NQLogDebug("KeithleyDAQ6510Model") << "data: " << sensor << " " << temperature << " " << relativeTime;

    if (temperatures_[card][channel] != temperature) {
      temperatures_[card][channel] = temperature;
      emit temperatureChanged(sensor, temperature);
      changed = true;
    }
  }

  if (changed) emit informationChanged();
}

void KeithleyDAQ6510Model::statusMessage(const QString & text)
{
  emit message(text);
}

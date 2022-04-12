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

#include <iostream>
#include <chrono>
#include <thread>

#include <QApplication>

#include <nqlogger.h>

#include "KeithleyModel.h"

KeithleyModel::KeithleyModel(const char* port,
                             int updateInterval,
                             QObject * /*parent*/) :
    QObject(),
    AbstractDeviceModel<Keithley2700_t>(),
    port_(port),
    updateInterval_(updateInterval),
    sensorStates_(SENSOR_COUNT, OFF),
    temperatures_(SENSOR_COUNT, 0.0),
    gradients_(SENSOR_COUNT, 0.0),
    temperatureBuffer_(temperatures_),
    absoluteTime_(std::chrono::system_clock::now())
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(scanTemperatures()) );

  setDeviceEnabled(false);
  setControlsEnabled(true);
}

/* DEVICE CONTROL */

/// Sets up the communication with the multimeter.
void KeithleyModel::initialize() {

  using namespace std::chrono_literals;

  setDeviceState(INITIALIZING);

  try {

    renewController(port_);

    // Give the device some time to process befor spamming it with upcoming
    // commands
    std::this_thread::sleep_for(1s);

    setDeviceState(READY);

    // Set empty string to disable all channels
    // controller_->SetActiveChannels("0-9");

    // ... then reenable previously enabled sensors.
    for (unsigned int i = 0; i < SENSOR_COUNT; ++i) {
      if ( sensorStates_[i] == READY )
        controller_->AddActiveChannels( constructString(i) );
    }

    // scanTemperatures();

  }
  catch (int e) {
    // TODO log failure
    setDeviceState(OFF);
  }
}

void KeithleyModel::setDeviceEnabled(bool enabled) {
  // Trivial reimplementation as slot.
  AbstractDeviceModel<Keithley2700_t>::setDeviceEnabled(enabled);

  // scanTemperatures();
}

void KeithleyModel::setDeviceState(State state) {
  if (state_ != state) {
    state_ = state;

    if ( state == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

/* SENSOR CONTROL */

/// Caches the new state for a sensor and emits a signal if needed
void KeithleyModel::setSensorState(unsigned int sensor, State state) {

  if ( sensorStates_.at(sensor) != state) {
    sensorStates_[sensor] = state;
    emit sensorStateChanged(sensor, state);
  }

}

/// Attempts to enable a sensor.
void KeithleyModel::setSensorEnabled(unsigned int sensor, bool enabled) {

  // Construct string of length 1 using ASCII offset of '0'
  std::string channel = constructString( sensor );

  if ( enabled && sensorStates_.at(sensor) == OFF ) {
    setSensorState( sensor, INITIALIZING );
    controller_->AddActiveChannels( channel );
    setSensorState( sensor, READY );
  }
  else if ( !enabled && sensorStates_.at(sensor) == READY ) {
    setSensorState( sensor, CLOSING );
    controller_->DisableActiveChannels( channel );
    setSensorState( sensor, OFF );
  }
}

void KeithleyModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

void KeithleyModel::setUpdateInterval(int updateInterval) {

  if (updateInterval<10) return;
  updateInterval_ = updateInterval;
  timer_->setInterval(updateInterval_ * 1000);
}

/// Returns the current cached state of the requested sensor.
const State & KeithleyModel::getSensorState(unsigned int sensor) const {
  return sensorStates_.at(sensor);
}

/// Returns the current cached temperature of the requested sensor.
double KeithleyModel::getTemperature(unsigned int sensor) const
{
  return temperatures_.at(sensor);
}

/**
  Calls the Keithley controller to get the current reading on the temperatures.
  These are then cached and signals are emitted upon changes.
  */
void KeithleyModel::scanTemperatures()
{
  VKeithley2700::reading_t reading = controller_->Scan();

  // Good scan, cache the retrieved temperatures
  if (controller_->IsScanOk()) {

    NQLog("KeithleyModel", NQLog::Debug) << reading.size() << " temperature readings";

    for (VKeithley2700::reading_t::const_iterator it = reading.begin();
         it < reading.end();
         ++it) {

      unsigned int sensor = it->first;
      double temperature = it->second;

      // Check for changes
      if ( temperatures_.at(sensor) != temperature ) {
        temperatures_[sensor] = temperature;
        emit temperatureChanged(sensor, temperature);
      }

      // Check for OVERFLOW readings, i.e. disconnected or malfunction
      // TODO log OVERFLOW readings
      NQLog("KeithleyModel", NQLog::Debug) << sensor << " : " << temperature;
      // if ( temperature == std::numeric_limits<float>::infinity() )
      //   setSensorEnabled(sensor, false);

    }

    timeBuffer_.push_back(absoluteTime_);
    temperatureBuffer_.push_back(temperatures_);

    std::chrono::time_point<std::chrono::system_clock> lastTime = timeBuffer_.front();
    const std::vector<double> &lastTemperatures = temperatureBuffer_.front();
    std::chrono::duration<double> dt = absoluteTime_ - lastTime;
    
    if (dt.count()>=30) {
      for (unsigned int i=0;i<SENSOR_COUNT;++i) {
	if (sensorStates_[i] != READY) continue;
	
	double gradient = (temperatures_[i] - lastTemperatures[i]) / (dt.count()/60.);
	if ( gradients_.at(i) != gradient ) {
	  gradients_[i] = gradient;
	  emit temperatureGradientChanged(i, gradient);
	}
      }
    }
  } else {
    
    NQLog("KeithleyModel", NQLog::Message) << " scanTemperatures failed";

    /*
    channels_t activeChannels = controller_->GetActiveChannels();

    controller_->Reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    controller_->SetActiveChannels(activeChannels);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    */
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  VKeithley2700::channels_t activeChannels = controller_->GetActiveChannels();
  for (VKeithley2700::channels_t::iterator it=activeChannels.begin();it!=activeChannels.end();++it) {
    NQLog("KeithleyModel", NQLog::Message) << " scanTemperatures active channel: " << *it;
  }
  controller_->Reset();
  
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  
  controller_->SetActiveChannels(activeChannels);
  
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  absoluteTime_ = std::chrono::system_clock::now();
}

/// Creates a string from sensor number.
std::string KeithleyModel::constructString(unsigned int sensor)
{
  // Construct string of length 1 using ASCII offset of '0'
  return std::string( 1, static_cast<char>( sensor + '0' ) );
}

void KeithleyModel::statusMessage(const QString & text)
{
  emit message(text);
}

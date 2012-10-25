#include "DefoKeithleyModel.h"

const QString DefoKeithleyModel::KEITHLEY_PORT = QString("/dev/ttyS4");


DefoKeithleyModel::DefoKeithleyModel(double updateInterval, QObject *parent) :
      QObject(parent)
    , DefoAbstractDeviceModel()
    , updateInterval_(updateInterval)
    , sensorStates_(SENSOR_COUNT, OFF)
    , temperatures_(SENSOR_COUNT, 0.0)
{

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(scanTemperatures()) );

  setDeviceEnabled(false);
  setControlsEnabled(true);
}


/* DEVICE CONTROL */

/// Sets up the communication with the multimeter.
void DefoKeithleyModel::initialize() {

  setDeviceState(INITIALIZING);

  try {

    renewController(KEITHLEY_PORT);

    std::cout << std::numeric_limits<float>::max() << ", "
              << std::numeric_limits<float>::infinity << std::endl;

    // Give the device some time to process befor spamming it with upcoming
    // commands
    sleep(1);

    setDeviceState(READY);

    // Set empty string to disable all channels
//    controller_->SetActiveChannels("0-9");

    // ... then reenable previously enabled sensors.
    for (unsigned int i = 0; i < SENSOR_COUNT; ++i) {
      if ( sensorStates_[i] == READY )
        controller_->AddActiveChannels( constructString(i) );
    }

//    scanTemperatures();

  }
  catch (int e) {
    // TODO log failure
    setDeviceState(OFF);
  }

}

void DefoKeithleyModel::setDeviceEnabled(bool enabled) {
  // Trivial reimplementation as slot.
  DefoAbstractDeviceModel::setDeviceEnabled(enabled);
}

void DefoKeithleyModel::setDeviceState(State state) {
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
void DefoKeithleyModel::setSensorState(unsigned int sensor, State state) {

  if ( sensorStates_.at(sensor) != state) {
    sensorStates_[sensor] = state;
    emit sensorStateChanged(sensor, state);
  }

}

/// Attempts to enable a sensor.
void DefoKeithleyModel::setSensorEnabled(unsigned int sensor, bool enabled) {

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

void DefoKeithleyModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

/// Returns the current cached state of the requested sensor.
const State & DefoKeithleyModel::getSensorState(unsigned int sensor) const {
  return sensorStates_.at(sensor);
}

/// Returns the current cached temperature of the requested sensor.
double DefoKeithleyModel::getTemperature(unsigned int sensor) const
{
  return temperatures_.at(sensor);
}

/**
  Calls the Keithley controller to get the current reading on the temperatures.
  These are then cached and signals are emitted upon changes.
  */
void DefoKeithleyModel::scanTemperatures() {

  reading_t reading = controller_->Scan();

  // Good scan, cache the retrieved temperatures
  if ( controller_->IsScanOk() ) {

    std::cout << reading.size() << " temperature readings" << std::endl;

    for (
           reading_t::const_iterator it = reading.begin()
         ; it < reading.end()
         ; ++it
    ) {

      unsigned int sensor = it->first;
      double temperature = it->second;

      // Check for changes
      if ( temperatures_.at(sensor) != temperature ) {
        temperatures_[sensor] = temperature;
        emit temperatureChanged(sensor, temperature);
      }

      // Check for OVERFLOW readings, i.e. disconnected or malfunction
      // TODO log OVERFLOW readings
      std::cout << sensor << " : " << temperature << std::endl;
//      if ( temperature == std::numeric_limits<float>::infinity() )
//        setSensorEnabled(sensor, false);

    }

  }
  // FIXME If bad scans don't normally happen, close(), otherwise ignore.
//  else
//    close();

}

/// Creates a string from sensor number.
std::string DefoKeithleyModel::constructString(unsigned int sensor)
{
  // Construct string of length 1 using ASCII offset of '0'
  return std::string( 1, static_cast<char>( sensor + '0' ) );
}

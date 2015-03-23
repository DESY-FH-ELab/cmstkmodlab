#include "DefoConradModel.h"

DefoConradModel::DefoConradModel(QObject *parent) :
    QObject(parent)
  , AbstractDeviceModel()
  , switchStates_(8, OFF)
{

  setDeviceEnabled(true);
  setControlsEnabled(true);

  // TODO Implement a timer to periodically check the device status.

}

/// Shuts down all the devices before closing down.
DefoConradModel::~DefoConradModel() {
  close();
}

/**
  Will try to enable communication with the switch.
  */
void DefoConradModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel::setDeviceEnabled(enabled);
}

void DefoConradModel::enableSwitch( DefoConradModel::DeviceSwitch device ) {
  setSwitchEnabled(device, true);
}

void DefoConradModel::disableSwitch( DefoConradModel::DeviceSwitch device ) {
  setSwitchEnabled(device, false);
}

void DefoConradModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}

/**
  Auto-detects Conrad card through /dev/ttyUSB* files and tests the connection.
  */
void DefoConradModel::initialize( void ) {

  // Set state to DefoConradModel::INITIALIZING so views
  // may be notified of non-responsiveness
  setDeviceState( INITIALIZING );

  // create a list with all available ttyUSB device (system) files
  QStringList filters("ttyUSB*");

  QDir devDir( "/dev" );
  devDir.setNameFilters(filters);
  devDir.setFilter(QDir::System);
  QStringList list = devDir.entryList();

  // Only loop over list when not empty
  if ( !list.empty() ) {

    // browse and try initialize() until conradController_ gets an answer
    QStringList::const_iterator it = list.begin();
    QString port = QString( "/dev/" ) + *it;

    renewController( port );

    while ( it < list.end() && !controller_->initialize() ) {

      // Compose full absolute location of USB device
      port = QString( "/dev/" ) + *it;
      renewController( port );

      ++it;
    }

    // check communication; if it is not at the end, switch was found
    if( it != list.end() ) {

      // read and init status
      std::vector<bool> status = controller_->queryStatus();

      // Announce new states
      if( status.size() == 8 ) {

        setAllSwitchesReady( status );
        setDeviceState( READY );

        // FIXME Redirect to central logger
        //    if( debugLevel_ >= 1 )
        std::cout << " [DefoConradModel::initialize] =1= "
                  << "connection to conrad via: " << port.toStdString() << "."
                  << std::endl;

      }
      else {

        /*
         would be 0 if query failed (according to ConradController::queryStatus)
         This means device malfunction, so set state accordingly
         */

        setDeviceFullOff();

        std::cerr << " [DefoConradModel::initialize] "
                  << "** ERROR: received malformed state vector."
                  << std::endl;

      }
    }
    else {
      // if not successful; i.e. DEVICE NOT FOUND
      setDeviceFullOff();
      // TODO Log why it failed
      std::cerr << " [DefoConradModel::initialize] "
                << "** ERROR: Cannot connect to Conrad. Make sure that "
                << "/dev/ttyUSB* is present and readable and no other process "
                << "is connecting to the device." << std::endl;
    }

  }
  else {

    setDeviceFullOff();

    std::cerr << " [DefoConradModel::initialize] "
              << "** ERROR: Cannot connect to Conrad. Make sure that "
              << "/dev/ttyUSB* is present and readable and no other process "
              << "is connecting to the device." << std::endl;

  }

}

/// Sets all the device states to OFF
void DefoConradModel::setDeviceFullOff() {
  setDeviceState( OFF );
  setAllSwitchesReady( std::vector<bool>( 8, false ) );
}

/**
  Shuts down all the connected devices and closes down the connection to the
  switch.
  */
void DefoConradModel::close() {

  // TODO Controlled shutdown for busy (initializing) device.

  if ( getDeviceState() == READY ) {

    // Notify that no enable commands should be sent.
    setDeviceState( CLOSING );

    // Disable all connected devices
    for ( int i = 0; i < 8; ++i )
      setSwitchEnabledRaw( static_cast<DeviceSwitch>(i), false );

    // Disable switch
    destroyController();

    setDeviceState( OFF );

  }

}

/**
  Sets the current state of the switch and emits a switchStateChanged upon
  change.
  */
void DefoConradModel::setDeviceState(State state) {

  if ( state_ != state ) {
    state_ = state;
    emit deviceStateChanged(state_);
  }

}


/**
  Enabled or disables a certain device channel on the switch.
  */
void DefoConradModel::setSwitchEnabled(
    DefoConradModel::DeviceSwitch device
  , bool enabled
) {

  // TODO Upon failure, check if the switch is still reachable

  // Check if the switch communication is ready
  if ( getDeviceState() == READY )
    setSwitchEnabledRaw(device, enabled);
  else
    setSwitchState(device, OFF);


  // TODO Notify of malfunction

}


/// Returns whether the requested device is currently enabled.
const State& DefoConradModel::getSwitchState(
    DefoConradModel::DeviceSwitch device
) const {
  return switchStates_.at( device );
}

/// Sets the current state for a device and emits a signal if necessary.
void DefoConradModel::setSwitchState(
    DefoConradModel::DeviceSwitch device
  , State state
) {

  if ( switchStates_.at( device ) != state ) {
    switchStates_[device] = state;
    emit switchStateChanged( device, state );
  }

}

/**
  Small convenience method to set all device status using the vector provided by
  ConradController::queryStatus().
  */
void DefoConradModel::setAllSwitchesReady( const std::vector<bool>& ready ) {

  DeviceSwitch device;
  for ( unsigned int i = 0; i < ready.size(); ++i ) {
    // Set device state according to queried state
    device = static_cast<DeviceSwitch>(i);
    setSwitchState( device, ready.at(device) ? READY : OFF );
  }

}

/// Sets the switch state without checking the device state
void DefoConradModel::setSwitchEnabledRaw(DeviceSwitch device, bool enabled) {

  if ( enabled && getSwitchState(device) == OFF ) {

    setSwitchState( device, INITIALIZING );
    // Add 1 to device because of shift required by setChannel
    bool success = controller_->setChannel( device + 1, enabled );
    setSwitchState( device, success ? READY : OFF );

  }
  else if ( !enabled && getSwitchState(device) == READY ) {

    setSwitchState( device, CLOSING );
    // Add 1 to device because of shift required by setChannel
    bool success = controller_->setChannel( device + 1, enabled );
    setSwitchState( device, success ? OFF : READY );

  }

}

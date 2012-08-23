#include "DefoConradModel.h"
#include <QFuture>

DefoConradModel::DefoConradModel(QObject *parent) :
    QObject(parent)
{

  controller_ = NULL;

  state_ = OFF;
  switchStates_.assign( 8, OFF );

  setDeviceEnabled(true);

  // TODO Implement a timer to periodically check the device status.

  // TODO initialise switch and devices

}

/**
  Shuts down the switch and connected devices and cleans up.
  */
DefoConradModel::~DefoConradModel() {

  close();
  delete controller_;

}

/**
  Will try to enable communication with the switch.
  */
void DefoConradModel::setDeviceEnabled(bool enabled) {

   // To be enabled and off
  if ( enabled && getDeviceState() == OFF )
    initialize();
  // To be disabled and on
  else if ( !enabled && getDeviceState() == READY )
    close();

  /*
   If in 'busy state', a signal for OFF/READY will follow soon, reverting
   any changes ignored by only checking for steady states (i.e. OFF and READY).
   */

}

/**
  Renews the current ConradController* for the given port.
  */
void DefoConradModel::renewController( const QString& port ) {

  if ( controller_ ) {
    delete controller_;
  }

  controller_ = new ConradController_t( port.toStdString().c_str() );

}

/**
  Auto-detects Conrad card through /dev/ttyUSB* files and tests the connection.
  */
void DefoConradModel::initialize( void ) {

  // Set state to DefoConradModel::INITIALIZING so views
  // may be notified of non-responsiveness
  setDeviceState( INITIALIZING );

  // create a list with all available ttyUSB device (system) files
  QStringList filters;
  filters.append("ttyUSB*");

  QDir devDir( "/dev" );
  QStringList list = devDir.entryList(
          filters
        , QDir::System // only system files!
  );

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

  // check communication; if it not at end, switch was found
  if( it != list.end() ) {

    // read and init status
    std::vector<bool> status = controller_->queryStatus();

    // Announce new states
    if( status.size() != 8 ) {

      // would be 0 if query failed (according to ConradController::queryStatus)
      // This means device malfunction, so set state accordingly

      setDeviceState( OFF );
      status.assign( 8, false );
      setAllSwitchesReady(status);

      std::cerr << " [DefoConradModel::initialize] "
                << "** ERROR: received malformed state vector." << std::endl;

    }
    else {

      setDeviceState( READY );
      setAllSwitchesReady( status );

    }

    // FIXME Redirect to central logger
//    if( debugLevel_ >= 1 )
      std::cout << " [DefoConradModel::initialize] =1= "
                << "connection to conrad via: " << port.toStdString() << "."
                << std::endl;

  }
  else { // if not successful; i.e. DEVICE NOT FOUND

    setDeviceState( OFF );
    setAllSwitchesReady( std::vector<bool>( 8, false ) );

    std::cerr << " [DefoConradModel::initialize] "
              << "** ERROR: Cannot connect to Conrad. Make sure that "
              << "/dev/ttyUSB* is present and readable and no other process "
              << "is connecting to the device." << std::endl;

  }

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
      setSwitchEnabled( static_cast<DeviceSwitch>(i), false );

    // Disable switch
    delete controller_;
    controller_ = NULL; // Essential step to prevent segfaults!

    setDeviceState( OFF );

  }

}


/**
  Returns the current state of the switch. If control of the Conrad relais
  switch is handled in a separate thread, the GUI can stay responsive and
  display the correct state of the device.
  */
const /*DefoConradModel::*/State& DefoConradModel::getDeviceState() const {

  return state_;

}


/**
  Sets the current state of the switch and emits a switchStateChanged upon
  change.
  */
void DefoConradModel::setDeviceState( /*DefoConradModel::*/State state ) {

  if ( state_ != state ) {
    state_ = state;
    emit deviceStateChanged(state);
  }

}


/**
  Enabled or disables a certain device channel on the switch.
  */
void DefoConradModel::setSwitchEnabled(
    DefoConradModel::DeviceSwitch device
  , bool enabled
) {

  // Enable switch if necessary
  if ( getDeviceState() == OFF && enabled && getSwitchState(device) != OFF )
    initialize();

  // Continue if possible
  // TODO Upon failure, check if the switch is still reachable
  if ( getDeviceState() == READY || getDeviceState() == CLOSING ) {

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

    // TODO Busy state does not correspond to new state

  }

  // TODO Notify of malfunction

}


/**
  Returns whether the requested device is currently enabled.
  */
const /*DefoConradModel::*/State& DefoConradModel::getSwitchState(
    DefoConradModel::DeviceSwitch device
) const {

  return switchStates_.at( device );

}

/**
  Sets the current state for a device and emits a signal if necessary.
  */
void DefoConradModel::setSwitchState(
      DefoConradModel::DeviceSwitch device
    , /*DefoConradModel::*/State state
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

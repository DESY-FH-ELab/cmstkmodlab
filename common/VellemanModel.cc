/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//                 Modeled on cmstkmodlab/common/ConradModel.cc                //
//                         Last Updated August 23, 2019                        //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


#include <nqlogger.h>

#include <QFileInfo>
#include <QDir>
#include <QStringList>

#include "VellemanModel.h"

#ifndef CMD_SWITCH_RELAY_ON
#define CMD_SWITCH_RELAY_ON 0x11
#endif

#ifndef CMD_SWITCH_RELAY_OFF
#define CMD_SWITCH_RELAY_OFF 0x12
#endif

/* 
   Constructor
*/
VellemanModel::VellemanModel(const std::string& port, QObject* /* parent */)
  : QObject()
  , AbstractDeviceModel()
  , port_(port.c_str())
  , relayStates_(8, OFF)
{
  setDeviceEnabled(true); // public slot
  setControlsEnabled(true); // public slot
}

/*
  Shut down all devices before closing down.
*/
VellemanModel::~VellemanModel()
{
  close();
}

/*
  Try to enable communication with relay card
*/
void VellemanModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel::setDeviceEnabled(enabled);
}

/* 
   Slots to enable and disable relays
*/
void VellemanModel::enableRelay(int relay)
{
  setRelayEnabled(relay, true);
}

void VellemanModel::disableRelay(int relay)
{
  setRelayEnabled(relay, false);
}

/*
  Slot that emits signal that control state changed
*/
void VellemanModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

/* 
   Auto-detect Velleman relay card through /dev/ttyACM*
   and test connection
*/
void VellemanModel::initialize( void )
{
  // Set state to VellemanModel::INITIALIZING so viewers
  // may be notified of card not responding
  setDeviceState( INITIALIZING );

#ifdef USE_FAKEIO
  renewController("/dev/ttyUSBFake");

  controller_->initialize();

  const std::vector<bool> status = controller_->queryRelayStatus();

  // Announce new states
  if (status.size() == 8) {

    setAllRelaysSame(status);
    setDeviceState( READY );

  }
#else
  if(port_.isEmpty())
    {
      setDeviceFullOff();

      NQLog("VellemanModel", NQLog::Critical) << "initialize"
	<< ": path to device file is empty, VellemanModel will not be initialized";
    }
  else
    {
      // Create a list with all available device (system) files
      const QFileInfo port_qfileinfo(port_);

      QDir portDir(port_qfileinfo.dir().canonicalPath());
      portDir.setNameFilters(QStringList(port_qfileinfo.fileName()));
      portDir.setFilter(QDir::System);

      const QStringList ports_list = portDir.entryList();

      // Only loop over port list when not empty
      if (not ports_list.empty())
	{
	  // Browse and try to initialize() device until VellemanController gets answer
	  bool controller_initialized(false);

	  for(const auto& port : ports_list)
	    {
	      renewController(portDir.canonicalPath()+"/"+port);

	      controller_initialized = controller_->initialize();

	      // Velleman controller initialized successfully, break loop and proceed
	      if(controller_initialized == true) { break; }
	    }

	  // Check communication. If it is not at the end, relay was found
	  // NOTE: what does this mean??
	  if(controller_initialized)
	    {
	      // Read and initialize status of relays
	      const std::vector<bool> status = controller_->queryRelayStatus();

	      // Announce new states of relays
	      if(status.size() == 8)
		{
		  setAllRelaysSame(status);
		  setDeviceState( READY );

		  NQLog("VellemanModel", NQLog::Message) << "initialize"
		    << ": successfully connected VellemanModel to port " << controller_->comPort();
		}
	      else
		{
		  // Size of status would be 0 if query failed (see VellemanController::queryRelayStatus).
		  // This status size indicates a device malfunction, so set state accordingly.
		  setDeviceFullOff();

		  NQLog("VellemanModel", NQLog::Fatal) << "initialize"
		    << ": ** ERROR: received malformed status vector from device (port: " << controller_->comPort() << ")";
		}
	    }
	  else
	    {
	      // If controller not successfully initialized (i.e. DEVICE NOTE FOUND)
	      setDeviceFullOff();

	      // TODO Log why this initialization failed
	      NQLog("VellemanModel::intialize()", NQLog::Fatal) << "Cannot connect to Velleman. Make sure that";
	      NQLog("VellemanModel::intialize()", NQLog::Fatal) << port_ << " is present and readable and no other process";
	      NQLog("VellemanModel::intialize()", NQLog::Fatal) << "is connecting to the device.";
	    }
	}
      else
	{
	  setDeviceFullOff();

	  NQLog("VellemanModel", NQLog::Fatal) << "initialize"
	    << "Failed to initialize VellemanModel due to empty list of device files to be checked (specified port(s): " << port_ << ")";
	}
    }
#endif
} // end of VellemanModel::initialize()

/*
  Sets all of the  states to OFF
*/
void VellemanModel::setDeviceFullOff()
{
  setDeviceState( OFF );
  setAllRelaysSame(std::vector<bool>(8, false));
}

/*
  Shuts down all connected devices and closes the connection to
  relay card.
*/
void VellemanModel::close()
{
  // TODO Controlled shutdown for busy (initializing) device.

  if (getDeviceState() == READY) {

    // Notify that no enable commands should be sent.
    setDeviceState( CLOSING );

    // Disable all connected devices
    for (int i = 0; i < 8; ++i)
      setRelayEnabledRaw(i, false);

    // Disable relay card
    destroyController();

    setDeviceState( OFF );
  }
}


/*
  Sets the current state of the relay card and emits a
  relayStateChanged signal upon this change.
*/
void VellemanModel::setDeviceState(State state)
{
  if (state_ != state) {
    state_ = state;
    emit deviceStateChanged(state_);
  }
}

/*
  Enables or disables a certain device relay on the relay
  card.
*/
void VellemanModel::setRelayEnabled(int relay, bool enabled)
{
  // TODO Upon failure, check if the relay is still reachable

  // Check if the relay communication is ready
  if (getDeviceState() == READY)
    setRelayEnabledRaw(relay, enabled);
  else
    setRelayState(relay, OFF);

  // TODO Notify of malfunction
}

/* 
   Returns whether the requested device is currently enabled.
*/
const State& VellemanModel::getRelayState(int relay) const
{
  return relayStates_.at(relay); // NOTE: relays here are offset by 1 (0-7)
}

/*
  Sets the current state for a device and emits a signal if 
  necessary.
*/
void VellemanModel::setRelayState(int relay, State state)
{
  if (relayStates_.at(relay) != state) { // NOTE: relays here are offset by 1 (0-7)
    relayStates_[relay] = state; // NOTE: relays here are offset by 1 (0-7)
    emit relayStateChanged(relay, state);
  }
}

/*
  Small convenience method to set all device relay states using
  the vector provided by VellemanController::queryRelayStatus().
*/
void VellemanModel::setAllRelaysSame(const std::vector<bool>& status)
{
  for (unsigned int i = 0; i < status.size(); ++i) {
    // Set device state according to relay status received
    setRelayState(i, status.at(i) ? READY : OFF);
  }
}

/* 
   Sets the relay state without checking the device state.
*/
void VellemanModel::setRelayEnabledRaw(int relay, bool enabled)
{
  if (enabled && getRelayState(relay) == OFF) {

    setRelayState(relay, INITIALIZING);

    // Add 1 to relay because of shift required by setRelay
    bool success = controller_->setRelay(relay+1, enabled ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    setRelayState(relay, success ? READY : OFF);
  }
  else if (!enabled && getRelayState(relay) == READY) {

    setRelayState(relay, CLOSING);
    // Add 1 to relay because of shift required by setRelay
    bool success = controller_->setRelay(relay+1, enabled ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    setRelayState(relay, success ? OFF : READY);
  }
}

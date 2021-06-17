/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//                 Modeled on cmstkmodlab/common/ConradModel.cc                //
//                         Last Updated August 28, 2019                        //
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

VellemanModel::VellemanModel(const std::string& port, QObject* parent)
  : VRelayCardModel(port, parent)
  , AbstractDeviceModel()
  , channelStates_(8, OFF)
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
void VellemanModel::enableChannel(int channel)
{
  setChannelEnabled(channel, true);
}

void VellemanModel::disableChannel(int channel)
{
  setChannelEnabled(channel, false);
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

    setAllChannelsSame(status);
    setDeviceState( READY );

  }
#else
  if(port_.isEmpty())
    {
      setAllChannelsOff();

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

	  // Check communication. If it is not at the end, of the whole method, card was found.
	  if(controller_initialized)
	    {
	      // Read and initialize status of relays
	      const std::vector<bool> status = controller_->queryRelayStatus();

	      // Announce new states of relays
	      if(status.size() == 8)
		{
		  setAllChannelsSame(status);
		  setDeviceState( READY );

		  NQLog("VellemanModel", NQLog::Message) << "initialize"
		    << ": successfully connected VellemanModel to port " << controller_->comPort();
		}
	      else
		{
		  // Size of status would be 0 if query failed (see VellemanController::queryRelayStatus).
		  // This status size indicates a device malfunction, so set state accordingly.
 		  setAllChannelsOff();

		  NQLog("VellemanModel", NQLog::Fatal) << "initialize"
		    << ": ** ERROR: received malformed status vector from device (port: " << controller_->comPort() << ")";
		}
	    }
	  else
	    {
	      // If controller not successfully initialized (i.e. DEVICE NOT FOUND)
	      setAllChannelsOff();

	      // Log why this initialization failed
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "Cannot connect to Velleman. List of potential ports";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "came up empty (i.e. device was not found).";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "Make sure that port path is correctly";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "entered under the VellemanDevices keyword in";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "/assembly/assembly.cfg and that there are no other";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "blocks to making port list in /common/VellemanModel.";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "Also ensure that " << port_ << " is present and";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "readable and no other process is connecting";
	      NQLog("VellemanModel::initialize()", NQLog::Fatal) << "to the device.";
	    }
	}
      else
	{
	  setAllChannelsOff();

	  NQLog("VellemanModel", NQLog::Fatal) << "initialize"
	    << "Failed to initialize VellemanModel due to empty list of device files to be checked (specified port(s): " << port_ << ")";
	}
    }
#endif
} // end of VellemanModel::initialize()

/*
  Sets all of the  states to OFF
*/
void VellemanModel::setAllChannelsOff()
{
  setDeviceState( OFF );
  setAllChannelsSame(std::vector<bool>(8, false));
}

/*
  Shuts down all connected devices and closes the connection to
  relay card.
*/
void VellemanModel::close()
{

  if (getDeviceState() == READY)
    {
      // Notify that no enable commands should be sent.
      setDeviceState( CLOSING );

      // Disable all connected devices
      for (int i = 0; i < 8; ++i)
	setChannelEnabledRaw(i+1, false);

      // Disable relay card
      destroyController();

      setDeviceState( OFF );
    }
  else if ((getDeviceState() == CLOSING) || (getDeviceState() == OFF))
    {
      NQLog("VellemanModel::close()", NQLog::Message) << "Device is already closed or is process of";
      NQLog("VellemanModel::close()", NQLog::Message) << "shutting down. No further action required.";
    }
  // TODO Controlled shutdown for busy (initializing) device.
  else if (getDeviceState() == INITIALIZING)
    {
      NQLog("VellemanModel::close()", NQLog::Message) << "Device was in the process of turning on,";
      NQLog("VellemanModel::close()", NQLog::Message) << "but is now shutting down.";

      // NOTE: How can this be a controlled shut down?
    }
}


/*
  Sets the current state of the relay card and emits a
  deviceStateChanged signal upon this change.
*/
void VellemanModel::setDeviceState(State state)
{
  if (state_ != state) {
    state_ = state;
    emit deviceStateChanged(state_);
  }
}

/*
  Enables or disables a certain device channel on the relay
  card.
*/
void VellemanModel::setChannelEnabled(int channel, bool enabled)
{
  // TODO Upon failure, check if the relay is still reachable

  // Check if the relay communication is ready
  if (getDeviceState() == READY)
    setChannelEnabledRaw(channel, enabled);
  else
    setChannelState(channel, OFF);

  // TODO Notify of malfunction
}

/* 
   Returns whether the requested device is currently enabled.
*/
const State& VellemanModel::getChannelState(int channel) const
{
  return channelStates_.at(channel-1); // NOTE: channels here are NO LONGER offset by 1 (0-7)
}

/*
  Sets the current state for a device and emits a signal if 
  necessary.
*/
void VellemanModel::setChannelState(int channel, State state)
{
  if (channelStates_.at(channel-1) != state) { // NOTE: channels here are NO LONGER offset by 1 (0-7)
    channelStates_[channel-1] = state; // NOTE: channels here are NO LONGER offset by 1 (0-7)
    emit channelStateChanged(channel, state);
  }
}

/*
  Small convenience method to set all device channel states using
  the vector provided by VellemanController::queryRelayStatus().
*/
void VellemanModel::setAllChannelsSame(const std::vector<bool>& status)
{
  for (unsigned int i = 0; i < status.size(); ++i) {
    // Set device state according to relay status received
    setChannelState(i+1, status.at(i) ? READY : OFF);
  }
}

/* 
   Sets the channel state without checking the device state.
*/
void VellemanModel::setChannelEnabledRaw(int channel, bool enabled)
{
  if (enabled && getChannelState(channel) == OFF) {

    setChannelState(channel, INITIALIZING);

    // NO LONGER NEED TO Add 1 to channel because of shift required by setRelay
    bool success = controller_->setRelay(channel, enabled ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    setChannelState(channel, success ? READY : OFF);
  }
  else if (!enabled && getChannelState(channel) == READY) {

    setChannelState(channel, CLOSING);
    // NO LONGER NEED TO Add 1 to channel because of shift required by setRelay
    bool success = controller_->setRelay(channel, enabled ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    setChannelState(channel, success ? OFF : READY);
  }
}

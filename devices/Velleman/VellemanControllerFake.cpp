/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//         Modeled on cmstkmodlab/devices/Conrad/ConradControllerFake.cpp      //
//                           Last Updated August 20, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "VellemanControllerFake.h"
#include "VellemanCommunication.h"

//! Default constructor
VellemanControllerFake::VellemanControllerFake(const std::string& ioPort)

  : VVellemanController(ioPort)

{
  for (int c=0; c<8; ++c) status_[c] = false;
}

//! Default destructor
VellemanControllerFake::~VellemanControllerFake()
{
}

//! Initialize Velleman IO communication
bool VellemanControllerFake::initialize()
{
  return true;
}

//! Query relay status (returns 8 items reflecting the relay state of on/off, or 0 items if failed)
std::vector<bool> VellemanControllerFake::queryRelayStatus() const
{
  std::vector<bool> result(status_, status_ + 8);
  return result;
}

//! Set a specific relay on or off (does not affect other relays)
bool VellemanControllerFake::setRelay(unsigned relay, unsigned char relayCMD) const
{
  if (relay > 8) {
    fprintf(stderr, "[Velleman] ERROR (port FAKE): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", relay, errno);
    return false;
  }

  if (relayCMD ^ CMD_SWITCH_RELAY_ON == 0x00) { status_[relay-1] = true; }
  else if (relayCMD ^ CMD_SWITCH_RELAY_OFF == 0x00) { status_[relay-1] = false; }
  else {
    fprintf(stderr, "[Velleman] ERROR (port FAKE): '%u' is not a 'Switch Relay On/Off' command and should not be used for this method, ERRNO: %i\n", relayCMD, errno);
    return false;
  }

  return true;
}

//! Set a specific relay on or off (all other relays turned off)
bool VellemanControllerFake::setSingleRelay(unsigned relay, unsigned char relayCMD) const
{
  if (relay > 8) {
    fprintf(stderr, "[Velleman] ERROR (port FAKE): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", relay, errno);
    return false;
  }

  for (int c=0; c<8; ++c) status_[c] = false;

  if (relayCMD ^ CMD_SWITCH_RELAY_ON == 0x00) { status_[relay-1] = true; }
  else if (relayCMD ^ CMD_SWITCH_RELAY_OFF == 0x00) { status_[relay-1] = false; }
  else {
    fprintf(stderr, "[Velleman] ERROR (port FAKE): '%u' is not a 'Switch Relay On/Off' command and should not be used for this method, ERRNO: %i\n", relayCMD, errno);
    return false;
  }

  return true;

}

//! Set multiple relays (those listed in input are turned on/off based on command; others are unaffected)
bool VellemanControllerFake::setMultiRelays(std::vector<unsigned> relays, unsigned char relayCMD) const
{
  for (int i=0; i<relays.size(); ++i) {
    if (relays[i] > 8) {
      fprintf(stderr, "[Velleman] ERROR (port FAKE): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", relays[i], errno);
      return false;
    }
  }

  if (relayCMD ^ CMD_SWITCH_RELAY_ON == 0x00) {
    for (int t=0; t<relays.size(); ++t) status_[t] = true;
  }
  else if (relayCMD ^ CMD_SWITCH_RELAY_OFF == 0x00) {
    for (int t=0; t<relays.size(); ++t) status_[t] = false;
  }
  
  return true;
}

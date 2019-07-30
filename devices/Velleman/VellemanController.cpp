/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/ConradController.cpp       //
//                          Last Updated August 13, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "VellemanCommunication.h"
#include "VellemanController.h"

#include <iostream>

//! Default constructor
VellemanController::VellemanController(const std::string& ioPort)
  : IVellemanController(ioPort)
  , m_communication(new VellemanCommunication(ioPort))
{
}

//! Default destructor
VellemanController::~VellemanController()
{
  m_communication->finish();

  delete m_communication;
}

//! Initialize Velleman IO communication
bool VellemanController::initialize()
{
  if (!m_communication) {
    fprintf(stderr, "[Velleman] ERROR: Cannot construct class instance for communication with Velleman relay card, ERRNO: %i\n", errno);
    return false;
  }

  // Initialize communication connection
  if (!m_communication->initialize()) {
    fprintf(stderr, "[Velleman] ERROR: Cannot initialize communication with Velleman relay card, ERRNO: %i", errno);
    return false;
  }
 
  // Initialize Velleman relay card by resetting factory defaults and querying relay status
  if (!m_communication->sendCommand(CMD_RESET_FACTORY_DEFAULTS, 0x00, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to send command to reset card's factory default settings, ERRNO %i\n", m_communication->comPort().c_str(), errno);
    return false;
  }
  
  if (!m_communication->sendCommand(CMD_QUERY_RELAY_STATUS, 0x00, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to send command to query relay status of card, ERRNO %i\n", m_communication->comPort().c_str(), errno);
    return false;
  }

  // Check whether initialization was successful
  unsigned char cmd, mask, param1, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to receive answer from card, ERRNO %i\n", m_communication->comPort().c_str(), errno);
    return false;
  }
  
  if (cmd != CMD_RELAY_STATUS || param1 != 0x00) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Relay card status reset and/or check failed, ERRNO: %i\n", m_communication->comPort().c_str(), errno);
    return false;
  }

  return true;
}

//! Return name of port used to initialize VellemanCommunication
std::string VellemanController::comPort() const
{
  if (!m_communication) {
    fprintf(stderr, "[Velleman] ERROR: Cannot construct class instance for communication with Velleman relay card, ERRNO: %i\n", errno);
    return "[Velleman] PORT NOT FOUND";
  }


  return m_communication->comPort();
}

//! Internal helper function
static inline bool isBitSet(unsigned char byte, unsigned bit)
{
  return (data & (1 << bit) != 0x00); 
}

//! Internal helper function
bool VellemanController::queryRawStatus(unsigned char& status) const
{
  // Request channel status
  if (!m_communication->sendCommand(CMD_QUERY_RELAY_STATUS, 0x00, 0x00,0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to send command to query relay status of card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }
    
  // Read result
  unsigned char cmd, mask, param1, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to receive answer from card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }
    
  if (cmd != CMD_RELAY_STATUS) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Response from card is inconclusive, ERRNO: %i\n", comPort().c_str(), errno);
    return false;
  }

  return true;
}

//! Query channel status (returns 8 items reflecting the channel state, on/off, 0 items in the case of failure
std::vector<bool> VellemanController::queryRelayStatus() const
{
  std::vector<bool> result;

  // Return nothing if query of statuses fails
  unsigned char status;
  if (!queryRawStatus(status)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Cannot obtain raw status of relays for Velleman relay card, ERRNO: %i\n", comPort().c_str(), errno)
    return result;
  }
    
  result.resize(8);
  for (unsigned i = 0; i < 8, ++i)
    result[i] = isBitSet(status, i);

  return result;
}

//! Set a specific relay on or off (does not change any other relay status)
bool VellemanController::setRelay(unsigned relay, unsigned relayCMD) const
{
  if (relay > 8) {
    fprintf(stderr, "[Velleman] ERROR (port %s): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  unsigned char relayByte = (unsigned char) pow(2.0, relay - 1.0);

  if (!m_communication->sendCommand(relayCMD, relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to set relay %u, ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }
  
  // Check success (or not) of switching
  unsigned char cmd, mask, param1, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to receive answer from card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }

  if (!isBitSet(mask, relay) ^ !isBitSet(param1, relay)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Changing status of relay '%u' failed, ERRNO %i/n", comPort().c_str(), relay, errno);
    return false;
  }

  return true;
}

//! Set a specific relay on or off and switch _all_ other relays to off
bool VellemanController::setSingleRelay(unsigned relay, unsigned relayCMD) const
{
  if (relay > 8) {
    fprintf(stderr, "[Velleman] ERROR (port %s): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", comPort().c_str(), relay, errno);
    return false;
  }
  
  unsigned char relayByte = (unsigned char) pow(2.0, relay - 1.0);

  if (!m_communication->sendCommand(relayCMD, relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to set relay %u, ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  if (!m_communication->sendCommand(CMD_SWITCH_RELAY_OFF, ~relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to turn off all relays except relay '%u', ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  //Check whether switching was successful
  unsigned char cmd, mask, param1, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to receive answer from card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }
 
  if (param1 != relayByte) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Changing status of relay '%u' and turning all other relays off failed, ERRNO: %i \n", comPort(), relay, errno);
    return false;
  }

  return true;
}

//! Set relays listed in input ON and all other relays OFF
bool VellemanController::setMultiRelays(std::vector<unsigned> relays) const
{
  unsigned char relayByte = 0x00;
  std::string relayString = "";
  std::vector<unsigned>::iterator it = relays.begin();
  std::vector<unsigned>::iterator end = relays.end()
    
  for (; it != end; ++it)
    {
     if (*it > 8) {
       fprintf(stderr, "[Velleman] ERROR (port %s): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", comPort().c_str(), *it, errno);
       return false;
     }
     relayByte += (unsigned char) pow(2.0, *it - 1.0);
     relayString += (std::to_string(*it) + ", ")
    }

  if (!m_communication->sendCommand(CMD_SWITCH_RELAY_ON, relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to set relays '%s' on, ERRNO %i\n", comPort().c_str(), relayString.c_str(), errno);
    return false;
  }

  if (!m_communication->sendCommand(CMD_SWITCH_RELAY_OFF, ~relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to set relays other than relays '%s' off, ERRNO %i\n", comPort().c_str(), relayString.c_str(), errno);
    return false;
  }
   
  //Check whether switching was successful
  unsigned char cmd, mask, param1, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to execute command to receive answer from card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }

  for (; it != end; ++it)
    {
      if (!isBitSet(param1, *it)) {
	fprintf(stderr, "[Velleman] ERROR (port %s): Changing status of relay '%i' failed, ERRNO: %i \n", comPort().c_str(), *it, errno);
	return false;
      }
    }

  return true;
}

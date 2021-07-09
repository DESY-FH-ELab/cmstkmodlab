/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/ConradController.cpp       //
//                          Last Updated August 30, 2019                       //
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
    fprintf(stderr, "[Velleman] ERROR: Cannot initialize communication with Velleman relay card, ERRNO: %i\n", errno);
    return false;
  }
 
  ///*** NOTE: Section is commented because status should not be reset during every connection ***///
  ///***       to card. Section can be removed after demonstration of full compatibility with  ***///
  ///***       DESY assembly code (or earlier if deemed safe).                                 ***///
  // Initialize Velleman relay card by resetting factory defaults and querying relay status
  //if (!m_communication->sendCommand(CMD_RESET_FACTORY_DEFAULTS, 0x00, 0x00, 0x00)) {
  //  fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to send command to reset card's factory default settings, ERRNO %i\n", m_communication->comPort().c_str(), errno);
  //   return false;
  // } 
  
  if (!m_communication->sendCommand(CMD_QUERY_RELAY_STATUS, 0x00, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to send command to query relay status of card during card initialization, ERRNO %i\n", m_communication->comPort().c_str(), errno);
    return false;
  }

  // Check whether initialization was successful
  unsigned char cmd, mask, param1, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to receive answer from card during card initialization, ERRNO %i\n", m_communication->comPort().c_str(), errno);
    return false;
  }

  // NOTE: This check may not be entirely necessary
  if (cmd != CMD_RELAY_STATUS) { 
    fprintf(stderr, "[Velleman] ERROR (port: %s): Relay card status check is inconclusive, ERRNO: %i\n", m_communication->comPort().c_str(), errno);
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
  return ((byte & (1 << bit)) != 0x00); 
}

//! Internal helper function
bool VellemanController::queryRawStatus(unsigned char& param1) const
{
  // Request channel status
  if (!m_communication->sendCommand(CMD_QUERY_RELAY_STATUS, 0x00, 0x00,0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to send command to query relay status of card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }
    
  // Read result
  unsigned char cmd, mask, param2;
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to receive answer from card, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }

  // NOTE: This check may not be entirely necessary  
  if (cmd != CMD_RELAY_STATUS) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Status check response from card is inconclusive, ERRNO: %i\n", comPort().c_str(), errno);
    return false;
  }

  return true;
}

//! Query channel status (returns 8 items reflecting the channel state, on/off, 0 items in the case of failure)
std::vector<bool> VellemanController::queryRelayStatus() const
{
  std::vector<bool> result;

  // Return nothing if query of statuses fails
  unsigned char status;
  if (!queryRawStatus(status)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Cannot obtain raw status of relays for Velleman relay card, ERRNO: %i\n", comPort().c_str(), errno);
    return result;
  }
    
  result.resize(8);
  for (unsigned i = 0; i < 8; ++i)
    result[i] = isBitSet(status, i);

  return result;
}

//! Function to read status check from card even if no relays change status after a command is sent
///*** NOTE: No WARNINGS sent as long as at least one relay changes state. ***///
bool VellemanController::readStatus(unsigned char& cmd, unsigned char& mask, unsigned char& param1, unsigned char& param2, std::string relays) const
{
  if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
    if (!m_communication->sendCommand(CMD_QUERY_RELAY_STATUS, 0x00, 0x00, 0x00)) {
      fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to check relay card status, ERRNO %i\n", comPort().c_str(), errno);
      return false;
    }
    if (!m_communication->receiveAnswer(&cmd, &mask, &param1, &param2)) {
      fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to receive relay status from card, ERRNO %i\n", comPort().c_str(), errno);
      return false;
    }
    if ((mask ^ param1) != 0x00) {
      fprintf(stderr, "[Velleman] WARNING (port: %s): Changes in relay status after setting relay(s) %s were not properly received\n", comPort().c_str(), relays.c_str());
      return false;
    }
    else {
      fprintf(stderr, "[Velleman] WARNING (port: %s): No changes in relay status after setting relay(s) %s \n", comPort().c_str(), relays.c_str());
      return true;
    }
  }
  return true;
}

//! Set a specific relay on or off (does not change any other relay status)
bool VellemanController::setRelay(unsigned relay, unsigned char relayCMD) const
{
  if (relay > 8) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  unsigned char relayByte = (unsigned char) pow(2.0, relay - 1.0);

  if (!m_communication->sendCommand(relayCMD, relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to set relay |%u|, ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }
  
  // Check success (or not) of switching
  unsigned char cmd, mask, param1, param2;
  if (!readStatus(cmd, mask, param1, param2, ("|" + std::to_string(relay)) + "|")) return false;
  
  if (!isBitSet(mask, relay) ^ !isBitSet(param1, relay)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Changing status of relay |%u| failed, ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  return true;
}

//! Set a specific relay on or off and switch _all_ other relays to off
bool VellemanController::setSingleRelay(unsigned relay, unsigned char relayCMD) const
{
  if (relay > 8) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", comPort().c_str(), relay, errno);
    return false;
  }
  
  unsigned char relayByte = (unsigned char) pow(2.0, relay - 1.0);

  if (!m_communication->sendCommand(CMD_SWITCH_RELAY_OFF, ~relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to turn off all relays except relay |%u|, ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  // Receive output of first (interim) command
  unsigned char c, m, p1, p2;
  std::string offRelays = ("other than relay |" + std::to_string(relay) + "| ");
  if (!readStatus(c, m, p1, p2, offRelays)) return false;
    
  if (!m_communication->sendCommand(relayCMD, relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to set relay |%u|, ERRNO %i\n", comPort().c_str(), relay, errno);
    return false;
  }

  //Check whether switching was successful
  unsigned char cmd, mask, param1, param2;
  if (!readStatus(cmd, mask, param1, param2, ("|" + std::to_string(relay)) + "|")) return false;
 
  if ((unsigned) param1 != (unsigned) relayByte) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Changing status of relay |%u| and turning all other relays off failed with '%x' and '%x' as the current status and desired status in hexadecimal representation, ERRNO: %i \n", comPort().c_str(), relay, (unsigned) param1, (unsigned) relayByte, errno);
    return false;
  }

  return true;
}

//! Set relays listed in input based on relayCMD
bool VellemanController::setMultiRelays(std::vector<unsigned> relays, unsigned char relayCMD) const
{
  unsigned char relayByte = 0x00;
  std::string relayString = "|";
  std::vector<unsigned>::iterator it = relays.begin();
  std::vector<unsigned>::iterator end = relays.end();
    
  for (; it != end; ++it)
    {
     if (*it > 8) {
       fprintf(stderr, "[Velleman] ERROR (port: %s): '%u' is out of range of relays 1-8 available for Velleman relay card, ERRNO: %i\n", comPort().c_str(), *it, errno);
       return false;
     }
     relayByte += (unsigned char) pow(2.0, *it - 1.0);
     relayString += (std::to_string(*it) + "|");
    }

  if (!m_communication->sendCommand(relayCMD, relayByte, 0x00, 0x00)) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to set relays %s, ERRNO %i\n", comPort().c_str(), relayString.c_str(), errno);
    return false;
  }

  // Receive output of command
  unsigned char cmd, mask, param1, param2;
  if (!readStatus(cmd, mask, param1, param2, relayString)) return false;
  
  ///*** NOTE: This section may be uncommented to restore capability to turn off all ***///
  ///***       relays not set/designated in the "relays" input. If uncommented,      ***///
  ///***       capabilities should also be added in for loop below to check that all ***///
  ///***       relays expected to be turned off are, in fact, off. This section may  ***///
  ///***       be removed if deemed unnecessary after full integration of Velleman   ***///
  ///***       driver into DESY assembly code.                                       ***///
  // // Send command to turn off all relays other than those in input
  // if (!m_communication->sendCommand(CMD_SWITCH_RELAY_OFF, ~relayByte, 0x00, 0x00)) {
  //   fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to execute command to set relays other than relays '%s' off, ERRNO %i\n", comPort().c_str(), relayString.c_str(), errno);
  //   return false;
  // }
  //  
  // // Receive output of second command
  // unsigned char c, m, p1, p2;
  // std::string offRelays = ("other than relays " + relayString); 
  // if (!readStatus(c, m, p1, p2, offRelays)) return false;
  
  // Check that relays in input have been set to the expect value (on/off)
  for (; it != end; ++it)
    {
      if (relayCMD ^ CMD_SWITCH_RELAY_ON == 0x00) {
	if (!isBitSet(param1, *it)) {
	  fprintf(stderr, "[Velleman] ERROR (port: %s): Turning on relay '%i' failed, ERRNO: %i \n", comPort().c_str(), *it, errno);
	  return false;
	}
      }
      if (relayCMD ^ CMD_SWITCH_RELAY_OFF == 0x00) {
	if (isBitSet(param1, *it)) {
	  fprintf(stderr, "[Velleman] ERROR (port: %s): Turning off relay '%i' failed, ERRNO: %i \n", comPort().c_str(), *it, errno);
	  return false;
	}
      }
    }

  return true;
}

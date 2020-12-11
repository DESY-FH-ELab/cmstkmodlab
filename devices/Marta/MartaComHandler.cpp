/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <iostream>

#include "MartaComHandler.h"

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
MartaComHandler::MartaComHandler( ipaddress_t IPAddress, port_t Port)
  : fDeviceAvailable(false),
    fMB(0)
{
  // save IPAddress and port 
  fIPAddress = IPAddress;
  fPort = Port;
  
  Connect();
  Disconnect();
}

MartaComHandler::~MartaComHandler( void )
{
  Disconnect();
}

void MartaComHandler::ReadRegisters(int addr, int nb, uint16_t *dest)
{
  Connect();

  if (fMB) {
    modbus_read_registers(fMB, addr, nb, dest);
  }
}

void MartaComHandler::Connect()
{
  fMB = modbus_new_tcp(fIPAddress.c_str(), fPort);

  if (modbus_connect(fMB) == -1) {
    printf("Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(fMB);
    fMB = 0;
    fDeviceAvailable = false;
  } else {
    modbus_set_slave(fMB, 1);
    fDeviceAvailable = true;
  }
}

void MartaComHandler::Disconnect()
{
  if (fMB) {
    modbus_close(fMB);
    modbus_free(fMB);
  }
  fMB = 0;
}

bool MartaComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

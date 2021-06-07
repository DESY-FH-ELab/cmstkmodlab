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
  : fDeviceAvailable(false)
{
#if !defined(USE_FAKEIO) && !defined(NOMARTA)
  fMB = 0;
#endif
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

#if !defined(USE_FAKEIO) && !defined(NOMARTA)
  if (fMB) {
    modbus_read_registers(fMB, addr, nb, dest);
  }
#endif
}

void MartaComHandler::WriteRegisters(int addr, int nb, const uint16_t *src)
{
  Connect();

#if !defined(USE_FAKEIO) && !defined(NOMARTA)
  if (fMB) {
    modbus_write_registers(fMB, addr, nb, src);
  }
#endif
}

void MartaComHandler::Connect()
{
#if !defined(USE_FAKEIO) && !defined(NOMARTA)
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
#endif
}

void MartaComHandler::Disconnect()
{
#if !defined(USE_FAKEIO) && !defined(NOMARTA)
  if (fMB) {
    modbus_close(fMB);
    modbus_free(fMB);
  }
  fMB = 0;
#endif
}

bool MartaComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

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

#ifndef _MARTACOMHANDLER_H_
#define _MARTACOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>

#if !defined(USE_FAKEIO) && !defined(NOMARTA)
#include <modbus.h>
#endif

/** @addtogroup devices
 *  @{
 */

/** @defgroup Marta Marta
 * Classes to handle low level communication with the Marta CO2 chiller
 *  @{
 */

typedef const char* ipaddress_t;
typedef unsigned int port_t;

class MartaComHandler {

 public:
  
  //! Constructor.
  MartaComHandler( ipaddress_t, port_t );

  //! Destructor.
  ~MartaComHandler();

  //! Default bitwise copy constructor.
  MartaComHandler( const MartaComHandler& );

  bool DeviceAvailable();

  void ReadRegisters(int addr, int nb, uint16_t *dest);
  void WriteRegisters(int addr, int nb, const uint16_t *src);
 
 private:

  void Connect();
  void Disconnect();
  
  bool fDeviceAvailable;

  std::string fIPAddress;
  port_t fPort;

#if !defined(USE_FAKEIO) && !defined(NOMARTA)
  modbus_t *fMB;
#endif
};

/** @} */

/** @} */

#endif // _MARTACOMHANDLER_H_

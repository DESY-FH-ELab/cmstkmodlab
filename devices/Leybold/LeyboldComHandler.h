/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _LEYBOLDCOMHANDLER_H_
#define _LEYBOLDCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

/** @addtogroup devices
 *  @{
 */

/** @defgroup Leybold Leybold
 * Classes to handle low level communication with Leybold devices
 *  @{
 */

typedef const char* ioport_t;
typedef struct termios termios_t;

class LeyboldComHandler {

 public:
  
  //! Constructor.
  LeyboldComHandler( ioport_t );

  //! Destructor.
  ~LeyboldComHandler();

  //! Default bitwise copy constructor.
  LeyboldComHandler( const LeyboldComHandler& );

  void SendCommand( const char* );
  void ReceiveString( char* );

  bool DeviceAvailable();

 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );

  bool fDeviceAvailable;
  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};
 
/** @} */

/** @} */

#endif // _LEYBOLDCOMHANDLER_H_




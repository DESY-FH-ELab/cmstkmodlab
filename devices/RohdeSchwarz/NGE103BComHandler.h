/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _NGE103BCOMHANDLER_H_
#define _NGE103BCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/** @addtogroup devices
 *  @{
 */

/** @defgroup RohdeSchwarz Rohde & Schwarz
 * Classes to handle low level communication with Rohde & Schwarz devices
 *  @{
 */

typedef const char* ioport_t;
typedef struct termios termios_t;

class NGE103BComHandler {

 public:
  
  //! Constructor.
  NGE103BComHandler( ioport_t );

  //! Destructor.
  ~NGE103BComHandler();

  //! Default bitwise copy constructor.
  NGE103BComHandler( const NGE103BComHandler& );

  void SendCommand( const char* );
  void ReceiveString( char* );

  bool DeviceAvailable();

 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  bool fDeviceAvailable;
  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};
 
/** @} */

/** @} */

#endif // _NGE103BCOMHANDLER_H_

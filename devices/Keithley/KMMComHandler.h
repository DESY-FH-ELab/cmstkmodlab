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

#ifndef _KMMCOMHANDLER_H_
#define _KMMCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define COM1 "/dev/ttyS0"
#define COM2 "/dev/ttyS1"
#define COM3 "/dev/ttyS2"
#define COM4 "/dev/ttyS3"

#define ttyS0 "/dev/ttyS0"
#define ttyS1 "/dev/ttyS1"
#define ttyS2 "/dev/ttyS2"
#define ttyS3 "/dev/ttyS3"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Keithley
 *  @{
 */

typedef const char* ioport_t;
typedef struct termios termios_t;

class KMMComHandler
{
 public:
  
  //! Constructor.
  KMMComHandler( ioport_t ioPort );

  //! Destructor.
  ~KMMComHandler();

  //! Default bitwise copy constructor.
  KMMComHandler( const KMMComHandler &dmmComHandler );

  void SendCommand( const char *commandString );
  void ReceiveString( char *receiveString );

 private:

  void OpenIoPort( void ) noexcept(false);
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

/** @} */

/** @} */

#endif

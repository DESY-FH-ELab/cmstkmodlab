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

#ifndef _GMH3750COMHANDLER_H_
#define _GMH3750COMHANDLER_H_

#include <termios.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>

#define COM1 "/dev/ttyS0"
#define COM2 "/dev/ttyS1"
#define COM3 "/dev/ttyS2"
#define COM4 "/dev/ttyS3"

#define ttyS0 "/dev/ttyS0"
#define ttyS1 "/dev/ttyS1"
#define ttyS2 "/dev/ttyS2"
#define ttyS3 "/dev/ttyS3"

#define _COMHANDLER_DELAY 1000

typedef const char* ioport_t;
typedef struct termios termios_t;

using namespace std;

class GMH3750ComHandler
{
 public:
  
  //! Constructor.
  GMH3750ComHandler( ioport_t );

  //! Destructor.
  ~GMH3750ComHandler();

  //! Default bitwise copy constructor.
  GMH3750ComHandler( const GMH3750ComHandler& );

  void SendCommand( const char*, int length = -1);
  void ReceiveString( char* );


 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;

};

#endif

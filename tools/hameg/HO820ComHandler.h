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

#ifndef _HO820COMHANDLER_H_
#define _HO820COMHANDLER_H_

#include <termios.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

#define ttyUSB0 "/dev/ttyUSB0"

typedef const char* ioport_t;
typedef struct termios termios_t;

using namespace std;

class HO820ComHandler {

 public:
  
  //! Constructor.
  HO820ComHandler( ioport_t );

  //! Destructor.
  ~HO820ComHandler();

  //! Default bitwise copy constructor.
  HO820ComHandler( const HO820ComHandler& );

  void SendCommand( const char* );
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




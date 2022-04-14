/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _KEITHLEYUSBTMCCOMHANDLER_H_
#define _KEITHLEYUSBTMCCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>

typedef const char* ioport_t;
typedef struct termios termios_t;

class KeithleyUSBTMCComHandler {

  friend class KeithleyDAQ6510;

 public:
  
  //! Constructor.
  KeithleyUSBTMCComHandler( ioport_t );

  //! Destructor.
  ~KeithleyUSBTMCComHandler();

  //! Default bitwise copy constructor.
  KeithleyUSBTMCComHandler( const KeithleyUSBTMCComHandler& );

  void SendCommand( const char* );
  void SendCommand( const std::string& );
  void ReceiveString( char* );

  bool DeviceAvailable();

 protected:
  
  void CloseIoPort( void );

 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );

  bool fDeviceAvailable;
  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

#endif // _KEITHLEYUSBTMCCOMHANDLER_H_

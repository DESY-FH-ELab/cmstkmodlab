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

#ifndef _KEYENCECOMHANDLER_H_
#define _KEYENCECOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef const char* ioport_t;
typedef struct termios termios_t;

/*
Class for establishing communication with the Keyence LK-G3000 laser 
*/

class KeyenceComHandler {

 public:
  
  //! Constructor.
  KeyenceComHandler( ioport_t );

  //! Destructor.
  ~KeyenceComHandler();

  //! Default bitwise copy constructor.
  KeyenceComHandler( const KeyenceComHandler & );

  void SendCommand( const char* );
  //  std::string ReceiveString(int samplingRate, int averagingRate );
  void ReceiveString(std::string & receiveString, char *temp_output, int samplingRate, int averagingRate );

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

#endif




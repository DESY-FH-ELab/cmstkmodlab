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

#ifndef USE_FAKEIO
#include <linux/usb/tmc.h>
#include <sys/ioctl.h>
#endif

#include <string.h>

#include <iostream>

#ifndef __DEBUG
// #define __DEBUG 1
#endif

#include "KeithleyUSBTMCComHandler.h"

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
KeithleyUSBTMCComHandler::KeithleyUSBTMCComHandler(ioport_t ioPort)
{
#ifdef __DEBUG
  std::cout << "KeithleyUSBTMCComHandler::KeithleyUSBTMCComHandler(ioport_t ioPort)" << std::endl;
#endif
  
  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

KeithleyUSBTMCComHandler::~KeithleyUSBTMCComHandler( void )
{
#ifdef __DEBUG
  std::cout << "KeithleyUSBTMCComHandler::~KeithleyUSBTMCComHandler( void )" << std::endl;
#endif
}

//! Send the command string &lt;commandString&gt; to device.
void KeithleyUSBTMCComHandler::SendCommand( const char *commandString )
{
  if (!fDeviceAvailable) return;

#ifdef __DEBUG
  std::cout << "void KeithleyUSBTMCComHandler::SendCommand( const char *commandString ) "
	    << commandString << std::endl;
#endif

  std::string theCommand = commandString;
  theCommand += "\n";
  
  write( fIoPortFileDescriptor, theCommand.c_str(), strlen(theCommand.c_str()));
  
  usleep(10);
}

void KeithleyUSBTMCComHandler::SendCommand( const std::string& commandString )
{
  SendCommand(commandString.c_str());
}

//! Read a string from device.
/*!
\par Input:
  <br><b>Receive string must be at least 41 characters long.</b>

  This function must be placed right in time before
  the device starts sending.

  See example program in class description.
*/
void KeithleyUSBTMCComHandler::ReceiveString( char *receiveString )
{
  if (!fDeviceAvailable) {
    receiveString[0] = 0;
    return;
  }
 
  int timeout = 0, readResult = 0;

  /*
  while (timeout < 100000)  {

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    // std::cout << "receiveString: " << readResult << " " << receiveString << std::endl;
    
    if ( readResult > 0 ) {
      receiveString[readResult] = 0;
      break;
    }
    
    timeout++;
  }
  */
  
  readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

#ifdef __DEBUG
  std::cout << "receiveString: " << readResult << " " << receiveString << std::endl;
#endif

  usleep(10);
}

//! Open I/O port.
/*!
  \internal
*/
void KeithleyUSBTMCComHandler::OpenIoPort( void )
{
#ifdef __DEBUG
  std::cout << "void KeithleyUSBTMCComHandler::OpenIoPort( void )" << std::endl;
#endif
  
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR);

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[NGE103BComHandler::OpenIoPort] ** ERROR: could not open device file "
              << fIoPort << "." << std::endl;
    std::cerr << "                               (probably it's not user-writable)."
              << std::endl;
    fDeviceAvailable = false;
    return;
  } else {
    // configure port with no delay
    // fcntl(fIoPortFileDescriptor, F_SETFL, FNDELAY);

#ifndef USE_FAKEIO
    int rv = ioctl(fIoPortFileDescriptor, USBTMC_IOCTL_CLEAR);
    if(rv==-1) {
      printf("Error: ioctl returned %d.\n",rv);
    }
#endif
  }

  fDeviceAvailable = true;
}

//! Initialize I/O port.
/*!
  \internal
*/
void KeithleyUSBTMCComHandler::InitializeIoPort( void )
{
  if (!fDeviceAvailable) return;

  // get and save current ioport settings for later restoring
  // tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void KeithleyUSBTMCComHandler::RestoreIoPort( void )
{
  if (!fDeviceAvailable) return;

  // restore old com port settings
  // tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void KeithleyUSBTMCComHandler::CloseIoPort( void )
{
  if (!fDeviceAvailable) return;

#ifndef USE_FAKEIO
  std::cout << "void KeithleyUSBTMCComHandler::CloseIoPort( void )" << std::endl;
#endif

  /*
#ifndef USE_FAKEIO
  int rv = ioctl(fIoPortFileDescriptor, USBTMC_IOCTL_CLEAR);
  if(rv==-1) {
    printf("Error: ioctl returned %d.\n",rv);
  }
#endif
  */
  
  close( fIoPortFileDescriptor );
}

bool KeithleyUSBTMCComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

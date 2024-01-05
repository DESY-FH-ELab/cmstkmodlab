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

#include <chrono>
#include <iostream>
#include <string>
#include <string.h>

#include "LStepExpressComHandler.h"

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
LStepExpressComHandler::LStepExpressComHandler(const std::string& ioPort)
 : fIoPort(ioPort)
{
  // initialize
  OpenIoPort();
  InitializeIoPort();
}

LStepExpressComHandler::~LStepExpressComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void LStepExpressComHandler::SendCommand( const char *commandString )
{
  if (!fDeviceAvailable) return;

  char singleCharacter = 0; 

  for ( unsigned int i = 0; i < strlen( commandString ); i++ ) {

    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );
  }

  // send feed characters
  SendFeedString();
}

//! Read a string from device.
/*!
\par Input:
  <br><b>Receive string must be at least 41 characters long.</b>

  This function must be placed right in time before
  the device starts sending.

  See example program in class description.
*/
std::string LStepExpressComHandler::ReceiveString()
{
  // Nothing ready
  if (!fDeviceAvailable) {
    return {};
  }

  // Define a timeout and start a timer:
  const auto timeout = std::chrono::duration<double, std::milli>(500);
  const auto start_time = std::chrono::steady_clock::now();

  std::string value{};

  char readbyte = 0;

  while(true) {
    // Let's read byte-by-byte from the file descriptor:
    size_t n_read = read(fIoPortFileDescriptor, &readbyte, 1);

    if(n_read != 0) {
      // Read until we see the end-of-command (CR) marker
      if(readbyte == '\r') {
        break;
      }

      // We read one byte, let's add it to the output and continue.
      value += readbyte;
    }

    // Check for communication timeout:
    if(std::chrono::steady_clock::now() - start_time > timeout) {
      std::cout << "[LStepExpressComHandler::ReceiveString] ** ERROR: Communication seems to have timed out" << std::endl;
      return {};
    }
  }

  return value;
}

//! Open I/O port.
/*!
  \internal
*/
void LStepExpressComHandler::OpenIoPort( void )
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort.c_str(), O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if(fIoPortFileDescriptor == -1)
  {
    std::cout << "[LStepExpressComHandler::OpenIoPort] ** ERROR: could not open device file " << fIoPort << "." << std::endl;
    std::cout << "                                               (probably it's not user-writable)." << std::endl;

    fDeviceAvailable = false;

    return;
  }
  else
  {
    // configure port with no delay
    int flags = fcntl(fIoPortFileDescriptor, F_GETFL, 0);
    flags |= O_NONBLOCK;
    flags |= FNDELAY;
    fcntl( fIoPortFileDescriptor, F_SETFL, flags );
  }

  fDeviceAvailable = true;
}

//! Initialize I/O port.
/*!
  \internal
*/
void LStepExpressComHandler::InitializeIoPort( void )
{
  if (!fDeviceAvailable) return;

  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

#ifndef USE_FAKEIO

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // all these settings copied from stty output..

  // baud rate
  cfsetispeed( &fThisTermios, B115200 );  // input speed
  cfsetospeed( &fThisTermios, B115200 );  // output speed

  // various settings, 8N1 (no parity, 1 stopbit)
  fThisTermios.c_cflag   &= ~PARENB;
  fThisTermios.c_cflag   &= ~PARODD;
  fThisTermios.c_cflag   |=  CS8;
  fThisTermios.c_cflag   |=  HUPCL;
  fThisTermios.c_cflag   &= ~CSTOPB;
  fThisTermios.c_cflag   |=  CREAD;
  fThisTermios.c_cflag   |=  CLOCAL;
  fThisTermios.c_cflag   &= ~CRTSCTS;

  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );
  
#endif
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void LStepExpressComHandler::RestoreIoPort( void )
{
  if (!fDeviceAvailable) return;

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void LStepExpressComHandler::CloseIoPort( void )
{
  if (!fDeviceAvailable) return;

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void LStepExpressComHandler::SendFeedString( void )
{
  if (!fDeviceAvailable) return;

  // feed string is <CR> ; see documentation page 4.1
  char feedString = 13;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );
}

bool LStepExpressComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

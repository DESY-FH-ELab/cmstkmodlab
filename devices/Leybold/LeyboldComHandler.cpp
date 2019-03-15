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

#include <string.h>

#include <iostream>

#include "LeyboldComHandler.h"

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
LeyboldComHandler::LeyboldComHandler(ioport_t ioPort)
{
  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

LeyboldComHandler::~LeyboldComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void LeyboldComHandler::SendCommand( const char *commandString )
{
  if (!fDeviceAvailable) return;

  char singleCharacter = 0; 

  for (unsigned int i=0;i<strlen(commandString);i++) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );
  }

  usleep(50000);
  // std::cout << "command: |" << commandString << "|" << std::endl;
}

//! Read a string from device.
/*!
\par Input:
  <br><b>Receive string must be at least 41 characters long.</b>

  This function must be placed right in time before
  the device starts sending.

  See example program in class description.
*/
void LeyboldComHandler::ReceiveString( char *receiveString )
{
  if (!fDeviceAvailable) {
    receiveString[0] = 0;
    return;
  }

  usleep(25000);

  int timeout = 0, readResult = 0;

  while (timeout < 100000)  {

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if (readResult > 0) {
      receiveString[readResult] = 0;
      break;
    }
    
    timeout++;
  }
}

//! Open I/O port.
/*!
  \internal
*/
void LeyboldComHandler::OpenIoPort( void )
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if (fIoPortFileDescriptor == -1) {
    std::cerr << "[LeyboldComHandler::OpenIoPort] ** ERROR: could not open device file "
              << fIoPort << "." << std::endl;
    std::cerr << "                               (probably it's not user-writable)."
              << std::endl;
    fDeviceAvailable = false;
    return;
  } else {
    // configure port with no delay
    fcntl( fIoPortFileDescriptor, F_SETFL, FNDELAY );
  }

  fDeviceAvailable = true;
}

//! Initialize I/O port.
/*!
  \internal
*/
void LeyboldComHandler::InitializeIoPort( void )
{
  if (!fDeviceAvailable) return;

  // get and save current ioport settings for later restoring
  tcgetattr(fIoPortFileDescriptor, &fCurrentTermios);

  // clear new settings struct
  bzero(&fThisTermios, sizeof(fThisTermios));

  // set input/output baud rate
  cfsetispeed(&fThisTermios, B38400);
  cfsetospeed(&fThisTermios, B38400);

  // enable the receiver and disable modem control signals
  fThisTermios.c_cflag |= CREAD;
  fThisTermios.c_cflag |= CLOCAL;

  // set 8 bits per character, no parity, 1 stop bit (8N1)
  fThisTermios.c_cflag &= ~PARENB;
  fThisTermios.c_cflag &= ~CSTOPB;
  fThisTermios.c_cflag &= ~CSIZE;
  fThisTermios.c_cflag |= (CLOCAL | CREAD | CS8);
  fThisTermios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  fThisTermios.c_oflag &= ~OPOST;
  fThisTermios.c_cc[VMIN] = 0;
  fThisTermios.c_cc[VTIME] = 10;

  // commit changes
  tcflush(fIoPortFileDescriptor, TCIOFLUSH);

  if (tcsetattr(fIoPortFileDescriptor, TCSAFLUSH, &fThisTermios) != 0)
    return;

  int modeLines = 0;

  // put card into exclusive mode
  if (ioctl(fIoPortFileDescriptor, TIOCEXCL, &modeLines) == -1)
    return;
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void LeyboldComHandler::RestoreIoPort( void )
{
  if (!fDeviceAvailable) return;

  // restore old com port settings
  tcsetattr(fIoPortFileDescriptor, TCSANOW, &fCurrentTermios);
}

//! Close I/O port.
/*!
  \internal
*/
void LeyboldComHandler::CloseIoPort( void )
{
  if (!fDeviceAvailable) return;

  close(fIoPortFileDescriptor);
}

bool LeyboldComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

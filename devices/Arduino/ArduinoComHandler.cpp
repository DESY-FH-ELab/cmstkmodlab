#include <string.h>

#include <iostream>

#include "ArduinoComHandler.h"

// SETTINGS ON THE DEVICE:
// (MENU RS-232)
// 
// BAUD: 9600
// HARDW HANDSHAKE: on
// PARITY: none (8N1)
// TX TERM: NL

/*!
  The serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  COM1 ... COM4<br>
  ttyS0 ... ttyS3<br>
  "/dev/ttyS1" ... "/dev/ttyS3"
  "/dev/ttyACM0"
*/
ArduinoComHandler::ArduinoComHandler( const ioport_t ioPort ) {

  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

///
///
///
ArduinoComHandler::~ArduinoComHandler( void ) {

  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void ArduinoComHandler::SendCommand( const char *commandString ) {

  if (!fDeviceAvailable) return;

  char singleCharacter = 0; 

  for ( unsigned int i = 0; i < strlen( commandString ); i++ ) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );
  }

//   std::cout << "write: -" << commandString <<"-"<< std::endl;

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
void ArduinoComHandler::ReceiveString( char *receiveString ) {

  if (!fDeviceAvailable) {
    receiveString[0] = 0;
    return;
  }

  usleep( ComHandlerDelay );

  int timeout = 0, readResult = 0;

  while ( timeout < 10 )  {

    usleep( 100000 );

    readResult = read( fIoPortFileDescriptor, receiveString, 12 );
    // std::cout << "read: -" << readResult << "- -" << receiveString <<"-"<< std::endl;

    if ( readResult > 0 ) {
      receiveString[readResult] = 0;
      // std::cout << "read: -" << readResult << "- -" << receiveString <<"-"<< std::endl;
      return;
    }
    timeout++;
  }

  //std::cout << "timeout" << std::endl;
}

//! Open I/O port.
/*!
  \internal
*/
void ArduinoComHandler::OpenIoPort( void ) throw (int) {

  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[ArduinoComHandler::OpenIoPort] ** ERROR: could not open device file "
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
void ArduinoComHandler::InitializeIoPort( void ) {

  if (!fDeviceAvailable) return;

#ifndef USE_FAKEIO
 
  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  /* 9600 baud */
  cfsetispeed(&fThisTermios, B9600);
  cfsetospeed(&fThisTermios, B9600);
  /* 8 bits, no parity, no stop bits */
  fThisTermios.c_cflag &= ~PARENB;
  fThisTermios.c_cflag &= ~CSTOPB;
  fThisTermios.c_cflag &= ~CSIZE;
  fThisTermios.c_cflag |= CS8;
  /* no hardware flow control */
  fThisTermios.c_cflag &= ~CRTSCTS;
  /* enable receiver, ignore status lines */
  fThisTermios.c_cflag |= CREAD | CLOCAL;
  /* disable input/output flow control, disable restart chars */
  fThisTermios.c_iflag &= ~(IXON | IXOFF | IXANY);
  /* disable canonical input, disable echo,
     disable visually erase chars,
     disable terminal-generated signals */
  fThisTermios.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  /* disable output processing */
  fThisTermios.c_oflag &= ~OPOST;

  /* wait for 24 characters to come in before read returns */
  fThisTermios.c_cc[VMIN] = 24;
  /* no minimum time to wait before read returns */
  fThisTermios.c_cc[VTIME] = 0;

  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );

#endif
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void ArduinoComHandler::RestoreIoPort( void ) {

  if (!fDeviceAvailable) return;

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void ArduinoComHandler::CloseIoPort( void ) {

  if (!fDeviceAvailable) return;

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<NL>).
/*!
  \internal
*/
void ArduinoComHandler::SendFeedString( void )
{
  write( fIoPortFileDescriptor, "\n", 1 );
}

bool ArduinoComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

#include <string.h>

#include <iostream>

#include "ArduinoPresComHandler.h"

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
ArduinoPresComHandler::ArduinoPresComHandler( const ioport_t ioPort ) {

  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

///
///
///
ArduinoPresComHandler::~ArduinoPresComHandler( void ) {

  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void ArduinoPresComHandler::SendCommand( const char *commandString ) {

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
void ArduinoPresComHandler::ReceiveString( char *receiveString ) {

  usleep( ComHandlerDelay );

  int timeout = 0, readResult = 0;

  while ( timeout < 10 )  {

    usleep( 100000 );

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if ( readResult > 0 ) {
      receiveString[readResult] = 0;
//       std::cout << "read: -" << readResult << "- -" << receiveString <<"-"<< std::endl;
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
void ArduinoPresComHandler::OpenIoPort( void ) throw (int) {

  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[ArduinoPresComHandler::OpenIoPort] ** ERROR: could not open device file "
	          << fIoPort << "." << std::endl;
    std::cerr << "                               (probably it's not user-writable)."
              << std::endl;
    throw int(-1);
  } else {
    // configure port with no delay
    fcntl( fIoPortFileDescriptor, F_SETFL, FNDELAY );
  }
}

//! Initialize I/O port.
/*!
  \internal
*/
void ArduinoPresComHandler::InitializeIoPort( void ) {

#ifndef USE_FAKEIO
 
  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // all these settings copied from stty output..

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
 fThisTermios.c_cc[VMIN] = 12;
 /* no minimum time to wait before read returns */
 fThisTermios.c_cc[VTIME] = 0;

//   fThisTermios.c_cflag   |=  NL0;
//   fThisTermios.c_cflag   |=  CR0;
//   fThisTermios.c_cflag   |=  TAB0;
//   fThisTermios.c_cflag   |=  BS0;
//   fThisTermios.c_cflag   |=  VT0;
//   fThisTermios.c_cflag   |=  FF0;

  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );

#endif
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void ArduinoPresComHandler::RestoreIoPort( void ) {

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void ArduinoPresComHandler::CloseIoPort( void ) {

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void ArduinoPresComHandler::SendFeedString( void )
{
  write( fIoPortFileDescriptor, "", 0 );
}

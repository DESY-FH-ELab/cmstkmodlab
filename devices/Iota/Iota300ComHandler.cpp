#include <string.h>

#include "Iota300ComHandler.h"

// SETTINGS ON THE DEVICE:
// (MENU RS-232)
// 
// BAUD: 19200
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
Iota300ComHandler::Iota300ComHandler( const ioport_t ioPort ) {

  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

///
///
///
Iota300ComHandler::~Iota300ComHandler( void ) {

  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void Iota300ComHandler::SendCommand( const char *commandString ) {

  char singleCharacter = 0; 

  for ( unsigned int i = 0; i < strlen( commandString ); i++ ) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );
  }

//   std::cout << "write: " << commandString << std::endl;

  // send feed characters
  SendFeedString();
}

//! flush the IO memory
void Iota300ComHandler::flush(void) {

  sleep(2);
  if(tcflush(fIoPortFileDescriptor, TCIOFLUSH)==0)
    printf("The input and output queues have been flushed\n\n");
    else{
      perror("tcflush error");
    }
    sleep(1);
}

//! Read a string from device.
/*!
\par Input:
  <br><b>Receive string must be at least 41 characters long.</b>

  This function must be placed right in time before
  the device starts sending.

  See example program in class description.
*/
void Iota300ComHandler::ReceiveString( char *receiveString ) {

  usleep( ComHandlerDelay );

  int timeout = 0, readResult = 0;

  while ( timeout < 10 )  {

    usleep( 100000 );

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if ( readResult > 0 ) {
      receiveString[readResult] = 0;
//       std::cout << "read: " << readResult << " " << receiveString << std::endl;
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
void Iota300ComHandler::OpenIoPort( void ) throw (int) {

  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[Iota300ComHandler::OpenIoPort] ** ERROR: could not open device file "
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
void Iota300ComHandler::InitializeIoPort( void ) {

#ifndef USE_FAKEIO

  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // all these settings copied from stty output..

  // baud rate
  cfsetispeed( &fThisTermios, B19200 );  // input speed
  cfsetospeed( &fThisTermios, B19200 );  // output speed

    // Enable the receiver and set local mode...
  fThisTermios.c_cflag |= (CLOCAL | CREAD);

  fThisTermios.c_cflag   &= ~PARENB;
  fThisTermios.c_cflag   &= ~PARODD;
  fThisTermios.c_cflag   &= ~CSTOPB;
  fThisTermios.c_cflag   &= ~CSIZE;
  fThisTermios.c_cflag   |=  CS8;

  fThisTermios.c_cflag &= ~CRTSCTS;                     /* enable hardware flow control */
  fThisTermios.c_lflag &= ~(ICANON | ECHO | ISIG);



  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );

#endif
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void Iota300ComHandler::RestoreIoPort( void ) {

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void Iota300ComHandler::CloseIoPort( void ) {

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void Iota300ComHandler::SendFeedString( void )
{
  write( fIoPortFileDescriptor, "\n", 2 ); // /r nicht noetig?
}

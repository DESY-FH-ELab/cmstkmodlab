
#include "KMMComHandler.h"


// SETTINGS ON THE DEVICE:
// (MENU RS-232)
// 
// RS 232: ON
// BAUD: 19200
// FLOW: NONE
// TX TERM: LF







/*!
  The serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  COM1 ... COM4<br>
  ttyS0 ... ttyS3<br>
  "/dev/ttyS1" ... "/dev/ttyS3"
*/
KMMComHandler::KMMComHandler( ioport_t ioPort ) {

  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();

}




///
///
///
KMMComHandler::~KMMComHandler( void ) {

  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();

}





//! Send the command string &lt;commandString&gt; to device.
void KMMComHandler::SendCommand( const char *commandString ) {


  char singleCharacter = 0; 

  for ( unsigned int i = 0; i < strlen( commandString ); i++ ) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );

  }


  // send feed characters ( <CR> <LF> )
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
void KMMComHandler::ReceiveString( char *receiveString ) {


  usleep( 1000000 );

  int timeout = 0, readResult = 0;

  while ( timeout < 100000 )  {

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if ( readResult > 0 )   {
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

void KMMComHandler::OpenIoPort( void ) {


  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[KMMComHandler::OpenIoPort] ** ERROR: could not open device file " << fIoPort << "." << endl;
    std::cerr << "                               (probably it's not user-writable)." << std::endl;
    throw;
  }

  else {
    // configure port with no delay
    fcntl( fIoPortFileDescriptor, F_SETFL, FNDELAY );
  }

}





//! Initialize I/O port.
/*!
  \internal
*/

void KMMComHandler::InitializeIoPort( void ) {

  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );


  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // baud rate
  cfsetispeed( &fThisTermios, B19200 );  // input speed
  cfsetospeed( &fThisTermios, B19200 );  // output speed

  // enable the receiver and disable modem control signals
  fThisTermios.c_cflag   |=  CREAD;
  fThisTermios.c_cflag   |=  CLOCAL;
  
  // set 8 bits per character, no parity, 1 stop bit (8N1)
  fThisTermios.c_cflag   &=  ~PARENB;
  fThisTermios.c_cflag   &=  ~CSTOPB;
  fThisTermios.c_cflag   &=  ~CSIZE; 
  fThisTermios.c_cflag   |=  CS8;    

  // enable hardware flow control (RTS/CTS)
  // DOCU !!
  //  fThisTermios.c_cflag        |=  CRTSCTS;

  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );

}





//! Restore former I/O port settings.
/*!
  \internal
*/

void KMMComHandler::RestoreIoPort( void ) {

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );

}





//! Close I/O port.
/*!
  \internal
*/
void KMMComHandler::CloseIoPort( void ) {

  close( fIoPortFileDescriptor );

}





//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void KMMComHandler::SendFeedString( void ) {

  // feed string is <NL>
  char feedString = 10;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );

}



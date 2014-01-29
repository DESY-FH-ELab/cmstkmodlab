#include <sys/ioctl.h>

#include <iostream>

#include "GMH3750ComHandler.h"

// SETTINGS ON THE DEVICE:
// (MENU RS-232)
// 
// BAUD: 4800
// PARITY: none (8N1)
// HARDW HANDSHAKE: off
// TX TERM: NL

/*!
  The serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  COM1 ... COM4<br>
  ttyS0 ... ttyS3<br>
  "/dev/ttyS1" ... "/dev/ttyS3"
*/
GMH3750ComHandler::GMH3750ComHandler( ioport_t ioPort )
{
  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

///
///
///
GMH3750ComHandler::~GMH3750ComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void GMH3750ComHandler::SendCommand( const char *commandString, int length )
{
  char singleCharacter = 0; 
  int size = length;
  if (length==-1) size = strlen( commandString );

  for ( unsigned int i = 0; i < size; i++ ) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];

    write( fIoPortFileDescriptor, &singleCharacter, 1 );
  }
  
  // send feed characters
  //SendFeedString();
}

//! Read a string from device.
/*!
\par Input:
  <br><b>Receive string must be at least 41 characters long.</b>

  This function must be placed right in time before
  the device starts sending.

  See example program in class description.
*/
void GMH3750ComHandler::ReceiveString( char *receiveString )
{
  usleep( _COMHANDLER_DELAY );

  int timeout = 0, readResult = 0;

  while ( timeout < 500000 )  {

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if ( readResult > 0 )   {
    
      receiveString[readResult] = 0;
      break;
    }
    
    timeout++;
  }
  
  std::cout << "TO: " << timeout << std::endl; /////////////////////////////////
}

//! Open I/O port.
/*!
  \internal
*/
void GMH3750ComHandler::OpenIoPort( void )
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    
    std::cerr << "[GMH3750ComHandler::OpenIoPort] ** ERROR: could not open device file "
              << fIoPort << "." << std::endl;
    std::cerr << "                               (probably it's not user-writable)."
              << std::endl;
    throw;
  
  } else {
  
    // configure port with no delay
    fcntl( fIoPortFileDescriptor, F_SETFL, FNDELAY );
  
  }

  int status;

  ioctl( fIoPortFileDescriptor, TIOCMGET, &status );
  if (status & TIOCM_RTS) status &= ~TIOCM_RTS;
  if (!(status & TIOCM_DTR)) status |= TIOCM_DTR;
  ioctl( fIoPortFileDescriptor, TIOCMSET, &status );

  // ioctl( fIoPortFileDescriptor, TIOCMGET, &status );
  // if (status & TIOCM_RTS) {
  //   std::cout << "RTS is set" << std::endl;
  // } else {
  //   std::cout << "RTS is not set" << std::endl;
  // }
  // if (status & TIOCM_DTR) {
  //   std::cout << "DTR is set" << std::endl;
  // } else {
  //   std::cout << "DTR is not set" << std::endl;
  // }  
}

//! Initialize I/O port.
/*!
  \internal
*/
void GMH3750ComHandler::InitializeIoPort( void )
{
  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // all these settings copied from stty output..

  // baud rate
  cfsetispeed( &fThisTermios, B4800 );  // input speed
  cfsetospeed( &fThisTermios, B4800 );  // output speed

  // enable the receiver and disable modem control signals
  fThisTermios.c_cflag   |=  CREAD;
  //fThisTermios.c_cflag   |=  CLOCAL;

  // set 8 bits per character, no parity, 1 stop bit (8N1)
  fThisTermios.c_cflag   &=  ~PARENB;
  fThisTermios.c_cflag   &=  ~CSTOPB;
  //fThisTermios.c_cflag   &=  ~CSIZE;
  fThisTermios.c_cflag   |=  CS8;
  
  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void GMH3750ComHandler::RestoreIoPort( void )
{
  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void GMH3750ComHandler::CloseIoPort( void )
{
  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void GMH3750ComHandler::SendFeedString( void )
{
  // feed string is <NL>
  char feedString = 10;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );
}

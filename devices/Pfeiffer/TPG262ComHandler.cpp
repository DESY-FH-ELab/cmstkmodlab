#include "TPG262ComHandler.h"


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
*/
TPG262ComHandler::TPG262ComHandler( const ioport_t ioPort )
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
TPG262ComHandler::~TPG262ComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void TPG262ComHandler::SendCommand( const char *commandString )
{
  char singleCharacter = 0; 

  for ( unsigned int i = 0; i < strlen( commandString ); i++ ) {
    
    // scan command string character wise & write
    singleCharacter = commandString[i];
    write( fIoPortFileDescriptor, &singleCharacter, 1 );

  }

  // send feed characters
  SendFeedString();
}

void TPG262ComHandler::SendEnquiry( )
{
  char singleCharacter = 0x05; 

  write( fIoPortFileDescriptor, &singleCharacter, 1 );

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
void TPG262ComHandler::ReceiveString( char *receiveString )
{
  usleep( _COMHANDLER_DELAY );

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
void TPG262ComHandler::OpenIoPort( void )
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[TPG262ComHandler::OpenIoPort] ** ERROR: could not open device file " << fIoPort << "." << endl;
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
void TPG262ComHandler::InitializeIoPort( void )
{
#ifndef USE_FAKEIO

  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

  // CONFIGURE NEW SETTINGS

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // all these settings copied from stty output..

  // baud rate
  cfsetispeed( &fThisTermios, B9600 );  // input speed
  cfsetospeed( &fThisTermios, B9600 );  // output speed

  // various settings, 8N1 (no parity, 1 stopbit)
  fThisTermios.c_cflag   &= ~PARENB;
  fThisTermios.c_cflag   &= ~PARODD;
  fThisTermios.c_cflag   |=  CS8;
  fThisTermios.c_cflag   |=  HUPCL;
  fThisTermios.c_cflag   &= ~CSTOPB;
  fThisTermios.c_cflag   |=  CREAD;
  fThisTermios.c_cflag   |=  CLOCAL;
  fThisTermios.c_cflag   &= ~CRTSCTS;

  fThisTermios.c_lflag   |=  ISIG;
  fThisTermios.c_lflag   |=  ICANON;
  fThisTermios.c_lflag   |=  ECHO;
  fThisTermios.c_lflag   |=  ECHOE;
  fThisTermios.c_lflag   |=  ECHOK;
  fThisTermios.c_lflag   &= ~ECHONL;
  fThisTermios.c_lflag   |=  IEXTEN;

  fThisTermios.c_iflag   &= ~IGNBRK;
  fThisTermios.c_iflag   &= ~BRKINT;
  fThisTermios.c_iflag   &= ~IGNPAR;
  fThisTermios.c_iflag   &= ~PARMRK;
  fThisTermios.c_iflag   &= ~INPCK;

  // right i/o/l flags ??
  fThisTermios.c_iflag   &= ~ISTRIP;
  fThisTermios.c_iflag   &= ~INLCR;
  fThisTermios.c_iflag   &= ~IGNCR;
  //  fThisTermios.c_iflag   |=  ICRNL; // DO NOT ENABLE!!
  fThisTermios.c_iflag   |=  IXON;
  fThisTermios.c_iflag   &= ~IXOFF;
  fThisTermios.c_iflag   &= ~IUCLC;
  fThisTermios.c_iflag   &= ~IXANY;
  fThisTermios.c_iflag   &= ~IMAXBEL;
  
  fThisTermios.c_iflag   &= ~IUTF8;

  // right i/o/l flags?
  fThisTermios.c_oflag   |=  OPOST;
  fThisTermios.c_oflag   &= ~OLCUC;
  fThisTermios.c_oflag   &= ~OCRNL;
  fThisTermios.c_oflag   |=  ONLCR;
  fThisTermios.c_oflag   &= ~ONOCR;
  fThisTermios.c_oflag   &= ~ONLRET;
  fThisTermios.c_oflag   &= ~OFILL;
  fThisTermios.c_oflag   &= ~OFDEL;

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

void TPG262ComHandler::RestoreIoPort( void )
{
  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void TPG262ComHandler::CloseIoPort( void )
{
  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void TPG262ComHandler::SendFeedString( void )
{
  // feed string is <NL>
  char feedString = 10;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );
}

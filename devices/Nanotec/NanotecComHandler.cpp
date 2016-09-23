#include <string.h>

#include <iostream>

#include "NanotecComHandler.h"

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
NanotecComHandler::NanotecComHandler(ioport_t ioPort)
{
  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();
}

NanotecComHandler::~NanotecComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void NanotecComHandler::SendCommand( const char *commandString )
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

  usleep(10000);
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
void NanotecComHandler::ReceiveString( char *receiveString )
{
  if (!fDeviceAvailable) {
    receiveString[0] = 0;
    return;
  }

  usleep( 10000 );

  int timeout = 0, readResult = 0;

  while ( timeout < 100000 )  {

    readResult = read( fIoPortFileDescriptor, receiveString, 1024 );

    if ( readResult > 0 ) {
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
void NanotecComHandler::OpenIoPort( void )
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[NanotecComHandler::OpenIoPort] ** ERROR: could not open device file "
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
void NanotecComHandler::InitializeIoPort( void )
{
  if (!fDeviceAvailable) return;

  // get and save current ioport settings for later restoring
  tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );

  // clear new settings struct
  bzero( &fThisTermios, sizeof( fThisTermios ) );

  // set input/output baud rate
  cfsetispeed(&fThisTermios, B115200);
  cfsetospeed(&fThisTermios, B115200);

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
void NanotecComHandler::RestoreIoPort( void )
{
  if (!fDeviceAvailable) return;

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void NanotecComHandler::CloseIoPort( void )
{
  if (!fDeviceAvailable) return;

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void NanotecComHandler::SendFeedString( void )
{
  if (!fDeviceAvailable) return;

  // feed string is <CR>
  char feedString = '\r';

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );
}

bool NanotecComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

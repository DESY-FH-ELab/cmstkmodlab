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

#include "AgilentTwisTorr304ComHandler.h"

AgilentTwisTorr304ComHandler::AgilentTwisTorr304ComHandler( const ioport_t ioPort )
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
AgilentTwisTorr304ComHandler::~AgilentTwisTorr304ComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void AgilentTwisTorr304ComHandler::SendCommand( const char *commandString )
{
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
void AgilentTwisTorr304ComHandler::flush(void)
{
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
void AgilentTwisTorr304ComHandler::ReceiveString( char *receiveString )
{
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
void AgilentTwisTorr304ComHandler::OpenIoPort( void ) noexcept(false)
{
  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[AgilentTwisTorr304ComHandler::OpenIoPort] ** ERROR: could not open device file "
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
void AgilentTwisTorr304ComHandler::InitializeIoPort( void )
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
void AgilentTwisTorr304ComHandler::RestoreIoPort( void )
{
  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void AgilentTwisTorr304ComHandler::CloseIoPort( void )
{
  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void AgilentTwisTorr304ComHandler::SendFeedString( void )
{
  write(fIoPortFileDescriptor, "\n", 1);
}

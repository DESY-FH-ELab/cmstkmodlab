/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "FP50ComHandler.h"


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
FP50ComHandler::FP50ComHandler( ioport_t ioPort ) {

  // save ioport 
  fIoPort = ioPort;

  // initialize
  OpenIoPort();
  InitializeIoPort();

}




///
///
///
FP50ComHandler::~FP50ComHandler( void ) {

  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();

}





//! Send the command string &lt;commandString&gt; to device.
void FP50ComHandler::SendCommand( const char *commandString ) {


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
void FP50ComHandler::ReceiveString( char *receiveString ) {


  usleep( 10000 );

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

void FP50ComHandler::OpenIoPort( void ) {


  // open io port ( read/write | no term control | no DCD line check )
  fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );

  // check if successful
  if ( fIoPortFileDescriptor == -1 ) {
    std::cerr << "[FP50ComHandler::OpenIoPort] ** ERROR: could not open device file " << fIoPort << "." << endl;
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

void FP50ComHandler::InitializeIoPort( void ) {

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
  //  fThisTermios.c_iflag   |=  ICRNL;
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

  fThisTermios.c_cflag   |=  NL0;
  fThisTermios.c_cflag   |=  CR0;
  fThisTermios.c_cflag   |=  TAB0;
  fThisTermios.c_cflag   |=  BS0;
  fThisTermios.c_cflag   |=  VT0;
  fThisTermios.c_cflag   |=  FF0;

  // commit changes
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );

}





//! Restore former I/O port settings.
/*!
  \internal
*/

void FP50ComHandler::RestoreIoPort( void ) {

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );

}





//! Close I/O port.
/*!
  \internal
*/
void FP50ComHandler::CloseIoPort( void ) {

  close( fIoPortFileDescriptor );

}





//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void FP50ComHandler::SendFeedString( void ) {

  // feed string is <NL>
  char feedString = 10;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );

}



#include <string.h>

#include <iostream>

#include "KeyenceComHandler.h"

#define KEYENCEDEBUG 1

/*!
  The USB serial port &lt;ioPort&gt; may be specified in several ways:<br><br>
  ttyUSB0 ... ttyUSB3<br>
  "/dev/ttyUSB0" ... "/dev/ttyUSB3"
*/
KeyenceComHandler::KeyenceComHandler(ioport_t ioPort)
{
    std::cout<<"[KeyenceComHandler] begin constructor"<<std::endl;
    // save ioport 
    fIoPort = ioPort;
    
    // initialize
    OpenIoPort();
    InitializeIoPort();
    //std::cout<<"[KeyenceComHandler] end constructor"<<std::endl;
}

KeyenceComHandler::~KeyenceComHandler( void )
{
  // restore ioport options as they were
  RestoreIoPort();
  
  // close device file
  CloseIoPort();
}

//! Send the command string &lt;commandString&gt; to device.
void KeyenceComHandler::SendCommand( const char *commandString )
{
    std::cout<<"[KeyenceComHandler::SendCommand] fdeviceavailable = "<<fDeviceAvailable<<std::endl;
  if (!fDeviceAvailable) return;

  // std::cout<<"[KeyenceComHandler::SendCommand] device available"<<std::endl;

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
void KeyenceComHandler::ReceiveString(std::string receiveString, char *temp_output, int samplingRate, int averagingRate )
{
    //std::string receiveString;

  std::cout<<"in KeyenceComHandler receive string"<<std::endl;

  //  std::unique_ptr<char[]> temp_output;
  //  char *temp_output;
  //  temp_output = NULL;

  std::cout<<"temp_output = "<<&temp_output<<std::endl;

  if (!fDeviceAvailable) {
    std::cout<<"[KeyenceComHandler::ReceiveString] device not available"<<std::endl;
    //    return "";
    return;
  }

  std::cout<<"in KeyenceComHandler device available"<<std::endl;

  temp_output[0] = 0;

  usleep( 5000 );

  std::cout<<"in KeyenceComHandler after sleep"<<std::endl;

  int timeout = 0;
  size_t readResult = 0;
  int limit = 1000 + 2*samplingRate*averagingRate;
  while ( timeout < limit)  {

    readResult = read( fIoPortFileDescriptor, temp_output, 1024 );
    std::cout<<"readResult = "<<readResult<<std::endl;
    if ( readResult > 0 ) {
      std::cout<<"temp_output = "<<temp_output<<std::endl;
      receiveString += std::string(temp_output, readResult);
      if(receiveString.find(13) != std::string::npos){
	//received end of command
	std::cout<<"[KeyenceComHandler::ReceiveString] end of command string received = "<<receiveString<<std::endl;
	break;
      }
    }
    usleep ( 5 );
    readResult = 0;
    std::cout<<"[KeyenceComHandler::ReceiveString] before clearing temp_output"<<std::endl;
    temp_output[0] = 0;
    timeout++;
  }

#ifdef KEYENCEDEBUG
  std::cout<<"[KeyenceComHandler::ReceiveString] waited for "<<timeout<<" microseconds"<<std::endl;
#endif

  if ( timeout == limit ) {
      std::cerr << "[KeyenceComHandler::ReceiveString] ** ERROR: command timed out! "
	    << std::endl;
      //      return "";
      return;
  }

  //delete temp_output;
  //if(temp_output){ std::cout<<"safely delete"<<std::endl; delete temp_output; std::cout<<"after delete"<<std::endl; temp_output = NULL;}
  //  return receiveString;
}

//! Open I/O port.
/*!
  \internal
*/
void KeyenceComHandler::OpenIoPort( void )
{
    std::cout<<"[KeyenceComHandler] begin OpenIoPort"<<std::endl;
    // open io port ( read/write | no term control | no DCD line check )
    fIoPortFileDescriptor = open( fIoPort, O_RDWR | O_NOCTTY  | O_NDELAY );
    
    // check if successful
    if ( fIoPortFileDescriptor == -1 ) {
        std::cerr << "[KeyenceComHandler::OpenIoPort] ** ERROR: could not open device file "
	      << fIoPort << "." << std::endl;
        std::cerr << "                               (probably it's not user-writable)."
	      << std::endl;
        fDeviceAvailable = false;
        return;
    } else {
        // configure port with no delay
        int flags = fcntl(fIoPortFileDescriptor, F_GETFL, 0);
        flags |= O_NONBLOCK;
        flags |= FNDELAY;
        fcntl( fIoPortFileDescriptor, F_SETFL, flags );
    }
    
    fDeviceAvailable = true;
    //    std::cout<<"[KeyenceComHandler] end OpenIoPort"<<std::endl;
}

//! Initialize I/O port.
/*!
  \internal
*/
void KeyenceComHandler::InitializeIoPort( void )
{
    //    std::cout<<"[KeyenceComHandler] begin InitializeIoPort"<<std::endl;
    if (!fDeviceAvailable) return;
    
    // get and save current ioport settings for later restoring
    tcgetattr( fIoPortFileDescriptor, &fCurrentTermios );
    
#ifndef USE_FAKEIO
    
    // CONFIGURE NEW SETTINGS
    
    // clear new settings struct
    bzero( &fThisTermios, sizeof( fThisTermios ) );
    
    // all these settings copied from stty output..
    
    // baud rate
    cfsetispeed( &fThisTermios, B115200 );  // input speed
    cfsetospeed( &fThisTermios, B115200 );  // output speed
    
    // various settings, 8N1 (no parity, 1 stopbit)
    fThisTermios.c_cflag   &= ~PARENB;
    fThisTermios.c_cflag   &= ~PARODD;
    fThisTermios.c_cflag   |=  CS8;
    fThisTermios.c_cflag   |=  HUPCL;
    fThisTermios.c_cflag   &= ~CSTOPB;
    fThisTermios.c_cflag   |=  CREAD;
    fThisTermios.c_cflag   |=  CLOCAL;
    fThisTermios.c_cflag   &= ~CRTSCTS;
    
    // commit changes
    tcsetattr( fIoPortFileDescriptor, TCSANOW, &fThisTermios );
    
#endif

    //    std::cout<<"[KeyenceComHandler] end InitializeIoPort"<<std::endl;
}

//! Restore former I/O port settings.
/*!
  \internal
*/
void KeyenceComHandler::RestoreIoPort( void )
{
  if (!fDeviceAvailable) return;

  // restore old com port settings
  tcsetattr( fIoPortFileDescriptor, TCSANOW, &fCurrentTermios );
}

//! Close I/O port.
/*!
  \internal
*/
void KeyenceComHandler::CloseIoPort( void )
{
  if (!fDeviceAvailable) return;

  close( fIoPortFileDescriptor );
}

//! Send command termination string (<CR><NL>).
/*!
  \internal
*/
void KeyenceComHandler::SendFeedString( void )
{
  if (!fDeviceAvailable) return;

  // feed string is <CR> ; see documentation page 4.1
  char feedString = 13;

  // write <CR> and get echo
  write( fIoPortFileDescriptor, &feedString, 1 );
}

bool KeyenceComHandler::DeviceAvailable()
{
  return fDeviceAvailable;
}

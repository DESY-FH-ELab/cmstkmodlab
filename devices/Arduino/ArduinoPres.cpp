#include <string.h>

#include <cstdlib>
#include <iostream>

//#####################
// TODO:
// query error codes
//#####################

#include "ArduinoComHandler.h"

#include "ArduinoPres.h"

//#define __ARDUINO_DEBUG

///
///
///
ArduinoPres::ArduinoPres( const ioport_t ioPort )
  : VArduinoPres(ioPort),
    isCommunication_(false),
    uDelay_(250000)
{
  comHandler_ = new ArduinoComHandler( ioPort );
  Device_Init();
}

ArduinoPres::~ArduinoPres()
{
  delete comHandler_;
}

bool ArduinoPres::IsCommunication( void ) const
{
  return isCommunication_; 
}

///
///
///
float ArduinoPres::GetPressureA( void ) const
{
  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::GetPressureA] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "PA" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
//   std::cout<<"bufferA = "<<buffer<<std::endl;
  StripBuffer( buffer );

  std::string buf = buffer;
  if (buf.substr(0, buf.find(','))!="PA") {
    std::cerr << " [ArduinoPres::GetPressureA] ** ERROR: Device communication problem. "
              << buf << std::endl;
    //throw;
  }
  buf = buf.substr(buf.find(',')+1);

  int value = std::atoi(buf.c_str());
  return ToPressure(value);
}

///
///
///
float ArduinoPres::GetPressureB( void ) const
{
  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::GetPressureB] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "PB" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
//   std::cout<<"bufferA = "<<buffer<<std::endl;
  StripBuffer( buffer );

  std::string buf = buffer;
  if (buf.substr(0, buf.find(','))!="PB") {
    std::cerr << " [ArduinoPres::GetPressureB] ** ERROR: Device communication problem. "
              << buf << std::endl;
    //throw;
  }
  buf = buf.substr(buf.find(',')+1);

  int value = std::atoi(buf.c_str());
  return ToPressure(value);
}
///
///
///
float ArduinoPres::GetFlow( void ) const
{
  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::GetFlow] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "PC" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::string buf = buffer;
  if (buf.substr(0, buf.find(','))!="PC") {
    std::cerr << " [ArduinoPres::GetFlow] ** ERROR: Device communication problem. "
              << buf << std::endl;
    //throw;
  }
  buf = buf.substr(buf.find(',')+1);

  int value = std::atoi(buf.c_str());
  return ToFlow(value);
}
///
/// strip trailing newlines & stuff
/// from machine answer
///
void ArduinoPres::StripBuffer( char* buffer ) const {

  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[ArduinoPres::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
  std::cout << " > " << buffer << std::endl;
  #endif

  for( unsigned int c = 0; c < strlen( buffer ); ++c ) {
    if( '\n' == buffer[c] || '\r' == buffer[c] ) {
      buffer[c] = 0;
      break;
    }
  }
}

///
/// read back software version
/// to check communication with device
///
void ArduinoPres::Device_Init( void ) {

  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  isCommunication_ = false;
  if (comHandler_->DeviceAvailable()) {

    char buffer[1000];
  
    comHandler_->SendCommand( "ID" );
    usleep( uDelay_ );
    
    comHandler_->ReceiveString( buffer );
    usleep( uDelay_ );
    StripBuffer( buffer );
    std::string temp(buffer);
    
    if (!temp.compare("ID,ArduinoPres")) {
      std::cerr << " [ArduinoPres::Device_Init] ** ERROR: Device communication problem."
		<< std::endl;
      isCommunication_ = false;
      return;
    }

    isCommunication_ = true;
  }
}

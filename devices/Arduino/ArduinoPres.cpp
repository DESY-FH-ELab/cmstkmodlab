#include <string.h>

#include <cstdlib>
#include <iostream>

//#####################
// TODO:
// query error codes
//#####################

#include "ArduinoComHandler.h"

#include "ArduinoPres.h"

#define __ARDUINO_DEBUG
///
///
///
ArduinoPres::ArduinoPres( const ioport_t ioPort )
  : VArduinoPres(ioPort),
    uDelay_(250000)
{
  comHandler_ = new ArduinoComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}

///
///
///
float ArduinoPres::GetPressureA( void ) const {

  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::GetPressureA] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "a" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
//   std::cout<<"bufferA = "<<buffer<<std::endl;
  StripBuffer( buffer );

  return ToInteger(buffer);
    std::cout<<"bufferA = "<<ToInteger(buffer)<<std::endl;
}

///
///
///
float ArduinoPres::GetPressureB( void ) const {

  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::GetPressureB] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "b" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
//   std::cout<<"bufferA = "<<buffer<<std::endl;
  StripBuffer( buffer );

  return ToInteger(buffer);
  std::cout<<"bufferA = "<<ToInteger(buffer)<<std::endl;
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

int ArduinoPres::ToInteger(const char* buffer) const
{
  std::string temp(buffer);
  temp.erase(0, 0);

  return std::atoi( temp.c_str() );
}

float ArduinoPres::ToFloat(const char* buffer) const
{
  std::string temp(buffer);
  temp.erase(0, 0);

  return std::atof( temp.c_str() )/100.;
}

///
/// read back software version
/// to check communication with device
///
void ArduinoPres::Device_Init( void ) {

  #ifdef __ARDUINO_DEBUG
  std::cout << "[ArduinoPres::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "?" );
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );
  usleep( uDelay_ );
  StripBuffer( buffer );
  std::string temp(buffer);

  if (!temp.compare("1")) {
    std::cerr << " [ArduinoPres::Device_Init] ** ERROR: Device communication problem."
        << std::endl;
    isCommunication_ = false;
    return;
  }

  isCommunication_ = true;
}

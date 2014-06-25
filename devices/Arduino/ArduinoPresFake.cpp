#include <unistd.h>

#include <iostream>
#include <string>

//#####################
// TODO:
// query error codes
//#####################

#include "ArduinoPresFake.h"

///
///
///
ArduinoPresFake::ArduinoPresFake( const ioport_t ioPort )
    :VArduinoPres(ioPort)
{
  pressureA_ = -11;
  pressureB_ = -15;
}

///
///
///
float ArduinoPresFake::GetPressureA( void ) const
{
  std::cout << " [ArduinoPresFake::GetPressureA] -- FAKE: Returning p(a) = "
	        << pressureA_ << std::endl;
  usleep( 10000 );
  return pressureA_ + (double)rand() / RAND_MAX;
}

///
///
///
float ArduinoPresFake::GetPressureB( void ) const
{
  std::cout << " [ArduinoPresFake::GetPressureA] -- FAKE: Returning p(b) = "
            << pressureB_ << std::endl;
  usleep( 10000 );
  return pressureB_ + (double)rand() / RAND_MAX;
}

///
///
///
void ArduinoPresFake::Device_Init( void )
{

}

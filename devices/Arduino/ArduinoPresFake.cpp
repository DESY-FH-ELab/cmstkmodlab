#include <unistd.h>

#include <iostream>
#include <sstream>
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
  pressureA_ = 435;
  pressureB_ = 168;
}

///
///
///
float ArduinoPresFake::GetPressureA( void ) const
{
  std::cout << " [ArduinoPresFake::GetPressureA] -- FAKE: Returning p(a) = "
	        << pressureA_ << std::endl;

  usleep( 10000 );

  std::ostringstream oss;
  oss << "PA,";
  oss << pressureA_;
  std::string buf = oss.str();

  if (buf.substr(0, buf.find(','))!="PA") {
    std::cerr << " [ArduinoPres::GetPressureA] ** ERROR: Device communication problem. "
              << buf << std::endl;
    throw;
  }
  buf = buf.substr(buf.find(',')+1);

  int value = std::atoi(buf.c_str());
  return ToPressure(value);
}

///
///
///
float ArduinoPresFake::GetPressureB( void ) const
{
  std::cout << " [ArduinoPresFake::GetPressureB] -- FAKE: Returning p(b) = "
            << pressureB_ << std::endl;

  usleep( 10000 );

  std::ostringstream oss;
  oss << "PB,";
  oss << pressureB_;
  std::string buf = oss.str();

  if (buf.substr(0, buf.find(','))!="PB") {
    std::cerr << " [ArduinoPres::GetPressureB] ** ERROR: Device communication problem. "
              << buf << std::endl;
    throw;
  }
  buf = buf.substr(buf.find(',')+1);

  int value = std::atoi(buf.c_str());
  return ToPressure(value);
}

///
///
///
void ArduinoPresFake::Device_Init( void )
{

}

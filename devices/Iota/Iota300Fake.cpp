#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

//#####################
// TODO:
// query error codes
//#####################

#include "Iota300Fake.h"

///
///
///
Iota300Fake::Iota300Fake( const ioport_t ioPort )
    :VIota300(ioPort),
     status_(0),
     flow_(20),
     pressure_(10)
{

}

///
/// returns success flag
///
bool Iota300Fake::SetFlow( const float flow ) const {

  if( flow > Iota300UpperFlowLimit || flow < Iota300LowerFlowLimit ) {
    std::cerr << " [Iota300Fake::SetFlow] ** ERROR: Flow Q="
	      << flow << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. Iota300Fake class definition)" << std::endl;
    return false;
  }

  std::cout << " [Iota300Fake::SetFlow] -- FAKE: Setting Flow = " 
	    << flow << "." << std::endl;

  flow_ = flow;

  return true;
}

///
/// returns success flag
///
bool Iota300Fake::SetPressure( const float pressure ) const {

  if( pressure > Iota300UpperPressureLimit || pressure < Iota300LowerPressureLimit ) {
    std::cerr << " [Iota300Fake::SetPressure] ** ERROR: p="
	      << pressure << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. Iota300Fake class definition)" << std::endl;
    return false;
  }

  std::cout << " [Iota300Fake::SetPressure] -- FAKE: Setting p = " 
	    << pressure << "." << std::endl;

  pressure_ = pressure;

  return true;
}

///
/// returns success flag
///
bool Iota300Fake::SetStatus( const float status ) const {

  /*
  if( status > __IOTA300_UPPER_STATUS_LIMIT || status < __IOTA300_LOWER_STATUS_LIMIT ) {
    std::cerr << " [Iota300Fake::SetStatus] ** ERROR: Status ="
	      << status << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. Iota300Fake class definition)" << std::endl;
    return false;
  }
  */

  std::cout << " [Iota300Fake::SetStatus] -- FAKE: Setting Status = " 
	    << status << "." << std::endl;

  status_ = status;

  return true;

}

///
///
///
float Iota300Fake::GetSetFlow( void ) const
{
  std::cout << " [Iota300Fake::GetSetFlow] -- FAKE: Returning Q = "
        << flow_ << std::endl;
  usleep( 10000 );
  return flow_;
}
///
///
///
float Iota300Fake::GetActFlow( void ) const
{
  std::cout << " [Iota300Fake::GetActFlow] -- FAKE: Returning Q = "
        << flow_ << std::endl;
  usleep( 10000 );
  return flow_;
}

///
///
///
float Iota300Fake::GetSetPressure( void ) const
{
  std::cout << " [Iota300Fake::GetSetPressure] -- FAKE: Returning Q = "
        << pressure_ << std::endl;
  usleep( 10000 );
  return pressure_;
}
///
///
///
float Iota300Fake::GetActPressure( void ) const
{
  std::cout << " [Iota300Fake::GetActPressure] -- FAKE: Returning Q = "
        << pressure_ << std::endl;
  usleep( 10000 );
  return pressure_;
}

///
///
///
float Iota300Fake::GetStatus( void ) const
{
  std::cout << " [Iota300Fake::GetStatus] -- FAKE: Returning Status = "
	    << status_ << std::endl;
  usleep( 10000 );
  return status_;
}

///
///
///
void Iota300Fake::Device_Init( void ) {

  
}

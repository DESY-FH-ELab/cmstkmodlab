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

#include "CoriFlowFake.h"

///
///
///
CoriFlowFake::CoriFlowFake( const ioport_t ioPort )
    :VCoriFlow(ioPort)
{

}

float CoriFlowFake::getTemp( void ) const
{
  return 0.;
}

float CoriFlowFake::getPres( void ) const
{
  return 0.;
}

float CoriFlowFake::getMeasure( void ) const
{
  return 0.;
}

/// additional stuff
float CoriFlowFake::getCapacity( void ) const
{
  return 0.;
}

std::string CoriFlowFake::getUnit( void ) const
{
  return std::string();
}

std::string CoriFlowFake::getFluid( void ) const
{
  return std::string();
}

std::string CoriFlowFake::getTag( void ) const
{
  return std::string();
}

std::string CoriFlowFake::setInit( void ) const
{
  return std::string();
}

std::string CoriFlowFake::setTag( void ) const
{
  return std::string();
}

float CoriFlowFake::getDensity( void ) const
{
  return 0.;
}

std::string CoriFlowFake::setCapacity( void ) const
{
  return std::string();
}


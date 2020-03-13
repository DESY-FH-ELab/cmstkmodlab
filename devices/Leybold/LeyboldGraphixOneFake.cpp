/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>

#include "LeyboldGraphixOneFake.h"

LeyboldGraphixOneFake::LeyboldGraphixOneFake( const ioport_t ioPort )
  :LeyboldGraphixThreeFake(ioPort)
{

}

LeyboldGraphixOneFake::~LeyboldGraphixOneFake()
{

}

int LeyboldGraphixOneFake::GetNumberOfChannels() const
{
  return 1;
}

VLeyboldGraphix::SensorDetectionMode LeyboldGraphixOneFake::GetSensorDetectionMode() const
{
  return LeyboldGraphixThreeFake::GetSensorDetectionMode(1);
}

void LeyboldGraphixOneFake::SetSensorDetectionMode(VLeyboldGraphix::SensorDetectionMode mode)
{
  LeyboldGraphixThreeFake::SetSensorDetectionMode(1, mode);
}

std::string LeyboldGraphixOneFake::GetSensorTypeName() const
{
  return LeyboldGraphixThreeFake::GetSensorTypeName(1);
}

void LeyboldGraphixOneFake::SetSensorTypeName(std::string type)
{
  LeyboldGraphixThreeFake::SetSensorTypeName(1, type);
}

std::string LeyboldGraphixOneFake::GetSensorName() const
{
  return LeyboldGraphixThreeFake::GetSensorName(1);
}

void LeyboldGraphixOneFake::SetSensorName(const std::string& name)
{
  LeyboldGraphixThreeFake::SetSensorName(1, name);
}

VLeyboldGraphix::SensorStatus LeyboldGraphixOneFake::GetSensorStatus() const
{
  return LeyboldGraphixThreeFake::GetSensorStatus(1);
}

double LeyboldGraphixOneFake::GetPressure() const
{
  return LeyboldGraphixThreeFake::GetPressure(1);
}

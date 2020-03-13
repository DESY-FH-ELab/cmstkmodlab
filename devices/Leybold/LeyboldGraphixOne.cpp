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

#include <cstring>
#include <cmath>
#include <sstream>

#include "LeyboldGraphixOne.h"

LeyboldGraphixOne::LeyboldGraphixOne( const ioport_t ioPort )
  :LeyboldGraphixThree(ioPort)
{

}

LeyboldGraphixOne::~LeyboldGraphixOne()
{

}

VLeyboldGraphix::SensorDetectionMode LeyboldGraphixOne::GetSensorDetectionMode() const
{
  return LeyboldGraphixThree::GetSensorDetectionMode(1);
}

void LeyboldGraphixOne::SetSensorDetectionMode(VLeyboldGraphix::SensorDetectionMode mode)
{
  LeyboldGraphixThree::SetSensorDetectionMode(1, mode);
}

std::string LeyboldGraphixOne::GetSensorTypeName() const
{
  return LeyboldGraphixThree::GetSensorTypeName(1);
}

void LeyboldGraphixOne::SetSensorTypeName(std::string type)
{
  LeyboldGraphixThree::SetSensorTypeName(1, type);
}

std::string LeyboldGraphixOne::GetSensorName() const
{
  return LeyboldGraphixThree::GetSensorName(1);
}

void LeyboldGraphixOne::SetSensorName(const std::string& name)
{
  LeyboldGraphixThree::SetSensorName(1, name);
}

VLeyboldGraphix::SensorStatus LeyboldGraphixOne::GetSensorStatus() const
{
  return LeyboldGraphixThree::GetSensorStatus(1);
}

double LeyboldGraphixOne::GetPressure() const
{
  return LeyboldGraphixThree::GetPressure(1);
}

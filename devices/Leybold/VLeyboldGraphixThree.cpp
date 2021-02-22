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

#include "VLeyboldGraphixThree.h"

VLeyboldGraphixThree::VLeyboldGraphixThree( const ioport_t ioPort )
 : VLeyboldGraphix(ioPort)
{

}

VLeyboldGraphixThree::~VLeyboldGraphixThree()
{

}


VLeyboldGraphixThree::SensorType VLeyboldGraphixThree::GetSensorType(int sensor) const
{
  if (sensor<1 || sensor>3) return SensorType_NOSEN;

  std::string type = GetSensorTypeName(sensor);

  return GetSensorTypeByTypeName(type);
}

void VLeyboldGraphixThree::SetSensorType(int sensor, VLeyboldGraphix::SensorType type)
{
  if (sensor<1 || sensor>3) return;

  switch (type) {
  case SensorType_NOSEN:
    SetSensorTypeName(sensor, "NOSEN");
    break;
  case SensorType_TTRx:
    SetSensorTypeName(sensor, "TTR?");
    break;
  case SensorType_TTR90:
    SetSensorTypeName(sensor, "TTR90");
    break;
  case SensorType_TTR91:
    SetSensorTypeName(sensor, "TTR91");
    break;
  case SensorType_TTR91N:
    SetSensorTypeName(sensor, "TTR91N");
    break;
  }
}

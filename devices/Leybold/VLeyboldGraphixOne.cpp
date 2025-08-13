/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "VLeyboldGraphixOne.h"

VLeyboldGraphixOne::VLeyboldGraphixOne( const ioport_t ioPort )
 : VLeyboldGraphix(ioPort)
{

}

VLeyboldGraphixOne::~VLeyboldGraphixOne()
{

}


VLeyboldGraphix::SensorType VLeyboldGraphixOne::GetSensorType() const
{
  std::string type = GetSensorTypeName();

  return GetSensorTypeByTypeName(type);
}

void VLeyboldGraphixOne::SetSensorType(VLeyboldGraphix::SensorType type)
{
  switch (type) {
  case SensorType_NOSEN:
    SetSensorTypeName("NOSEN");
    break;
  case SensorType_TTRx:
    SetSensorTypeName("TTR?");
    break;
  case SensorType_TTR90:
    SetSensorTypeName("TTR90");
    break;
  case SensorType_TTR91:
    SetSensorTypeName("TTR91");
    break;
  case SensorType_TTR91N:
    SetSensorTypeName("TTR91N");
    break;
  }
}

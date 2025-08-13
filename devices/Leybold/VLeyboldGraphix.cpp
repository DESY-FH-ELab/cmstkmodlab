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

#include "VLeyboldGraphix.h"

VLeyboldGraphix::VLeyboldGraphix( const ioport_t ioPort )
{

}

VLeyboldGraphix::~VLeyboldGraphix()
{

}

VLeyboldGraphix::SensorType VLeyboldGraphix::GetSensorTypeByTypeName(const std::string& type) const
{
  if (type=="NOSEN")  return SensorType_NOSEN;
  if (type=="TTR?")   return SensorType_TTRx;
  if (type=="TTR90")  return SensorType_TTR90;
  if (type=="TTR91")  return SensorType_TTR91;
  if (type=="TTR91N") return SensorType_TTR91N;

  return SensorType_NOSEN;
}

std::string VLeyboldGraphix::GetSensorStatusText(SensorStatus status) const
{
  switch (status) {
  case SensorStatus_nosen:
    return "NOSEN";
    break;
  case SensorStatus_ok:
    return "OK";
    break;
  case SensorStatus_range:
    return "Range?";
    break;
  case SensorStatus_soff:
    return "S-OFF";
    break;
  case SensorStatus_ErrorH:
    return "Error-H";
    break;
  case SensorStatus_ErrorL:
    return "Error-L";
    break;
  case SensorStatus_ErrorS:
    return "Error-S";
    break;
  default:
    return "NOSEN";
  }
}

VLeyboldGraphix::SensorStatus VLeyboldGraphix::GetSensorStatusByStatusText(const std::string& status) const
{
  if (status=="NOSEN")   return SensorStatus_nosen;
  if (status=="OK")      return SensorStatus_ok;
  if (status=="Range?")  return SensorStatus_range;
  if (status=="S-OFF")   return SensorStatus_soff;
  if (status=="Error-H") return SensorStatus_ErrorH;
  if (status=="Error-L") return SensorStatus_ErrorL;
  if (status=="Error-S") return SensorStatus_ErrorS;

  return SensorStatus_nosen;
}

std::string VLeyboldGraphix::GetDisplayUnitName(DisplayUnit unit) const
{
  switch (unit) {
  case DisplayUnit_mbar:
    return "mbar";
    break;
  case DisplayUnit_Torr:
    return "Torr";
    break;
  case DisplayUnit_Pa:
    return "Pa";
    break;
  case DisplayUnit_psi:
    return "psi";
    break;
  case DisplayUnit_Micron:
    return "Micron";
    break;
  default:
    return "unknown";
  }
}

VLeyboldGraphix::DisplayUnit VLeyboldGraphix::GetDisplayUnitByUnitText(const std::string& unit) const
{
  if (unit=="mbar")   return DisplayUnit_mbar;
  if (unit=="Torr")   return DisplayUnit_Torr;
  if (unit=="Pa")     return DisplayUnit_Pa;
  if (unit=="psi")    return DisplayUnit_psi;
  if (unit=="Micron") return DisplayUnit_Micron;

  return DisplayUnit_unknown;
}

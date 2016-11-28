#include "VLeyboldGraphixThree.h"

VLeyboldGraphixThree::VLeyboldGraphixThree( const ioport_t ioPort )
{
  sensorStatusText_["NO-SEN"] = SensorStatus_nosen;
  sensorStatusText_["OK"] = SensorStatus_ok;
  sensorStatusText_["Range?"] = SensorStatus_range;
  sensorStatusText_["S-OFF"] = SensorStatus_soff;
  sensorStatusText_["Error-H"] = SensorStatus_ErrorH;
  sensorStatusText_["Error-L"] = SensorStatus_ErrorL;
  sensorStatusText_["Error-S"] = SensorStatus_ErrorS;

  sensorTextStatus_[SensorStatus_nosen] = "NO-SEN";
  sensorTextStatus_[SensorStatus_ok] = "OK";
  sensorTextStatus_[SensorStatus_range] = "Range?";
  sensorTextStatus_[SensorStatus_soff] = "S-OFF";
  sensorTextStatus_[SensorStatus_ErrorH] = "Error-H";
  sensorTextStatus_[SensorStatus_ErrorL] = "Error-L";
  sensorTextStatus_[SensorStatus_ErrorS] = "Error-S";

  displayUnitNames_[DisplayUnit_mbar] = "mbar";
  displayUnitNames_[DisplayUnit_Torr] = "Torr";
  displayUnitNames_[DisplayUnit_Pa] = "Pa";
  displayUnitNames_[DisplayUnit_psi] = "psi";
  displayUnitNames_[DisplayUnit_Micron] = "Micron";

  displayNameUnits_["mbar"] = DisplayUnit_mbar;
  displayNameUnits_["Torr"] = DisplayUnit_Torr;
  displayNameUnits_["Pa"] = DisplayUnit_Pa;
  displayNameUnits_["psi"] = DisplayUnit_psi;
  displayNameUnits_["Micron"] = DisplayUnit_Micron;
}

VLeyboldGraphixThree::~VLeyboldGraphixThree()
{

}


VLeyboldGraphixThree::SensorType VLeyboldGraphixThree::GetSensorType(int sensor) const
{
  if (sensor<1 || sensor>3) return SensorType_NOSEN;

  std::string type = GetSensorTypeName(sensor);

  if (type=="NOSEN")  return SensorType_NOSEN;
  if (type=="TTR?")   return SensorType_TTRx;
  if (type=="TTR90")  return SensorType_TTR90;
  if (type=="TTR91")  return SensorType_TTR91;
  if (type=="TTR91N") return SensorType_TTR91N;

  return SensorType_NOSEN;
}

void VLeyboldGraphixThree::SetSensorType(int sensor, VLeyboldGraphixThree::SensorType type)
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

std::string VLeyboldGraphixThree::GetSensorStatusText(SensorStatus status) const
{
  std::map<SensorStatus,std::string>::const_iterator itFind = sensorTextStatus_.find(status);
  if (itFind!=sensorTextStatus_.end()) {
    return itFind->second;
  }

  return "NO-SEN";
}

std::string VLeyboldGraphixThree::GetDisplayUnitName(DisplayUnit unit) const
{
  std::map<DisplayUnit,std::string>::const_iterator itFind = displayUnitNames_.find(unit);
  if (itFind!=displayUnitNames_.end()) {
    return itFind->second;
  }

  return "unknown";
}

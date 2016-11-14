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

std::string VLeyboldGraphixThree::GetDisplayUnitName() const
{
  DisplayUnit unit = GetDisplayUnit();

  std::map<DisplayUnit,std::string>::const_iterator itFind = displayUnitNames_.find(unit);
  if (itFind!=displayUnitNames_.end()) {
    return itFind->second;
  } else {
    return "unknown";
  }
}

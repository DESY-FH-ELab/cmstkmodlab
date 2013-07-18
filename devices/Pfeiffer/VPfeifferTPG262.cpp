#include "VPfeifferTPG262.h"

///
///
///
VPfeifferTPG262::VPfeifferTPG262( const ioport_t ioPort )
{

}

VPfeifferTPG262::~VPfeifferTPG262()
{

}

std::string VPfeifferTPG262::GetGaugeStatusAsString(GaugeStatus status)
{
     switch (status) {
        case tpg262GaugeOkay:
            return std::string("Gauge Okay");
        case tpg262GaugeUnderrange:
            return std::string("Underrange");
        case tpg262GaugeOverrange:
            return std::string("Overrange");
        case tpg262GaugeSensorError:
            return std::string("Sensor Error");
        case tpg262GaugeSensorOff:
            return std::string("Sensor Off");
        case tpg262GaugeNoSensor:
            return std::string("No Sensor");
        case tpg262GaugeIdentificationError:
            return std::string("Identification Error");
        default:
            return std::string("Invalid Status");
    }
}

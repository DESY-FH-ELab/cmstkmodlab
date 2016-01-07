#include <sstream>

#include "VLStepExpress.h"

VLStepExpress::VLStepExpress( const ioport_t ioPort )
{

}

VLStepExpress::~VLStepExpress()
{

}

char VLStepExpress::GetAxisName(VLStepExpress::Axis axis)
{
  switch (axis) {
  case 0:
    return 'x';
  case 1:
    return 'y';
  case 2:
    return 'z';
  case 3:
    return 'a';
  }
  return 'x';
}

const char * VLStepExpress::GetAxisDimensionName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "microsteps";
  case MICROMETER:
    return "micrometer";
  case MILLIMETER:
    return "millimeter";
  case DEGREE:
    return "degree";
  case ROTATIONS:
    return "rotations";
  }
  return "microsteps";
}

char VLStepExpress::GetAxisStatusText(VLStepExpress::AxisStatus status)
{
  switch (status) {
  case AXISSTANDSANDREADY:
    return '@';
  case AXISMOVING:
    return 'M';
  case AXISJOYSTICK:
    return 'J';
  case AXISINCONTROL:
    return 'C';
  case AXISLIMITSWITCHTRIPPED:
    return 'S';
  case AXISACKAFTERCALIBRATION:
    return 'A';
  case AXISERRACKAFTERCALIBRATION:
   return 'E';
  case AXISACKAFTERTBLSTROKEMSR:
  return 'D';
  case AXISINSETUP:
    return 'U';
  case AXISTIMEOUT:
    return 'T';
  case AXISERROR:
    return 'F';
  case AXISDISABLED:
    return '-';
  case AXISSTATEUNKNOWN:
    return 'X';
  }
  return 'X';
}

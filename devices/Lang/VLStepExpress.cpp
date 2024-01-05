/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "VLStepExpress.h"

VLStepExpress::VLStepExpress(const std::string& /* ioPort */)
{
}

VLStepExpress::~VLStepExpress()
{
}

void VLStepExpress::ValidConfig()
{
  this->SendCommand("!validconfig");
}

void VLStepExpress::ValidParameter()
{
  this->SendCommand("!validpar");
}

void VLStepExpress::SetValue(const std::string & command,
                             const std::string & value)
{
  std::ostringstream os;
  os << command << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, const std::string & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1)
{
  std::ostringstream os;
  os << command << " " << value1;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1, int value2)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1, int value2, int value3)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1, int value2, int value3, int value4)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3 << " " << value4;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, int value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1)
{
  std::ostringstream os;
  os << command << " " << value1;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1, double value2)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1, double value2, double value3)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1, double value2, double value3, double value4)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3 << " " << value4;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, double value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, double value1, double value2)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             const std::vector<int> & values)
{
  std::ostringstream os;
  os << command;
  for (std::vector<int>::const_iterator it = values.begin();
      it!=values.end();
      ++it) {
    os << " " << *it;
  }
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             const std::vector<double> & values)
{
  std::ostringstream os;
  os << command;
  for (std::vector<double>::const_iterator it = values.begin();
      it!=values.end();
      ++it) {
    os << " " << *it;
  }
  this->SendCommand(os.str());
}

void VLStepExpress::GetValue(const std::string & command,
                             std::string & value)
{
  value = this->ReceiveString(command);
}

void VLStepExpress::GetValue(const std::string & command,
                             int & value)
{
  int temp;
  auto buffer = this->ReceiveString(command);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command,
                             double & value)
{
  double temp;
  auto buffer = this->ReceiveString(command);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command, VLStepExpress::Axis axis,
                             std::string & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis);
  value = this->ReceiveString(os.str());
}

void VLStepExpress::GetValue(const std::string & command,
                             std::vector<int> & values)
{
  int temp;
  auto buffer = this->ReceiveString(command);

  values.clear();
  std::istringstream is(buffer);
  while (is >> temp) {
    values.push_back(temp);
  }
}

void VLStepExpress::GetValue(const std::string & command,
                             VLStepExpress::Axis axis, int & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis);

  int temp;
  auto buffer = this->ReceiveString(os.str());
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command,
                             std::vector<double> & values)
{
  double temp;
  auto buffer = this->ReceiveString(command);

  values.clear();
  std::istringstream is(buffer);
  while (is >> temp) {
    values.push_back(temp);
  }
}

void VLStepExpress::GetValue(const std::string & command, VLStepExpress::Axis axis,
                             double & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis);

  double temp;
  auto buffer = this->ReceiveString(os.str());
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command, VLStepExpress::Axis axis,
                             std::vector<double> & values)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis);

  double temp;
  auto buffer = this->ReceiveString(os.str());

  values.clear();
  std::istringstream is(buffer);
  while (is >> temp){
    values.push_back(temp);
  }
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

const char * VLStepExpress::GetAxisDimensionShortName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "usteps";
  case MICROMETER:
    return "um";
  case MILLIMETER:
    return "mm";
  case DEGREE:
    return "deg";
  case ROTATIONS:
    return "rot";
  }
  return "usteps";
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

const char * VLStepExpress::GetAxisVelocityShortName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "usteps/s";
  case MICROMETER:
    return "um/s";
  case MILLIMETER:
    return "mm/s";
  case DEGREE:
    return "deg/s";
  case ROTATIONS:
    return "rot/s";
  }
  return "usteps/s";
}

const char * VLStepExpress::GetAxisVelocityName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "microsteps/s";
  case MICROMETER:
    return "micrometer/s";
  case MILLIMETER:
    return "millimeter/s";
  case DEGREE:
    return "degree/s";
  case ROTATIONS:
    return "rotations/s";
  }
  return "microsteps/s";
}

const char * VLStepExpress::GetAxisAccelerationShortName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "usteps/s^2";
  case MICROMETER:
    return "um/s^2";
  case MILLIMETER:
    return "mm/s^2";
  case DEGREE:
    return "deg/s^2";
  case ROTATIONS:
    return "rot/s^2";
  }
  return "usteps/s^2";
}

const char * VLStepExpress::GetAxisAccelerationName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "microsteps/s^2";
  case MICROMETER:
    return "micrometer/s^2";
  case MILLIMETER:
    return "millimeter/s^2";
  case DEGREE:
    return "degree/s^2";
  case ROTATIONS:
    return "rotations/s^2";
  }
  return "microsteps/s^2";
}

const char * VLStepExpress::GetAxisAccelerationJerkShortName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "usteps/s^3";
  case MICROMETER:
    return "um/s^3";
  case MILLIMETER:
    return "mm/s^3";
  case DEGREE:
    return "deg/s^3";
  case ROTATIONS:
    return "rot/s^3";
  }
  return "usteps/s^3";
}

const char * VLStepExpress::GetAxisAccelerationJerkName(VLStepExpress::Dimension dimension)
{
  switch (dimension) {
  case MICROSTEPS:
    return "microsteps/s^3";
  case MICROMETER:
    return "micrometer/s^3";
  case MILLIMETER:
    return "millimeter/s^3";
  case DEGREE:
    return "degree/s^3";
  case ROTATIONS:
    return "rotations/s^3";
  }
  return "microsteps/s^3";
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

#include <sstream>

#include "VLStepExpress.h"

VLStepExpress::VLStepExpress( const ioport_t ioPort )
{

}

VLStepExpress::~VLStepExpress()
{

}

void VLStepExpress::GetAutoStatus(int & value)
{
  GetValue("autostatus", value);
}

void VLStepExpress::SetAutoStatus(int value)
{
  SetValue("!autostatus", value);
}

void VLStepExpress::GetAxisStatus(std::vector<int> & values)
{
  std::string line;
  GetValue("statusaxis", line);
  std::istringstream is(line);
  std::string token;

  values.clear();

  while (is >> token) {
    char t = token[0];

    switch (t) {
    case '@': {
      values.push_back(AXISREADY);
      break;
    }
    case 'M': {
      values.push_back(AXISREADY);
      break;
    }
    case 'J': {
      values.push_back(AXISJOYSTICK);
      break;
    }
    case 'C': {
      values.push_back(AXISINCONTROL);
      break;
    }
    case 'S': {
      values.push_back(AXISLIMITSWITCHTRIPPED);
      break;
    }
    case '-': {
      values.push_back(AXISDISABLED);
      break;
    }
    default:
      values.push_back(AXISSTATEUNKNOWN);
    }
  }
}

void VLStepExpress::GetAxisEnabled(std::vector<int> & values)
{
  GetValue("axis", values);
}

void VLStepExpress::GetAxisEnabled(VLStepExpress::Axis axis, int & value)
{
  GetValue("axis", axis, value);
}

void VLStepExpress::SetAxisEnabled(std::vector<int> & values)
{
  SetValue("!axis", values);
}

void VLStepExpress::SetAxisEnabled(VLStepExpress::Axis axis, int value)
{
  SetValue("!axis", axis, value);
}

void VLStepExpress::GetAxisDirection(std::vector<int> & values)
{
  GetValue("axisdir", values);
}

void VLStepExpress::GetAxisDirection(VLStepExpress::Axis axis, int & value)
{
  GetValue("axisdir", axis, value);
}

void VLStepExpress::SetAxisDirection(std::vector<int> & values)
{
  SetValue("!axisdir", values);
}

void VLStepExpress::SetAxisDirection(VLStepExpress::Axis axis, int value)
{
  SetValue("!axisdir", axis, value);
}

void VLStepExpress::GetDimension(std::vector<int> & values)
{
  GetValue("dim", values);
}

void VLStepExpress::GetDimension(VLStepExpress::Axis axis, int & value)
{
  GetValue("dim", axis, value);
}

void VLStepExpress::SetDimension(std::vector<int> & values)
{
  SetValue("!dim", values);
}

void VLStepExpress::SetDimension(VLStepExpress::Axis axis, int value)
{
  SetValue("!dim", axis, value);
}

void VLStepExpress::GetPowerAmplifierStatus(std::vector<int> & values)
{
  GetValue("pa", values);
}

void VLStepExpress::GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value)
{
  GetValue("pa", axis, value);
}

void VLStepExpress::SetPowerAmplifierStatus(std::vector<int> & values)
{
  SetValue("!pa", values);
}

void VLStepExpress::SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value)
{
  SetValue("!pa", axis, value);
}

void VLStepExpress::GetPosition(std::vector<double> & values)
{
  GetValue("pos", values);
}

void VLStepExpress::GetPosition(VLStepExpress::Axis axis, double & value)
{
  GetValue("pos", axis, value);
}

void VLStepExpress::SetPosition(std::vector<double> & values)
{
  SetValue("!pos", values);
}

void VLStepExpress::SetPosition(VLStepExpress::Axis axis, double value)
{
  SetValue("!pos", axis, value);
}

void VLStepExpress::MoveAbsolute(std::vector<double> & values)
{
  SetValue("!moa", values);
}

void VLStepExpress::MoveAbsolute(double x, double y, double z, double a)
{
  SetValue("!moa", x, y, z, a);
}

void VLStepExpress::MoveAbsolute(VLStepExpress::Axis axis, double value)
{
  SetValue("!moa", axis, value);
}

void VLStepExpress::MoveRelative(std::vector<double> & values)
{
  SetValue("!mor", values);
}

void VLStepExpress::MoveRelative(double x, double y, double z, double a)
{
  SetValue("!mor", x, y, z, a);
}

void VLStepExpress::MoveRelative(VLStepExpress::Axis axis, double value)
{
  SetValue("!mor", axis, value);
}

void VLStepExpress::MoveRelative()
{
  this->SendCommand("!m");
}

void VLStepExpress::Reset()
{
  this->SendCommand("!Reset");
}

void VLStepExpress::ConfirmErrorRectification()
{
  this->SendCommand("!quit");
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
  this->SendCommand(command);
  this->ReceiveString(value);
}

void VLStepExpress::GetValue(const std::string & command,
                             int & value)
{
  this->SendCommand(command);

  std::string buffer;
  int temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command,
                             double & value)
{
  this->SendCommand(command);

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command, VLStepExpress::Axis axis,
                             std::string & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis);
  this->SendCommand(os.str());
  this->ReceiveString(value);
}

void VLStepExpress::GetValue(const std::string & command,
                             std::vector<int> & values)
{
  this->SendCommand(command);

  std::string buffer;
  int temp;
  this->ReceiveString(buffer);

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
  this->SendCommand(os.str());

  std::string buffer;
  int temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command,
                             std::vector<double> & values)
{
  this->SendCommand(command);

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);

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
  this->SendCommand(os.str());

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
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

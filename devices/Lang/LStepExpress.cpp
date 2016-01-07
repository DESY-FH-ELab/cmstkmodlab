#include <cmath>
#include <sstream>
#include <iostream>

#include "LStepExpress.h"

// #define LSTEPDEBUG 1

LStepExpress::LStepExpress( const ioport_t ioPort )
  :VLStepExpress(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new LStepExpressComHandler( ioPort );
  DeviceInit();
}

LStepExpress::~LStepExpress()
{
  delete comHandler_;
}

bool LStepExpress::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

// low level debugging methods
void LStepExpress::SendCommand(const std::string & command)
{
#ifdef LSTEPDEBUG
  std::cout << "SendCommand: " << command << std::endl;
#endif
  comHandler_->SendCommand(command.c_str());
}

void LStepExpress::ReceiveString(std::string & buffer)
{
  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);
  buffer = buf;
}

void LStepExpress::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\r') {
      buffer[c] = '\0';
      break;
    }
  }
}

void LStepExpress::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {

    isDeviceAvailable_ = true;

    char buffer[1000];
    std::string buf;

    comHandler_->SendCommand("ver"); // read version

    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if (buf.find("PE43 1.00.01", 0)!=0) {
      isDeviceAvailable_ = false;
      return;
    }

    comHandler_->SendCommand("iver"); // read internal version
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if (buf.find("E2015.02.27-3012", 0)!=0) {
      isDeviceAvailable_ = false;
      return;
    }

    comHandler_->SendCommand("readsn"); // read serial number
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    unsigned long serialNumber = std::atol(buffer);

    if (serialNumber != 40052435759) {
      isDeviceAvailable_ = false;
      return;
    }
  }
}

void LStepExpress::GetAutoStatus(int & value)
{
  GetValue("autostatus", value);
}

void LStepExpress::SetAutoStatus(int value)
{
  SetValue("!autostatus", value);
}

void LStepExpress::GetAxisStatus(std::vector<int> & values)
{
  std::string line;
  GetValue("statusaxis", line);
  
  std::string token;

  values.clear();

  for (unsigned int i=0;i<4;++i) {
    char token = line[i];
  
    switch (token) {
    case '@': {
      values.push_back(AXISSTANDSANDREADY);
      break;
    }
    case 'M': {
      values.push_back(AXISMOVING);
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
    case 'A': {
      values.push_back(AXISACKAFTERCALIBRATION);
      break;
    }
    case 'E': {
      values.push_back(AXISERRACKAFTERCALIBRATION);
      break;
    }
    case 'D': {
      values.push_back(AXISACKAFTERTBLSTROKEMSR);
      break;
    }
    case 'U': {
      values.push_back(AXISINSETUP);
      break;
    }
    case 'T': {
      values.push_back(AXISTIMEOUT);
      break;
    }
    case 'F': {
      values.push_back(AXISERROR);
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

void LStepExpress::GetAxisEnabled(std::vector<int> & values)
{
  GetValue("axis", values);
}

void LStepExpress::GetAxisEnabled(VLStepExpress::Axis axis, int & value)
{
  GetValue("axis", axis, value);
}

void LStepExpress::SetAxisEnabled(std::vector<int> & values)
{
  SetValue("!axis", values);
}

void LStepExpress::SetAxisEnabled(VLStepExpress::Axis axis, int value)
{
  SetValue("!axis", axis, value);
}

void LStepExpress::GetAxisDirection(std::vector<int> & values)
{
  GetValue("axisdir", values);
}

void LStepExpress::GetAxisDirection(VLStepExpress::Axis axis, int & value)
{
  GetValue("axisdir", axis, value);
}

void LStepExpress::SetAxisDirection(std::vector<int> & values)
{
  SetValue("!axisdir", values);
}

void LStepExpress::SetAxisDirection(VLStepExpress::Axis axis, int value)
{
  SetValue("!axisdir", axis, value);
}

void LStepExpress::GetDimension(std::vector<int> & values)
{
  GetValue("dim", values);
}

void LStepExpress::GetDimension(VLStepExpress::Axis axis, int & value)
{
  GetValue("dim", axis, value);
}

void LStepExpress::SetDimension(std::vector<int> & values)
{
  SetValue("!dim", values);
}

void LStepExpress::SetDimension(VLStepExpress::Axis axis, int value)
{
  SetValue("!dim", axis, value);
}

void LStepExpress::GetPowerAmplifierStatus(std::vector<int> & values)
{
  GetValue("pa", values);
}

void LStepExpress::GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value)
{
  GetValue("pa", axis, value);
}

void LStepExpress::SetPowerAmplifierStatus(std::vector<int> & values)
{
  SetValue("!pa", values);
}

void LStepExpress::SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value)
{
  SetValue("!pa", axis, value);
}

void LStepExpress::GetPosition(std::vector<double> & values)
{
  GetValue("pos", values);
}

void LStepExpress::GetPosition(VLStepExpress::Axis axis, double & value)
{
  GetValue("pos", axis, value);
}

void LStepExpress::SetPosition(std::vector<double> & values)
{
  SetValue("!pos", values);
}

void LStepExpress::SetPosition(VLStepExpress::Axis axis, double value)
{
  SetValue("!pos", axis, value);
}

void LStepExpress::MoveAbsolute(std::vector<double> & values)
{
  SetValue("!moa", values);
}

void LStepExpress::MoveAbsolute(double x, double y, double z, double a)
{
  SetValue("!moa", x, y, z, a);
}

void LStepExpress::MoveAbsolute(VLStepExpress::Axis axis, double value)
{
  SetValue("!moa", axis, value);
}

void LStepExpress::MoveRelative(std::vector<double> & values)
{
  SetValue("!mor", values);
}

void LStepExpress::MoveRelative(double x, double y, double z, double a)
{
  SetValue("!mor", x, y, z, a);
}

void LStepExpress::MoveRelative(VLStepExpress::Axis axis, double value)
{
  SetValue("!mor", axis, value);
}

void LStepExpress::MoveRelative()
{
  this->SendCommand("!m");
}

bool LStepExpress::GetStatus()
{
  std::string value;
  this->GetValue("status", value);

  if (value.compare("OK...")==0) return true;
  return false;
}

void LStepExpress::GetSystemStatus(std::vector<int>& values)
{
  this->GetValue("?sysstatus", values);
}

void LStepExpress::GetSystemStatus(VLStepExpress::Axis axis, int & value)
{
  this->GetValue("?sysstatus", axis, value);
}

bool LStepExpress::GetJoystickEnabled()
{
  int value;
  this->GetValue("joy", value);
  if (value==1) return true;
  return false;
}

void LStepExpress::SetJoystickEnabled(bool enabled)
{
  if (enabled) {
    this->SendCommand("!joy 1");
  } else {
    this->SendCommand("!joy 0");
  }
}

void LStepExpress::GetJoystickAxisEnabled(std::vector<int> & values)
{
  GetValue("joyenable", values);
}

void LStepExpress::GetJoystickAxisEnabled(VLStepExpress::Axis axis, int & value)
{
  GetValue("joyenable", axis, value);
}

void LStepExpress::SetJoystickAxisEnabled(std::vector<int> & values)
{
  SetValue("!joyenable", values);
}

void LStepExpress::SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value)
{
  SetValue("!joyenable", axis, value);
}

int LStepExpress::GetError()
{
  int value;
  this->GetValue("err", value);
  return value;
}

void LStepExpress::Reset()
{
  this->SendCommand("!Reset");
}

void LStepExpress::ConfirmErrorRectification()
{
  this->SendCommand("!quit");
}

void LStepExpress::ValidConfig()
{
  this->SendCommand("!validconfig");
}

void LStepExpress::ValidParameter()
{
  this->SendCommand("!validpar");
}

void LStepExpress::SetValue(const std::string & command,
                             const std::string & value)
{
  std::ostringstream os;
  os << command << " " << value;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                            LStepExpress::Axis axis, const std::string & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             int value1)
{
  std::ostringstream os;
  os << command << " " << value1;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             int value1, int value2)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             int value1, int value2, int value3)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             int value1, int value2, int value3, int value4)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3 << " " << value4;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             LStepExpress::Axis axis, int value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             double value1)
{
  std::ostringstream os;
  os << command << " " << value1;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             double value1, double value2)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             double value1, double value2, double value3)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             double value1, double value2, double value3, double value4)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3 << " " << value4;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
                             LStepExpress::Axis axis, double value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis) << " " << value;
  this->SendCommand(os.str());
}

void LStepExpress::SetValue(const std::string & command,
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

void LStepExpress::SetValue(const std::string & command,
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

void LStepExpress::GetValue(const std::string & command,
                             std::string & value)
{
  this->SendCommand(command);
  this->ReceiveString(value);
}

void LStepExpress::GetValue(const std::string & command,
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

void LStepExpress::GetValue(const std::string & command,
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

void LStepExpress::GetValue(const std::string & command, LStepExpress::Axis axis,
                             std::string & value)
{
  std::ostringstream os;
  os << command << " " << GetAxisName(axis);
  this->SendCommand(os.str());
  this->ReceiveString(value);
}

void LStepExpress::GetValue(const std::string & command,
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

void LStepExpress::GetValue(const std::string & command,
                             LStepExpress::Axis axis, int & value)
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

void LStepExpress::GetValue(const std::string & command,
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

void LStepExpress::GetValue(const std::string & command, LStepExpress::Axis axis,
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

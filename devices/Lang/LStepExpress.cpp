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

#include <unistd.h>

#include <cstring>
#include <cmath>
#include <sstream>
#include <iostream>

#include "LStepExpress.h"

//#define LSTEPDEBUG 0

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
  std::cout << "Device SendCommand: " << command << std::endl;
#endif

  comHandler_->SendCommand(command.c_str());
}

void LStepExpress::ReceiveString(std::string & buffer)
{
  usleep(1000);

  char buf[1000];
  comHandler_->ReceiveString(buf);

#ifdef LSTEPDEBUG
  std::cout << "Device ReceiveString: " << buf << std::endl;
#endif

  StripBuffer(buf);
  buffer = buf;
}

void LStepExpress::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
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

    if(buf.find("PE43 1.00.01", 0) != 0)
    {
      std::cout << std::endl;
      std::cout << " LStepExpress::DeviceInit ---";
      std::cout << " device with invalid version [Command(\"ver\") = " << buf << "]";
      std::cout << ", device set to NON AVAILABLE";
      std::cout << std::endl << std::endl;

      isDeviceAvailable_ = false;

      return;
    }

    comHandler_->SendCommand("iver"); // read internal version
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if(   (buf.find("E2015.02.27-3012", 0) != 0) // pre-DAF
       && (buf.find("E2018.02.27-2002", 0) != 0) // DAF
    ){
      std::cout << std::endl;
      std::cout << " LStepExpress::DeviceInit ---";
      std::cout << " device with invalid internal version [Command(\"iver\") = " << buf << "]";
      std::cout << ", device set to NON AVAILABLE";
      std::cout << std::endl << std::endl;

      isDeviceAvailable_ = false;

      return;
    }

    /*

    comHandler_->SendCommand("readsn"); // read serial number
    comHandler_->ReceiveString(buffer);

    StripBuffer(buffer);
    unsigned long serialNumber = std::atol(buffer);

    if(   (serialNumber != 40052435759) // pre-DAF
       && (serialNumber != 40051635759) // pre-DAF
       && (serialNumber != 80050323881) // DAF
    ){
      std::cout << std::endl;
      std::cout << " LStepExpress::DeviceInit ---";
      std::cout << " device with invalid serial number [Command(\"readsn\") = " << serialNumber << "]";
      std::cout << ", device set to NON AVAILABLE";
      std::cout << std::endl << std::endl;

      isDeviceAvailable_ = false;

      return;
    }

    */
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

void LStepExpress::SetAxisEnabled(const std::vector<int> & values)
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

void LStepExpress::SetAxisDirection(const std::vector<int> & values)
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

void LStepExpress::SetDimension(const std::vector<int> & values)
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

void LStepExpress::SetPowerAmplifierStatus(const std::vector<int> & values)
{
  SetValue("!pa", values);
}

void LStepExpress::SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value)
{
  SetValue("!pa", axis, value);
}

void LStepExpress::GetAccelerationJerk(std::vector<double> & values)
{
  GetValue("acceljerk", values);
}

void LStepExpress::GetAccelerationJerk(VLStepExpress::Axis axis, double & value)
{
  GetValue("acceljerk", axis, value);
}

void LStepExpress::SetAccelerationJerk(const std::vector<double> & values)
{
  SetValue("acceljerk", values);
}

void LStepExpress::SetAccelerationJerk(VLStepExpress::Axis axis, double value)
{
  SetValue("acceljerk", axis, value);
}

void LStepExpress::GetDecelerationJerk(std::vector<double> & values)
{
  GetValue("deceljerk", values);
}

void LStepExpress::GetDecelerationJerk(VLStepExpress::Axis axis, double & value)
{
  GetValue("deceljerk", axis, value);
}

void LStepExpress::SetDecelerationJerk(const std::vector<double> & values)
{
  SetValue("deceljerk", values);
}

void LStepExpress::SetDecelerationJerk(VLStepExpress::Axis axis, double value)
{
  SetValue("deceljerk", axis, value);
}

void LStepExpress::GetAcceleration(std::vector<double> & values)
{
  GetValue("accel", values);
}

void LStepExpress::GetAcceleration(VLStepExpress::Axis axis, double & value)
{
  GetValue("accel", axis, value);
}

void LStepExpress::SetAcceleration(const std::vector<double> & values)
{
  SetValue("accel", values);
}

void LStepExpress::SetAcceleration(VLStepExpress::Axis axis, double value)
{
  SetValue("accel", axis, value);
}

void LStepExpress::GetDeceleration(std::vector<double> & values)
{
  GetValue("decel", values);
}

void LStepExpress::GetDeceleration(VLStepExpress::Axis axis, double & value)
{
  GetValue("decel", axis, value);
}

void LStepExpress::SetDeceleration(const std::vector<double> & values)
{
  SetValue("decel", values);
}

void LStepExpress::SetDeceleration(VLStepExpress::Axis axis, double value)
{
  SetValue("decel", axis, value);
}

void LStepExpress::GetVelocity(std::vector<double> & values)
{
  GetValue("vel", values);
}

void LStepExpress::GetVelocity(VLStepExpress::Axis axis, double & value)
{
  GetValue("vel", axis, value);
}

void LStepExpress::SetVelocity(const std::vector<double> & values)
{
  SetValue("vel", values);
}

void LStepExpress::SetVelocity(VLStepExpress::Axis axis, double value)
{
  SetValue("vel", axis, value);
}

void LStepExpress::GetPosition(std::vector<double> & values)
{
  GetValue("pos", values);
}

void LStepExpress::GetPosition(VLStepExpress::Axis axis, double & value)
{
  GetValue("pos", axis, value);
}

void LStepExpress::SetPosition(const std::vector<double> & values)
{
  SetValue("!pos", values);
}

void LStepExpress::SetPosition(VLStepExpress::Axis axis, double value)
{
  SetValue("!pos", axis, value);
}

void LStepExpress::MoveAbsolute(const std::vector<double> & values)
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

void LStepExpress::MoveRelative(const std::vector<double> & values)
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
  this->GetValue("?sysstat", values);
}

void LStepExpress::GetSystemStatusText(std::string& value)
{
  this->GetValue("?sysstatus", value);
}

void LStepExpress::GetSystemStatus(VLStepExpress::Axis axis, int & value)
{
  this->GetValue("?sysstat", axis, value);
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

void LStepExpress::SetJoystickAxisEnabled(const std::vector<int> & values)
{
  SetValue("!joyenable", values);
  usleep(100000);
  this->SendCommand("!joy 0");
  usleep(100000);
  this->SendCommand("!joy 1");
}

void LStepExpress::SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value)
{
  SetValue("!joyenable", axis, value);
  usleep(100000);
  this->SendCommand("!joy 0");
  usleep(100000);
  this->SendCommand("!joy 1");
}

int LStepExpress::GetError()
{
  int value;
  this->GetValue("err", value);
  return value;
}

void LStepExpress::ErrorQuit()
{
  this->SendCommand("!quit");
}

void LStepExpress::Restart()
{
//  this->SendCommand("!quit");
}

bool LStepExpress::GetPositionControllerEnabled()
{
  int posctrl_status(-1);
  GetValue("?poscon", posctrl_status);

  return bool(posctrl_status == 1);
}

void LStepExpress::SetPositionControllerEnabled(const bool enable)
{
  if(enable){ this->SendCommand("!poscon 1"); }
  else      { this->SendCommand("!poscon 0"); }
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

void LStepExpress::SaveConfig()
{
    this->SendCommand("Save");
}

void LStepExpress::Calibrate()
{
  this->SendCommand("!cal");
}

void LStepExpress::EmergencyStop()
{
    this->SendCommand("!a");
}

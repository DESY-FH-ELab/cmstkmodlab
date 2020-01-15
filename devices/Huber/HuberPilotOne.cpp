/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

// https://www.huber-online.com/download/manuals/Handbuch_Datenkommunikation_PB_en.pdf

#include <string>
#include <iomanip>
#include <cstdlib>
#include <iostream>

#include "PilotOneComHandler.h"

#include "HuberPilotOne.h"

HuberPilotOne::HuberPilotOne(const ioport_t ioPort)
 : VHuberPilotOne(ioPort),
   uDelay_(250000)
{
  comHandler_ = new PilotOneComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}

bool HuberPilotOne::SetTemperatureSetPoint(const float temperatureSetPoint)
{
  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::SetTemperatureSetPoint] -- DEBUG: Called." << std::endl;
  #endif

  if (temperatureSetPoint > PilotOneUpperTempLimit ||
      temperatureSetPoint < PilotOneLowerTempLimit ) {
    std::cerr << " [HuberPilotOne::SetTemperatureSetPoint] ** ERROR: working temp T="
	          << temperatureSetPoint << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M00"
      << std::setfill('0') << std::setw(4)
      << std::hex << std::uppercase
      << temperatureSetPoint * 100;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);
  
  float ret = ToFloat(buffer);

  if (temperatureSetPoint != ret) {
    std::cerr << " [HuberPilotOne::SetTemperatureSetPoint] ** ERROR: check failed."
              << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: T=" << temperatureSetPoint
                << " but received T=" << ret << std::endl;

    return false;
  }
  
  return true;
}

float HuberPilotOne::GetTemperatureSetPoint()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetTemperatureSetPoint] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M00****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer);
}

float HuberPilotOne::GetBathTemperature()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetBathTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M01****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer);
}

float HuberPilotOne::GetReturnTemperature()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetBathTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M02****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer);
}

float HuberPilotOne::GetPumpPressure()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetPumpPressure] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M03****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/1000.;
}

int HuberPilotOne::GetPower()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetPower] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M04****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetTemperatureControlMode(bool external)
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::SetTemperatureControlMode] -- DEBUG: Called." << std::endl;
#endif

  std::stringstream theCommand;
  theCommand << "{M13"
      << std::setfill('0') << std::setw(4)
      << external;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  bool ret = ToInteger(buffer);

  if (external != ret) {
    std::cerr << " [HuberPilotOne::SetTemperatureControlMode] ** ERROR: check failed."
              << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: " << external
                << " but received " << ret << std::endl;

    return false;
  }

return true;
}

bool HuberPilotOne::GetTemperatureControlMode()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetTemperatureControlMode] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M13****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetTemperatureControlEnabled(bool enabled)
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::SetTemperatureControlEnabled] -- DEBUG: Called." << std::endl;
#endif

  std::stringstream theCommand;
  theCommand << "{M14"
      << std::setfill('0') << std::setw(4)
      << enabled;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  bool ret = ToInteger(buffer);

  if (enabled != ret) {
    std::cerr << " [HuberPilotOne::SetTemperatureControlEnabled] ** ERROR: check failed."
              << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: " << enabled
                << " but received " << ret << std::endl;

    return false;
  }

return true;
}

bool HuberPilotOne::GetTemperatureControlEnabled()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetTemperatureControlEnabled] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M14****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetCirculatorEnabled(bool enabled)
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::SetCirculatorEnabled] -- DEBUG: Called." << std::endl;
#endif

  std::stringstream theCommand;
  theCommand << "{M16"
      << std::setfill('0') << std::setw(4)
      << enabled;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  bool ret = ToInteger(buffer);

  if (enabled != ret) {
    std::cerr << " [HuberPilotOne::SetCirculatorEnabled] ** ERROR: check failed."
              << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: " << enabled
                << " but received " << ret << std::endl;

    return false;
  }

return true;
}

bool HuberPilotOne::GetCirculatorEnabled()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetCirculatorEnabled] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M16****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

float HuberPilotOne::GetCoolingWaterInletTemperature()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetCoolingWaterInletTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M2C****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer);
}

float HuberPilotOne::GetCoolingWaterOutletTemperature()
{
#ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::GetCoolingWaterOutletTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M4C****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer);
}

void HuberPilotOne::StripBuffer(char* buffer) const
{
  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[HuberPilotOne::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
  std::cout << " > " << buffer << std::endl;
  #endif

  for (unsigned int c=0;c<strlen(buffer);++c) {
    if('\n'==buffer[c] || '\r'==buffer[c]) {
      buffer[c] = 0;
      break;
    }
  }
}

int HuberPilotOne::ToInteger(const char* buffer) const
{
  std::string temp(buffer);
  temp.erase(0, 4);
  return std::stoi(temp, 0, 16);
}

float HuberPilotOne::ToFloat(const char* buffer) const
{
  float temp = ToInteger(buffer);
  return temp/100.;
}

void HuberPilotOne::Device_Init()
{
  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPilotOne::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "CA?" );
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );
  usleep( uDelay_ );
  StripBuffer( buffer );
  std::string temp(buffer);

  if (temp.compare(0, 2, "CA")!=0) {
    std::cerr << " [HuberPilotOne::Device_Init] ** ERROR: Device communication problem."
        << std::endl;
    isCommunication_ = false;
    return;
  }

  isCommunication_ = true;
}

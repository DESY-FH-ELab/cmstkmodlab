/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

// https://www.huber-online.com/download/manuals/Handbuch_Datenkommunikation_PB_en.pdf

#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <iostream>

#include "PilotOneComHandler.h"

#include "HuberPilotOne.h"

// #define __HUBERPILOTONE_DEBUG 1

HuberPilotOne::HuberPilotOne(const ioport_t ioPort)
 : VHuberPilotOne(ioPort),
   uDelay_(100000)
{
  comHandler_ = new PilotOneComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}

bool HuberPilotOne::SetTemperatureSetPoint(const float temperatureSetPoint)
{
  #ifdef __HUBERPILOTONE_DEBUG
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
      << (int)(temperatureSetPoint * 100);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);
  
  float ret = ToFloat(buffer)/100.;

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
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTemperatureSetPoint] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M00****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/100.;
}

float HuberPilotOne::GetInternalTemperature()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetInternalTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M01****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/100.;
}

float HuberPilotOne::GetProcessTemperature()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetProcessTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M07****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/100.;
}

float HuberPilotOne::GetReturnTemperature()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetBathTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M02****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/100.;
}

float HuberPilotOne::GetPumpPressure()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetPumpPressure] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M03****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return (ToFloat(buffer)-1000.)/1000.;
}

int HuberPilotOne::GetPower()
{
#ifdef __HUBERPILOTONE_DEBUG
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

bool HuberPilotOne::SetTemperatureControlMode(bool process)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTemperatureControlMode] -- DEBUG: Called." << std::endl;
#endif

  std::stringstream theCommand;
  theCommand << "{M13"
      << std::setfill('0') << std::setw(4)
      << process;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  bool ret = ToInteger(buffer);

  if (process != ret) {
    std::cerr << " [HuberPilotOne::SetTemperatureControlMode] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: " << process
      << " but received " << ret << std::endl;

    return false;
  }

  return true;
}

bool HuberPilotOne::GetTemperatureControlMode()
{
#ifdef __HUBERPILOTONE_DEBUG
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
#ifdef __HUBERPILOTONE_DEBUG
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
#ifdef __HUBERPILOTONE_DEBUG
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
#ifdef __HUBERPILOTONE_DEBUG
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
#ifdef __HUBERPILOTONE_DEBUG
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
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetCoolingWaterInletTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M2C****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/100.;
}

float HuberPilotOne::GetCoolingWaterOutletTemperature()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetCoolingWaterOutletTemperature] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M4C****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/100.;
}

bool HuberPilotOne::SetAutoPID(bool autoPID)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetCirculatorEnabled] -- DEBUG: Called." << std::endl;
#endif

  std::stringstream theCommand;
  theCommand << "{M12"
      << std::setfill('0') << std::setw(4)
      << autoPID;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  bool ret = ToInteger(buffer);

  if (autoPID != ret) {
    std::cerr << " [HuberPilotOne::SetCirculatorEnabled] ** ERROR: check failed."
              << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: " << autoPID
                << " but received " << ret << std::endl;

    return false;
  }

  return true;
}

bool HuberPilotOne::GetAutoPID()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetAutoPID] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M12****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetKpInternal(int Kp)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetKpInternal] -- DEBUG: Called." << std::endl;
#endif

  if (Kp > 32000 ||
      Kp < 0 ) {
    std::cerr << " [HuberPilotOne::SetKpInternal] ** ERROR: Kp = "
        << Kp << " exceeds soft limits [0,32000]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M1D"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << Kp;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToInteger(buffer);

  if (Kp != ret) {
    std::cerr << " [HuberPilotOne::SetKpInternal] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Kp = " << Kp
      << " but received Kp = " << ret << std::endl;

    return false;
  }

  return true;
}

int HuberPilotOne::GetKpInternal()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetKpInternal] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M1D****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetTnInternal(float Tn)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTnInternal] -- DEBUG: Called." << std::endl;
#endif

  if (Tn > 3200. ||
      Tn < 0. ) {
    std::cerr << " [HuberPilotOne::SetTnInternal] ** ERROR: Tn = "
        << Tn << " exceeds soft limits [0.0,3200.0]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M1E"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << (int)(Tn * 10.);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToFloat(buffer)/10.;

  if (Tn != ret) {
    std::cerr << " [HuberPilotOne::SetTnInternal] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Tn = " << Tn
      << " but received Tn = " << ret << std::endl;

    return false;
  }

  return true;
}

float HuberPilotOne::GetTnInternal()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTnInternal] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M1E****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/10.;
}

bool HuberPilotOne::SetTvInternal(float Tv)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTvInternal] -- DEBUG: Called." << std::endl;
#endif

  if (Tv > 3200. ||
      Tv < 0. ) {
    std::cerr << " [HuberPilotOne::SetTvInternal] ** ERROR: Tv = "
        << Tv << " exceeds soft limits [0.0,3200.0]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M1F"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << (int)(Tv * 10);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToFloat(buffer)/10.;

  if (Tv != ret) {
    std::cerr << " [HuberPilotOne::SetTvInternal] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Tv = " << Tv
      << " but received Tv = " << ret << std::endl;

    return false;
  }

  return true;
}

float HuberPilotOne::GetTvInternal()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTvInternal] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M1F****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/10.;
}

bool HuberPilotOne::SetKpJacket(int Kp)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetKpJacket] -- DEBUG: Called." << std::endl;
#endif

  if (Kp > 32000 ||
      Kp < 0 ) {
    std::cerr << " [HuberPilotOne::SetKpJacket] ** ERROR: Kp = "
        << Kp << " exceeds soft limits [0,32000]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M20"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << Kp;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToInteger(buffer);

  if (Kp != ret) {
    std::cerr << " [HuberPilotOne::SetKpJacket] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Kp = " << Kp
      << " but received Kp = " << ret << std::endl;

    return false;
  }

  return true;
}

int HuberPilotOne::GetKpJacket()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetKpJacket] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M20****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetTnJacket(float Tn)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTnJacket] -- DEBUG: Called." << std::endl;
#endif

  if (Tn > 3200. ||
      Tn < 0. ) {
    std::cerr << " [HuberPilotOne::SetTnJacket] ** ERROR: Tn = "
        << Tn << " exceeds soft limits [0.0,3200.0]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M21"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << (int)(Tn * 10);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToFloat(buffer)/10.;

  if (Tn != ret) {
    std::cerr << " [HuberPilotOne::SetTnJacket] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Tn = " << Tn
      << " but received Tn = " << ret << std::endl;

    return false;
  }

  return true;
}

float HuberPilotOne::GetTnJacket()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTnJacket] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M21****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/10.;
}

bool HuberPilotOne::SetTvJacket(float Tv)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTvJacket] -- DEBUG: Called." << std::endl;
#endif

  if (Tv > 3200. ||
      Tv < 0. ) {
    std::cerr << " [HuberPilotOne::SetTvJacket] ** ERROR: Tv = "
        << Tv << " exceeds soft limits [0.0,3200.0]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M22"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << (int)(Tv * 10);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToFloat(buffer)/10.;

  if (Tv != ret) {
    std::cerr << " [HuberPilotOne::SetTvJacket] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Tv = " << Tv
      << " but received Tv = " << ret << std::endl;

    return false;
  }

  return true;
}

float HuberPilotOne::GetTvJacket()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTvJacket] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M22****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/10.;
}

bool HuberPilotOne::SetKpProcess(int Kp)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetKpProcess] -- DEBUG: Called." << std::endl;
#endif

  if (Kp > 32000 ||
      Kp < 0 ) {
    std::cerr << " [HuberPilotOne::SetKpProcess] ** ERROR: Kp = "
        << Kp << " exceeds soft limits [0,32000]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M23"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << Kp;

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToInteger(buffer);

  if (Kp != ret) {
    std::cerr << " [HuberPilotOne::SetKpProcess] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Kp = " << Kp
      << " but received Kp = " << ret << std::endl;

    return false;
  }

  return true;
}

int HuberPilotOne::GetKpProcess()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetKpProcess] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M23****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToInteger(buffer);
}

bool HuberPilotOne::SetTnProcess(float Tn)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTnProcess] -- DEBUG: Called." << std::endl;
#endif

  if (Tn > 3200. ||
      Tn < 0. ) {
    std::cerr << " [HuberPilotOne::SetTnProcess] ** ERROR: Tn = "
        << Tn << " exceeds soft limits [0.0,3200.0]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M24"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << (int)(Tn * 10);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToFloat(buffer)/10.;

  if (Tn != ret) {
    std::cerr << " [HuberPilotOne::SetTnProcess] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Tn = " << Tn
      << " but received Tn = " << ret << std::endl;

    return false;
  }

  return true;
}

float HuberPilotOne::GetTnProcess()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTnProcess] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M24****");
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);

  StripBuffer(buffer);

  return ToFloat(buffer)/10.;
}

bool HuberPilotOne::SetTvProcess(float Tv)
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::SetTvProcess] -- DEBUG: Called." << std::endl;
#endif

  if (Tv > 3200. ||
      Tv < 0. ) {
    std::cerr << " [HuberPilotOne::SetTvProcess] ** ERROR: Tv = "
        << Tv << " exceeds soft limits [0.0,3200.0]." << std::endl;
    std::cerr << "  > (s. HuberPilotOne class definition)" << std::endl;
    return false;
  }

  std::stringstream theCommand;
  theCommand << "{M25"
      << std::setfill('0') << std::setw(4)
  << std::hex << std::uppercase
  << (int)(Tv * 10);

  comHandler_->SendCommand(theCommand.str().c_str());
  usleep(uDelay_);

  char buffer[1000];
  memset(buffer, 0, sizeof(buffer));
  comHandler_->ReceiveString(buffer);
  usleep(uDelay_);
  StripBuffer(buffer);

  float ret = ToFloat(buffer)/10.;

  if (Tv != ret) {
    std::cerr << " [HuberPilotOne::SetTvProcess] ** ERROR: check failed."
        << std::endl;
    if (strlen(buffer)== 0)
      std::cerr << "  > Got no reply. (timeout?)" << std::endl;
    else
      std::cerr << "  > Expected: Tv = " << Tv
      << " but received Tv = " << ret << std::endl;

    return false;
  }

  return true;
}

float HuberPilotOne::GetTvProcess()
{
#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::GetTvProcess] -- DEBUG: Called." << std::endl;
#endif

  comHandler_->SendCommand("{M25****");
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
  #ifdef __HUBERPILOTONE_DEBUG
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

#ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::ToInteger] -- | " << buffer << "|" << std::endl;
  std::cout << "[HuberPilotOne::ToInteger] -- |" << temp << "|" << std::endl;  
#endif
  
  try {
    return std::stoi(temp, 0, 16);
  }
  catch (const std::invalid_argument& ia) {
    std::cerr << "[HuberPilotOne::ToInteger] -- Invalid argument: " << ia.what() << std::endl;
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "[HuberPilotOne::ToInteger] -- Undefined error: " << e.what() << std::endl;
    return 0;
  }

  return 0;
}

float HuberPilotOne::ToFloat(const char* buffer) const
{
  float temp = ToInteger(buffer);
  return temp;
}

void HuberPilotOne::Device_Init()
{
  #ifdef __HUBERPILOTONE_DEBUG
  std::cout << "[HuberPilotOne::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "{M1B****" );
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );
  usleep( uDelay_ );
  StripBuffer( buffer );
  std::string temp(buffer);
  
  if (temp.compare(0, 8, "{S1BD98B")!=0) {
    std::cerr << " [HuberPilotOne::Device_Init] ** ERROR: Device communication problem."
        << std::endl;
    isCommunication_ = false;
    return;
  }

  isCommunication_ = true;
}

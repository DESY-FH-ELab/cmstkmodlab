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

#include <unistd.h>

#include <iostream>
#include <string>

//#####################
// TODO:
// query error codes
//#####################

#include "HuberPilotOneFake.h"

HuberPilotOneFake::HuberPilotOneFake(const ioport_t ioPort)
 : VHuberPilotOne(ioPort)
{
  temperatureSetPoint_ = 20.0;
  processTemperatureControl_ = false;
  temperatureControlEnabled_ = false;
  circulatorEnabled_ = false;

  autoPID_ = false;
  KpInternal_ = 100;
  TnInternal_ = 123.4;
  TvInternal_ = 567.8;
  KpJacket_ = 100;
  TnJacket_ = 123.4;
  TvJacket_ = 567.8;
  KpProcess_ = 100;
  TnProcess_ = 123.4;
  TvProcess_ = 567.8;
}

bool HuberPilotOneFake::SetTemperatureSetPoint(const float temperatureSetPoint)
{
  if (temperatureSetPoint_ > PilotOneUpperTempLimit ||
      temperatureSetPoint_ < PilotOneLowerTempLimit ) {
    std::cerr << " [HuberPilotOneFake::SetSetTemperatureSetPointWorkingTemp] ** ERROR: working temp T="
              << temperatureSetPoint_ << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. HuberPilotOneFake class definition)" << std::endl;
    return false;
  }

  std::cout << " [HuberPilotOneFake::SetTemperatureSetPoint] -- FAKE: Setting temperature setpoint = "
            << temperatureSetPoint << "." << std::endl;

  temperatureSetPoint_ = temperatureSetPoint;

  usleep(10000);

  return true;
}

float HuberPilotOneFake::GetTemperatureSetPoint()
{
  std::cout << " [HuberPilotOneFake::GetTemperatureSetPoint] -- FAKE: Returning T = "
            << temperatureSetPoint_ << std::endl;

  usleep(10000);

  return temperatureSetPoint_;
}

float HuberPilotOneFake::GetInternalTemperature()
{
  float temp = temperatureSetPoint_;
  temp += normalDistribution_(randomGenerator_);
  return temp;
}

float HuberPilotOneFake::GetProcessTemperature()
{
  float temp = temperatureSetPoint_;
  temp += normalDistribution_(randomGenerator_);
  return temp;
}

float HuberPilotOneFake::GetReturnTemperature()
{
  if (circulatorEnabled_) {
    return GetInternalTemperature() + 2.0;
  } else {
    return GetInternalTemperature();
  }
}

float HuberPilotOneFake::GetPumpPressure()
{
  return 1.3 + normalDistribution_(randomGenerator_);
}

int HuberPilotOneFake::GetPower()
{
  return 500 + 100*normalDistribution_(randomGenerator_);
}

bool HuberPilotOneFake::SetTemperatureControlMode(bool process)
{
  processTemperatureControl_ = process;
  return true;
}

bool HuberPilotOneFake::GetTemperatureControlMode()
{
  return processTemperatureControl_;
}

bool HuberPilotOneFake::SetTemperatureControlEnabled(bool enabled)
{
  temperatureControlEnabled_ = enabled;
  return true;
}

bool HuberPilotOneFake::GetTemperatureControlEnabled()
{
  return temperatureControlEnabled_;
}

bool HuberPilotOneFake::SetCirculatorEnabled(bool enabled)
{
  circulatorEnabled_ = enabled;
  return true;
}

bool HuberPilotOneFake::GetCirculatorEnabled()
{
  return circulatorEnabled_;
}

float HuberPilotOneFake::GetCoolingWaterInletTemperature()
{
  return 13.0 + 2.*normalDistribution_(randomGenerator_);
}

float HuberPilotOneFake::GetCoolingWaterOutletTemperature()
{
  float temp = GetCoolingWaterInletTemperature();
  if (temperatureControlEnabled_) temp += 1.0;
  if (circulatorEnabled_) temp += 2.0;
  return temp;
}

bool HuberPilotOneFake::SetAutoPID(bool autoPID)
{
  autoPID_ = autoPID;
  return true;
}

bool HuberPilotOneFake::GetAutoPID()
{
  return autoPID_;
}

bool HuberPilotOneFake::SetKpInternal(int Kp)
{
  KpInternal_ = Kp;
  return true;
}

int HuberPilotOneFake::GetKpInternal()
{
  return KpInternal_;
}

bool HuberPilotOneFake::SetTnInternal(float Tn)
{
  TnInternal_ = Tn;
  return true;
}

float HuberPilotOneFake::GetTnInternal()
{
  return TnInternal_;
}

bool HuberPilotOneFake::SetTvInternal(float Tv)
{
  TvInternal_ = Tv;
  return true;
}

float HuberPilotOneFake::GetTvInternal()
{
  return TvInternal_;
}

bool HuberPilotOneFake::SetKpJacket(int Kp)
{
  KpJacket_ = Kp;
  return true;
}

int HuberPilotOneFake::GetKpJacket()
{
  return KpJacket_;
}

bool HuberPilotOneFake::SetTnJacket(float Tn)
{
  TnJacket_ = Tn;
  return true;
}

float HuberPilotOneFake::GetTnJacket()
{
  return TnJacket_;
}

bool HuberPilotOneFake::SetTvJacket(float Tv)
{
  TvJacket_ = Tv;
  return true;
}

float HuberPilotOneFake::GetTvJacket()
{
  return TvJacket_;
}

bool HuberPilotOneFake::SetKpProcess(int Kp)
{
  KpProcess_ = Kp;
  return true;
}

int HuberPilotOneFake::GetKpProcess()
{
  return KpProcess_;
}

bool HuberPilotOneFake::SetTnProcess(float Tn)
{
  TnProcess_ = Tn;
  return true;
}

float HuberPilotOneFake::GetTnProcess()
{
  return TnProcess_;
}

bool HuberPilotOneFake::SetTvProcess(float Tv)
{
  TvProcess_ = Tv;
  return true;
}

float HuberPilotOneFake::GetTvProcess()
{
  return TvProcess_;
}

void HuberPilotOneFake::Device_Init()
{

}

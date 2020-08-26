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

#ifndef _HUBERPILOTONEFAKE_H_
#define _HUBERPILOTONEFAKE_H_

#include <algorithm>
#include <random>

#include "VHuberPilotOne.h"

class HuberPilotOneFake : public VHuberPilotOne
{
 public:

  HuberPilotOneFake( ioport_t );

  bool IsCommunication() const { return true; }

  bool SetTemperatureSetPoint(const float);
  float GetTemperatureSetPoint();

  float GetBathTemperature();
  float GetReturnTemperature();

  float GetPumpPressure();
  int GetPower();

  bool SetTemperatureControlMode(bool external);
  bool GetTemperatureControlMode();

  bool SetTemperatureControlEnabled(bool enabled);
  bool GetTemperatureControlEnabled();

  bool SetCirculatorEnabled(bool enabled);
  bool GetCirculatorEnabled();

  float GetCoolingWaterInletTemperature();
  float GetCoolingWaterOutletTemperature();

 private:

  float temperatureSetPoint_;
  bool externalTemperatureControl_;
  bool temperatureControlEnabled_;
  bool circulatorEnabled_;

  std::random_device randomDevice_{};
  std::mt19937 randomGenerator_{randomDevice_()};
  std::normal_distribution<> normalDistribution_{0,0.025};

  void Device_Init();
};

#endif // _HUBERPILOTONEFAKE_H_

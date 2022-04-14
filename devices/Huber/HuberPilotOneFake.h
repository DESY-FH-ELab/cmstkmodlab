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

  float GetInternalTemperature();
  float GetProcessTemperature();
  float GetReturnTemperature();

  float GetPumpPressure();
  int GetPower();

  bool SetTemperatureControlMode(bool process);
  bool GetTemperatureControlMode();

  bool SetTemperatureControlEnabled(bool enabled);
  bool GetTemperatureControlEnabled();

  bool SetCirculatorEnabled(bool enabled);
  bool GetCirculatorEnabled();

  float GetCoolingWaterInletTemperature();
  float GetCoolingWaterOutletTemperature();

  bool SetAutoPID(bool autoPID);
  bool GetAutoPID();

  bool SetKpInternal(int Kp);
  int GetKpInternal();

  bool SetTnInternal(float Tn);
  float GetTnInternal();

  bool SetTvInternal(float Tn);
  float GetTvInternal();

  bool SetKpJacket(int Kp);
  int GetKpJacket();

  bool SetTnJacket(float Tn);
  float GetTnJacket();

  bool SetTvJacket(float Tn);
  float GetTvJacket();

  bool SetKpProcess(int Kp);
  int GetKpProcess();

  bool SetTnProcess(float Tn);
  float GetTnProcess();

  bool SetTvProcess(float Tn);
  float GetTvProcess();

 private:

  float temperatureSetPoint_;
  bool processTemperatureControl_;
  bool temperatureControlEnabled_;
  bool circulatorEnabled_;
  bool autoPID_;
  int KpInternal_;
  float TnInternal_;
  float TvInternal_;
  int KpJacket_;
  float TnJacket_;
  float TvJacket_;
  int KpProcess_;
  float TnProcess_;
  float TvProcess_;

  std::random_device randomDevice_{};
  std::mt19937 randomGenerator_{randomDevice_()};
  std::normal_distribution<> normalDistribution_{0,0.025};

  void Device_Init();
};

#endif // _HUBERPILOTONEFAKE_H_

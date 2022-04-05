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

#ifndef _HUBERPILOTONE_H_
#define _HUBERPILOTONE_H_

#include "VHuberPilotOne.h"

class PilotOneComHandler;

class HuberPilotOne : public VHuberPilotOne
{
 public:

  HuberPilotOne(ioport_t);

  bool IsCommunication() const { return isCommunication_; }

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

  const int uDelay_;
  void StripBuffer(char*) const;
  int ToInteger(const char*) const;
  float ToFloat(const char*) const;

  void Device_Init();

  PilotOneComHandler* comHandler_;
  bool isCommunication_;
};

#endif // _HUBERPILOTONE_H_

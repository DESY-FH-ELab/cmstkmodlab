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

#ifndef _VHUBERPILOTONE_H_
#define _VHUBERPILOTONE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

typedef const char* ioport_t;

class VHuberPilotOne
{
 public:

  VHuberPilotOne( ioport_t );
  virtual ~VHuberPilotOne() {}
  
  virtual bool IsCommunication( void ) const = 0;

  virtual bool SetTemperatureSetPoint(const float) = 0;
  virtual float GetTemperatureSetPoint() = 0;

  virtual float GetInternalTemperature() = 0;
  virtual float GetProcessTemperature() = 0;
  virtual float GetReturnTemperature() = 0;

  virtual float GetPumpPressure() = 0;
  virtual int GetPower() = 0;

  virtual bool SetTemperatureControlMode(bool process) = 0;
  virtual bool GetTemperatureControlMode() = 0;

  virtual bool SetTemperatureControlEnabled(bool enabled) = 0;
  virtual bool GetTemperatureControlEnabled() = 0;

  virtual bool SetCirculatorEnabled(bool enabled) = 0;
  virtual bool GetCirculatorEnabled() = 0;

  virtual float GetCoolingWaterInletTemperature() = 0;
  virtual float GetCoolingWaterOutletTemperature() = 0;

  virtual bool SetAutoPID(bool autoPID) = 0;
  virtual bool GetAutoPID() = 0;

  virtual bool SetKpInternal(int Kp) = 0;
  virtual int GetKpInternal() = 0;

  virtual bool SetTnInternal(float Tn) = 0;
  virtual float GetTnInternal() = 0;

  virtual bool SetTvInternal(float Tn) = 0;
  virtual float GetTvInternal() = 0;

  virtual bool SetKpJacket(int Kp) = 0;
  virtual int GetKpJacket() = 0;

  virtual bool SetTnJacket(float Tn) = 0;
  virtual float GetTnJacket() = 0;

  virtual bool SetTvJacket(float Tn) = 0;
  virtual float GetTvJacket() = 0;

  virtual bool SetKpProcess(int Kp) = 0;
  virtual int GetKpProcess() = 0;

  virtual bool SetTnProcess(float Tn) = 0;
  virtual float GetTnProcess() = 0;

  virtual bool SetTvProcess(float Tn) = 0;
  virtual float GetTvProcess() = 0;

  static constexpr int PilotOneLowerTempLimit = -40;
  static constexpr int PilotOneUpperTempLimit = 40;
};

#endif // _VHUBERPILOTONE_H_

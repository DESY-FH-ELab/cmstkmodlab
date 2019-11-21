/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __VROHDESCHWARZNGE103B_H
#define __VROHDESCHWARZNGE103B_H

#include <string>

typedef const char* ioport_t;

class VRohdeSchwarzNGE103B
{
public:

  enum OutputMode {
    RohdeSchwarzNGE103B_OFF = 0x00,
    RohdeSchwarzNGE103B_CV =  0x01,
    RohdeSchwarzNGE103B_CC =  0x02
  };

  VRohdeSchwarzNGE103B( const ioport_t );
  virtual ~VRohdeSchwarzNGE103B();

public:

  virtual bool DeviceAvailable() const = 0;

  virtual void GetIdentification(std::string& id) const = 0;

  virtual void SelectChannel(unsigned int ch) = 0;
  virtual unsigned int SelectedChannel() const = 0;

  constexpr static float MaxVoltage = 32.0;
  virtual void SetVoltage(float v) = 0;
  virtual float GetVoltage() const = 0;
  virtual float MeasureVoltage() const = 0;

  constexpr static float MaxCurrent = 3.0;
  virtual void SetCurrent(float i) = 0;
  virtual float GetCurrent() const = 0;
  virtual float MeasureCurrent() const = 0;

  constexpr static float MaxPower = 33.6;
  virtual float MeasurePower() const = 0;

  virtual void SetOutputState(bool s) = 0;
  virtual bool GetOutputState() const = 0;

  virtual unsigned int GetOutputMode() const = 0;
};

#endif // __VROHDESCHWARZNGE103B_H

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

#ifndef __HAMEG8143_H
#define __HAMEG8143_H

#include "VHameg8143.h"

class Hameg8143Fake : public VHameg8143
{
 public:

  Hameg8143Fake( const ioport_t );
  ~Hameg8143Fake();

 public:

  bool DeviceAvailable() const { return true; }

  unsigned int GetStatus() const;
  bool SetRemoteMode(bool remote = true) const;
  bool SetMixedMode(bool mixed = true) const;
  bool SetVoltage(int channel, float voltage) const;
  bool SetTrackingVoltage(float voltage) const;
  float GetSetVoltage(int channel) const;
  float GetVoltage(int channel) const;
  bool SetCurrent(int channel, float current) const;
  bool SetTrackingCurrent(float current) const;
  float GetSetCurrent(int channel) const;
  float GetCurrent(int channel) const;

  bool SwitchOutputOn() const;
  bool SwitchOutputOff() const;

  private:

  static constexpr float resistance_ = 100.;
  
  mutable unsigned int statusBits_;
  mutable float voltage_[2];
  mutable float current_[2];
};

#endif

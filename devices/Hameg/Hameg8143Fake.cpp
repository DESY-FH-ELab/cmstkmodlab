/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "Hameg8143Fake.h"

Hameg8143Fake::Hameg8143Fake( const ioport_t ioPort )
  :VHameg8143(ioPort)
{
  statusBits_ = hmOP0 | hmCV1 | hmCV2 | hmRM0;
  voltage_[0] = 0.0;
  voltage_[1] = 0.0;
  current_[0] = 0.0;
  current_[1] = 0.0;
}

Hameg8143Fake::~Hameg8143Fake()
{

}

unsigned int Hameg8143Fake::GetStatus() const
{
    return statusBits_;
}

bool Hameg8143Fake::SetRemoteMode(bool remote) const
{
  if (statusBits_&hmRM0) statusBits_ -= hmRM0;
  if (statusBits_&hmRM1) statusBits_ -= hmRM1;
  if (remote) {
      statusBits_ |= hmRM1;
  } else {
      statusBits_ |= hmRM0;
  }

  return true;
}

bool Hameg8143Fake::SetMixedMode(bool mixed) const
{
  SetRemoteMode(true);
  return true;
}

bool Hameg8143Fake::SetVoltage(int channel, float voltage) const
{
  if (channel==1) {
    if (statusBits_&hmCC1) statusBits_ -= hmCC1;
    statusBits_ |= hmCV1;
  } else if (channel==2) {
    if (statusBits_&hmCC2) statusBits_ -= hmCC2;
    statusBits_ |= hmCV2;
  }
  voltage_[channel-1] = voltage;
  return true;
}
  
float Hameg8143Fake::GetSetVoltage(int channel) const
{
  return voltage_[channel-1];
}
  
float Hameg8143Fake::GetVoltage(int channel) const
{
  if ((channel==1 && statusBits_&hmCC1) ||
      (channel==2 && statusBits_&hmCC2)) return resistance_ * current_[channel-1];
  return voltage_[channel-1];
}

bool Hameg8143Fake::SetCurrent(int channel, float current) const
{
  if (channel==1) {
    if (statusBits_&hmCV1) statusBits_ -= hmCV1;
    statusBits_ |= hmCC1;
  } else if (channel==2) {
    if (statusBits_&hmCV2) statusBits_ -= hmCV2;
    statusBits_ |= hmCC2;
  }
  current_[channel-1] = current;
  return true;
}

float Hameg8143Fake::GetSetCurrent(int channel) const
{
  return current_[channel-1];
}

float Hameg8143Fake::GetCurrent(int channel) const
{
  if ((channel==1 && statusBits_&hmCV1) ||
      (channel==2 && statusBits_&hmCV2)) return voltage_[channel-1] / resistance_;
  return current_[channel-1];
}
 
bool Hameg8143Fake::SetTrackingVoltage(float /*voltage*/) const
{
  return true;
}

bool Hameg8143Fake::SetTrackingCurrent(float /*current*/) const
{
  return true;
}

bool Hameg8143Fake::SwitchOutputOn() const
{
  if (statusBits_&hmOP0) statusBits_ -= hmOP0;
  statusBits_ |= hmOP1;
  return true;
}

bool Hameg8143Fake::SwitchOutputOff() const
{
  if (statusBits_&hmOP1) statusBits_ -= hmOP1;
  statusBits_ |= hmOP0;
  return true;
}

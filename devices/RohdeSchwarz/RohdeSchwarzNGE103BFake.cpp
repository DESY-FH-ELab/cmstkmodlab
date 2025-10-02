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

#include "RohdeSchwarzNGE103BFake.h"

RohdeSchwarzNGE103BFake::RohdeSchwarzNGE103BFake( const ioport_t ioPort )
  :VRohdeSchwarzNGE103B(ioPort)
{
  selectedChannel_ = 1;
  voltage_ = { 0., 0., 0. };
  current_ = { MinCurrent, MinCurrent, MinCurrent };
  outputState_ = { false, false, false };
  easyRampDuration_ = { 0.01, 0.01, 0.01 };
  easyRampState_ = { false, false, false };
}

RohdeSchwarzNGE103BFake::~RohdeSchwarzNGE103BFake()
{

}

void RohdeSchwarzNGE103BFake::GetIdentification(std::string& id) const
{
  id = "Rohde&Schwarz,NGE103B,5601.3800k03/100897,1.52";
}

void RohdeSchwarzNGE103BFake::SelectChannel(unsigned int ch)
{
  if (ch>=1 && ch<=3) selectedChannel_ = ch-1;
}

unsigned int RohdeSchwarzNGE103BFake::SelectedChannel() const
{
  return selectedChannel_+1;
}

void RohdeSchwarzNGE103BFake::SetVoltage(float v)
{
  if (v>=MinVoltage && v<=MaxVoltage)
    voltage_[selectedChannel_] = v;
}

float RohdeSchwarzNGE103BFake::GetVoltage() const
{
  return voltage_[selectedChannel_];
}

float RohdeSchwarzNGE103BFake::MeasureVoltage() const
{
  if (outputState_[selectedChannel_]) {

    if (GetOutputMode()==RohdeSchwarzNGE103B_CC) {
      float voltage = MaxPower / current_[selectedChannel_];
      if (voltage>voltage_[selectedChannel_]) voltage = voltage_[selectedChannel_];
      return voltage;
    }

    return voltage_[selectedChannel_];
  }

  return 0;
}

void RohdeSchwarzNGE103BFake::SetEasyRampDuration(float d)
{
  if (d>=MinEasyRampDuration && d<=MaxEasyRampDuration) {
    float temp = 0.01 * int(100.*d);
    easyRampDuration_[selectedChannel_] = temp;
  }
}

float RohdeSchwarzNGE103BFake::GetEasyRampDuration() const
{
  return easyRampDuration_[selectedChannel_];
}

void RohdeSchwarzNGE103BFake::SetEasyRampState(bool s)
{
  easyRampState_[selectedChannel_] = s;
}

bool RohdeSchwarzNGE103BFake::GetEasyRampState() const
{
  return easyRampState_[selectedChannel_];
}

void RohdeSchwarzNGE103BFake::SetCurrent(float i)
{
  if (i>=MinCurrent && i<=MaxCurrent)
    current_[selectedChannel_] = i;
}

float RohdeSchwarzNGE103BFake::GetCurrent() const
{
  return current_[selectedChannel_];
}

float RohdeSchwarzNGE103BFake::MeasureCurrent() const
{
  if (outputState_[selectedChannel_]) {

    if (GetOutputMode()==RohdeSchwarzNGE103B_CV) {
      float current = MaxPower / voltage_[selectedChannel_];
      if (current>current_[selectedChannel_])
        current = current_[selectedChannel_];
      return current;
    }

    return current_[selectedChannel_];
  }

  return 0;
}

float RohdeSchwarzNGE103BFake::MeasurePower() const
{
  return voltage_[selectedChannel_] * current_[selectedChannel_];
}

void RohdeSchwarzNGE103BFake::SetOutputState(bool s)
{
  outputState_[selectedChannel_] = s;
}

bool RohdeSchwarzNGE103BFake::GetOutputState() const
{
  return outputState_[selectedChannel_];
}

unsigned int RohdeSchwarzNGE103BFake::GetOutputMode() const
{
  if (!outputState_[selectedChannel_]) return RohdeSchwarzNGE103B_OFF;

  if (voltage_[selectedChannel_]<MaxVoltage && current_[selectedChannel_]==MaxCurrent)
    return RohdeSchwarzNGE103B_CV;

  if (current_[selectedChannel_]<MaxCurrent && voltage_[selectedChannel_]==MaxVoltage)
    return RohdeSchwarzNGE103B_CC;

  return RohdeSchwarzNGE103B_CV;
}

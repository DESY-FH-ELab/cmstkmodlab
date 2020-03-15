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

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>

#include "LeyboldGraphixOneFake.h"

LeyboldGraphixOneFake::LeyboldGraphixOneFake( const ioport_t ioPort )
  :VLeyboldGraphixOne(ioPort)
{
  sensorDetectionMode_ = SensorDetectionAuto;

  sensorType_ = "TTR?";

  pressure_ = 110.0;

  displayUnit_ = DisplayUnit_mbar;

  for (int i=0;i<6;++i) {
    setPointChannel_[i] = SetPointChannelOff;
    setPointOnPressure_[i] = 1.e3;
    setPointOffPressure_[i] = 1.e3;
  }
}

LeyboldGraphixOneFake::~LeyboldGraphixOneFake()
{

}

std::string LeyboldGraphixOneFake::GetVersion() const
{
  return std::string("HW:1.01 SW:1.10.00");
}

int LeyboldGraphixOneFake::GetSerialNumber() const
{
  return 453;
}

std::string LeyboldGraphixOneFake::GetItemNumber() const
{
  return std::string("230682V01");
}

int LeyboldGraphixOneFake::GetNumberOfChannels() const
{
  return 3;
}

VLeyboldGraphix::SensorDetectionMode LeyboldGraphixOneFake::GetSensorDetectionMode() const
{
  return sensorDetectionMode_;
}

void LeyboldGraphixOneFake::SetSensorDetectionMode(VLeyboldGraphix::SensorDetectionMode mode)
{
  sensorDetectionMode_ = mode;
}

std::string LeyboldGraphixOneFake::GetSensorTypeName() const
{
  return sensorType_;
}

void LeyboldGraphixOneFake::SetSensorTypeName(std::string type)
{
  sensorType_ = type;
}

std::string LeyboldGraphixOneFake::GetSensorName() const
{
  return sensorName_;
}

void LeyboldGraphixOneFake::SetSensorName(const std::string& name)
{
  sensorName_ = name;
}

VLeyboldGraphix::SensorStatus LeyboldGraphixOneFake::GetSensorStatus() const
{
  return SensorStatus_ok;
}

double LeyboldGraphixOneFake::GetPressure() const
{
  return pressure_ + rand() % 10;
}

LeyboldGraphixOneFake::DisplayUnit LeyboldGraphixOneFake::GetDisplayUnit() const
{
  return displayUnit_;
}

void LeyboldGraphixOneFake::SetDisplayUnit(VLeyboldGraphix::DisplayUnit unit)
{
  displayUnit_ = unit;
}

VLeyboldGraphix::SetPointChannel LeyboldGraphixOneFake::GetSetPointChannelAssignment(int sp) const
{
  if (sp<1 || sp>6) return SetPointChannelOff;
  return setPointChannel_[sp-1];
}

void LeyboldGraphixOneFake::SetSetPointChannelAssignment(int sp, VLeyboldGraphix::SetPointChannel channel)
{
  if (sp<1 || sp>6) return;
  setPointChannel_[sp-1] = channel;
}

double LeyboldGraphixOneFake::GetSetPointOnPressure(int sp) const
{
  if (sp<1 || sp>6) return -1;
  return setPointOnPressure_[sp-1];
}

void LeyboldGraphixOneFake::SetSetPointOnPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;
  setPointOnPressure_[sp-1] = pressure;
}

double LeyboldGraphixOneFake::GetSetPointOffPressure(int sp) const
{
  if (sp<1 || sp>6) return -1;
  return setPointOffPressure_[sp-1];
}

void LeyboldGraphixOneFake::SetSetPointOffPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;
  setPointOffPressure_[sp-1] = pressure;
}

bool LeyboldGraphixOneFake::GetSetPointStatus(int sp) const
{
  if (sp<1 || sp>6) return false;
  return false;
}

std::string LeyboldGraphixOneFake::GetDate() const
{
  std::time_t t = std::time(NULL);
  char buffer[100];

  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&t));

  return std::string(buffer);
}

void LeyboldGraphixOneFake::SetDate(const std::string&)
{

}

std::string LeyboldGraphixOneFake::GetTime() const
{
  std::time_t t = std::time(NULL);
  char buffer[100];

  std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&t));

  return std::string(buffer);
}

void LeyboldGraphixOneFake::SetTime(const std::string&)
{

}

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
#include <ctime>
#include <iostream>
#include <cstdlib>

#include "LeyboldGraphixThreeFake.h"

LeyboldGraphixThreeFake::LeyboldGraphixThreeFake( const ioport_t ioPort )
  :VLeyboldGraphixThree(ioPort)
{
  sensorDetectionMode_[0] = SensorDetectionAuto;
  sensorDetectionMode_[1] = SensorDetectionAuto;
  sensorDetectionMode_[2] = SensorDetectionAuto;

  sensorType_[0] = "TTR?";
  sensorType_[1] = "TTR?";
  sensorType_[2] = "TTR?";

  pressure_[0] = 110.0;
  pressure_[1] = 120.0;
  pressure_[2] = 130.0;

  displayUnit_ = DisplayUnit_mbar;

  for (int i=0;i<6;++i) {
    setPointChannel_[i] = SetPointChannelOff;
    setPointOnPressure_[i] = 1.e3;
    setPointOffPressure_[i] = 1.e3;
  }
}

LeyboldGraphixThreeFake::~LeyboldGraphixThreeFake()
{

}

std::string LeyboldGraphixThreeFake::GetVersion() const
{
  return std::string("HW:1.01 SW:1.10.00");
}

int LeyboldGraphixThreeFake::GetSerialNumber() const
{
  return 453;
}

std::string LeyboldGraphixThreeFake::GetItemNumber() const
{
  return std::string("230682V01");
}

int LeyboldGraphixThreeFake::GetNumberOfChannels() const
{
  return 3;
}

VLeyboldGraphix::SensorDetectionMode LeyboldGraphixThreeFake::GetSensorDetectionMode(int sensor) const
{
  if (sensor<1 || sensor>3) return SensorDetectionAuto;

  return sensorDetectionMode_[sensor-1];
}

void LeyboldGraphixThreeFake::SetSensorDetectionMode(int sensor, VLeyboldGraphix::SensorDetectionMode mode)
{
  if (sensor<1 || sensor>3) return;

  sensorDetectionMode_[sensor-1] = mode;
}

std::string LeyboldGraphixThreeFake::GetSensorTypeName(int sensor) const
{
  if (sensor<1 || sensor>3) return std::string("out of range");

  return sensorType_[sensor-1];
}

void LeyboldGraphixThreeFake::SetSensorTypeName(int sensor, std::string type)
{
  if (sensor<1 || sensor>3) return;

  sensorType_[sensor-1] = type;
}

std::string LeyboldGraphixThreeFake::GetSensorName(int sensor) const
{
  if (sensor<1 || sensor>3) return std::string("out of range");

  return sensorName_[sensor-1];
}

void LeyboldGraphixThreeFake::SetSensorName(int sensor, const std::string& name)
{
  if (sensor<1 || sensor>3) return;

  sensorName_[sensor-1] = name;
}

VLeyboldGraphix::SensorStatus LeyboldGraphixThreeFake::GetSensorStatus(int sensor) const
{
  if (sensor<1 || sensor>3) return SensorStatus_nosen;

  return SensorStatus_ok;
}

double LeyboldGraphixThreeFake::GetPressure(int sensor) const
{
  if (sensor<1 || sensor>3) return -1;
  return pressure_[sensor-1] + rand() % 10;
}

VLeyboldGraphix::DisplayUnit LeyboldGraphixThreeFake::GetDisplayUnit() const
{
  return displayUnit_;
}

void LeyboldGraphixThreeFake::SetDisplayUnit(VLeyboldGraphix::DisplayUnit unit)
{
  displayUnit_ = unit;
}

VLeyboldGraphix::SetPointChannel LeyboldGraphixThreeFake::GetSetPointChannelAssignment(int sp) const
{
  if (sp<1 || sp>6) return SetPointChannelOff;
  return setPointChannel_[sp-1];
}

void LeyboldGraphixThreeFake::SetSetPointChannelAssignment(int sp, VLeyboldGraphix::SetPointChannel channel)
{
  if (sp<1 || sp>6) return;
  setPointChannel_[sp-1] = channel;
}

double LeyboldGraphixThreeFake::GetSetPointOnPressure(int sp) const
{
  if (sp<1 || sp>6) return -1;
  return setPointOnPressure_[sp-1];
}

void LeyboldGraphixThreeFake::SetSetPointOnPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;
  setPointOnPressure_[sp-1] = pressure;
}

double LeyboldGraphixThreeFake::GetSetPointOffPressure(int sp) const
{
  if (sp<1 || sp>6) return -1;
  return setPointOffPressure_[sp-1];
}

void LeyboldGraphixThreeFake::SetSetPointOffPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;
  setPointOffPressure_[sp-1] = pressure;
}

bool LeyboldGraphixThreeFake::GetSetPointStatus(int sp) const
{
  if (sp<1 || sp>6) return false;
  return false;
}

std::string LeyboldGraphixThreeFake::GetDate() const
{
  std::time_t t = std::time(NULL);
  char buffer[100];

  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&t));

  return std::string(buffer);
}

void LeyboldGraphixThreeFake::SetDate(const std::string&)
{

}

std::string LeyboldGraphixThreeFake::GetTime() const
{
  std::time_t t = std::time(NULL);
  char buffer[100];

  std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&t));

  return std::string(buffer);
}

void LeyboldGraphixThreeFake::SetTime(const std::string&)
{

}

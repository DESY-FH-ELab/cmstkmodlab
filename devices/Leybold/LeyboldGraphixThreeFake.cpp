#include <cmath>
#include <iostream>

#include "LeyboldGraphixThreeFake.h"

LeyboldGraphixThreeFake::LeyboldGraphixThreeFake( const ioport_t ioPort )
  :VLeyboldGraphixThree(ioPort)
{
  pressure_[0] = 100.0;
  pressure_[1] = 100.1;
  pressure_[2] = 100.2;

  displayUnit_ = DisplayUnit_mbar;
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

int LeyboldGraphixThreeFake::GetItemNumber() const
{
  return 230682;
}

int LeyboldGraphixThreeFake::GetNumberOfChannels() const
{
  return 3;
}

std::string LeyboldGraphixThreeFake::GetSensorType(int sensor) const
{
  if (sensor<1 || sensor>3) return std::string("out of range");

  return "TTR91";
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

LeyboldGraphixThreeFake::SensorStatus LeyboldGraphixThreeFake::GetSensorStatus(int sensor) const
{
  if (sensor<1 || sensor>3) return SensorStatus_nosen;

  return SensorStatus_ok;
}

double LeyboldGraphixThreeFake::GetPressure(int sensor) const
{
  if (sensor<1 || sensor>3) return -1;
  return pressure_[sensor-1];
}

LeyboldGraphixThreeFake::DisplayUnit LeyboldGraphixThreeFake::GetDisplayUnit() const
{
  return displayUnit_;
}

void LeyboldGraphixThreeFake::SetDisplayUnit(LeyboldGraphixThreeFake::DisplayUnit unit)
{
  displayUnit_ = unit;
}

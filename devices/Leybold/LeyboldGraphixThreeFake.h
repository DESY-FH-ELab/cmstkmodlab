#ifndef _LEYBOLDGRAPHIXTHREEFAKE_H_
#define _LEYBOLDGRAPHIXTHREEFAKE_H_

#include "VLeyboldGraphixThree.h"

class LeyboldGraphixThreeFake : public VLeyboldGraphixThree
{
 public:

  LeyboldGraphixThreeFake( const ioport_t );
  ~LeyboldGraphixThreeFake();

  bool DeviceAvailable() const { return true; }

  std::string GetVersion() const;
  int GetSerialNumber() const;
  int GetItemNumber() const;

  int GetNumberOfChannels() const;

  std::string GetSensorType(int sensor) const;

  std::string GetSensorName(int sensor) const;
  void SetSensorName(int sensor, const std::string& name);

  SensorStatus GetSensorStatus(int sensor) const;

  double GetPressure(int sensor) const;

  DisplayUnit GetDisplayUnit() const;
  void SetDisplayUnit(DisplayUnit);

 protected:

  std::string sensorName_[3];
  double pressure_[3];
  DisplayUnit displayUnit_;
};

#endif // _LEYBOLDGRAPHIXTHREEFAKE_H_

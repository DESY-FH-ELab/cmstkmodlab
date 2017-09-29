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
  std::string GetItemNumber() const;

  int GetNumberOfChannels() const;

  SensorDetectionMode GetSensorDetectionMode(int sensor) const;
  void SetSensorDetectionMode(int sensor, SensorDetectionMode mode);

  std::string GetSensorTypeName(int sensor) const;
  void SetSensorTypeName(int sensor, std::string type);

  std::string GetSensorName(int sensor) const;
  void SetSensorName(int sensor, const std::string& name);

  SensorStatus GetSensorStatus(int sensor) const;

  double GetPressure(int sensor) const;

  DisplayUnit GetDisplayUnit() const;
  void SetDisplayUnit(DisplayUnit);

  SetPointChannel GetSetPointChannelAssignment(int sp) const;
  void SetSetPointChannelAssignment(int sp, SetPointChannel channel);

  double GetSetPointOnPressure(int sp) const;
  void SetSetPointOnPressure(int sp, double pressure);

  double GetSetPointOffPressure(int sp) const;
  void SetSetPointOffPressure(int sp, double pressure);

  bool GetSetPointStatus(int sp) const;

  std::string GetDate() const;
  void SetDate(const std::string& date);

  std::string GetTime() const;
  void SetTime(const std::string& date);

 protected:

  SensorDetectionMode sensorDetectionMode_[3];
  std::string sensorType_[3];
  std::string sensorName_[3];
  double pressure_[3];
  DisplayUnit displayUnit_;
  SetPointChannel setPointChannel_[6];
  double setPointOnPressure_[6];
  double setPointOffPressure_[6];
};

#endif // _LEYBOLDGRAPHIXTHREEFAKE_H_

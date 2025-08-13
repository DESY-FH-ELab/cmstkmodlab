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

#ifndef _LEYBOLDGRAPHIXONEFAKE_H_
#define _LEYBOLDGRAPHIXONEFAKE_H_

#include "VLeyboldGraphixOne.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Leybold
 *  @{
 */

class LeyboldGraphixOneFake : public VLeyboldGraphixOne
{
 public:

  LeyboldGraphixOneFake( const ioport_t );
  ~LeyboldGraphixOneFake();

  bool DeviceAvailable() const { return true; }

  std::string GetVersion() const;
  int GetSerialNumber() const;
  std::string GetItemNumber() const;

  int GetNumberOfChannels() const;

  SensorDetectionMode GetSensorDetectionMode() const;
  void SetSensorDetectionMode(SensorDetectionMode mode);

  std::string GetSensorTypeName() const;
  void SetSensorTypeName(std::string type);

  std::string GetSensorName() const;
  void SetSensorName(const std::string& name);

  SensorStatus GetSensorStatus() const;

  double GetPressure() const;

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

  SensorDetectionMode sensorDetectionMode_;
  std::string sensorType_;
  std::string sensorName_;
  double pressure_;
  DisplayUnit displayUnit_;
  SetPointChannel setPointChannel_[6];
  double setPointOnPressure_[6];
  double setPointOffPressure_[6];
};
 
/** @} */

/** @} */

#endif // _LEYBOLDGRAPHIXONEFAKE_H_

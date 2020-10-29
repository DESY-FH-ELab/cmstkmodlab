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

#ifndef _LEYBOLDGRAPHIXONE_H_
#define _LEYBOLDGRAPHIXONE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>
#include <array>

#include "VLeyboldGraphixOne.h"
#include "LeyboldComHandler.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Leybold
 *  @{
 */

class LeyboldGraphixOne : public VLeyboldGraphixOne
{
 public:

  LeyboldGraphixOne( const ioport_t );
  ~LeyboldGraphixOne();

  bool DeviceAvailable() const;

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
  void SetTime(const std::string& time);

 protected:

  void SendCommand(std::string& command) const;
  bool ReceiveData(std::string& buffer) const;
  void StripBuffer(char*) const;
  void DeviceInit();

  static constexpr const char Separator = 0x3B;
  static constexpr const char EOT       = 0x04;
  static constexpr const char SI        = 0x0F;
  static constexpr const char SO        = 0x0E;
  static constexpr const char ACK       = 0x06;
  static constexpr const char NACK      = 0x15;

  char GetChecksum(const std::string& buffer) const;

  LeyboldComHandler* comHandler_;
  bool isDeviceAvailable_;

  mutable SensorStatus sensorStatus_;
};
 
/** @} */

/** @} */

#endif // _LEYBOLDGRAPHIXONE_H_

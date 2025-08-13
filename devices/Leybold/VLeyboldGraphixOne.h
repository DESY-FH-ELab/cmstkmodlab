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

#ifndef _VLEYBOLDGRAPHIXONE_H_
#define _VLEYBOLDGRAPHIXONE_H_

#include "VLeyboldGraphix.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Leybold
 *  @{
 */

class VLeyboldGraphixOne : public VLeyboldGraphix
{
 public:

  VLeyboldGraphixOne( const ioport_t );
  virtual ~VLeyboldGraphixOne();

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetVersion() const = 0;
  virtual int GetSerialNumber() const = 0;
  virtual std::string GetItemNumber() const = 0;

  virtual int GetNumberOfChannels() const = 0;

  virtual SensorDetectionMode GetSensorDetectionMode() const = 0;
  virtual void SetSensorDetectionMode(SensorDetectionMode mode) = 0;

  virtual std::string GetSensorTypeName() const = 0;
  SensorType GetSensorType() const;
  virtual void SetSensorTypeName(std::string type) = 0;
  void SetSensorType(SensorType type);

  virtual std::string GetSensorName() const = 0;
  virtual void SetSensorName(const std::string& name) = 0;

  virtual SensorStatus GetSensorStatus() const = 0;

  virtual double GetPressure() const = 0;

  virtual DisplayUnit GetDisplayUnit() const = 0;
  virtual void SetDisplayUnit(DisplayUnit unit) = 0;

  virtual SetPointChannel GetSetPointChannelAssignment(int sp) const = 0;
  virtual void SetSetPointChannelAssignment(int sp, SetPointChannel channel) = 0;

  virtual double GetSetPointOnPressure(int sp) const = 0;
  virtual void SetSetPointOnPressure(int sp, double pressure) = 0;

  virtual double GetSetPointOffPressure(int sp) const = 0;
  virtual void SetSetPointOffPressure(int sp, double pressure) = 0;

  virtual bool GetSetPointStatus(int sp) const = 0;

  virtual std::string GetDate() const = 0;
  virtual void SetDate(const std::string& date) = 0;

  virtual std::string GetTime() const = 0;
  virtual void SetTime(const std::string& date) = 0;
  
 protected:

};
 
/** @} */

/** @} */

#endif // _VLEYBOLDGRAPHIXONE_H_

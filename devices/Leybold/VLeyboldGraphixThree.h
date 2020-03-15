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

#ifndef _VLEYBOLDGRAPHIXTHREE_H_
#define _VLEYBOLDGRAPHIXTHREE_H_

#include "VLeyboldGraphix.h"

class VLeyboldGraphixThree : public VLeyboldGraphix
{
 public:

  VLeyboldGraphixThree( const ioport_t );
  virtual ~VLeyboldGraphixThree();

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetVersion() const = 0;
  virtual int GetSerialNumber() const = 0;
  virtual std::string GetItemNumber() const = 0;

  virtual int GetNumberOfChannels() const = 0;

  virtual SensorDetectionMode GetSensorDetectionMode(int sensor) const = 0;
  virtual void SetSensorDetectionMode(int sensor, SensorDetectionMode mode) = 0;

  virtual std::string GetSensorTypeName(int sensor) const = 0;
  SensorType GetSensorType(int sensor) const;
  virtual void SetSensorTypeName(int sensor, std::string type) = 0;
  void SetSensorType(int sensor, SensorType type);

  virtual std::string GetSensorName(int sensor) const = 0;
  virtual void SetSensorName(int sensor, const std::string& name) = 0;

  virtual SensorStatus GetSensorStatus(int sensor) const = 0;

  virtual double GetPressure(int sensor) const = 0;

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

#endif // _VLEYBOLDGRAPHIXTHREE_H_

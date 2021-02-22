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

#ifndef _VLEYBOLDGRAPHIX_H_
#define _VLEYBOLDGRAPHIX_H_

#include <string>
#include <utility>

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Leybold
 *  @{
 */

typedef const char* ioport_t;

class VLeyboldGraphix
{
 public:

  enum SensorDetectionMode {
    SensorDetectionAuto    = 0,
    SensorDetectionManual  = 1
  };

  enum SensorType {
    SensorType_NOSEN     = 0,
    SensorType_TTRx      = 1000,
    SensorType_TTR90     = 1900,
    SensorType_TTR91     = 1910,
    SensorType_TTR91N    = 1911
  };

  enum SensorStatus {
    SensorStatus_nosen   = 0,
    SensorStatus_ok      = 1,
    SensorStatus_range   = 2,
    SensorStatus_soff    = 3,
    SensorStatus_ErrorH  = 4,
    SensorStatus_ErrorL  = 5,
    SensorStatus_ErrorS  = 6,
    SensorStatus_unknown
  };

  enum DisplayUnit {
    DisplayUnit_mbar   = 0,
    DisplayUnit_Torr   = 1,
    DisplayUnit_Pa     = 2,
    DisplayUnit_psi    = 3,
    DisplayUnit_Micron = 4,
    DisplayUnit_unknown
  };

  enum SetPointChannel {
    SetPointChannelOff   = 0,
    SetPointChannel1     = 1,
    SetPointChannel2     = 2,
    SetPointChannel3     = 3
  };

  VLeyboldGraphix( const ioport_t );
  virtual ~VLeyboldGraphix();

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetVersion() const = 0;
  virtual int GetSerialNumber() const = 0;
  virtual std::string GetItemNumber() const = 0;

  virtual int GetNumberOfChannels() const = 0;

  std::string GetSensorStatusText(SensorStatus status) const;

  virtual DisplayUnit GetDisplayUnit() const = 0;
  virtual void SetDisplayUnit(DisplayUnit unit) = 0;
  std::string GetDisplayUnitName(DisplayUnit unit) const;

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

  SensorType GetSensorTypeByTypeName(const std::string& name) const;
  SensorStatus GetSensorStatusByStatusText(const std::string& status) const;
  DisplayUnit GetDisplayUnitByUnitText(const std::string& unit) const;
};
 
/** @} */

/** @} */

#endif // _VLEYBOLDGRAPHIX_H_

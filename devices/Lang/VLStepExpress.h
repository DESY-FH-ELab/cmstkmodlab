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

#ifndef __VLSTEPEXPRESS_H
#define __VLSTEPEXPRESS_H

#include <string>
#include <vector>

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Lang Lang
 *  @{
 */

class VLStepExpress
{
 public:

  enum Axis {
    X = 0,
    Y = 1,
    Z = 2,
    A = 3
  };

  enum Dimension {
    MICROSTEPS = 0,
    MICROMETER = 1,
    MILLIMETER = 2,
    DEGREE     = 3,
    ROTATIONS  = 4
  };

  enum AxisStatus {
    AXISSTANDSANDREADY         = 0x00,
    AXISMOVING                 = 0x01,
    AXISJOYSTICK               = 0x02,
    AXISINCONTROL              = 0x03,
    AXISLIMITSWITCHTRIPPED     = 0x04,
    AXISACKAFTERCALIBRATION    = 0x05,
    AXISERRACKAFTERCALIBRATION = 0x06,
    AXISACKAFTERTBLSTROKEMSR   = 0x07,
    AXISINSETUP                = 0x08,
    AXISTIMEOUT                = 0x09,
    AXISERROR                  = 0x0a,
    AXISDISABLED               = 0x0b,
    AXISSTATEUNKNOWN           = 0xff
  };

  VLStepExpress(const std::string&);

  virtual ~VLStepExpress();

  virtual std::string ioPort() = 0;

  virtual bool DeviceAvailable() const = 0;

  virtual void GetAutoStatus(int & value) = 0;
  virtual void SetAutoStatus(int value) = 0;

  virtual void GetAxisStatus(std::vector<int> & values) = 0;

  virtual void GetAxisEnabled(std::vector<int> & values) = 0;
  virtual void GetAxisEnabled(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetAxisEnabled(const std::vector<int> & values) = 0;
  virtual void SetAxisEnabled(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetAxisDirection(std::vector<int> & values) = 0;
  virtual void GetAxisDirection(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetAxisDirection(const std::vector<int> & values) = 0;
  virtual void SetAxisDirection(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetDimension(std::vector<int> & values) = 0;
  virtual void GetDimension(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetDimension(const std::vector<int> & values) = 0;
  virtual void SetDimension(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetPowerAmplifierStatus(std::vector<int> & values) = 0;
  virtual void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetPowerAmplifierStatus(const std::vector<int> & values) = 0;
  virtual void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetAccelerationJerk(std::vector<double> & values) = 0;
  virtual void GetAccelerationJerk(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetAccelerationJerk(const std::vector<double> & values) = 0;
  virtual void SetAccelerationJerk(VLStepExpress::Axis axis, double value) = 0;

  virtual void GetDecelerationJerk(std::vector<double> & values) = 0;
  virtual void GetDecelerationJerk(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetDecelerationJerk(const std::vector<double> & values) = 0;
  virtual void SetDecelerationJerk(VLStepExpress::Axis axis, double value) = 0;

  virtual void GetAcceleration(std::vector<double> & values) = 0;
  virtual void GetAcceleration(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetAcceleration(const std::vector<double> & values) = 0;
  virtual void SetAcceleration(VLStepExpress::Axis axis, double value) = 0;

  virtual void GetDeceleration(std::vector<double> & values) = 0;
  virtual void GetDeceleration(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetDeceleration(const std::vector<double> & values) = 0;
  virtual void SetDeceleration(VLStepExpress::Axis axis, double value) = 0;

  virtual void GetVelocity(std::vector<double> & values) = 0;
  virtual void GetVelocity(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetVelocity(const std::vector<double> & values) = 0;
  virtual void SetVelocity(VLStepExpress::Axis axis, double value) = 0;

  virtual void GetPosition(std::vector<double> & values) = 0;
  virtual void GetPosition(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetPosition(const std::vector<double> & values) = 0;
  virtual void SetPosition(VLStepExpress::Axis axis, double value) = 0;

  virtual void MoveAbsolute(const std::vector<double> & values) = 0;
  virtual void MoveAbsolute(double x, double y, double z, double a) = 0;
  virtual void MoveAbsolute(VLStepExpress::Axis axis, double value) = 0;

  virtual void MoveRelative(const std::vector<double> & values) = 0;
  virtual void MoveRelative(double x, double y, double z, double a) = 0;
  virtual void MoveRelative(VLStepExpress::Axis axis, double value) = 0;
  virtual void MoveRelative() = 0;

  virtual bool GetStatus() = 0;
  virtual void GetSystemStatus(std::vector<int>& values) = 0;
  virtual void GetSystemStatusText(std::string& value) = 0;
  virtual void GetSystemStatus(VLStepExpress::Axis axis, int & value) = 0;
  virtual int  GetError() = 0;
  virtual void ErrorQuit() = 0;

  virtual bool GetPositionControllerEnabled() = 0;
  virtual void SetPositionControllerEnabled(const bool enable) = 0;

  virtual bool GetJoystickEnabled() = 0;
  virtual void SetJoystickEnabled(bool enabled) = 0;

  virtual void GetJoystickAxisEnabled(std::vector<int> & values) = 0;
  virtual void GetJoystickAxisEnabled(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetJoystickAxisEnabled(const std::vector<int> & values) = 0;
  virtual void SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value) = 0;

  virtual void Reset() = 0;
  virtual void ConfirmErrorRectification() = 0;
  void ValidConfig();
  void ValidParameter();
  virtual void Calibrate() = 0;

  // low level methods
  virtual void SendCommand(const std::string &) = 0;
  virtual std::string ReceiveString(const std::string &) = 0;

  void SetValue(const std::string & command, const std::string & value);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, const std::string & value);
  void SetValue(const std::string & command, int value1);
  void SetValue(const std::string & command, int value1, int value2);
  void SetValue(const std::string & command, int value1, int value2, int value3);
  void SetValue(const std::string & command, int value1, int value2, int value3, int value4);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, int value);
  void SetValue(const std::string & command, double value1);
  void SetValue(const std::string & command, double value1, double value2);
  void SetValue(const std::string & command, double value1, double value2, double value3);
  void SetValue(const std::string & command, double value1, double value2, double value3, double value4);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, double value);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, double value1, double value2);
  void SetValue(const std::string & command, const std::vector<int> & values);
  void SetValue(const std::string & command, const std::vector<double> & values);

  void GetValue(const std::string & command, std::string & value);
  void GetValue(const std::string & command, int & value);
  void GetValue(const std::string & command, double & value);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, std::string & value);
  void GetValue(const std::string & command, std::vector<int> & values);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, int & value);
  void GetValue(const std::string & command, std::vector<double> & values);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, double & value);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, std::vector<double> & values);

  char GetAxisName(VLStepExpress::Axis axis);
  const char * GetAxisDimensionShortName(VLStepExpress::Dimension dimension);
  const char * GetAxisDimensionName(VLStepExpress::Dimension dimension);
  const char * GetAxisVelocityShortName(VLStepExpress::Dimension dimension);
  const char * GetAxisVelocityName(VLStepExpress::Dimension dimension);
  const char * GetAxisAccelerationShortName(VLStepExpress::Dimension dimension);
  const char * GetAxisAccelerationName(VLStepExpress::Dimension dimension);
  const char * GetAxisAccelerationJerkShortName(VLStepExpress::Dimension dimension);
  const char * GetAxisAccelerationJerkName(VLStepExpress::Dimension dimension);
  char GetAxisStatusText(VLStepExpress::AxisStatus status);
};

/** @} */

/** @} */

#endif

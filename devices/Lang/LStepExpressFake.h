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

#ifndef __LSTEPEXPRESSFAKE_H
#define __LSTEPEXPRESSFAKE_H

#include <string>
#include <vector>

#include "VLStepExpress.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Lang Lang
 *  @{
 */

class LStepExpressFake : public VLStepExpress
{
 public:

  LStepExpressFake(const std::string&, const std::string& lstep_ver="", const std::string& lstep_iver="");
  ~LStepExpressFake();

  std::string ioPort() const;

  bool DeviceAvailable() const { return true; }

  void GetAutoStatus(int & value);
  void SetAutoStatus(int value);

  void GetAxisStatus(std::vector<int> & values);

  void GetAxisEnabled(std::vector<int> & values);
  void GetAxisEnabled(VLStepExpress::Axis axis, int & value);
  void SetAxisEnabled(const std::vector<int> & values);
  void SetAxisEnabled(VLStepExpress::Axis axis, int value);

  void GetAxisDirection(std::vector<int> & values);
  void GetAxisDirection(VLStepExpress::Axis axis, int & value);
  void SetAxisDirection(const std::vector<int> & values);
  void SetAxisDirection(VLStepExpress::Axis axis, int value);

  void GetDimension(std::vector<int> & values);
  void GetDimension(VLStepExpress::Axis axis, int & value);
  void SetDimension(const std::vector<int> & values);
  void SetDimension(VLStepExpress::Axis axis, int value);

  void GetPowerAmplifierStatus(std::vector<int> & values);
  void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value);
  void SetPowerAmplifierStatus(const std::vector<int> & values);
  void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value);

  void GetAccelerationJerk(std::vector<double> & values);
  void GetAccelerationJerk(VLStepExpress::Axis axis, double & value);
  void SetAccelerationJerk(const std::vector<double> & values);
  void SetAccelerationJerk(VLStepExpress::Axis axis, double value);

  void GetDecelerationJerk(std::vector<double> & values);
  void GetDecelerationJerk(VLStepExpress::Axis axis, double & value);
  void SetDecelerationJerk(const std::vector<double> & values);
  void SetDecelerationJerk(VLStepExpress::Axis axis, double value);

  void GetAcceleration(std::vector<double> & values);
  void GetAcceleration(VLStepExpress::Axis axis, double & value);
  void SetAcceleration(const std::vector<double> & values);
  void SetAcceleration(VLStepExpress::Axis axis, double value);

  void GetDeceleration(std::vector<double> & values);
  void GetDeceleration(VLStepExpress::Axis axis, double & value);
  void SetDeceleration(const std::vector<double> & values);
  void SetDeceleration(VLStepExpress::Axis axis, double value);

  void GetVelocity(std::vector<double> & values);
  void GetVelocity(VLStepExpress::Axis axis, double & value);
  void SetVelocity(const std::vector<double> & values);
  void SetVelocity(VLStepExpress::Axis axis, double value);

  void GetPosition(std::vector<double> & values);
  void GetPosition(VLStepExpress::Axis axis, double & value);
  void SetPosition(const std::vector<double> & values);
  void SetPosition(VLStepExpress::Axis axis, double value);

  void MoveAbsolute(const std::vector<double> & values);
  void MoveAbsolute(double x, double y, double z, double a);
  void MoveAbsolute(VLStepExpress::Axis axis, double value);

  void MoveRelative(const std::vector<double> & values);
  void MoveRelative(double x, double y, double z, double a);
  void MoveRelative(VLStepExpress::Axis axis, double value);
  void MoveRelative();

  bool GetStatus() { return true; }
  void GetSystemStatus(std::vector<int> & values);
  void GetSystemStatusText(std::string& value);
  void GetSystemStatus(VLStepExpress::Axis axis, int & value);
  int  GetError() { return 0; }
  void ErrorQuit() { }

  bool GetPositionControllerEnabled();
  void SetPositionControllerEnabled(const bool enable);

  bool GetJoystickEnabled();
  void SetJoystickEnabled(bool enabled);

  void GetJoystickAxisEnabled(std::vector<int> & values);
  void GetJoystickAxisEnabled(VLStepExpress::Axis axis, int & value);
  void SetJoystickAxisEnabled(const std::vector<int> & values);
  void SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value);

  void Reset() {}
  void ConfirmErrorRectification() {}
  void ValidConfig() {}
  void ValidParameter() {}
  void SaveConfig() {}
  void Calibrate() {}
  void EmergencyStop() {}

  // low level debugging methods
  void SendCommand(const std::string & command);
  void ReceiveString(std::string &) { }

 private:

  std::string ioPort_;

  int autoStatus_;
  std::vector<int> axisStatus_;
  std::vector<int> axis_;
  std::vector<int> axisDirection_;
  std::vector<int> dim_;
  std::vector<int> pa_;
  std::vector<double> accelerationJerk_;
  std::vector<double> decelerationJerk_;
  std::vector<double> acceleration_;
  std::vector<double> deceleration_;
  std::vector<double> velocity_;
  std::vector<double> position_;
  std::vector<double> moverel_;
  std::string port_;

  bool joystickEnabled_;
  std::vector<int> joystickAxisEnabled_;

  bool posCtrl_enabled_;
};

/** @} */

/** @} */

#endif

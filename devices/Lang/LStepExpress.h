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

#ifndef __LSTEPEXPRESS_H
#define __LSTEPEXPRESS_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VLStepExpress.h"
#include "LStepExpressComHandler.h"

class LStepExpress : public VLStepExpress
{
 public:

  LStepExpress( const ioport_t );
  ~LStepExpress();

 public:

  bool DeviceAvailable() const;

  void GetAutoStatus(int & value);
  void SetAutoStatus(int value);

  void GetAxisStatus(std::vector<int> & values);

  void GetAxisEnabled(std::vector<int> & values);
  void GetAxisEnabled(VLStepExpress::Axis axis, int & value);
  void SetAxisEnabled(std::vector<int> & values);
  void SetAxisEnabled(VLStepExpress::Axis axis, int value);

  void GetAxisDirection(std::vector<int> & values);
  void GetAxisDirection(VLStepExpress::Axis axis, int & value);
  void SetAxisDirection(std::vector<int> & values);
  void SetAxisDirection(VLStepExpress::Axis axis, int value);

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
  void SetAcceleration(std::vector<double> & values);
  void SetAcceleration(VLStepExpress::Axis axis, double value);

  void GetDeceleration(std::vector<double> & values);
  void GetDeceleration(VLStepExpress::Axis axis, double & value);
  void SetDeceleration(std::vector<double> & values);
  void SetDeceleration(VLStepExpress::Axis axis, double value);

  void GetVelocity(std::vector<double> & values);
  void GetVelocity(VLStepExpress::Axis axis, double & value);
  void SetVelocity(std::vector<double> & values);
  void SetVelocity(VLStepExpress::Axis axis, double value);

  void GetDimension(std::vector<int> & values);
  void GetDimension(VLStepExpress::Axis axis, int & value);
  void SetDimension(std::vector<int> & values);
  void SetDimension(VLStepExpress::Axis axis, int value);

  void GetPowerAmplifierStatus(std::vector<int> & values);
  void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value);
  void SetPowerAmplifierStatus(std::vector<int> & values);
  void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value);

  void GetPosition(std::vector<double> & values);
  void GetPosition(VLStepExpress::Axis axis, double & value);
  void SetPosition(std::vector<double> & values);
  void SetPosition(VLStepExpress::Axis axis, double value);

  void MoveAbsolute(std::vector<double> & values);
  void MoveAbsolute(double x, double y, double z, double a);
  void MoveAbsolute(VLStepExpress::Axis axis, double value);

  void MoveRelative(std::vector<double> & values);
  void MoveRelative(double x, double y, double z, double a);
  void MoveRelative(VLStepExpress::Axis axis, double value);
  void MoveRelative();

  bool GetStatus();
  void GetSystemStatus(std::vector<int>& values);
  void GetSystemStatusText(std::string& value);
  void GetSystemStatus(VLStepExpress::Axis axis, int & value);
  int GetError();

  bool GetJoystickEnabled();
  void SetJoystickEnabled(bool enabled);

  void GetJoystickAxisEnabled(std::vector<int> & values);
  void GetJoystickAxisEnabled(VLStepExpress::Axis axis, int & value);
  void SetJoystickAxisEnabled(std::vector<int> & values);
  void SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value);

  void Reset();
  void ConfirmErrorRectification();
  void ValidConfig();
  void ValidParameter();
  void SaveConfig();
  void Calibrate();
  void EmergencyStop();

  // low level debugging methods
  void SendCommand(const std::string &);
  void ReceiveString(std::string &);

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  LStepExpressComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif

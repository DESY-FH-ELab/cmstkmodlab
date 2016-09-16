#ifndef __NANOTECSMCI36_H
#define __NANOTECSMCI36_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VNanotecSMCI36.h"
#include "NanotecComHandler.h"

class NanotecSMCI36 : public VNanotecSMCI36
{
 public:

  NanotecSMCI36( const ioport_t );
  ~NanotecSMCI36();

 public:

  bool DeviceAvailable() const;

  std::string GetFirmwareVersion() const;
  int GetStatus() const;

  void SetMotorType(int type);
  int GetMotorType() const;

  void SetPhaseCurrent(int current);
  int GetPhaseCurrent() const;

  void SetStandStillPhaseCurrent(int current);
  int GetStandStillPhaseCurrent() const;

  void SetStepMode(int mode);
  int GetStepMode() const;

  void SetMotorID(int ID);
  int GetMotorID() const;

  void SetErrorCorrectionMode(int mode);
  int GetErrorCorrectionMode() const;

  void SetEncoderDirection(int direction);
  int GetEncoderDirection() const;

  void SetSwingOutTime(int time);
  int GetSwingOutTime() const;

  void SetMaxEncoderDeviation(int deviation);
  int GetMaxEncoderDeviation() const;

  int GetPosition() const;
  int GetEncoderPosition() const;
  void ResetPositionError();
  void ResetPositionError(int position);

  void SetInputPinFunction(int pin, int function);
  int GetInputPinFunction(int pin) const;

  void SetOutputPinFunction(int pin, int function);
  int GetOutputPinFunction(int pin) const;

  void SetReversePolarityMask(int mask);
  int GetReversePolarityMask() const;

  void SetRampMode(int ramp);
  int GetRampMode() const;

  void SetQuickstopRamp(int ramp);
  int GetQuickstopRamp() const;

  void SetQuickstopRampHzPerSecond(int ramp);
  int GetQuickstopRampHzPerSecond() const;

  void SetAccelerationRamp(int ramp);
  int GetAccelerationRamp() const;

  void SetAccelerationRampHzPerSecond(int ramp);
  int GetAccelerationRampHzPerSecond() const;

  void SetDecelerationRamp(int ramp);
  int GetDecelerationRamp() const;

  void SetDecelerationRampHzPerSecond(int ramp);
  int GetDecelerationRampHzPerSecond() const;

  void SetPositioningMode(int mode);
  int GetPositioningMode() const;

  void SetTravelDistance(int distance);
  int GetTravelDistance() const;

  void SetDirection(bool direction);
  bool GetDirection() const;

  void SetMinimumFrequency(int frequency);
  int GetMinimumFrequency() const;

  void SetMaximumFrequency(int frequency);
  int GetMaximumFrequency() const;

  void SetMaximumFrequency2(int frequency);
  int GetMaximumFrequency2() const;

  void Start();
  void Stop(bool quickstop = false);

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  NanotecComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif

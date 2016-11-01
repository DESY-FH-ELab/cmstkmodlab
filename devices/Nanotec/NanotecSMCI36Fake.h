#ifndef __NANOTECSMCI36FAKE_H
#define __NANOTECSMCI36FAKE_H

#include "VNanotecSMCI36.h"

class NanotecSMCI36Fake : public VNanotecSMCI36
{
 public:

  NanotecSMCI36Fake( const ioport_t );
  ~NanotecSMCI36Fake();

 public:

  bool DeviceAvailable() const { return true; }

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

  void SetEncoderDirection(bool direction);
  bool GetEncoderDirection() const;

  void SetSwingOutTime(int time);
  int GetSwingOutTime() const;

  void SetMaxEncoderDeviation(int deviation);
  int GetMaxEncoderDeviation() const;

  int GetPosition() const;
  int GetEncoderPosition() const;
  void ResetPositionError(int position);

  void SetInputPinFunction(int pin, int function);
  int GetInputPinFunction(int pin) const;

  void SetOutputPinFunction(int pin, int function);
  int GetOutputPinFunction(int pin) const;

  void SetIOMask(unsigned int mask);
  unsigned int GetIOMask() const;

  void SetReversePolarityMask(unsigned int mask);
  unsigned int GetReversePolarityMask() const;

  void SetIO(unsigned int mask);
  unsigned int GetIO() const;

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

  int status_;
  int motorType_;
  int phaseCurrent_;
  int stepMode_;
  int motorID_;
  int errorCorrectionMode_;
  bool encoderDirection_;
  int swingOutTime_;
  int maxEncoderDeviation_;
  int position_;
  int encoderPosition_;
  int standStillPhaseCurrent_;
  int inputPinFunction_[7];
  int outputPinFunction_[4];
  unsigned int ioMask_;
  unsigned int reversePolarityMask_;
  unsigned int io_;
  int rampMode_;
  int quickstopRamp_;
  int accelerationRamp_;
  int decelerationRamp_;
  int positioningMode_;
  int travelDistance_;
  bool direction_;
  int minFrequency_;
  int maxFrequency_;
  int maxFrequency2_;
};

#endif

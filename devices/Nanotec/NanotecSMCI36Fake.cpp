#include <cmath>
#include <iostream>

#include "NanotecSMCI36Fake.h"

NanotecSMCI36Fake::NanotecSMCI36Fake( const ioport_t ioPort )
  :VNanotecSMCI36(ioPort)
{
  status_ = smciReady | smciReserved5 | smciReserved7;

  motorType_ = smciStepper;
  phaseCurrent_ = 50;
  standStillPhaseCurrent_ = 10;

  stepMode_ = smci02MicroStepsPerFullStep;

  motorID_ = 1;
  errorCorrectionMode_ = smciErrCorrectionAfterTravel;
  encoderDirection_ = 0;
  swingOutTime_ = 5;
  maxEncoderDeviation_ = 2;

  position_ = 0;
  encoderPosition_ = 0;

  inputPinFunction_[1] = smciIPinUserDefined;
  inputPinFunction_[2] = smciIPinUserDefined;
  inputPinFunction_[3] = smciIPinUserDefined;
  inputPinFunction_[4] = smciIPinUserDefined;
  inputPinFunction_[5] = smciIPinUserDefined;
  inputPinFunction_[6] = smciIPinUserDefined;

  outputPinFunction_[1] = smciOPinUserDefined;
  outputPinFunction_[2] = smciOPinUserDefined;
  outputPinFunction_[3] = smciOPinUserDefined;

  ioMask_ = 0x0007003F;
  reversePolarityMask_ = 0x0107003F;
  io_ = 0;

  rampMode_ = 0;
  quickstopRamp_ = 0;
  accelerationRamp_ = 2364;
  decelerationRamp_ = 0;

  positioningMode_ = 1;
  travelDistance_ = 400;
  direction_ = false;
  minFrequency_ = 400;
  maxFrequency_ = 1000;
  maxFrequency2_ = 1000;
}

NanotecSMCI36Fake::~NanotecSMCI36Fake()
{

}

std::string NanotecSMCI36Fake::GetFirmwareVersion() const
{
  return std::string("001v SMCI47-S_RS485_17-05-2011-rev3711");
}

int NanotecSMCI36Fake::GetStatus() const
{
  return status_;
}

void NanotecSMCI36Fake::SetMotorType(int type)
{
  if (type < smciStepper || type > smciBLDCEncoder) return;
  motorType_ = type;
}

int NanotecSMCI36Fake::GetMotorType() const
{
  return motorType_;
}

void NanotecSMCI36Fake::SetPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;
  phaseCurrent_ = current;
}

int NanotecSMCI36Fake::GetPhaseCurrent() const
{
  return phaseCurrent_;
}

void NanotecSMCI36Fake::SetStandStillPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;
  standStillPhaseCurrent_ = current;
}

int NanotecSMCI36Fake::GetStandStillPhaseCurrent() const
{
  return standStillPhaseCurrent_;
}

void NanotecSMCI36Fake::SetStepMode(int mode)
{
  if (mode != smci01MicroStepsPerFullStep &&
      mode != smci02MicroStepsPerFullStep &&
      mode != smci04MicroStepsPerFullStep &&
      mode != smci05MicroStepsPerFullStep &&
      mode != smci08MicroStepsPerFullStep &&
      mode != smci10MicroStepsPerFullStep &&
      mode != smci16MicroStepsPerFullStep &&
      mode != smci32MicroStepsPerFullStep &&
      mode != smci64MicroStepsPerFullStep &&
      mode != smciFeedRateMode &&
      mode != smciAdaptiveStepMode) return;

  stepMode_ = mode;
}

int NanotecSMCI36Fake::GetStepMode() const
{
  return stepMode_;
}

void NanotecSMCI36Fake::SetMotorID(int ID)
{
  motorID_ = ID;
}

int NanotecSMCI36Fake::GetMotorID() const
{
  return motorID_;
}

void NanotecSMCI36Fake::SetErrorCorrectionMode(int mode)
{
  if (mode < smciErrCorrectionOff || mode > smciErrCorrectionDuringTravel) return;
  errorCorrectionMode_ = mode;
}

int NanotecSMCI36Fake::GetErrorCorrectionMode() const
{
  return errorCorrectionMode_;
}

void NanotecSMCI36Fake::SetEncoderDirection(int direction)
{
  if (direction < 0 || direction > 1) return;
  encoderDirection_ = direction;
}

int NanotecSMCI36Fake::GetEncoderDirection() const
{
  return encoderDirection_;
}

void NanotecSMCI36Fake::SetSwingOutTime(int time)
{
  if (time<0 || time > 255) return;
  swingOutTime_ = time;
}

int NanotecSMCI36Fake::GetSwingOutTime() const
{
  return swingOutTime_;
}

void NanotecSMCI36Fake::SetMaxEncoderDeviation(int deviation)
{
  if (deviation < 0 || deviation > 255) return;
  maxEncoderDeviation_ = deviation;
}

int NanotecSMCI36Fake::GetMaxEncoderDeviation() const
{
  return maxEncoderDeviation_;
}

int NanotecSMCI36Fake::GetPosition() const
{
  return position_;
}

int NanotecSMCI36Fake::GetEncoderPosition() const
{
  return encoderPosition_;
}

void NanotecSMCI36Fake::ResetPositionError(int position)
{
  position_ = position;
  encoderPosition_ = position;
}

void NanotecSMCI36Fake::SetInputPinFunction(int pin, int function)
{
  if (pin<1 || pin>6) return;
  if (function<smciIPinUserDefined || function>smciIClockDirectionMode2) return;
  inputPinFunction_[pin] = function;
}

int NanotecSMCI36Fake::GetInputPinFunction(int pin) const
{
  if (pin<1 || pin>6) return smciIInvalid;
  return inputPinFunction_[pin];
}

void NanotecSMCI36Fake::SetOutputPinFunction(int pin, int function)
{
  if (pin<1 || pin>3) return;
  if (function<smciOPinUserDefined || function>smciOError) return;
  outputPinFunction_[pin] = function;
}

int NanotecSMCI36Fake::GetOutputPinFunction(int pin) const
{
  if (pin<1 || pin>3) return smciOInvalid;
  return outputPinFunction_[pin];
}

void NanotecSMCI36Fake::SetIOMask(int mask)
{
  ioMask_ = mask;
}

int NanotecSMCI36Fake::GetIOMask() const
{
  return ioMask_;
}

void NanotecSMCI36Fake::SetReversePolarityMask(int mask)
{
  reversePolarityMask_ = mask;
}

int NanotecSMCI36Fake::GetReversePolarityMask() const
{
  return reversePolarityMask_;
}

void NanotecSMCI36Fake::SetIO(int mask)
{
  io_ = mask;
}

int NanotecSMCI36Fake::GetIO() const
{
  return io_;
}

void NanotecSMCI36Fake::SetRampMode(int ramp)
{
  if (ramp < smciTrapezoidalRamp || ramp > smciJerkFreeRamp) return;
  rampMode_ = ramp;
}

int NanotecSMCI36Fake::GetRampMode() const
{
  return rampMode_;
}

void NanotecSMCI36Fake::SetQuickstopRamp(int ramp)
{
  if (ramp < 0 || ramp > 8000) return;
  quickstopRamp_ = ramp;
}

int NanotecSMCI36Fake::GetQuickstopRamp() const
{
  return quickstopRamp_;
}

void NanotecSMCI36Fake::SetQuickstopRampHzPerSecond(int ramp)
{
  if (ramp < 0 || ramp > 3000000) return;
  quickstopRamp_ = std::pow(3000.0 / (static_cast<float>(ramp) / 1000 + 11.7), 2.0);
}

int NanotecSMCI36Fake::GetQuickstopRampHzPerSecond() const
{
  if (quickstopRamp_==0) return 3000000;
  return 1000 * (3000.0 / std::sqrt(static_cast<float>(quickstopRamp_)) - 11.7);
}

void NanotecSMCI36Fake::SetAccelerationRamp(int ramp)
{
  if (ramp < 1 || ramp > 65535) return;
  accelerationRamp_ = ramp;
}

int NanotecSMCI36Fake::GetAccelerationRamp() const
{
  return accelerationRamp_;
}

void NanotecSMCI36Fake::SetAccelerationRampHzPerSecond(int ramp)
{
  if (ramp < 1 || ramp > 3000000) return;
  accelerationRamp_ = std::pow(3000.0 / (static_cast<float>(ramp) / 1000 + 11.7), 2.0);
}

int NanotecSMCI36Fake::GetAccelerationRampHzPerSecond() const
{
  return 1000 * (3000.0 / std::sqrt(static_cast<float>(accelerationRamp_)) - 11.7);
}

void NanotecSMCI36Fake::SetDecelerationRamp(int ramp)
{
  if (ramp < 0 || ramp > 65535) return;
  decelerationRamp_ = ramp;
}

int NanotecSMCI36Fake::GetDecelerationRamp() const
{
  return decelerationRamp_;
}

void NanotecSMCI36Fake::SetDecelerationRampHzPerSecond(int ramp)
{
  if (ramp < 0 || ramp > 3000000) return;
  if (ramp==0) {
    decelerationRamp_ = 0;
  } else {
    decelerationRamp_ = std::pow(3000.0 / (static_cast<float>(ramp) / 1000 + 11.7), 2.0);
  }
}

int NanotecSMCI36Fake::GetDecelerationRampHzPerSecond() const
{
  if (decelerationRamp_==0) return 0;
  return 1000 * (3000.0 / std::sqrt(static_cast<float>(decelerationRamp_)) - 11.7);
}

void NanotecSMCI36Fake::SetPositioningMode(int mode)
{
  if (mode < smciRelativePositioning || mode >= smciMaxPositioningMode) return;
  positioningMode_ = mode;
}

int NanotecSMCI36Fake::GetPositioningMode() const
{
  return positioningMode_;
}

void NanotecSMCI36Fake::SetTravelDistance(int distance)
{
  travelDistance_ = distance;
}

int NanotecSMCI36Fake::GetTravelDistance() const
{
  return travelDistance_;
}

void NanotecSMCI36Fake::SetDirection(bool direction)
{
  direction_ = direction;
}

bool NanotecSMCI36Fake::GetDirection() const
{
  return direction_;
}

void NanotecSMCI36Fake::SetMinimumFrequency(int frequency)
{
  if (frequency < 1 || frequency > 160000) return;
  minFrequency_ = frequency;
}

int NanotecSMCI36Fake::GetMinimumFrequency() const
{
  return minFrequency_;
}

void NanotecSMCI36Fake::SetMaximumFrequency(int frequency)
{
  if (frequency < 1 || frequency > 1000000) return;
  maxFrequency_ = frequency;
}

int NanotecSMCI36Fake::GetMaximumFrequency() const
{
  return maxFrequency2_;
}

void NanotecSMCI36Fake::SetMaximumFrequency2(int frequency)
{
  if (frequency < 1 || frequency > 1000000) return;
  maxFrequency2_ = frequency;
}

int NanotecSMCI36Fake::GetMaximumFrequency2() const
{
  return maxFrequency2_;
}

void NanotecSMCI36Fake::Start()
{
  if (positioningMode_==smciExternalRefRun) {
    position_ = 0;
  } else if (positioningMode_==smciRelativePositioning) {
    if (direction_) {
      position_ -= travelDistance_;
    } else {
      position_ += travelDistance_;
    }
  } else if (positioningMode_==smciAbsolutePositioning) {
    position_ = travelDistance_;
  }

  encoderPosition_ = position_;
}

void NanotecSMCI36Fake::Stop(bool /* quickstop */)
{

}

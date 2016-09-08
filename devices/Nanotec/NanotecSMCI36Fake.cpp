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

  inputPinFunction_[1] = smciIPinUserDefined;
  inputPinFunction_[2] = smciIPinUserDefined;
  inputPinFunction_[3] = smciIPinUserDefined;
  inputPinFunction_[4] = smciIPinUserDefined;
  inputPinFunction_[5] = smciIPinUserDefined;
  inputPinFunction_[6] = smciIPinUserDefined;

  outputPinFunction_[1] = smciOPinUserDefined;
  outputPinFunction_[2] = smciOPinUserDefined;
  outputPinFunction_[3] = smciOPinUserDefined;

  reversePolarityMask_ = 0x0107003F;
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

void NanotecSMCI36Fake::SetReversePolarityMask(int mask)
{
  reversePolarityMask_ = mask;
}

int NanotecSMCI36Fake::GetReversePolarityMask() const
{
  return reversePolarityMask_;
}

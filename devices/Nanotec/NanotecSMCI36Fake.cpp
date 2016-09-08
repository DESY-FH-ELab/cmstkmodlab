#include <cmath>

#include "NanotecSMCI36Fake.h"

NanotecSMCI36Fake::NanotecSMCI36Fake( const ioport_t ioPort )
  :VNanotecSMCI36(ioPort)
{
  status_ = smciReady | smciReserved5 | smciReserved7;

  motorType_ = smciStepper;
  phaseCurrent_ = 50;
  standStillPhaseCurrent_ = 10;
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

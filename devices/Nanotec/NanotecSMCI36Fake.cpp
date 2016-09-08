#include <cmath>

#include "NanotecSMCI36Fake.h"

NanotecSMCI36Fake::NanotecSMCI36Fake( const ioport_t ioPort )
  :VNanotecSMCI36(ioPort)
{
  status_ = smciReady | smciReserved5 | smciReserved7;

  motorType_ = smciStepper;
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

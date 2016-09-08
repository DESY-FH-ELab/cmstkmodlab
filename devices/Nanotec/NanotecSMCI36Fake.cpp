#include <cmath>

#include "NanotecSMCI36Fake.h"

NanotecSMCI36Fake::NanotecSMCI36Fake( const ioport_t ioPort )
  :VNanotecSMCI36(ioPort)
{

}

NanotecSMCI36Fake::~NanotecSMCI36Fake()
{

}

std::string NanotecSMCI36Fake::GetFirmwareVersion() const
{
  return std::string("001v SMCI47-S_RS485_17-05-2011-rev3711");
}

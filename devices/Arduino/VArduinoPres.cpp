#include "VArduinoPres.h"

///
///
///
VArduinoPres::VArduinoPres( const ioport_t ioPort )
{

}

float VArduinoPres::ToPressure(int value) const
{
  // 5V = 1023 = 100 bar

  if (value>1023) return 100.;
  float ret = value;
  return 100.*ret/1023.;
}

float VArduinoPres::ToFlow(int value) const
{
  // 5V = 1023 = 50 ml/min

  if (value>1023) return 50.;
  float ret = value;
  return 50.*ret/1023.;
}

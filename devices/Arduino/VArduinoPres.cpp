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

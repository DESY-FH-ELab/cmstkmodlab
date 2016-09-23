#include "VArduinoMotor.h"

///
///
///
VArduinoMotor::VArduinoMotor( const ioport_t ioPort )
{

}

float VArduinoMotor::ToPressure(int value) const
{
  // 5V = 1023 = 100 bar

  if (value>1023) return 100.;
  float ret = value;
  return 100.*ret/1023.;
}


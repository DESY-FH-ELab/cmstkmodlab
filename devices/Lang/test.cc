#include <iostream>

#ifdef USE_FAKEIO
#include "LStepExpressFake.h"
typedef LStepExpressFake LStepExpress_t;
#else
#include "LStepExpress.h"
typedef LStepExpress LStepExpress_t;
#endif

int main()
{
  LStepExpress_t lstep("/dev/ttyUSB0");

  lstep.SendCommand("!reset");
  lstep.SetValue("!axisdir", VLStepExpress::X, 1);
  lstep.SetValue("!axisdir", VLStepExpress::A, 0);

  return 0;
}

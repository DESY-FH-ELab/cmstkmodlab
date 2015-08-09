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
  std::cout << "test" << std::endl;

  LStepExpress_t lstep("/dev/ttyUSB0");

  std::cout << lstep.GetStatus() << std::endl;

  return 0;
}

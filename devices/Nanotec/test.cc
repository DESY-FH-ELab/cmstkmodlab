#include <iostream>

#ifdef USE_FAKEIO
#include "NanotecSMCI36Fake.h"
typedef NanotecSMCI36Fake NanotecSMCI36_t;
#else
#include "NanotecSMCI36.h"
typedef NanotecSMCI36 NanotecSMCI36_t;
#endif

int main()
{
  std::cout << "test" << std::endl;

  NanotecSMCI36_t smci36("/dev/ttyUSB0");

  std::cout << "firmware version:          " << smci36.GetFirmwareVersion() << std::endl;
  std::cout << "status:                    " << std::hex << smci36.GetStatus() << std::endl;

  std::cout << "motor type:                " << smci36.GetMotorType() << std::endl;
  std::cout << "phase current:             " << smci36.GetPhaseCurrent() << std::endl;
  std::cout << "standstill phase current:  " << smci36.GetStandStillPhaseCurrent() << std::endl;
  std::cout << "step mode:                 " << smci36.GetStepMode() << std::endl;

  return 0;
}

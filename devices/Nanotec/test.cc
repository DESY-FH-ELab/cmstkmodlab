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
  std::cout << "status:                    0x" << std::hex << smci36.GetStatus() << std::dec << std::endl;

  std::cout << "motor type:                " << smci36.GetMotorType() << std::endl;
  std::cout << "phase current:             " << smci36.GetPhaseCurrent() << std::endl;
  std::cout << "standstill phase current:  " << smci36.GetStandStillPhaseCurrent() << std::endl;
  std::cout << "step mode:                 " << smci36.GetStepMode() << std::endl;
  std::cout << "motor ID:                  " << smci36.GetMotorID() << std::endl;
  std::cout << "error correction mode:     " << smci36.GetErrorCorrectionMode() << std::endl;
  std::cout << "encoder direction:         " << smci36.GetEncoderDirection() << std::endl;
  std::cout << "swing out time:            " << smci36.GetSwingOutTime() << std::endl;
  std::cout << "maximum encoder deviation: " << smci36.GetMaxEncoderDeviation() << std::endl;

  std::cout << "input pin function:        ";
  for (int p=1;p<=6;++p) std::cout << smci36.GetInputPinFunction(p) << " ";
  std::cout << std::endl;

  std::cout << "output pin function:       ";
  for (int p=1;p<=3;++p) std::cout << smci36.GetOutputPinFunction(p) << " ";
  std::cout << std::endl;

  std::cout << "pin polarity mask:         0x" << std::hex << smci36.GetReversePolarityMask() << std::dec << std::endl;

  return 0;
}

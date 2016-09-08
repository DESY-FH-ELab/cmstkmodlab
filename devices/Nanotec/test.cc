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
  std::cout << "position:                  " << smci36.GetPosition() << std::endl;
  std::cout << "encoder position:          " << smci36.GetEncoderPosition() << std::endl;

  std::cout << "input pin function:        ";
  for (int p=1;p<=6;++p) std::cout << smci36.GetInputPinFunction(p) << " ";
  std::cout << std::endl;

  std::cout << "output pin function:       ";
  for (int p=1;p<=3;++p) std::cout << smci36.GetOutputPinFunction(p) << " ";
  std::cout << std::endl;

  std::cout << "pin polarity mask:         0x" << std::hex << smci36.GetReversePolarityMask() << std::dec << std::endl;

  std::cout << "ramp mode:                 " << smci36.GetRampMode() << std::endl;

  std::cout << "quickstop ramp:            " << smci36.GetQuickstopRamp() << std::endl;
  std::cout << "quickstop ramp [Hz/s]:     " << smci36.GetQuickstopRampHzPerSecond() << std::endl;
  std::cout << "acceleration ramp:         " << smci36.GetAccelerationRamp() << std::endl;
  std::cout << "acceleration ramp [Hz/s]:  " << smci36.GetAccelerationRampHzPerSecond() << std::endl;
  std::cout << "deceleration ramp:         " << smci36.GetDecelerationRamp() << std::endl;
  std::cout << "deceleration ramp [Hz/s]:  " << smci36.GetDecelerationRampHzPerSecond() << std::endl;

  std::cout << "positioning mode:          " << smci36.GetPositioningMode() << std::endl;
  std::cout << "travel distance:           " << smci36.GetTravelDistance() << std::endl;
  std::cout << "minimum frequency:         " << smci36.GetMinimumFrequency() << std::endl;
  std::cout << "maximum frequency:         " << smci36.GetMaximumFrequency() << std::endl;
  std::cout << "maximum frequency 2:       " << smci36.GetMaximumFrequency2() << std::endl;

  return 0;
}

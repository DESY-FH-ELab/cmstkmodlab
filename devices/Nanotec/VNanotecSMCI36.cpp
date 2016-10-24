#include "VNanotecSMCI36.h"

VNanotecSMCI36::VNanotecSMCI36( const ioport_t ioPort )
{
  stepModeNames_.push_back(std::pair<int,std::string>(smci01MicroStepsPerFullStep, "1 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci02MicroStepsPerFullStep, "2 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci04MicroStepsPerFullStep, "4 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci05MicroStepsPerFullStep, "5 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci08MicroStepsPerFullStep, "8 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci10MicroStepsPerFullStep, "10 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci16MicroStepsPerFullStep, "16 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci32MicroStepsPerFullStep, "32 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci64MicroStepsPerFullStep, "64 uStep per Full Step"));
  // stepModeNames_.push_back(std::pair<int,std::string>(smciFeedRateMode, "Feed Rate Mode"));
  // stepModeNames_.push_back(std::pair<int,std::string>(smciAdaptiveStepMode, "Adaptive Step Mode"));

  errorCorrectionModeNames_.push_back(std::pair<int,std::string>(smciErrCorrectionOff, "Off"));
  errorCorrectionModeNames_.push_back(std::pair<int,std::string>(smciErrCorrectionAfterTravel, "Correction After Travel"));
  errorCorrectionModeNames_.push_back(std::pair<int,std::string>(smciErrCorrectionDuringTravel, "Correction During Travel"));

  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIPinUserDefined, "User Defined"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIStartRecordErrorReset, "Start Record / Error Reset"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIRecordSelect0, "Record Selection Bit 0"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIRecordSelect1, "Record Selection Bit 1"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIRecordSelect2, "Record Selection Bit 2"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIRecordSelect3, "Record Selection Bit 3"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIRecordSelect4, "Record Selection Bit 4"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIExternalReferenceSwitch, "External Reference Switch"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciITrigger, "Trigger"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIDirection, "Direction"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIEnable, "Enable"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIClock, "Clock"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIClockDirectionMode1, "Clock Direction Mode 1"));
  inputPinFunctionNames_.push_back(std::pair<int,std::string>(smciIClockDirectionMode2, "Clock Direction Mode 2"));

  outputPinFunctionNames_.push_back(std::pair<int,std::string>(smciOPinUserDefined, "User Defined"));
  outputPinFunctionNames_.push_back(std::pair<int,std::string>(smciOReady, "Ready"));
  outputPinFunctionNames_.push_back(std::pair<int,std::string>(smciORunning, "Running"));
  outputPinFunctionNames_.push_back(std::pair<int,std::string>(smciOError, "Error"));

  rampModeNames_.push_back(std::pair<int,std::string>(smciTrapezoidalRamp, "Trapezoidal Ramp"));
  rampModeNames_.push_back(std::pair<int,std::string>(smciSinusRamp, "Sinus Ramp"));
  rampModeNames_.push_back(std::pair<int,std::string>(smciJerkFreeRamp, "Jerk-Free Ramp"));

  positioningModeNames_.push_back(std::pair<int,std::string>(smciRelativePositioning, "Relative Positioning"));
  positioningModeNames_.push_back(std::pair<int,std::string>(smciAbsolutePositioning, "Absolute Positioning"));
  positioningModeNames_.push_back(std::pair<int,std::string>(smciExternalRefRun, "External Reference Run"));
}

VNanotecSMCI36::~VNanotecSMCI36()
{

}

unsigned int VNanotecSMCI36::GetInputBitForPin(int pin) const
{
  switch (pin) {
  case 1:
    return  VNanotecSMCI36::smciInput1;
  case 2:
    return  VNanotecSMCI36::smciInput2;
  case 3:
    return  VNanotecSMCI36::smciInput3;
  case 4:
    return  VNanotecSMCI36::smciInput4;
  case 5:
    return  VNanotecSMCI36::smciInput5;
  case 6:
    return  VNanotecSMCI36::smciInput6;
  default:
    return 0;
  }
}

unsigned int VNanotecSMCI36::GetOutputBitForPin(int pin) const
{
  switch (pin) {
  case 1:
    return VNanotecSMCI36::smciOutput1;
    break;
  case 2:
    return VNanotecSMCI36::smciOutput2;
    break;
  case 3:
    return VNanotecSMCI36::smciOutput3;
    break;
  default:
    return 0;
  }
}

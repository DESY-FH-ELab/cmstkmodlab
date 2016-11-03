#include "VNanotecSMCI36.h"

VNanotecSMCI36::VNanotecSMCI36( const ioport_t ioPort )
{
  stepModeNames_[smci01MicroStepsPerFullStep] = "1 uStep per Full Step";
  stepModeNames_[smci02MicroStepsPerFullStep] = "2 uSteps per Full Step";
  stepModeNames_[smci04MicroStepsPerFullStep] = "4 uSteps per Full Step";
  stepModeNames_[smci05MicroStepsPerFullStep] = "5 uSteps per Full Step";
  stepModeNames_[smci08MicroStepsPerFullStep] = "8 uSteps per Full Step";
  stepModeNames_[smci10MicroStepsPerFullStep] = "10 uSteps per Full Step";
  stepModeNames_[smci16MicroStepsPerFullStep] = "16 uSteps per Full Step";
  stepModeNames_[smci32MicroStepsPerFullStep] = "32 uSteps per Full Step";
  stepModeNames_[smci64MicroStepsPerFullStep] = "64 uSteps per Full Step";
  // stepModeNames_[smciFeedRateMode] = "Feed Rate Mode";
  // stepModeNames_[smciAdaptiveStepMode] = "Adaptive Step Mode";

  errorCorrectionModeNames_[smciErrCorrectionOff] =          "Off";
  errorCorrectionModeNames_[smciErrCorrectionAfterTravel] =  "Correction After Travel";
  errorCorrectionModeNames_[smciErrCorrectionDuringTravel] = "Correction During Travel";

  inputPinFunctionNames_[smciIPinUserDefined] =          "User Defined";
  inputPinFunctionNames_[smciIStartRecordErrorReset] =   "Start Record / Error Reset";
  inputPinFunctionNames_[smciIRecordSelect0] =           "Record Selection Bit 0";
  inputPinFunctionNames_[smciIRecordSelect1] =           "Record Selection Bit 1";
  inputPinFunctionNames_[smciIRecordSelect2] =           "Record Selection Bit 2";
  inputPinFunctionNames_[smciIRecordSelect3] =           "Record Selection Bit 3";
  inputPinFunctionNames_[smciIRecordSelect4] =           "Record Selection Bit 4";
  inputPinFunctionNames_[smciIExternalReferenceSwitch] = "External Reference Switch";
  inputPinFunctionNames_[smciITrigger] =                 "Trigger";
  inputPinFunctionNames_[smciIDirection] =               "Direction";
  inputPinFunctionNames_[smciIEnable] =                  "Enable";
  inputPinFunctionNames_[smciIClock] =                   "Clock";
  inputPinFunctionNames_[smciIClockDirectionMode1] =     "Clock Direction Mode 1";
  inputPinFunctionNames_[smciIClockDirectionMode2] =     "Clock Direction Mode 2";

  outputPinFunctionNames_[smciOPinUserDefined] = "User Defined";
  outputPinFunctionNames_[smciOReady] =          "Ready";
  outputPinFunctionNames_[smciORunning] =        "Running";
  outputPinFunctionNames_[smciOError] =          "Error";

  rampModeNames_[smciTrapezoidalRamp] = "Trapezoidal Ramp";
  rampModeNames_[smciSinusRamp] =       "Sinus Ramp";
  rampModeNames_[smciJerkFreeRamp] =    "Jerk-Free Ramp";

  positioningModeNames_[smciRelativePositioning] = "Relative Positioning";
  positioningModeNames_[smciAbsolutePositioning] = "Absolute Positioning";
  positioningModeNames_[smciExternalRefRun] =      "External Reference Run";

  minFrequencyLimits_ = std::pair<int,int>(1, 160000);
  maxFrequencyLimits_ = std::pair<int,int>(1, 1000000);
  maxFrequency2Limits_ = std::pair<int,int>(1, 1000000);
}

VNanotecSMCI36::~VNanotecSMCI36()
{

}

const std::string VNanotecSMCI36::GetStepModeName(int mode) const
{
  auto search = stepModeNames_.find(GetStepMode());
  if (search != stepModeNames_.end()) {
    return search->second;
  }
  return "unknown";
}

const std::string VNanotecSMCI36::GetErrorCorrectionModeName(int mode) const
{
  auto search = errorCorrectionModeNames_.find(GetErrorCorrectionMode());
  if (search != errorCorrectionModeNames_.end()) {
    return search->second;
  }
  return "unknown";
}

const std::string VNanotecSMCI36::GetInputPinFunctionName(int function) const
{
  auto search = inputPinFunctionNames_.find(function);
  if (search != inputPinFunctionNames_.end()) {
    return search->second;
  }
  return "unknown";
}

const std::string VNanotecSMCI36::GetOutputPinFunctionName(int function) const
{
  auto search = outputPinFunctionNames_.find(function);
  if (search != outputPinFunctionNames_.end()) {
    return search->second;
  }
  return "unknown";
}

const std::string VNanotecSMCI36::GetRampModeName(int mode) const
{
  auto search = rampModeNames_.find(mode);
  if (search != rampModeNames_.end()) {
    return search->second;
  }
  return "unknown";
}

const std::string VNanotecSMCI36::GetPositioningModeName(int mode) const
{
  auto search = positioningModeNames_.find(mode);
  if (search != positioningModeNames_.end()) {
    return search->second;
  }
  return "unknown";
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

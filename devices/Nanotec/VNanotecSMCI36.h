/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __VNANOTECSMCI36_H
#define __VNANOTECSMCI36_H

#include <string>
#include <map>
#include <utility>

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Nanotec
 *  @{
 */

typedef const char* ioport_t;

class VNanotecSMCI36
{
 public:

  enum StatusBits {
    smciReady     = 0x01,
    smciZero      = 0x02,
    smciPosError  = 0x04,
    smciInput1On  = 0x08,
    smciReserved4 = 0x10,
    smciReserved5 = 0x20,
    smciReserved6 = 0x40,
    smciReserved7 = 0x80
  };

  enum MotorType {
    smciStepper     = 0,
    smciBLDC        = 1,
    smciBLDCEncoder = 2
  };

  enum StepMode {
    smci01MicroStepsPerFullStep =   1,
    smci02MicroStepsPerFullStep =   2,
    smci04MicroStepsPerFullStep =   4,
    smci05MicroStepsPerFullStep =   5,
    smci08MicroStepsPerFullStep =   8,
    smci10MicroStepsPerFullStep =  10,
    smci16MicroStepsPerFullStep =  16,
    smci32MicroStepsPerFullStep =  32,
    smci64MicroStepsPerFullStep =  64,
    smciFeedRateMode            = 254,
    smciAdaptiveStepMode        = 255,
  };

  enum ErrorCorrectionMode {
    smciErrCorrectionOff          = 0,
    smciErrCorrectionAfterTravel  = 1,
    smciErrCorrectionDuringTravel = 2
  };

  enum InputPinFunction {
    smciIPinUserDefined          =  0,
    smciIStartRecordErrorReset   =  1,
    smciIRecordSelect0           =  2,
    smciIRecordSelect1           =  3,
    smciIRecordSelect2           =  4,
    smciIRecordSelect3           =  5,
    smciIRecordSelect4           =  6,
    smciIExternalReferenceSwitch =  7,
    smciITrigger                 =  8,
    smciIDirection               =  9,
    smciIEnable                  = 10,
    smciIClock                   = 11,
    smciIClockDirectionMode1     = 12,
    smciIClockDirectionMode2     = 13,
    smciIInvalid                 = 14
  };

  enum OutputPinFunction {
    smciOPinUserDefined          = 0,
    smciOReady                   = 1,
    smciORunning                 = 2,
    smciOError                   = 3,
    smciOInvalid                 = 4
  };

  enum IOBits {
    smciInput1            = 0x00000001,
    smciInput2            = 0x00000002,
    smciInput3            = 0x00000004,
    smciInput4            = 0x00000008,
    smciInput5            = 0x00000010,
    smciInput6            = 0x00000020,
    smciOutput1           = 0x00010000,
    smciOutput2           = 0x00020000,
    smciOutput3           = 0x00040000,
    smciBallastResistance = 0x01000000
  };

  enum RampMode {
    smciTrapezoidalRamp  = 0,
    smciSinusRamp        = 1,
    smciJerkFreeRamp     = 2
  };

  enum PositioningMode {
    smciRelativePositioning           =  1,
    smciAbsolutePositioning           =  2,
    smciInternalRefRun                =  3,
    smciExternalRefRun                =  4,
    smciSpeed                         =  5,
    smciFlagPositioning               =  6,
    smciClockDirectionLeft            =  7,
    smciClockDirectionRight           =  8,
    smciClockDirectionInternalRefRun  =  9,
    smciClockDirectionExternalRefRun  = 10,
    smciMaxPositioningMode
  };

  VNanotecSMCI36( const ioport_t );
  virtual ~VNanotecSMCI36();

 public:

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetFirmwareVersion() const = 0;
  virtual int GetStatus() const = 0;

  virtual void SetMotorType(int type) = 0;
  virtual int GetMotorType() const = 0;

  virtual void SetPhaseCurrent(int current) = 0;
  virtual int GetPhaseCurrent() const = 0;

  virtual void SetStandStillPhaseCurrent(int current) = 0;
  virtual int GetStandStillPhaseCurrent() const = 0;

  virtual void SetStepMode(int mode) = 0;
  virtual int GetStepMode() const = 0;
  const std::string GetStepModeName(int mode) const;
  const std::map<int,std::string>& GetStepModeNames() const {
    return stepModeNames_;
  }

  virtual void SetDriveAddress(int address) = 0;
  virtual int GetDriveAddress() = 0;

  virtual void SetMotorID(int ID) = 0;
  virtual int GetMotorID() const = 0;

  /* TODO: Limit Switch Behaviour */

  virtual void SetErrorCorrectionMode(int mode) = 0;
  virtual int GetErrorCorrectionMode() const = 0;
  const std::string GetErrorCorrectionModeName(int mode) const;
  const std::map<int,std::string>& GetErrorCorrectionModeNames() const {
    return errorCorrectionModeNames_;
  }

  virtual void SetEncoderDirection(bool direction) = 0;
  virtual bool GetEncoderDirection() const = 0;

  virtual void SetSwingOutTime(int time) = 0;
  virtual int GetSwingOutTime() const = 0;

  virtual void SetMaxEncoderDeviation(int deviation) = 0;
  virtual int GetMaxEncoderDeviation() const = 0;

  virtual int GetPosition() const = 0;
  virtual int GetEncoderPosition() const = 0;

  virtual void ResetPositionError(int position) = 0;

  virtual void SetInputPinFunction(int pin, int function) = 0;
  virtual int GetInputPinFunction(int pin) const = 0;
  const std::string GetInputPinFunctionName(int function) const;
  const std::map<int,std::string>& GetInputPinFunctionNames() const {
    return inputPinFunctionNames_;
  }

  virtual void SetOutputPinFunction(int pin, int function) = 0;
  virtual int GetOutputPinFunction(int pin) const = 0;
  const std::string GetOutputPinFunctionName(int function) const;
  const std::map<int,std::string>& GetOutputPinFunctionNames() const {
    return outputPinFunctionNames_;
  }

  virtual void SetIOMask(unsigned mask) = 0;
  virtual unsigned GetIOMask() const = 0;

  virtual void SetReversePolarityMask(unsigned mask) = 0;
  virtual unsigned GetReversePolarityMask() const = 0;

  virtual void SetIO(unsigned mask) = 0;
  virtual unsigned int GetIO() const = 0;

  unsigned int GetInputBitForPin(int pin) const;
  unsigned int GetOutputBitForPin(int pin) const;

  /* TODO: Debounce time of inputs */

  /* TODO: Setting the outputs */

  virtual void SetRampMode(int ramp) = 0;
  virtual int GetRampMode() const = 0;
  const std::string GetRampModeName(int mode) const;
  const std::map<int,std::string>& GetRampModeNames() const {
    return rampModeNames_;
  }

  virtual void SetQuickstopRamp(int ramp) = 0;
  virtual int GetQuickstopRamp() const = 0;

  virtual void SetQuickstopRampHzPerSecond(int ramp) = 0;
  virtual int GetQuickstopRampHzPerSecond() const = 0;

  virtual void SetAccelerationRamp(int ramp) = 0;
  virtual int GetAccelerationRamp() const = 0;

  virtual void SetAccelerationRampHzPerSecond(int ramp) = 0;
  virtual int GetAccelerationRampHzPerSecond() const = 0;

  virtual void SetDecelerationRamp(int ramp) = 0;
  virtual int GetDecelerationRamp() const = 0;

  virtual void SetDecelerationRampHzPerSecond(int ramp) = 0;
  virtual int GetDecelerationRampHzPerSecond() const = 0;

  virtual void SetPositioningMode(int mode) = 0;
  virtual int GetPositioningMode() const = 0;
  const std::string GetPositioningModeName(int mode) const;
  const std::map<int,std::string>& GetPositioningModeNames() const {
    return positioningModeNames_;
  }

  virtual void SetTravelDistance(int distance) = 0;
  virtual int GetTravelDistance() const = 0;

  virtual void SetDirection(bool direction) = 0;
  virtual bool GetDirection() const = 0;

  virtual void SetMinimumFrequency(int frequency) = 0;
  const std::pair<int,int>& GetMinFrequencyLimits() const { return minFrequencyLimits_; }
  virtual int GetMinimumFrequency() const = 0;

  virtual void SetMaximumFrequency(int frequency) = 0;
  const std::pair<int,int>& GetMaxFrequencyLimits() const { return maxFrequencyLimits_; }
  virtual int GetMaximumFrequency() const = 0;

  virtual void SetMaximumFrequency2(int frequency) = 0;
  const std::pair<int,int>& GetMaxFrequency2Limits() const { return maxFrequency2Limits_; }
  virtual int GetMaximumFrequency2() const = 0;

  virtual void Start() = 0;
  virtual void Stop(bool quickstop = false) = 0;

 protected:

  std::map<int,std::string> stepModeNames_;
  std::map<int,std::string> errorCorrectionModeNames_;
  std::map<int,std::string> inputPinFunctionNames_;
  std::map<int,std::string> outputPinFunctionNames_;
  std::map<int,std::string> rampModeNames_;
  std::map<int,std::string> positioningModeNames_;

  std::pair<int,int> minFrequencyLimits_;
  std::pair<int,int> maxFrequencyLimits_;
  std::pair<int,int> maxFrequency2Limits_;
};

/** @} */

/** @} */

#endif

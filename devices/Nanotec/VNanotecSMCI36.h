#ifndef __VNANOTECSMCI36_H
#define __VNANOTECSMCI36_H

#include <string>

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

  enum ReversePolaryBits {
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

  virtual void SetMotorID(int ID) = 0;
  virtual int GetMotorID() const = 0;

  /* TODO: Limit Switch Behaviour */

  virtual void SetInputPinFunction(int pin, int function) = 0;
  virtual int GetInputPinFunction(int pin) const = 0;

  virtual void SetOutputPinFunction(int pin, int function) = 0;
  virtual int GetOutputPinFunction(int pin) const = 0;

  virtual void SetReversePolarityMask(int mask) = 0;
  virtual int GetReversePolarityMask() const = 0;
};

#endif

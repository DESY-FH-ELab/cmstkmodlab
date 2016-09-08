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
};

#endif

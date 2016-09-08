#ifndef __NANOTECSMCI36_H
#define __NANOTECSMCI36_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VNanotecSMCI36.h"
#include "NanotecComHandler.h"

class NanotecSMCI36 : public VNanotecSMCI36
{
 public:

  NanotecSMCI36( const ioport_t );
  ~NanotecSMCI36();

 public:

  bool DeviceAvailable() const;

  std::string GetFirmwareVersion() const;
  int GetStatus() const;

  void SetMotorType(int type);
  int GetMotorType() const;

  void SetPhaseCurrent(int current);
  int GetPhaseCurrent() const;

  void SetStandStillPhaseCurrent(int current);
  int GetStandStillPhaseCurrent() const;

  void SetStepMode(int mode);
  int GetStepMode() const;

  void SetMotorID(int ID);
  int GetMotorID() const;

  void SetInputPinFunction(int pin, int function);
  int GetInputPinFunction(int pin) const;

  void SetOutputPinFunction(int pin, int function);
  int GetOutputPinFunction(int pin) const;

  void SetReversePolarityMask(int mask);
  int GetReversePolarityMask() const;

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  NanotecComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif

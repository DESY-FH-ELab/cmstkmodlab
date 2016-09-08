#ifndef __NANOTECSMCI36FAKE_H
#define __NANOTECSMCI36FAKE_H

#include "VNanotecSMCI36.h"

class NanotecSMCI36Fake : public VNanotecSMCI36
{
 public:

  NanotecSMCI36Fake( const ioport_t );
  ~NanotecSMCI36Fake();

 public:

  bool DeviceAvailable() const { return true; }

  std::string GetFirmwareVersion() const;
  int GetStatus() const;

  void SetMotorType(int type);
  int GetMotorType() const;

 private:

  int status_;
};

#endif

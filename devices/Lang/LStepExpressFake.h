#ifndef __LSTEPEXPRESSFAKE_H
#define __LSTEPEXPRESSFAKE_H

#include "VLStepExpress.h"

class LStepExpressFake : public VLStepExpress
{
 public:

  LStepExpressFake( const ioport_t );
  ~LStepExpressFake();

 public:

  bool DeviceAvailable() const { return true; }

  // low level debugging methods
  void SendCommand(const std::string & command);
  void ReceiveString(std::string &) { }

  private:

};

#endif

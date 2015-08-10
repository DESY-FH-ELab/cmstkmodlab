#ifndef __VLSTEPEXPRESS_H
#define __VLSTEPEXPRESS_H

#include <string>

typedef const char* ioport_t;

class VLStepExpress {

 public:

  VLStepExpress( const ioport_t );
  virtual ~VLStepExpress();

 public:

  virtual bool DeviceAvailable() const = 0;

  virtual unsigned int GetStatus() const = 0;

  // low level debugging methods
  virtual void SendCommand(const std::string &) = 0;
  virtual void ReceiveString(std::string &) = 0;
};

#endif

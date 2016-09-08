#ifndef __VNANOTECSMCI36_H
#define __VNANOTECSMCI36_H

#include <string>

typedef const char* ioport_t;

class VNanotecSMCI36
{
 public:

  VNanotecSMCI36( const ioport_t );
  virtual ~VNanotecSMCI36();

 public:

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetFirmwareVersion() const = 0;
};

#endif

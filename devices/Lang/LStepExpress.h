#ifndef __LSTEPEXPRESS_H
#define __LSTEPEXPRESS_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VLStepExpress.h"
#include "LStepExpressComHandler.h"

class LStepExpress : public VLStepExpress
{
 public:

  LStepExpress( const ioport_t );
  ~LStepExpress();

 public:

  bool DeviceAvailable() const;

  unsigned int GetStatus() const;

  // low level debugging methods
  void SendCommand(const std::string &);
  void ReceiveString(std::string &);

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  LStepExpressComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif

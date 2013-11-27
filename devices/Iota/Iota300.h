#ifndef __IOTA300_H
#define __IOTA300_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "VIota300.h"

class IotaComHandler;

class Iota300 : public VIota300
{
 public:

  Iota300( ioport_t );


  unsigned int GetPumpPressure( void ) const;
  void StripBuffer( char* ) const;

 private:

  void Device_Init( void );
  IotaComHandler* comHandler_;
  bool isCommunication_;

};

#endif

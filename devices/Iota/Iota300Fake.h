#ifndef __IOTA300FAKE_H
#define __IOTA300FAKE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "VIota300.h"

class Iota300Fake : public VIota300
{
 public:

  Iota300Fake( ioport_t );


  unsigned int GetPumpPressure( void ) const;


 private:

  float parameterProp_;
  int parameterIntl_;
  int parameterDiff_;
  void Device_Init( void );
  bool circulatorStatus;
};

#endif

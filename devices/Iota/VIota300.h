#ifndef __VIOTA300_H
#define __VIOTA300_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#define __IOTA_LOWER_TEMP_LIMIT -50
#define __IOTA_UPPER_TEMP_LIMIT  30

typedef const char* ioport_t;

class VIota300
{
 public:

  VIota300( ioport_t );
  virtual ~VIota300() {}
  

  virtual unsigned int GetPumpPressure( void ) const = 0;
 
};

#endif

#ifndef __VARDUINOPRES_H
#define __VARDUINOPRES_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

typedef const char* ioport_t;

class VArduinoPres
{
 public:

  VArduinoPres( ioport_t );
  virtual ~VArduinoPres() {}
  
  virtual bool IsCommunication( void ) const = 0;
  virtual float GetPressureA( void ) const = 0;
  virtual float GetPressureB( void ) const = 0;

};

#endif

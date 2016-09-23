#ifndef __VARDUINOMOTOR_H
#define __VARDUINOMOTOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

typedef const char* ioport_t;

class VArduinoMotor
{
 public:

  VArduinoMotor( ioport_t );
  virtual ~VArduinoMotor() {}
  
  virtual bool IsCommunication( void ) const = 0;
  virtual float GetPressureA( void ) const = 0;
  virtual float GetPressureB( void ) const = 0;

 protected:

  float ToPressure(int) const;
};

#endif

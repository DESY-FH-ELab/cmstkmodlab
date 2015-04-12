#ifndef __ARDUINOPRESFAKE_H
#define __ARDUINOPRESFAKE_H

#include "VArduinoPres.h"

class ArduinoPresFake : public VArduinoPres
{
 public:

  ArduinoPresFake( ioport_t );

  bool IsCommunication( void ) const { return true; }
  float GetPressureA( void ) const;
  float GetPressureB( void ) const;
  float GetFlow( void ) const;
 
 private:

  mutable float pressureA_; 
  mutable float pressureB_;
  mutable float flow_;
  void Device_Init( void );
};

#endif

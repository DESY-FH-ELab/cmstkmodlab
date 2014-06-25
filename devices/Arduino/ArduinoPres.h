#ifndef __ARDUINOPRES_H
#define __ARDUINOPRES_H

#include "VArduinoPres.h"

class ArduinoComHandler;

class ArduinoPres : public VArduinoPres
{
 public:

  ArduinoPres( ioport_t );

  bool IsCommunication( void ) const { return isCommunication_; }
  float GetPressureA( void ) const; //a
  float GetPressureB( void ) const; //b

 private:

  const int uDelay_;
  void StripBuffer( char* ) const;

  void Device_Init( void );
  ArduinoComHandler* comHandler_;
  bool isCommunication_;
};

#endif

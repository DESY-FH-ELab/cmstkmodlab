#ifndef __ARDUINOMOTOR_H
#define __ARDUINOMOTOR_H

#include "VArduinoMotor.h"

class ArduinoComHandler;

class ArduinoMotor : public VArduinoMotor
{
 public:

  ArduinoMotor( ioport_t );
  ~ArduinoMotor();

  void SendCommand( const char* );
  bool IsCommunication( void ) const;
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

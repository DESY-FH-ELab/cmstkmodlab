#ifndef __IOTA300_H
#define __IOTA300_H

#include "VIota300.h"

class Iota300ComHandler;

class Iota300 : public VIota300
{
 public:

  Iota300( ioport_t );

  bool SetFlow( const float ) const; //#FSw
  bool SetPressure( const float ) const; //#PSw
  bool SetStatus( const float ) const; //#STw
  bool SetPumpOn( void ) const;
  bool SetPumpOff( void ) const;

  bool IsCommunication( void ) const { return isCommunication_; }
  float GetSetFlow( void ) const; //#FSr
  float GetActFlow( void ) const; //#FAr
  float GetSetPressure( void ) const; //#PSr
  float GetActPressure( void ) const; //#PAr
  float GetStatus( void ) const; //#STr

 private:

  const int uDelay_;
  void StripBuffer( char* ) const;
  int ToInteger(const char*) const;
  float ToFloat(const char*) const;

  void Device_Init( void );
  Iota300ComHandler* comHandler_;
  bool isCommunication_;
};

#endif

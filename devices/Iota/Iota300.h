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
  bool SetStatus( const void ) const; //#STw
  //bool SetControlParameters( float, int, int ) const; //not included yet -> switch external/internal mode?

  bool IsCommunication( void ) const { return isCommunication_; }
  float GetFlow( void ) const; //#FSr
  float GetPressure( void ) const; //#PAr
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

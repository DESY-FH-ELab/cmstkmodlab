#ifndef __IOTA300FAKE_H
#define __IOTA300FAKE_H

#include "VIota300.h"

class Iota300Fake : public VIota300
{
 public:

  Iota300Fake( ioport_t );

  bool SetFlow( const float ) const;
  bool SetPressure( const float ) const;
  bool SetStatus( const float ) const;
  //bool SetControlParameters( float, int, int ) const;

  bool IsCommunication( void ) const { return true; }
  float GetFlow( void ) const;
  float GetPressure( void ) const;
  float GetStatus( void ) const;
 
 private:

  mutable float status_;
  mutable float flow_;
  mutable float pressure_;  
  void Device_Init( void );
};

#endif

#ifndef __CoriFlowFAKE_H
#define __CoriFlowFAKE_H

#include "VCoriFlow.h"

class CoriFlowFake : public VCoriFlow
{
 public:

  CoriFlowFake( ioport_t );

 
  bool IsCommunication( void ) const { return true; }
  virtual float test( void ) const;

 
 private:
  float test_;

  void Device_Init( void );
};

#endif

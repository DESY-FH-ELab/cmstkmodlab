#ifndef __CoriFlowFAKE_H
#define __CoriFlowFAKE_H

#include "VCoriFlow.h"

class CoriFlowFake : public VCoriFlow
{
 public:

  CoriFlowFake( ioport_t );

 
  bool IsCommunication( void ) const { return true; }

  virtual float getTemp( void ) const;
  virtual float getPres( void ) const;
  virtual float getMeasure( void ) const;
  /// additional stuff
  virtual float getCapacity( void ) const;
  virtual std::string getUnit( void ) const;
  virtual std::string getFluid( void ) const;
  virtual std::string getTag( void ) const;
  virtual std::string setInit( void ) const;
  virtual std::string setTag( void ) const;
  virtual float getDensity( void ) const;
  virtual std::string setCapacity( void ) const;
};

#endif

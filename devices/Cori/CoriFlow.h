#ifndef __CORIFLOW_H
#define __CORIFLOW_H

#include "VCoriFlow.h"
#include <string>
using std::string;

class CoriFlowComHandler;

class CoriFlow : public VCoriFlow
{
 public:

  CoriFlow( ioport_t );
  ~CoriFlow();

  bool IsCommunication( void ) const;
  float getTemp( void ) const;
  float getPres( void ) const;
  float getMeasure( void ) const;
  /// /////////////////////////////////////////////
  float getCapacity( void ) const;
  float getDensity( void ) const;
  std::string getUnit( void ) const;
  std::string getFluid( void ) const;
  std::string getTag( void ) const;
  std::string setTag( void ) const;
  std::string setCapacity( void ) const;
  std::string setInit( void ) const;

 private:

  const int uDelay_;
  void StripBuffer( char* ) const;


  void Device_Init( void );
  CoriFlowComHandler* comHandler_;
  bool isCommunication_;
};

#endif


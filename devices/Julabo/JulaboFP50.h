#ifndef __JULABOFP50_H
#define __JULABOFP50_H

#include "VJulaboFP50.h"

class FP50ComHandler;

class JulaboFP50 : public VJulaboFP50
{
 public:

  JulaboFP50( ioport_t );

  bool SetWorkingTemperature( const float ) const;
  bool SetPumpPressure( const unsigned int ) const;
  bool SetCirculatorOn( void ) const;
  bool SetCirculatorOff( void ) const;
  bool SetControlParameters( float, int, int ) const;

  bool IsCommunication( void ) const { return isCommunication_; }
  float GetBathTemperature( void ) const;
  float GetSafetySensorTemperature( void ) const;
  float GetWorkingTemperature( void ) const;
  int GetHeatingPower( void ) const;
  unsigned int GetPumpPressure( void ) const;
  bool GetCirculatorStatus( void ) const;
  std::pair<int,std::string> GetStatus( void ) const;
  float GetProportionalParameter( void ) const;
  int GetIntegralParameter( void ) const;
  int GetDifferentialParameter( void ) const;
  
  bool SaveControlParameters( const std::string& ) const;
  bool LoadControlParametersAndApply( const std::string& ) const;
  void StripBuffer( char* ) const;

 private:

  void Device_Init( void );
  FP50ComHandler* comHandler_;
  bool isCommunication_;

};

#endif

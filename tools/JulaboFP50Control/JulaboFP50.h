
#ifndef __FP50COMHANDLER_H
#define __FP50COMHANDLER_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "FP50ComHandler.h"


#define __FP50_LOWER_TEMP_LIMIT -50
#define __FP50_UPPER_TEMP_LIMIT  30



class JulaboFP50 {

 public:

  JulaboFP50( const ioport_t );

 public:

  bool SetWorkingTemp( const float ) const;
  bool SetPumpPressure( const unsigned int ) const;
  bool SetCirculatorOn( void ) const;
  bool SetCirculatorOff( void ) const;
  bool SetControlParameters( float, int, int ) const;

  float GetBathTemperature( void ) const;
  float GetSafetySensorTemperature( void ) const;
  unsigned int GetHeatingPower( void ) const;
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

};

#endif

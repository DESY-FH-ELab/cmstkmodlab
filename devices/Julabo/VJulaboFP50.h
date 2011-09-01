#ifndef __VJULABOFP50_H
#define __VJULABOFP50_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#define __FP50_LOWER_TEMP_LIMIT -50
#define __FP50_UPPER_TEMP_LIMIT  30

typedef const char* ioport_t;

class VJulaboFP50
{
 public:

  VJulaboFP50( ioport_t );

  virtual bool SetWorkingTemperature( const float ) const = 0;
  virtual bool SetPumpPressure( const unsigned int ) const = 0;
  virtual bool SetCirculatorOn( void ) const = 0;
  virtual bool SetCirculatorOff( void ) const = 0;
  virtual bool SetControlParameters( float, int, int ) const = 0;

  virtual float GetBathTemperature( void ) const = 0;
  virtual float GetSafetySensorTemperature( void ) const = 0;
  virtual float GetWorkingTemperature( void ) const = 0;
  virtual int GetHeatingPower( void ) const = 0;
  virtual unsigned int GetPumpPressure( void ) const = 0;
  virtual bool GetCirculatorStatus( void ) const = 0;
  virtual std::pair<int,std::string> GetStatus( void ) const = 0;
  virtual float GetProportionalParameter( void ) const = 0;
  virtual int GetIntegralParameter( void ) const = 0;
  virtual int GetDifferentialParameter( void ) const = 0;

  virtual bool SaveControlParameters( const std::string& ) const = 0;
  virtual bool LoadControlParametersAndApply( const std::string& ) const = 0;
};

#endif

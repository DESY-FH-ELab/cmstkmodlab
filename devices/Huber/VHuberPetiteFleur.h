#ifndef __VHUBERPETITEFLEUR_H
#define __VHUBERPETITEFLEUR_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#define __PETITEFLEUR_LOWER_TEMP_LIMIT -40
#define __PETITEFLEUR_UPPER_TEMP_LIMIT  40

typedef const char* ioport_t;

class VHuberPetiteFleur
{
 public:

  VHuberPetiteFleur( ioport_t );
  virtual ~VHuberPetiteFleur() {}
  
  virtual bool SetWorkingTemperature( const float ) const = 0;
  virtual bool SetPumpPressure( const unsigned int ) const = 0;
  virtual bool SetCirculatorOn( void ) const = 0;
  virtual bool SetCirculatorOff( void ) const = 0;
  virtual bool SetControlParameters( float, int, int ) const = 0;

  virtual bool IsCommunication( void ) const = 0;
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

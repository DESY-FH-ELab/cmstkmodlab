#ifndef __HUBERPETITEFLEUR_H
#define __HUBERPETITEFLEUR_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "VHuberPetiteFleur.h"

class PetiteFleurComHandler;

class HuberPetiteFleur : public VHuberPetiteFleur
{
 public:

  HuberPetiteFleur( ioport_t );

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
  PetiteFleurComHandler* comHandler_;
  bool isCommunication_;
};

#endif

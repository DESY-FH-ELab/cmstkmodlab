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

  bool SetWorkingTemperature( const float ) const; //SP@
  bool SetCirculatorOn( void ) const; //CA@ +00001
  bool SetCirculatorOff( void ) const; //CA@ +00000
  //bool SetControlParameters( float, int, int ) const; //not included yet -> switch external/internal mode?

  bool IsCommunication( void ) const { return isCommunication_; }
  float GetBathTemperature( void ) const; //TI?
  float GetWorkingTemperature( void ) const; //SP?
  bool GetCirculatorStatus( void ) const; //CA?

 private:

  void StripBuffer( char* ) const;
  int ToInteger(const char*) const;
  float ToFloat(const char*) const;

  void Device_Init( void );
  PetiteFleurComHandler* comHandler_;
  bool isCommunication_;
};

#endif

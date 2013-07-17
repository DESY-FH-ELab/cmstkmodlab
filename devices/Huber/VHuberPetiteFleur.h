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
  virtual bool SetCirculatorOn( void ) const = 0;
  virtual bool SetCirculatorOff( void ) const = 0;
  //virtual bool SetControlParameters( float, int, int ) const = 0;

  virtual bool IsCommunication( void ) const = 0;
  virtual float GetBathTemperature( void ) const = 0;
  virtual float GetWorkingTemperature( void ) const = 0;
  virtual bool GetCirculatorStatus( void ) const = 0;
};

#endif

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __HUBERPETITEFLEUR_H
#define __HUBERPETITEFLEUR_H

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

  const int uDelay_;
  void StripBuffer( char* ) const;
  int ToInteger(const char*) const;
  float ToFloat(const char*) const;

  void Device_Init( void );
  PetiteFleurComHandler* comHandler_;
  bool isCommunication_;
};

#endif

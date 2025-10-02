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

#ifndef __VHUBERPETITEFLEUR_H
#define __VHUBERPETITEFLEUR_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

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

  static constexpr int PetiteFleurLowerTempLimit = -40;
  static constexpr int PetiteFleurUpperTempLimit = 40;
};

#endif

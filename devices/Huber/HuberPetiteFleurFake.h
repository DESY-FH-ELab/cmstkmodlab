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

#ifndef __HUBERPETITEFLEURFAKE_H
#define __HUBERPETITEFLEURFAKE_H

#include "VHuberPetiteFleur.h"

class HuberPetiteFleurFake : public VHuberPetiteFleur
{
 public:

  HuberPetiteFleurFake( ioport_t );

  bool SetWorkingTemperature( const float ) const;
  bool SetCirculatorOn( void ) const;
  bool SetCirculatorOff( void ) const;
  //bool SetControlParameters( float, int, int ) const;

  bool IsCommunication( void ) const { return true; }
  float GetBathTemperature( void ) const;
  float GetWorkingTemperature( void ) const;
  bool GetCirculatorStatus( void ) const;
 
 private:

  mutable bool circulatorStatus_;
  mutable float workingTemp_;
  void Device_Init( void );
};

#endif

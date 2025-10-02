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

#include <unistd.h>

#include <iostream>
#include <string>

//#####################
// TODO:
// query error codes
//#####################

#include "HuberPetiteFleurFake.h"

///
///
///
HuberPetiteFleurFake::HuberPetiteFleurFake( const ioport_t ioPort )
    :VHuberPetiteFleur(ioPort)
{
  circulatorStatus_ = false;
  workingTemp_ = -12.34;
}

///
/// returns success flag
///
bool HuberPetiteFleurFake::SetWorkingTemperature( const float workingTemp ) const {

  if( workingTemp > PetiteFleurUpperTempLimit || workingTemp < PetiteFleurLowerTempLimit ) {
    std::cerr << " [HuberPetiteFleurFake::SetWorkingTemp] ** ERROR: working temp T="
	          << workingTemp << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. HuberPetiteFleurFake class definition)" << std::endl;
    return false;
  }

  std::cout << " [HuberPetiteFleurFake::SetWorkingTemp] -- FAKE: Setting workingTemp = " 
	        << workingTemp << "." << std::endl;

  workingTemp_ = workingTemp;

  return true;
}

///
/// return success flag
///
bool HuberPetiteFleurFake::SetCirculatorOn( void ) const
{
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOn] -- FAKE: Setting circulator ON "
            << std::endl;

  circulatorStatus_ = true;

  return true;
}



///
/// return success flag
///
bool HuberPetiteFleurFake::SetCirculatorOff( void ) const
{
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOff] -- FAKE: Setting circulator OFF"
            << std::endl;

  circulatorStatus_ = false;

  return true;
}

///
///
///
float HuberPetiteFleurFake::GetBathTemperature( void ) const
{
  std::cout << " [HuberPetiteFleurFake::GetBathTemperature] -- FAKE: Returning T = "
	        << workingTemp_ << std::endl;
  usleep( 10000 );
  return workingTemp_ + (double)rand() / RAND_MAX;
}

///
///
///
float HuberPetiteFleurFake::GetWorkingTemperature( void ) const
{
  std::cout << " [HuberPetiteFleurFake::GetWorkingTemperature] -- FAKE: Returning T = "
	        << workingTemp_ << std::endl;
  usleep( 10000 );
  return workingTemp_;
}

///
/// true = on / false = off
///
bool HuberPetiteFleurFake::GetCirculatorStatus( void ) const {

  std::cout << " [HuberPetiteFleurFake::GetCirculatorStatus] -- FAKE: Returning: " 
	        << (circulatorStatus_?"TRUE":"FALSE") << std::endl;
  usleep( 10000 );
  return circulatorStatus_;
}

///
///
///
void HuberPetiteFleurFake::Device_Init( void ) {

}

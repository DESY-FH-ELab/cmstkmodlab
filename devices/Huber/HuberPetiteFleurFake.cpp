#include <unistd.h>

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

  if( workingTemp > __PETITEFLEUR_UPPER_TEMP_LIMIT || workingTemp < __PETITEFLEUR_LOWER_TEMP_LIMIT ) {
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
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOn] -- FAKE: Setting circulator ON " << std::endl;

  circulatorStatus_ = true;

  return true;
}



///
/// return success flag
///
bool HuberPetiteFleurFake::SetCirculatorOff( void ) const
{
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOff] -- FAKE: Setting circulator OFF" << std::endl;

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
  return workingTemp_;
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

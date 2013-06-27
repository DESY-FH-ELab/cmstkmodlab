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
  circulatorStatus = false;
  parameterProp_ = 77.2;
  parameterIntl_ = 3456;
  parameterDiff_ = 128;
}

///
/// returns success flag
///
bool HuberPetiteFleurFake::SetWorkingTemperature( const float workingTemp ) const {

  if( workingTemp > __PETITEFLEUR_UPPER_TEMP_LIMIT || workingTemp < __PETITEFLEUR_LOWER_TEMP_LIMIT ) {
    std::cerr << " [HuberPetiteFleurFake::SetWorkingTemp] ** ERROR: working temp T=" << workingTemp << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. HuberPetiteFleurFake class definition)" << std::endl;
    return false;
  }

  std::cout << " [HuberPetiteFleurFake::SetWorkingTemp] -- FAKE: Setting workingTemp = " << workingTemp << "." << std::endl;

  return true;

}

///
/// return success flag
/// pressureStage = 1..4
///
bool HuberPetiteFleurFake::SetPumpPressure( const unsigned int pressureStage ) const {

  if( pressureStage < 1 || pressureStage > 4 ) {
    std::cerr << " [HuberPetiteFleurFake::SetPumpPressure] ** ERROR: Invalid pressure stage: " << pressureStage << "." << std::endl;
    return false;
  }

  std::cout << " [HuberPetiteFleurFake::SetPumpPressure] -- FAKE: Setting pressureStage = " << pressureStage << "." << std::endl;

  return true;

}

///
/// return success flag
///
bool HuberPetiteFleurFake::SetCirculatorOn( void ) const
{
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOn] -- FAKE: Setting circulator ON " << std::endl;

  return true;
}



///
/// return success flag
///
bool HuberPetiteFleurFake::SetCirculatorOff( void ) const
{
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOff] -- FAKE: Setting circulator OFF" << std::endl;

  return true;
}



///
/// return success flag
/// xp = prop. / tn = int / tv = diff
///
bool HuberPetiteFleurFake::SetControlParameters( float xp, int tn, int tv ) const
{
  std::cout << " [HuberPetiteFleurFake::SetCirculatorOff] -- FAKE: Setting control pars: ";
  std::cout << "xp=" << xp << " tn=" << tn << " tv=" << tv << "." << std::endl;
  
  return true;
}

///
///
///
float HuberPetiteFleurFake::GetBathTemperature( void ) const
{
  std::cout << " [HuberPetiteFleurFake::GetBathTemperature] -- FAKE: Returning T = -23.5" << std::endl;
  usleep( 10000 );
  return -23.5;
}

///
///
///
float HuberPetiteFleurFake::GetSafetySensorTemperature( void ) const
{
  std::cout << " [HuberPetiteFleurFake::GetSafetySensorTemperature] -- FAKE: Returning T = -21.0" << std::endl;
  usleep( 10000 );
  return -21.0;

}

///
///
///
float HuberPetiteFleurFake::GetWorkingTemperature( void ) const
{
  std::cout << " [HuberPetiteFleurFake::GetWorkingTemperature] -- FAKE: Returning T = -23.5" << std::endl;
  usleep( 10000 );
  return -23.5;
}

///
///
///
int HuberPetiteFleurFake::GetHeatingPower( void ) const
{
  std::cout << " [HuberPetiteFleurFake::GetHeatingPower] -- FAKE: Returning P = 34 %" << std::endl;
  usleep( 10000 );
  return 34;
}

///
///
///
unsigned int HuberPetiteFleurFake::GetPumpPressure( void ) const {
  
  std::cout << " [HuberPetiteFleurFake::GetPumpPressure] -- FAKE: Returning: 2" << std::endl;
  usleep( 10000 );
  return 2;

}



///
/// true = on / false = off
///
bool HuberPetiteFleurFake::GetCirculatorStatus( void ) const {

  std::cout << " [HuberPetiteFleurFake::GetCirculatorStatus] -- FAKE: Returning: " << (circulatorStatus?"TRUE":"FALSE") << std::endl;
  usleep( 10000 );
  return circulatorStatus;

}



///
///
///
std::pair<int,std::string> HuberPetiteFleurFake::GetStatus( void ) const {

  const int out1 = -13;
  const std::string out2("WARNING : VALUE EXCEEDS TEMPERATURE LIMITS");

  
  std::cout << " [HuberPetiteFleurFake::GetStatus] -- FAKE: returning:" << std::endl;
  std::cout << "  > " << out1 << " / " << out2 << std::endl;

  usleep( 10000 );
  return std::pair<int,std::string>( out1, out2 );

}



///
///
///
float HuberPetiteFleurFake::GetProportionalParameter( void ) const {

  std::cout << " [HuberPetiteFleurFake::GetProportionalParameter] -- FAKE: returning: " << parameterProp_ << std::endl;
  usleep( 10000 );
  return( parameterProp_ );

}



///
///
///
int HuberPetiteFleurFake::GetIntegralParameter( void ) const {

  std::cout << " [HuberPetiteFleurFake::GetIntegralParameter] -- FAKE: returning: " <<  parameterIntl_ << std::endl;
  usleep( 10000 );
  return( parameterIntl_ );

}



///
///
///
int HuberPetiteFleurFake::GetDifferentialParameter( void ) const {

  std::cout << " [HuberPetiteFleurFake::GetDifferentialParameter] -- FAKE: returning: " << parameterDiff_ << std::endl;
  usleep( 10000 );
  return( parameterDiff_ );

}



///
///
///
bool HuberPetiteFleurFake::SaveControlParameters( const std::string& filepath ) const {

  std::ofstream file( filepath.c_str() );
  if ( file.bad() ) {
    std::cerr << " [HuberPetiteFleurFake::SaveControlParameters] ** ERROR: Could not save to file:" << std::endl;
    std::cerr << "  > \"" << filepath.c_str() << "\"." << std::endl;
    return false;
  }

  file << "<PID_PROP> " << GetProportionalParameter() << std::endl;
  file << "<PID_INTL> " << GetIntegralParameter() << std::endl;
  file << "<PID_DIFF> " << GetDifferentialParameter() << std::endl;
  
  file.close();
  return true;

}



///
/// return success flag
///
bool HuberPetiteFleurFake::LoadControlParametersAndApply( const std::string& filepath ) const
{

  std::ifstream file( filepath.c_str(), std::ios::in );
  if ( file.bad() ) {
    std::cerr << " [HuberPetiteFleurFake::LoadControlParametersAndApply] ** ERROR: Could not read from file:" << std::endl;
    std::cerr << "  > \"" << filepath.c_str() << "\"." << std::endl;
    return false;
  }

  std::string item;
  float propP = 0.; bool isProp = false;
  int intlP = 0; bool isIntl = false;
  int diffP = 0; bool isDiff = false;

  while( ! ( ( isProp && isIntl && isDiff ) || file.eof() ) ) {

    file >> item;
    std::cout << "##########: " << item << std::endl; /////////////////////////////////

    if( std::string::npos != item.find( "<PID_PROP>" ) ) {
      file >> propP;
      isProp = true;
      std::cout << "FOUND PROP " << propP << std::endl; /////////////////////////////////
    }

    if( std::string::npos != item.find( "<PID_INTL>" ) ) {
      file >> intlP;
      isIntl = true;
      std::cout << "FOUND INTL " << intlP <<  std::endl; /////////////////////////////////
    }

    if( std::string::npos != item.find( "<PID_DIFF>" ) ) {
      file >> diffP;
      isDiff = true;
      std::cout << "FOUND DIFF " << diffP << std::endl; /////////////////////////////////
    }

  }

  file.close();

  if( isProp && isIntl && isDiff ) {
    SetControlParameters( propP, intlP, diffP );
    return true;
  } 
  else {
    std::cerr << " [LoadControlParametersAndApply] ** ERROR: could not parse input file: " << std::endl;
    std::cerr << "  > " << filepath.c_str() << "." << std::endl;
    return false;
  }

}



///
///
///
void HuberPetiteFleurFake::Device_Init( void ) {

  
}

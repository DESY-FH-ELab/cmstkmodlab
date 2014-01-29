#include <unistd.h>

//#####################
// TODO:
// query error codes
//#####################

#include "JulaboFP50Fake.h"

///
///
///
JulaboFP50Fake::JulaboFP50Fake( const ioport_t ioPort )
    :VJulaboFP50(ioPort)
{
  circulatorStatus = false;
  parameterProp_ = 77.2;
  parameterIntl_ = 3456;
  parameterDiff_ = 128;
}

///
/// returns success flag
///
bool JulaboFP50Fake::SetWorkingTemperature( const float workingTemp ) const {

  if( workingTemp > FP50UpperTempLimit || workingTemp < FP50LowerTempLimit ) {
    std::cerr << " [JulaboFP50Fake::SetWorkingTemp] ** ERROR: working temp T="
              << workingTemp << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. JulaboFP50Fake class definition)" << std::endl;
    return false;
  }

  std::cout << " [JulaboFP50Fake::SetWorkingTemp] -- FAKE: Setting workingTemp = "
            << workingTemp << "." << std::endl;

  return true;

}

///
/// return success flag
/// pressureStage = 1..4
///
bool JulaboFP50Fake::SetPumpPressure( const unsigned int pressureStage ) const {

  if( pressureStage < 1 || pressureStage > 4 ) {
    std::cerr << " [JulaboFP50Fake::SetPumpPressure] ** ERROR: Invalid pressure stage: "
              << pressureStage << "." << std::endl;
    return false;
  }

  std::cout << " [JulaboFP50Fake::SetPumpPressure] -- FAKE: Setting pressureStage = "
            << pressureStage << "." << std::endl;

  return true;

}

///
/// return success flag
///
bool JulaboFP50Fake::SetCirculatorOn( void ) const
{
  std::cout << " [JulaboFP50Fake::SetCirculatorOn] -- FAKE: Setting circulator ON "
            << std::endl;

  return true;
}

///
/// return success flag
///
bool JulaboFP50Fake::SetCirculatorOff( void ) const
{
  std::cout << " [JulaboFP50Fake::SetCirculatorOff] -- FAKE: Setting circulator OFF"
            << std::endl;

  return true;
}

///
/// return success flag
/// xp = prop. / tn = int / tv = diff
///
bool JulaboFP50Fake::SetControlParameters( float xp, int tn, int tv ) const
{
  std::cout << " [JulaboFP50Fake::SetCirculatorOff] -- FAKE: Setting control pars: ";
  std::cout << "xp=" << xp << " tn=" << tn << " tv=" << tv << "." << std::endl;
  
  return true;
}

///
///
///
float JulaboFP50Fake::GetBathTemperature( void ) const
{
  std::cout << " [JulaboFP50Fake::GetBathTemperature] -- FAKE: Returning T = -23.5"
            << std::endl;
  usleep( 10000 );
  return -23.5;
}

///
///
///
float JulaboFP50Fake::GetSafetySensorTemperature( void ) const
{
  std::cout << " [JulaboFP50Fake::GetSafetySensorTemperature] -- FAKE: Returning T = -21.0"
            << std::endl;
  usleep( 10000 );
  return -21.0;

}

///
///
///
float JulaboFP50Fake::GetWorkingTemperature( void ) const
{
  std::cout << " [JulaboFP50Fake::GetWorkingTemperature] -- FAKE: Returning T = -23.5"
            << std::endl;
  usleep( 10000 );
  return -23.5;
}

///
///
///
int JulaboFP50Fake::GetHeatingPower( void ) const
{
  std::cout << " [JulaboFP50Fake::GetHeatingPower] -- FAKE: Returning P = 34 %"
            << std::endl;
  usleep( 10000 );
  return 34;
}

///
///
///
unsigned int JulaboFP50Fake::GetPumpPressure( void ) const {
  
  std::cout << " [JulaboFP50Fake::GetPumpPressure] -- FAKE: Returning: 2"
            << std::endl;
  usleep( 10000 );
  return 2;

}

///
/// true = on / false = off
///
bool JulaboFP50Fake::GetCirculatorStatus( void ) const {

  std::cout << " [JulaboFP50Fake::GetCirculatorStatus] -- FAKE: Returning: "
            << (circulatorStatus?"TRUE":"FALSE") << std::endl;
  usleep( 10000 );
  return circulatorStatus;

}

///
///
///
std::pair<int,std::string> JulaboFP50Fake::GetStatus( void ) const {

  const int out1 = -13;
  const std::string out2("WARNING : VALUE EXCEEDS TEMPERATURE LIMITS");

  
  std::cout << " [JulaboFP50Fake::GetStatus] -- FAKE: returning:" << std::endl;
  std::cout << "  > " << out1 << " / " << out2 << std::endl;

  usleep( 10000 );
  return std::pair<int,std::string>( out1, out2 );

}

///
///
///
float JulaboFP50Fake::GetProportionalParameter( void ) const {

  std::cout << " [JulaboFP50Fake::GetProportionalParameter] -- FAKE: returning: "
            << parameterProp_ << std::endl;
  usleep( 10000 );
  return( parameterProp_ );
}

///
///
///
int JulaboFP50Fake::GetIntegralParameter( void ) const {

  std::cout << " [JulaboFP50Fake::GetIntegralParameter] -- FAKE: returning: "
            <<  parameterIntl_ << std::endl;
  usleep( 10000 );
  return( parameterIntl_ );
}

///
///
///
int JulaboFP50Fake::GetDifferentialParameter( void ) const {

  std::cout << " [JulaboFP50Fake::GetDifferentialParameter] -- FAKE: returning: "
            << parameterDiff_ << std::endl;
  usleep( 10000 );
  return( parameterDiff_ );
}

///
///
///
bool JulaboFP50Fake::SaveControlParameters( const std::string& filepath ) const {

  std::ofstream file( filepath.c_str() );
  if ( file.bad() ) {
    std::cerr << " [JulaboFP50Fake::SaveControlParameters] ** ERROR: Could not save to file:"
              << std::endl;
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
bool JulaboFP50Fake::LoadControlParametersAndApply( const std::string& filepath ) const
{

  std::ifstream file( filepath.c_str(), std::ios::in );
  if ( file.bad() ) {
    std::cerr << " [JulaboFP50Fake::LoadControlParametersAndApply] ** ERROR: Could not read from file:"
              << std::endl;
    std::cerr << "  > \"" << filepath.c_str() << "\"." << std::endl;
    return false;
  }

  std::string item;
  float propP = 0.; bool isProp = false;
  int intlP = 0; bool isIntl = false;
  int diffP = 0; bool isDiff = false;

  while( ! ( ( isProp && isIntl && isDiff ) || file.eof() ) ) {

    file >> item;
    std::cout << "##########: " << item << std::endl;

    if( std::string::npos != item.find( "<PID_PROP>" ) ) {
      file >> propP;
      isProp = true;
      std::cout << "FOUND PROP " << propP << std::endl;
    }

    if( std::string::npos != item.find( "<PID_INTL>" ) ) {
      file >> intlP;
      isIntl = true;
      std::cout << "FOUND INTL " << intlP <<  std::endl;
    }

    if( std::string::npos != item.find( "<PID_DIFF>" ) ) {
      file >> diffP;
      isDiff = true;
      std::cout << "FOUND DIFF " << diffP << std::endl;
    }

  }

  file.close();

  if( isProp && isIntl && isDiff ) {
    SetControlParameters( propP, intlP, diffP );
    return true;
  } 
  else {
    std::cerr << " [LoadControlParametersAndApply] ** ERROR: could not parse input file: "
              << std::endl;
    std::cerr << "  > " << filepath.c_str() << "." << std::endl;
    return false;
  }
}

///
///
///
void JulaboFP50Fake::Device_Init( void ) {

}

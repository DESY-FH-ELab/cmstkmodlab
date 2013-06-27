

//#####################
// TODO:
// query error codes
//#####################


#include "PetiteFleurComHandler.h"

#include "HuberPetiteFleur.h"

///
///
///
HuberPetiteFleur::HuberPetiteFleur( const ioport_t ioPort )
  : VHuberPetiteFleur(ioPort)
{
  comHandler_ = new PetiteFleurComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}



///
/// returns success flag
///
bool HuberPetiteFleur::SetWorkingTemperature( const float workingTemp ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::SetWorkingTemp] -- DEBUG: Called." << std::endl;
  #endif

  if( workingTemp > __PETITEFLEUR_UPPER_TEMP_LIMIT || workingTemp < __PETITEFLEUR_LOWER_TEMP_LIMIT ) {
    std::cerr << " [HuberPetiteFleur::SetWorkingTemp] ** ERROR: working temp T=" << workingTemp << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. HuberPetiteFleur class definition)" << std::endl;
    return false;
  }

  char buffer[1000];

  stringstream theCommand;
  theCommand << "out_sp_00 " << workingTemp;
  comHandler_->SendCommand( theCommand.str().c_str() );
  usleep( 20000 );
  comHandler_->SendCommand( "in_sp_00" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( fabs( workingTemp - atof( buffer ) ) > 1.e-3 ) {
    std::cerr << " [HuberPetiteFleur::SetWorkingTemp] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: T=" << workingTemp << " but received (string):" << buffer << "." << std::endl;
    return false;
  }

  return true;

}



///
/// return success flag
/// pressureStage = 1..4
///
bool HuberPetiteFleur::SetPumpPressure( const unsigned int pressureStage ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::SetPumpPressure] -- DEBUG: Called." << std::endl;
  #endif

  if( pressureStage < 1 || pressureStage > 4 ) {
    std::cerr << " [HuberPetiteFleur::SetPumpPressure] ** ERROR: Invalid pressure stage: " << pressureStage << "." << std::endl;
    return false;
  }
  
  char buffer[1000];

  stringstream theCommand;
  theCommand << "out_sp_07 " << pressureStage;
  comHandler_->SendCommand( theCommand.str().c_str() );
  usleep( 20000 );
  comHandler_->SendCommand( "in_sp_07" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( static_cast<unsigned int>( atoi( buffer ) ) != pressureStage ) {
    std::cerr << " [HuberPetiteFleur::SetPumpPressure] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: P=" << pressureStage << " but received (string):" << buffer << "." << std::endl;
    return false;
  }

  return true;

}



///
/// return success flag
///
bool HuberPetiteFleur::SetCirculatorOn( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::SetCirculatorOn] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "out_mode_05 1" );
  usleep( 20000 );
  comHandler_->SendCommand( "in_mode_05" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( static_cast<unsigned int>( atoi( buffer ) ) != 1 ) {
    std::cerr << " [HuberPetiteFleur::SetCirculatorOn] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: ON (1) but received (string):" << buffer << "." << std::endl;
    return false;
  }

  return true;

}



///
/// return success flag
///
bool HuberPetiteFleur::SetCirculatorOff( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::SetCirculatorOf] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "out_mode_05 0" );
  usleep( 20000 );
  comHandler_->SendCommand( "in_mode_05" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( static_cast<unsigned int>( atoi( buffer ) ) != 0 ) {
    std::cerr << " [HuberPetiteFleur::SetCirculatorOff] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: OFF (0) but received (string):" << buffer << "." << std::endl;
    return false;
  }

  return true;

}



///
/// pass pid parameters to machine;
/// return success flag
/// xp = prop. / tn = int / tv = diff
///
bool HuberPetiteFleur::SetControlParameters( float xp, int tn, int tv ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::SetControlParameters] -- DEBUG: Called." << std::endl;
  #endif

  if( xp < 0.1f || xp > 99.9f ) {
    std::cerr << " [HuberPetiteFleur::SetControlParameters] ** ERROR: Invalid proportional parameter xp: " << xp << "." << std::endl;
    return false;
  }
  
  if( tn < 3 || tn > 9999 ) {
    std::cerr << " [HuberPetiteFleur::SetControlParameters] ** ERROR: Invalid integral parameter tn: " << tn << "." << std::endl;
    return false;
  }
  
  if( tv < 0 || tv > 999 ) {
    std::cerr << " [HuberPetiteFleur::SetControlParameters] ** ERROR: Invalid differential parameter tv: " << tv << "." << std::endl;
    return false;
  }

  stringstream theCommand;
  char buffer[200];

  // proportional
  theCommand << "out_par_06 " << xp;
  comHandler_->SendCommand( theCommand.str().c_str() );
  usleep( 10000 );

  // verify
  comHandler_->SendCommand( "in_par_06" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( fabs( atof( buffer ) - xp ) > 1.e-3 ) {
    std::cerr << " [HuberPetiteFleur::SetControlParameters] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: xp=" << xp << " but received (string):" << buffer << "." << std::endl;
    return false;
  }

  // integral
  theCommand.str( "" );
  theCommand << "out_par_07 " << tn;
  comHandler_->SendCommand( theCommand.str().c_str() );
  usleep( 10000 );

  // verify
  comHandler_->SendCommand( "in_par_07" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( atoi( buffer ) !=  tn ) {
    std::cerr << " [HuberPetiteFleur::SetControlParameters] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: tn=" << tn << " but received (string):" << buffer << "." << std::endl;
    return false;
  }

  // differential
  theCommand.str( "" );
  theCommand << "out_par_08 " << tv;
  comHandler_->SendCommand( theCommand.str().c_str() );
  usleep( 10000 );

  // verify
  comHandler_->SendCommand( "in_par_08" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if( atoi( buffer ) !=  tv ) {
    std::cerr << " [HuberPetiteFleur::SetControlParameters] ** ERROR: check failed." << std::endl;
    std::cerr << "  > Expected: tv=" << tv << " but received (string):" << buffer << "." << std::endl;
    return false;
  }
  
  return true;

}



///
///
///
float HuberPetiteFleur::GetBathTemperature( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetBathTemperature] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "in_pv_00" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  return( atof( buffer ) );

}



///
///
///
float HuberPetiteFleur::GetSafetySensorTemperature( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetSafetySensorTemperature] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "in_pv_03" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atof( buffer ) );

}



///
///
///
float HuberPetiteFleur::GetWorkingTemperature( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetWorkingTemperature] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "in_sp_00" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atof( buffer ) );


}



///
///
///
int HuberPetiteFleur::GetHeatingPower( void ) const
{
  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetHeatingPower] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "in_pv_01" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atoi( buffer ) );
}



///
///
///
unsigned int HuberPetiteFleur::GetPumpPressure( void ) const {
  
  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetPumpPressure] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "in_sp_07" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atoi( buffer ) );
  
}



///
/// true = on / false = off
///
bool HuberPetiteFleur::GetCirculatorStatus( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetCirculatorStatus] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "in_mode_05" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  unsigned int status = atoi( buffer );

  if( status ) return true;
  else return false;

}



///
/// get status message &
/// separate extracted status/error number
///
std::pair<int,std::string> HuberPetiteFleur::GetStatus( void ) const {
  
  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetStatus] -- DEBUG: Called." << std::endl;
  #endif

  std::pair<int,std::string> returnValue;

  char buffer[1000];
  comHandler_->SendCommand( "status" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  string message( buffer );

  if( 0 == message.size() ) {
    std::cerr << " [HuberPetiteFleur::GetStatus] ** WARNING: Machine returned empty message." << std::endl;
    return std::pair<int,std::string>( 0, string( "---" ) );
  }

  // error type messages begin with "-" (negative number)
  if( string( "-" ) == message.substr( 0, 1 ) ) {
    returnValue.first  = atoi( message.substr( 0, 3 ).c_str() );
    returnValue.second = message.substr( 4, 100 );
  }

  // normal (non-error) message, positive number
  else {
    returnValue.first  = atoi( message.substr( 0, 2 ).c_str() );
    returnValue.second = message.substr( 3, 100 );
  }

  return returnValue;

}



///
///
///
float HuberPetiteFleur::GetProportionalParameter( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetProportionalParameter] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "in_par_06" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  return( atof( buffer ) );

}



///
///
///
int HuberPetiteFleur::GetIntegralParameter( void ) const {

  #ifdef __JULABOHUBERPETITEFLEUR_DEBUG_DEBUG
  std::cout << "[HuberPetiteFleur::GetIntegralParameter] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "in_par_07" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atoi( buffer ) );

}



///
///
///
int HuberPetiteFleur::GetDifferentialParameter( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetDifferentialParameter] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "in_par_08" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atoi( buffer ) );

}



///
/// save pid parameters to file
///
bool HuberPetiteFleur::SaveControlParameters( const std::string& filepath ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::SaveControlParameters] -- DEBUG: Called." << std::endl;
  #endif

  ofstream file( filepath.c_str() );
  if ( file.bad() ) {
    std::cerr << " [HuberPetiteFleur::SaveControlParameters] ** ERROR: Could not save to file:" << std::endl;
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
/// load pid parameters from file & pass to the machine;
/// return success flag
///
bool HuberPetiteFleur::LoadControlParametersAndApply( const std::string& filepath ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::LoadControlParametersAndApply] -- DEBUG: Called." << std::endl;
  #endif

  std::ifstream file( filepath.c_str(), std::ios::in );
  if ( file.bad() ) {
    std::cerr << " [HuberPetiteFleur::LoadControlParametersAndApply] ** ERROR: Could not read from file:" << std::endl;
    std::cerr << "  > \"" << filepath.c_str() << "\"." << std::endl;
    return false;
  }

  std::string item;
  float propP = 0.; bool isProp = false;
  int intlP = 0; bool isIntl = false;
  int diffP = 0; bool isDiff = false;

  while( ! ( ( isProp && isIntl && isDiff ) || file.eof() ) ) {

    file >> item;

    if( std::string::npos != item.find( "<PID_PROP>" ) ) {
      file >> propP;
      isProp = true;
    }

    if( std::string::npos != item.find( "<PID_INTL>" ) ) {
      file >> intlP;
      isIntl = true;
    }

    if( std::string::npos != item.find( "<PID_DIFF>" ) ) {
      file >> diffP;
      isDiff = true;
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
/// strip trailing newlines & stuff
/// from machine answer
///
void HuberPetiteFleur::StripBuffer( char* buffer ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[HuberPetiteFleur::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
  std::cout << " > " << buffer << std::endl;
  #endif

  for( unsigned int c = 0; c < sizeof( buffer ); ++c ) {
    if( '\n' == buffer[c] || '\r' == buffer[c] ) {
      buffer[c] = 0;
      break;
    }
  }
  
}



///
/// read back software version
/// to check communication with device
///
void HuberPetiteFleur::Device_Init( void ) {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[200];
  comHandler_->SendCommand( "version" );
  usleep( 200000 );
  comHandler_->ReceiveString( buffer );

  std::cout << buffer << std::endl;

  if( std::string::npos == std::string( buffer ).find( "JULABO TOPTECH-SERIES MC-2 VERSION 3.0" ) ) {
    std::cerr << " [HuberPetiteFleur::Device_Init] ** ERROR: Device communication problem." << std::endl;
    std::cerr << "  > (could not read software version string)" << std::endl;
    isCommunication_ = false;
    return;
  }

  isCommunication_ = true;

}

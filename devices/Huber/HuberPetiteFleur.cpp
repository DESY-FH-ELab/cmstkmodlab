#include <string.h>

#include <cstdlib>
#include <iostream>

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
  : VHuberPetiteFleur(ioPort),
    uDelay_(250000)
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

  if( workingTemp > PetiteFleurUpperTempLimit || workingTemp < PetiteFleurLowerTempLimit ) {
    std::cerr << " [HuberPetiteFleur::SetWorkingTemp] ** ERROR: working temp T=" 
	      << workingTemp << " exceeds soft limits." << std::endl;
    std::cerr << "  > (s. HuberPetiteFleur class definition)" << std::endl;
    return false;
  }

  char buffer[1000];

  int iTemp = workingTemp * 100.;
  sprintf(buffer, "%+06d", iTemp);

  std::stringstream theCommand;
  theCommand << "SP! " << buffer;

  comHandler_->SendCommand( theCommand.str().c_str() );
  usleep( uDelay_ );

  comHandler_->SendCommand( "SP?" );
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );
  usleep( uDelay_ );
  StripBuffer( buffer );

  if( std::fabs( iTemp - ToInteger(buffer) ) > 1 ) {
    std::cerr << " [HuberPetiteFleur::SetWorkingTemp] ** ERROR: check failed."
        << std::endl;
    std::cerr << "  > Expected: T=" << workingTemp << " but received (string):"
	      << buffer << "." << std::endl;
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
  
  usleep( uDelay_ );
  comHandler_->SendCommand( "CA@ +00001" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  int status = ToInteger(buffer);

  if( status != 1 ) {
    std::cerr << " [HuberPetiteFleur::SetCirculatorOn] ** ERROR: check failed."
        << std::endl;
    std::cerr << "  > Expected: ON (1) but received (string):"
        << buffer << "." << std::endl;
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
  
  usleep( uDelay_ );
  comHandler_->SendCommand( "CA@ +00000" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  int status = ToInteger(buffer);

  if( status != 0 ) {
    std::cerr << " [HuberPetiteFleur::SetCirculatorOn] ** ERROR: check failed."
        << std::endl;
    std::cerr << "  > Expected: OFF (0) but received (string):"
        << buffer << "." << std::endl;
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

  usleep( uDelay_ );
  comHandler_->SendCommand( "TI?" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ToFloat(buffer);
}

///
///
///
float HuberPetiteFleur::GetWorkingTemperature( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetWorkingTemperature] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "SP?" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  //std::cout << buffer << std::endl;

  return ToFloat(buffer);
}

///
/// true = on / false = off
///
bool HuberPetiteFleur::GetCirculatorStatus( void ) const {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::GetCirculatorStatus] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  usleep( uDelay_ );
  comHandler_->SendCommand( "CA?" );

  usleep( uDelay_ );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  int status = ToInteger(buffer);

  //std::cout << buffer << std::endl;

  if( status>0 ) return true;
  else return false;
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

  for( unsigned int c = 0; c < strlen( buffer ); ++c ) {
    if( '\n' == buffer[c] || '\r' == buffer[c] ) {
      buffer[c] = 0;
      break;
    }
  }
}

int HuberPetiteFleur::ToInteger(const char* buffer) const
{
  std::string temp(buffer);
  temp.erase(0, 3);

  return std::atoi( temp.c_str() );
}

float HuberPetiteFleur::ToFloat(const char* buffer) const
{
  std::string temp(buffer);
  temp.erase(0, 3);

  return std::atof( temp.c_str() )/100.;
}

///
/// read back software version
/// to check communication with device
///
void HuberPetiteFleur::Device_Init( void ) {

  #ifdef __HUBERPETITEFLEUR_DEBUG
  std::cout << "[HuberPetiteFleur::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "CA?" );
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );
  usleep( uDelay_ );
  StripBuffer( buffer );
  std::string temp(buffer);

  if (temp.compare(0, 2, "CA")!=0) {
    std::cerr << " [HuberPetiteFleur::Device_Init] ** ERROR: Device communication problem."
        << std::endl;
    isCommunication_ = false;
    return;
  }

  isCommunication_ = true;
}

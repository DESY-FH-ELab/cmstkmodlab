
#include "TPG262ComHandler.h"

#include "PfeifferTPG262.h"

///
///
///
PfeifferTPG262::PfeifferTPG262( const ioport_t ioPort )
  : VPfeifferTPG262(ioPort)
{
  comHandler_ = new TPG262ComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}
///
///
///
PfeifferTPG262::~PfeifferTPG262()
{
  delete comHandler_;
}

///
///
///
int PfeifferTPG262::GetErrorStatus( void ) const
{
  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::GetErrorStatus] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "ERR" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  //buffer[0] should be 0x06
  std::cout << strlen(buffer) << "  " << (int)buffer[0] << endl;
  
  comHandler_->SendEnquiry();
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  std::cout << strlen(buffer) << "  " << buffer << endl;
  
  return 0;

}

///
///
///
float PfeifferTPG262::GetPressure1( void ) const
{
  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::GetPressure1] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "PR1" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  //buffer[0] should be 0x06
  std::cout << strlen(buffer) << "  " << (int)buffer[0] << endl;
  
  comHandler_->SendEnquiry();
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  std::cout << strlen(buffer) << "  " << buffer << endl;
  
  return 0;
}

///
///
///
float PfeifferTPG262::GetPressure2( void ) const
{
  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::GetPressure2] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];

  comHandler_->SendCommand( "PR2" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  //buffer[0] should be 0x06
  std::cout << strlen(buffer) << "  " << (int)buffer[0] << endl;
  
  comHandler_->SendEnquiry();
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  std::cout << strlen(buffer) << "  " << buffer << endl;
  
  return 0;
}

///
/// strip trailing newlines & stuff
/// from machine answer
///
void PfeifferTPG262::StripBuffer( char* buffer ) const {

  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[PfeifferTPG262::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
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
void PfeifferTPG262::Device_Init( void ) {

  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

  isCommunication_ = true;
}

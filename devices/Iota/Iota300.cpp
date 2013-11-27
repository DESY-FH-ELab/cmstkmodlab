

//#####################
// TODO:
// query error codes
//#####################


#include "IotaComHandler.h"

#include "Iota300.h"

///
///
///
Iota300::Iota300( const ioport_t ioPort )
  : VIota300(ioPort)
{
  comHandler_ = new IotaComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}




///
///
///
unsigned int Iota300::GetPumpPressure( void ) const {
  
  #ifdef __IOTA300_DEBUG
  std::cout << "[Iota300::GetPumpPressure] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
  
  comHandler_->SendCommand( "#FSr\n" );
  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return( atoi( buffer ) );
  
}




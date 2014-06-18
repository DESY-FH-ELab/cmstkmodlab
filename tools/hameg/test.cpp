

#include <iostream>
#include <string>

#include "HO820ComHandler.h"


int main()
{
  char buffer[1000];
  HO820ComHandler comHandler( "/dev/ttyUSB0" );

  comHandler.SendCommand( "OP1" );
  comHandler.ReceiveString( buffer );

  std::cout << "BUFFER: " << buffer << std::endl; /////////////////////////////////

  sleep( 1 );

  comHandler.SendCommand( "SU1:12.34" );
  comHandler.ReceiveString( buffer );
  
  std::cout << "BUFFER: " << buffer << std::endl; /////////////////////////////////
}

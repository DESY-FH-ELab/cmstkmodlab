

#include <iostream>
#include <string>

#include "FP50ComHandler.h"


int main() {

  char buffer[1000];
  FP50ComHandler comHandler( "/dev/ttyS0" );

  comHandler.SendCommand( "in_pv_00" );
  comHandler.ReceiveString( buffer );

  std::cout << "BUFFER: " << buffer << std::endl; /////////////////////////////////

    //    sleep( 2 );

  comHandler.SendCommand( "in_pv_03" );
  comHandler.ReceiveString( buffer );

  std::cout << "BUFFER: " << buffer << std::endl; /////////////////////////////////

}

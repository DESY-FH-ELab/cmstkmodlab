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

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <iostream>

#include "GMH3750.h"

GMH3750::GMH3750( ioport_t ioPort )
{
  comHandler_ = new GMH3750ComHandler( ioPort );
}

///
///
///
GMH3750::~GMH3750( void )
{
  delete comHandler_;
}

bool GMH3750::Read(double & temperature)
{
  char command[10];
  char buffer[1000];
  
  command[0] = 254;
  command[1] = 0;
  command[2] = 61;
  command[3] = 0;

  comHandler_->SendCommand( command, 3 );
  comHandler_->ReceiveString( buffer );

  unsigned char byte3 = buffer[3];
  unsigned char byte4 = buffer[4];
  unsigned char byte6 = buffer[6];
  unsigned char byte7 = buffer[7];

  int DP;
  DP = ( (0xFF-byte3) >> 3 ) - 15;

  unsigned long data;
  data = 0xFF - byte3;
  data &= 0x07;
  data *= 0x1000000;
  data += 0x10000 * byte4;
  data += 0x100 * (0xFF-byte6);
  data += byte7;
  
  if ( data < 0x07F5E100 ) {
    
    data += 0x02000000;

    if ( (data & 0x00400000) == 0x00400000 ) {    
      data |= 0xF8000000;
    } else {
      data &= 0x07ffffff;
    }
    
    temperature = (double)(int)data;
    temperature /= pow10(DP);

  } else {
    
    data -= 0x07F5E100;
    temperature = (double)(int)data;
    
    return false;
  }
  
  return true;
}

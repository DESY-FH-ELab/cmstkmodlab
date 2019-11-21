/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <cstring>
#include <cmath>
#include <sstream>

#include "RohdeSchwarzNGE103B.h"

RohdeSchwarzNGE103B::RohdeSchwarzNGE103B( const ioport_t ioPort )
  :VRohdeSchwarzNGE103B(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new NGE103BComHandler( ioPort );
  DeviceInit();
}

RohdeSchwarzNGE103B::~RohdeSchwarzNGE103B()
{
  delete comHandler_;
}

bool RohdeSchwarzNGE103B::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

void RohdeSchwarzNGE103B::GetIdentification(std::string& id) const
{
  comHandler_->SendCommand("*IDN?");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  id = buffer;
}

void RohdeSchwarzNGE103B::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}

void RohdeSchwarzNGE103B::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    comHandler_->SendCommand("*IDN?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    std::string buf = buffer;
    
    if (buf.find("HAMEG Instruments,HM8143", 0)==0) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }
}

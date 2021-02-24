/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
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
#include <iostream>

#include "Marta.h"

Marta::Marta( ipaddress_t ipaddress )
  :VMarta(ipaddress),
   isDeviceAvailable_(false)
{
  std::string ipandport = ipaddress;
  auto const pos = ipandport.find_last_of(':');
  std::string ip = ipandport.substr(0, pos);
  std::string port = ipandport.substr(pos+1);
  
  comHandler_ = new MartaComHandler(ip.c_str(), std::atoi(port.c_str()));
}

Marta::Marta( ipaddress_t ipaddress, port_t port )
  :VMarta(ipaddress, port),
   isDeviceAvailable_(false)
{
  comHandler_ = new MartaComHandler(ipaddress, port);
}

Marta::~Marta()
{
  delete comHandler_;
}

void Marta::ReadRegisters(int addr, int nb, uint16_t *dest)
{
  comHandler_->ReadRegisters(addr, nb, dest);
}

void Marta::WriteRegisters(int addr, int nb, const uint16_t *src)
{
  comHandler_->WriteRegisters(addr, nb, src);
}

bool Marta::DeviceAvailable() const
{
  return comHandler_->DeviceAvailable();
}

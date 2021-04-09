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

#include <cmath>

#include "MartaFake.h"

MartaFake::MartaFake( ipaddress_t ipaddress )
  :VMarta(ipaddress)
{

}

MartaFake::MartaFake( ipaddress_t ipaddress, port_t port)
  :VMarta(ipaddress, port)
{

}

MartaFake::~MartaFake()
{

}

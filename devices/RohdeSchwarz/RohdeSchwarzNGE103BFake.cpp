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

#include <cmath>

#include "RohdeSchwarzNGE103BFake.h"

RohdeSchwarzNGE103BFake::RohdeSchwarzNGE103BFake( const ioport_t ioPort )
  :VRohdeSchwarzNGE103B(ioPort)
{

}

RohdeSchwarzNGE103BFake::~RohdeSchwarzNGE103BFake()
{

}

void RohdeSchwarzNGE103BFake::GetIdentification(std::string& id) const
{
  id = "Rohde&Schwarz,NGE103B,5601.3800k03/100897,1.52";
}

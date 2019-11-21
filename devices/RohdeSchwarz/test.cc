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

#include <iostream>

#ifdef USE_FAKEIO
#include "RohdeSchwarzNGE103BFake.h"
typedef RohdeSchwarzNGE103BFake RohdeSchwarzNGE103B_t;
#else
#include "RohdeSchwarzNGE103B.h"
typedef RohdeSchwarzNGE103B RohdeSchwarzNGE103B_t;
#endif

int main()
{
  std::cout << "test" << std::endl;

  std::string buffer;

  RohdeSchwarzNGE103B_t NGE103B("/dev/ttyUSB0");

  NGE103B.GetIdentification(buffer);
  std::cout << buffer << std::endl;

  return 0;
}

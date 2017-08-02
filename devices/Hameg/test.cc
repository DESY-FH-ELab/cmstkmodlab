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

#include <iostream>

#ifdef USE_FAKEIO
#include "Hameg8143Fake.h"
typedef Hameg8143Fake Hameg8143_t;
#else
#include "Hameg8143.h"
typedef Hameg8143 Hameg8143_t;
#endif

int main()
{
  std::cout << "test" << std::endl;

  Hameg8143_t hameg("/dev/ttyUSB0");

  std::cout << hameg.GetVoltage(1) << std::endl;

  return 0;
}

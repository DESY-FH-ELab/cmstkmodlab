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

#include <iostream>

#ifdef USE_FAKEIO
#include "MartaFake.h"
typedef MartaFake Marta_t;
#else
#include "Marta.h"
typedef Marta Marta_t;
#endif

int main()
{
  std::cout << "test" << std::endl;

  std::string buffer;

  // Marta_t marta("131.169.43.224", 502);
  // Marta_t marta("131.169.43.224:502");
  // Marta_t marta("10.255.21.224", 502);
  Marta_t marta("10.255.21.224:502");

  std::cout << (int) marta.DeviceAvailable() << std::endl;

  uint16_t tab_reg[32];
  tab_reg[0] = 0;
  tab_reg[1] = 0;

  marta.ReadRegisters(67, 2, tab_reg);

  printf("0x%04x 0x%04x\n", tab_reg[0], tab_reg[1]);

  printf("%f\n", marta.ToFloatDCBA(tab_reg));
  
  return 0;
}

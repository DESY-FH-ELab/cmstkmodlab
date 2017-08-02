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
#include <string>

#include "JulaboFP50.h"


int main() {

  JulaboFP50 julabo( "/dev/ttyS0" );

  usleep( 100000 );

  std::cout << julabo.GetBathTemperature() << std::endl;
  std::cout << julabo.GetSafetySensorTemperature() << std::endl;

}

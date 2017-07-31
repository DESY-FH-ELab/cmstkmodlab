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

#include "GMH3750.h"

int main()
{
  GMH3750 gmh3750( "/dev/ttyS4" );
  
  double temperature;
  if (gmh3750.Read(temperature)) {
    std::cout << "temperature = " << temperature << std::endl;
  }
}

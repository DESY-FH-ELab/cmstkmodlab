/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <ctime>

#ifdef USE_FAKEIO
#include "GMH3750Fake.h"
#else
#include "GMH3750.h"
#endif

int main(int argc, char * argv[])
{
  if (argc>2) {
    std::cout << "usage: ReadGMH3750 [<filename>]" << std::endl;
  }
  
  VGMH3750 * gmh3750;  
#ifdef USE_FAKEIO
  gmh3750 = new GMH3750Fake("/dev/ttyS5");
#else
  gmh3750 = new GMH3750("/dev/ttyS5");
#endif

  double temperature;
  gmh3750->Read(temperature);

  if (argc==1) {
    std::cout.precision(3);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout << temperature << std::endl;
    std::ofstream ofile("ReadGMH3750.last");
    ofile.precision(3);
    ofile.setf(std::ios::fixed, std::ios::floatfield);
    ofile << temperature << std::endl;
  } else {
    std::ofstream ofile(argv[1], std::fstream::app);
    if (ofile.good()) {
      time_t seconds = time(NULL);
      ofile << seconds << "   ";
      ofile.precision(3);
      ofile.setf(std::ios::fixed, std::ios::floatfield);
      ofile << temperature << std::endl;
    }
  }

  delete gmh3750;
  
  return 0;
}

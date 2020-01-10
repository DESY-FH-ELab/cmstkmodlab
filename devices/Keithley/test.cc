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
#include "KeithleyDAQ6510Fake.h"
typedef KeithleyDAQ6510Fake KeithleyDAQ6510_t;
#else
#include "KeithleyDAQ6510.h"
typedef KeithleyDAQ6510 KeithleyDAQ6510_t;
#endif

int main()
{
  std::string buffer;

  KeithleyDAQ6510_t daq("/dev/usbtmc0");

  std::cout << (int) daq.DeviceAvailable() << std::endl;

  channels_t channels;
  channels[0] = true;
  channels[1] = true;
  channels[2] = true;
  channels[3] = true;
  channels[4] = false;
  channels[5] = true;
  channels[6] = true;
  channels[7] = true;
  channels[8] = true;
  channels[9] = false;
  
  buffer = daq.CreateChannelString(1, channels);
  
  std::cout << buffer << std::endl;

  return 0;
}

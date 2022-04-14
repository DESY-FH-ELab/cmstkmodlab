/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <tuple>
#include <chrono>
#include <thread>

#ifdef USE_FAKEIO
#include "KeithleyDAQ6510Fake.h"
typedef KeithleyDAQ6510Fake KeithleyDAQ6510_t;
#else
#include "KeithleyDAQ6510.h"
typedef KeithleyDAQ6510 KeithleyDAQ6510_t;
#endif

int main(int argc, char* argv[])
{
  using namespace std::chrono_literals;

  std::string buffer;

  std::string ioPort = "/dev/usbtmc0";
  if (argc==2) ioPort = argv[1];

  std::cout << "test on " << ioPort << std::endl;

  KeithleyDAQ6510_t daq(ioPort.c_str());

  std::cout << (int) daq.DeviceAvailable() << std::endl;

  VKeithleyDAQ6510::channels_t channels;
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

  daq.ActivateChannel(1,3, true);
  daq.ActivateChannel(1,8, true);
  daq.ActivateChannel(2,4, true);
  daq.ActivateChannel(2,5, true);

  daq.Scan();

  std::this_thread::sleep_for(2s);

  VKeithleyDAQ6510::reading_t data;
  daq.GetScanData(data);

  for (VKeithleyDAQ6510::reading_t::iterator it=data.begin();it!=data.end();++it) {
    unsigned int sensor;
    double temperature;
    double relativeTime;
    std::tie(sensor, temperature, relativeTime) = *it;

    unsigned int card = sensor / 100 - 1;
    unsigned int channel = sensor % 100 - 1;

    std::cout << "data: " << sensor << " " << temperature << " " << relativeTime << std::endl;
  }

  return 0;
}

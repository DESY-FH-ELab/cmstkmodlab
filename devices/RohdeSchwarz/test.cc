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

int main(int argc, char* argv[])
{
  std::string ioPort = "/dev/ttyACM0";
  if (argc==2) ioPort = argv[1];

  std::cout << "test on " << ioPort << std::endl;
  
  std::string buffer;

  RohdeSchwarzNGE103B_t nge103B(ioPort.c_str());

  std::cout << (int) nge103B.DeviceAvailable() << std::endl;
  
  nge103B.GetIdentification(buffer);
  std::cout << "OUTP:STAT?\t" << nge103B.GetOutputState() << std::endl;
  std::cout << buffer << std::endl;

  nge103B.SelectChannel(1);
  std::cout << "INST:NSEL?\t" << nge103B.SelectedChannel() << std::endl;

  nge103B.SetVoltage(2.3);
  std::cout << "VOLT?\t" << nge103B.GetVoltage() << std::endl;

  std::cout << "INST:NSEL?\t" << nge103B.SelectedChannel() << std::endl;

  nge103B.SetCurrent(0.1);
  std::cout << "CURR?\t" << nge103B.GetCurrent() << std::endl;

  std::cout << "MEAS:VOLT?\t" << nge103B.MeasureVoltage() << std::endl;
  std::cout << "MEAS:CURR?\t" << nge103B.MeasureCurrent() << std::endl;

  //nge103B.SetOutputState(1);
  std::cout << "OUTP:STAT?\t" << nge103B.GetOutputState() << std::endl;
  std::cout << "OUTP:MODE?\t" << nge103B.GetOutputMode() << std::endl;

  return 0;
}

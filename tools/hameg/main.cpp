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
#include <string>

#include "Hameg8143.h"


int main()
{
  char buffer[1000];
  Hameg8143 hameg( "/dev/ttyUSB0" );
  
  usleep( 100000 );

  hameg.SetRemoteMode(true);
  hameg.SetMixedMode(false);

  sleep(1);
  hameg.SetVoltage(1, 5.000);
  sleep(1);
  hameg.SetCurrent(1, 0.010);

  // sleep(1);
  // hameg.SetVoltage(2, 1.43);
  // sleep(1);
  // hameg.SetCurrent(2, 0.132);

  hameg.GetStatus();
  
  std::cout << hameg.GetSetVoltage(1) << std::endl;
  std::cout << hameg.GetSetCurrent(1) << std::endl;
  std::cout << hameg.GetVoltage(1) << std::endl;
  std::cout << hameg.GetCurrent(1) << std::endl;

  //hameg.SetTrackingVoltage(5.03);
  
  hameg.SwitchOutputOn();
  
  sleep(2);

  hameg.GetStatus();
  
  std::cout << hameg.GetSetVoltage(1) << std::endl;
  std::cout << hameg.GetSetCurrent(1) << std::endl;
  std::cout << hameg.GetVoltage(1) << std::endl;
  std::cout << hameg.GetCurrent(1) << std::endl;
  
  hameg.SwitchOutputOff();
  
  sleep(2);

  hameg.GetStatus();

  hameg.SetRemoteMode(false);
  hameg.SetMixedMode(true);
  
  sleep(2);

  hameg.GetStatus();
}

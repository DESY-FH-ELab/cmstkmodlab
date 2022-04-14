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

#include <unistd.h>

#include <iostream>
#include <cmath>

#ifdef USE_FAKEIO
#include "HuberPetiteFleurFake.h"
#include "HuberPilotOneFake.h"
typedef HuberPetiteFleurFake HuberPetiteFleur_t;
typedef HuberPilotOneFake HuberPilotOne_t;
#else
#include "HuberPetiteFleur.h"
#include "HuberPilotOne.h"
typedef HuberPetiteFleur HuberPetiteFleur_t;
typedef HuberPilotOne HuberPilotOne_t;
#endif

int main(int argc, char* argv[])
{
  std::string ioPort = "/dev/ttyACM0";
  if (argc==2) ioPort = argv[1];

  std::cout << "test on " << ioPort << std::endl;

  HuberPilotOne_t huber(ioPort.c_str());

  std::cout << huber.GetTemperatureSetPoint() << std::endl;
  //huber.SetTemperatureSetPoint(1.5);
  //std::cout << huber.GetTemperatureSetPoint() << std::endl;
    
  /*
  std::cout << huber.GetBathTemperature() << std::endl;
  std::cout << huber.GetReturnTemperature() << std::endl;
  std::cout << huber.GetPumpPressure() << std::endl;
  std::cout << huber.GetPower() << std::endl;
  std::cout << huber.GetTemperatureControlEnabled() << std::endl;
  std::cout << huber.GetCoolingWaterInletTemperature() << std::endl;
  std::cout << huber.GetCoolingWaterOutletTemperature() << std::endl;
  */
  
  /*
  {
    HuberPetiteFleur_t huber("/dev/ttyACM0");
  
    huber.SetWorkingTemperature(12.34);

    long suffering = 0;

    while (1) {
      std::cout << "bath = " << huber.GetBathTemperature() << std::endl;
      float work = huber.GetWorkingTemperature();
      std::cout << "work = " << work << std::endl;
      std::cout << "circ = " << (int)huber.GetCirculatorStatus() << std::endl;
      if (fabs(work-12.34)>0.01) break;
      std::cout << std::endl;
      usleep(500000);

      suffering++;
  
      std::cout << "suffering: " << suffering << std::endl;
    }
  }
  */
  
  return 0;
}

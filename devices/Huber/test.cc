#include <iostream>
#include <cmath>

#include "HuberPetiteFleur.h"

int main()
{
  std::cout << "test" << std::endl;
  
  HuberPetiteFleur huber("/dev/ttyACM0");
  
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

  return 0;
}

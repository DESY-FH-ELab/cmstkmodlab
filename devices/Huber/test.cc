#include <iostream>

#include "HuberPetiteFleur.h"

int main()
{
  std::cout << "test" << std::endl;

  HuberPetiteFleur huber("/dev/ttyACM0");

  huber.GetBathTemperature();
  huber.GetWorkingTemperature();
  huber.SetWorkingTemperature(12.34);
  huber.GetCirculatorStatus();
  
  //huber.SetCirculatorOn();
  //huber.SetCirculatorOff();

  return 0;
}

#include <iostream>

#include "Hameg8143.h"

int main()
{
  std::cout << "test" << std::endl;

  Hameg8143 hameg("/dev/ttyUSB0");

  std::cout << hameg.GetVoltage(1) << std::endl;

  return 0;
}

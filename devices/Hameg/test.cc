#include <iostream>

#ifdef USE_FAKEIO
#include "Hameg8143Fake.h"
typedef Hameg8143Fake Hameg8143_t;
#else
#include "Hameg8143.h"
typedef Hameg8143 Hameg8143_t;
#endif

int main()
{
  std::cout << "test" << std::endl;

  Hameg8143_t hameg("/dev/ttyUSB0");

  std::cout << hameg.GetVoltage(1) << std::endl;

  return 0;
}

#include <unistd.h>

#include <iostream>
#include <cmath>

#ifdef USE_FAKEIO
#include "Iota300Fake.h"
typedef Iota300Fake Iota300_t;
#else
#include "Iota300.h"
typedef Iota300 Iota300_t;
#endif

int main()
{
  std::cout << "test" << std::endl;
  
  Iota300_t iota("/dev/ttyS4");
  
  iota.SetFlow(7);

  long suffering = 0;

  while (1) {
    std::cout << "flow = " << iota.GetFlow() << std::endl;
    float pressure = iota.GetPressure();
    std::cout << "pressure = " << pressure << std::endl;
    std::cout << "status = " << (int)iota.GetStatus() << std::endl;
    if (fabs(pressure-12.34)>0.01) break;
    std::cout << std::endl;
    usleep(500000);

    suffering++;
  
    std::cout << "suffering: " << suffering << std::endl;
  }

  return 0;
}

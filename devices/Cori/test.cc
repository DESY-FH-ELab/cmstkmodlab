#include <unistd.h>

#include <iostream>
#include <cmath>

#ifdef USE_FAKEIO
#include "CoriFlow.h"
typedef CoriFlowFake CoriFlow_t;
#else
#include "CoriFlow.h"
typedef CoriFlow CoriFlow_t;
#endif

int main()
{

  CoriFlow_t cori("/dev/ttyS2");
  
//   std::cout << "Temperature = " << cori.getTemp() << std::endl;
  std::cout << "Flow = " << cori.getMeasure() << std::endl;
//   std::cout << "Pressure = " << cori.getPres() << std::endl;
//   std::cout << "Capacity 100% = " << cori.getCapacity() << std::endl;
  std::cout << "Density = " << cori.getDensity() << std::endl;
//   std::cout << "Unit = " << cori.getUnit() << std::endl;
//   std::cout << "Fluid = " << cori.getFluid() << std::endl;
//   std::cout << "User Tag = " << cori.getTag() << std::endl;
//   std::cout << "Init status = " << cori.getInit() << std::endl;
//   std::cout << "set Capacity 100% = " << cori.setCapacity() << std::endl;
//   std::cout << "Capacity 100% = " << cori.getCapacity() << std::endl;
//   std::cout << "Unit Index = " << cori.getUnitIndex() << std::endl;

  return 0;
}

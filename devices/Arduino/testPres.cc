#include <unistd.h>

#include <iostream>
#include <cmath>

#ifdef USE_FAKEIO
#include "ArduinoPresFake.h"
typedef ArduinoPresFake ArduinoPres_t;
#else
#include "ArduinoPres.h"
typedef ArduinoPres ArduinoPres_t;
#endif

int main()
{
  std::cout << "test" << std::endl;
  
  ArduinoPres_t ArduinoPres("/dev/ttyArduino1");
  
  std::cout<<"Pressure on Sensor a = "<<ArduinoPres.GetPressureA()<<"\n"<<std::endl;
  std::cout<<"Pressure on Sensor b = "<<ArduinoPres.GetPressureB()<<"\n"<<std::endl;
  // std::cout<<"Flow on Sensor c = "<<ArduinoPres.GetFlow()<<"\n"<<std::endl;
  
  long suffering = 0;

  return 0;
}

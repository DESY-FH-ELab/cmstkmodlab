

#include <iostream>
#include <string>

#include "JulaboFP50.h"


int main() {

  char buffer[1000];
  JulaboFP50 julabo( "/dev/ttyS0" );

  usleep( 100000 );

  std::cout << julabo.GetBathTemperature() << std::endl;
  std::cout << julabo.GetSafetySensorTemperature() << std::endl;

}

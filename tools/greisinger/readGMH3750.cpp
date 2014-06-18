#include <iostream>

#include "GMH3750.h"

int main()
{
  GMH3750 gmh3750( "/dev/ttyS4" );
  
  double temperature;
  if (gmh3750.Read(temperature)) {
    std::cout << "temperature = " << temperature << std::endl;
  }
}

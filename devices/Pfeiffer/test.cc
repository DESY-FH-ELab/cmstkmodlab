#include <iostream>

#include "PfeifferTPG262.h"

int main()
{
  std::cout << "test" << std::endl;

  PfeifferTPG262 pfeiffer("/dev/ttyS4");

  std::pair<VPfeifferTPG262::GaugeStatus,float> reading1;
  std::pair<VPfeifferTPG262::GaugeStatus,float> reading2;
  bool ret;

  //ret = pfeiffer.GetPressure1(reading);
  //ret = pfeiffer.GetPressure2(reading);
  //ret = pfeiffer.GetPressures(reading1, reading2);

  return 0;
}

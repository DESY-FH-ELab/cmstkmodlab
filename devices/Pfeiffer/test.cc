#include <iostream>

#ifdef USE_FAKEIO
#include "PfeifferTPG262Fake.h"
typedef PfeifferTPG262Fake PfeifferTPG262_t;
#else
#include "PfeifferTPG262.h"
typedef PfeifferTPG262 PfeifferTPG262_t;
#endif

int main()
{
  std::cout << "test" << std::endl;

  PfeifferTPG262_t pfeiffer("/dev/ttyS4");

  std::pair<VPfeifferTPG262::GaugeStatus,float> reading1;
  std::pair<VPfeifferTPG262::GaugeStatus,float> reading2;
  bool ret;

  //ret = pfeiffer.GetPressure1(reading);
  //ret = pfeiffer.GetPressure2(reading);
  //ret = pfeiffer.GetPressures(reading1, reading2);

  return 0;
}

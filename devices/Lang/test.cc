#include <string>
#include <vector>
#include <iostream>

#ifdef USE_FAKEIO
#include "LStepExpressFake.h"
typedef LStepExpressFake LStepExpress_t;
#else
#include "LStepExpress.h"
typedef LStepExpress LStepExpress_t;
#endif

template<typename T, typename A> void dumpVector(const std::vector<T,A>& v)
{
  for (typename std::vector<T,A>::const_iterator it = v.begin();
       it!=v.end();
       ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

int main()
{
  LStepExpress_t lstep("/dev/ttyUSB0");

  std::string svalue;
  int ivalue;
  double dvalue;
  std::vector<int> ivalues;
  std::vector<double> dvalues;

  lstep.GetDimension(ivalues);
  dumpVector(ivalues);
  lstep.GetDimension(VLStepExpress::Z, ivalue);
  std::cout << ivalue << std::endl;
  
  ivalues[VLStepExpress::Y] = 1;
  lstep.SetDimension(ivalues);
  lstep.GetDimension(ivalues);
  dumpVector(ivalues);
  
  lstep.SetDimension(VLStepExpress::Y, 2);
  lstep.GetDimension(ivalues);
  dumpVector(ivalues);

  lstep.GetPowerAmplifierStatus(ivalues);
  dumpVector(ivalues);
  lstep.GetPowerAmplifierStatus(VLStepExpress::Y, ivalue);
  std::cout << ivalue << std::endl;

  lstep.GetPosition(dvalues);
  dumpVector(dvalues);

  lstep.MoveRelative(1, 2, 3, 4);
  lstep.GetPosition(dvalues);
  dumpVector(dvalues);

  lstep.MoveRelative();
  lstep.GetPosition(dvalues);
  dumpVector(dvalues);

  return 0;
}

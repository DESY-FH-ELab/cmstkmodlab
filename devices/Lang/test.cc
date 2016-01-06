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

template<typename T, typename A> void dumpVectorHex(const std::vector<T,A>& v)
{
  for (typename std::vector<T,A>::const_iterator it = v.begin();
       it!=v.end();
       ++it) {
    std::cout << "0x" << std::hex << *it << " ";
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

  lstep.GetDimension(ivalues); dumpVector(ivalues);
  lstep.GetDimension(VLStepExpress::Z, ivalue); std::cout << ivalue << std::endl;
  
  //ivalues[VLStepExpress::Y] = VLStepExpress::MICROMETER;
  //lstep.SetDimension(ivalues);
  //lstep.GetDimension(ivalues); dumpVector(ivalues);
  
  lstep.SetDimension(VLStepExpress::Y, VLStepExpress::MILLIMETER);
  lstep.GetDimension(ivalues); dumpVector(ivalues);

  lstep.GetPowerAmplifierStatus(ivalues); dumpVector(ivalues);
  lstep.GetPowerAmplifierStatus(VLStepExpress::Y, ivalue); std::cout << ivalue << std::endl;

  lstep.GetPosition(dvalues); dumpVector(dvalues);

  lstep.SetAutoStatus(0);

  lstep.GetAxisStatus(ivalues); dumpVectorHex(ivalues);

  //lstep.ConfirmErrorRectification();
  //lstep.GetAxisStatus(ivalues); dumpVectorHex(ivalues);
    
  //lstep.MoveRelative(1.0000, 0.0000, 0.0000, 0.0000);

  //lstep.GetAxisStatus(ivalues); dumpVectorHex(ivalues);
  

  //lstep.MoveRelative(10.0000, 0.0000, 0.0000, 0.0000);
  lstep.MoveAbsolute(0.0000, 0.0000, 0.0000, 0.0000);

  lstep.GetAxisStatus(ivalues); dumpVector(ivalues);
  while (ivalues[0]==1) {
    usleep(100);
    lstep.GetAxisStatus(ivalues); dumpVector(ivalues);
  }

  lstep.GetAxisStatus(ivalues); dumpVector(ivalues);

  lstep.GetPosition(dvalues); dumpVector(dvalues);

  lstep.GetSystemStatus(ivalues); dumpVectorHex(ivalues);

  return 0;
}

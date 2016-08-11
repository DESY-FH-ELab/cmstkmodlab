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

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  lstep.SetValue("!pa", 1);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  lstep.GetDimension(ivalues); dumpVector(ivalues);
  lstep.GetDimension(VLStepExpress::Z, ivalue); std::cout << ivalue << std::endl;
  std::cout<<"get error = "<<lstep.GetError()<<std::endl;
  
  //ivalues[VLStepExpress::Y] = VLStepExpress::MICROMETER;
  //lstep.SetDimension(ivalues);
  //lstep.GetDimension(ivalues); dumpVector(ivalues);
  
  lstep.SetDimension(VLStepExpress::Y, VLStepExpress::MILLIMETER);
  lstep.GetDimension(ivalues); dumpVector(ivalues);
  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"power amplifier status"<<std::endl;
  lstep.GetPowerAmplifierStatus(ivalues); dumpVector(ivalues);
  lstep.GetPowerAmplifierStatus(VLStepExpress::Y, ivalue); std::cout << ivalue << std::endl;
  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"set status enabled"<<std::endl;
  std::vector<int> ivalues_on = {1,1,1,0};
  lstep.SetAxisEnabled(ivalues_on);
  lstep.GetAxisEnabled(ivalues); dumpVector(ivalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"get position"<<std::endl;
  lstep.GetPosition(dvalues); dumpVector(dvalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  lstep.SetAutoStatus(0);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"get axis status"<<std::endl;
  lstep.GetAxisStatus(ivalues); dumpVectorHex(ivalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  lstep.ConfirmErrorRectification();
  //lstep.GetAxisStatus(ivalues); dumpVectorHex(ivalues);
    
  //lstep.MoveRelative(1.0000, 0.0000, 0.0000, 0.0000);

  //lstep.GetAxisStatus(ivalues); dumpVectorHex(ivalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;
  //  lstep.GetError();

  std::cout<<"get status of limit switches"<<std::endl;
  std::vector<double> limvalues;
  lstep.GetValue("swact", VLStepExpress::X, limvalues); dumpVector(limvalues);
  lstep.GetValue("swact", VLStepExpress::Y, limvalues); dumpVector(limvalues);
  lstep.GetValue("swact", VLStepExpress::Z, limvalues); dumpVector(limvalues);
  lstep.GetValue("swact", VLStepExpress::A, limvalues); dumpVector(limvalues);
  std::cout<<"read all the limit switches"<<std::endl;
  std::string output;
  lstep.GetValue("readsw", output); std::cout<<output<<std::endl;

  //  std::string output;
  std::vector<double> limits;
  std::cout<<"get limits"<<std::endl;
  //lstep.SetValue("!lim", "x -150 150");
  lstep.SetValue("!lim", VLStepExpress::X, -150.0, 150.0);
  lstep.SetValue("!lim", VLStepExpress::Y, -150.0, 150.0);
  lstep.SetValue("!lim", VLStepExpress::Z, -150.0, 150.0);
  lstep.SetValue("!lim", VLStepExpress::A, -150.0, 150.0);
  lstep.GetValue("lim", VLStepExpress::X, limits); dumpVector(limits);
  //lstep.GetValue("lim", output); std::cout<<output<<std::endl;
  lstep.GetValue("lim", VLStepExpress::Y, limits); dumpVector(limits);
  lstep.GetValue("lim", VLStepExpress::Z, limits); dumpVector(limits);
  lstep.GetValue("lim", VLStepExpress::A, limits); dumpVector(limits);
  std::vector<int> limiton;
  std::cout<<"limit monitoring on?"<<std::endl;
  std::vector<int> OnI = {1,1,1,1};
  std::vector<int> ZerosI = {0,0,0,0};
  lstep.SetValue("!limctr", ZerosI);
  lstep.GetValue("limctr", limiton); dumpVector(limiton);
  std::cout<<"no software limit?"<<std::endl;
  lstep.SetValue("!nosetlimit", OnI);
  lstep.GetValue("nosetlimit", limiton); dumpVector(limiton);

  std::cout<<"move command"<<std::endl;
  //lstep.MoveRelative(10.0000, 0.0000, 0.0000, 0.0000);
  lstep.MoveAbsolute(10.0000, 0.0000, 0.0000, 0.0000);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  lstep.ConfirmErrorRectification();
  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"move command"<<std::endl;
  //lstep.MoveRelative(10.0000, 0.0000, 0.0000, 0.0000);
  lstep.MoveAbsolute(10.0000, 0.0000, 0.0000, 0.0000);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"speed of axes"<<std::endl;
  std::vector<double> speedvalues;
  lstep.GetValue("vel", speedvalues); dumpVector(speedvalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"axis status"<<std::endl;
  lstep.GetAxisStatus(ivalues); dumpVector(ivalues);
  while (ivalues[0]==1) {
    usleep(100);
    lstep.GetAxisStatus(ivalues); dumpVector(ivalues);
  }

  lstep.GetAxisStatus(ivalues); dumpVector(ivalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  lstep.GetPosition(dvalues); dumpVector(dvalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"system status"<<std::endl;
  lstep.GetSystemStatus(ivalues); dumpVectorHex(ivalues);

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  std::cout<<"status = "<<lstep.GetStatus()<<std::endl;

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  int st;
  lstep.GetAutoStatus(st);
  std::cout<<"autostatus = "<<st<<std::endl;

  std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  //std::cout << lstep.GetJoystickEnabled() << std::endl;
  //lstep.SetJoystickEnabled(true);
  //std::cout << lstep.GetJoystickEnabled() << std::endl;
  
  //  lstep.Calibrate();

  //std::cout<<"get error = "<<lstep.GetError()<<std::endl;

  return 0;
}

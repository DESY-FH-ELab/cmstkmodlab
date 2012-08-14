#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <ctime>

#ifdef USE_FAKEIO
#include "PfeifferTPG262Fake.h"
#else
#include "PfeifferTPG262.h"
#endif

int main(int argc, char * argv[])
{
  if (argc>2) {
    std::cout << "usage: ReadTPG262 [<filename>]" << std::endl;
  }
  
  VPfeifferTPG262 * tpg262;  
#ifdef USE_FAKEIO
  tpg262 = new PfeifferTPG262Fake("/dev/ttyS4");
#else
  tpg262 = new PfeifferTPG262("/dev/ttyS4");
#endif

  double p1, p2;
  int errorStatus = tpg262->GetErrorStatus();

  if (argc==1) {
    std::cout.precision(3);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout << p1 << std::endl;
    std::ofstream ofile("ReadTPG262.last");
    ofile.precision(3);
    ofile.setf(std::ios::fixed, std::ios::floatfield);
    ofile << p1 << std::endl;
  } else {
    std::ofstream ofile(argv[1], std::fstream::app);
    if (ofile.good()) {
      time_t seconds = time(NULL);
      ofile << seconds << "   ";
      ofile.precision(3);
      ofile.setf(std::ios::fixed, std::ios::floatfield);
      ofile << p1 << std::endl;
    }
  }

  delete tpg262;
  
  return 0;
}

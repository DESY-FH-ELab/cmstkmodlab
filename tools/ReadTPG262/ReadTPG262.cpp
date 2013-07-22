
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <ctime>

#ifdef USE_FAKEIO
#include "PfeifferTPG262Fake.h"
typedef PfeifferTPG262Fake PfeifferTPG262_t;
#else
#include "PfeifferTPG262.h"
typedef PfeifferTPG262 PfeifferTPG262_t;
#endif

int main(int argc, char * argv[])
{
  if (argc>3) {
    std::cout << "\nusage: ReadTPG262 [<filename>]\n" << std::endl;
  }
  
  VPfeifferTPG262 * tpg262;  

  tpg262 = new PfeifferTPG262_t("/dev/ttyS4");

  VPfeifferTPG262::reading_t p1, p2;

  while(tpg262->GetPressures( p1, p2)) {

    if (argc==1) {
      std::cout.precision(2);
      std::cout.setf(std::ios::scientific, std::ios::floatfield);
      std::ofstream ofile("ReadTPG262.last");
      ofile.precision(2);
      ofile.setf(std::ios::scientific, std::ios::floatfield);
      ofile << p1.first << "  " << p1.second << "  ";
      ofile << p2.first << "  " << p2.second << std::endl;
    } else {
      std::ofstream ofile(argv[1], std::fstream::app);
      if (ofile.good()) {
	time_t seconds = time(NULL);
	ofile << seconds << "   ";
	ofile.precision(2);
	ofile.setf(std::ios::scientific, std::ios::floatfield);
	ofile << p1.first << "  " << p1.second << "  ";
	ofile << p2.first << "  " << p2.second << std::endl;
      }
    }

    sleep(10);

  }
  
  delete tpg262;
  
  return 0;
}

#include <unistd.h>

#include <iostream>
#include <cmath>

#ifdef USE_FAKEIO
#include "AgilentTwisTorr304Fake.h"
typedef AgilentTwisTorr304Fake AgilentTwisTorr304_t;
#else
#include "AgilentTwisTorr304.h"
typedef AgilentTwisTorr304 AgilentTwisTorr304_t;
#endif

int main()
{
  AgilentTwisTorr304_t twistorr("/dev/ttyUSB0");

  /*
  std::string command, hexcommand;

  twistorr.MakeReadCommand(command, 205);
  twistorr.GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  twistorr.MakeWriteCommand(command, 0, true);
  twistorr.GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  twistorr.MakeWriteCommand(command, 0, false);
  twistorr.GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  twistorr.MakeWriteCommand(command, 100, true);
  twistorr.GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  twistorr.MakeWriteCommand(command, 100, false);
  twistorr.GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;
  */

  twistorr.GetPumpStatus();

  std::cout << twistorr.GetPumpStatus() << std::endl;
  
  return 0;
}

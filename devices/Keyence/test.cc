#include <string>
#include <vector>
#include <iostream>
#include "Keyence.h"

int main()
{
  Keyence laser("/dev/ttyUSB0");

  laser.ModeChange();
  double value = 0;
  std::cout << "MeasurementValueOutput: "; laser.MeasurementValueOutput(1, value);
  std::cout << "Timing: "; laser.Timing(1, 0);
  std::cout << "Timing: "; laser.Timing(1, 1);
  std::cout << "AutoZero: "; laser.AutoZero(1, 0);
  std::cout << "AutoZero: "; laser.AutoZero(1, 1);
  std::cout << "Reset: "; laser.Reset(1);
  std::cout << "PanelLock: "; laser.PanelLock(0);
  std::cout << "PanelLock: "; laser.PanelLock(1);
  std::cout << "ProgramChange: "; laser.ProgramChange(3);
  int value2= 0;
  std::cout << "ProgramCheck: "; laser.ProgramCheck(value2);
  std::string value3;
  std::cout << "StatResultOutput: "; laser.StatResultOutput(1, value3);
  std::cout << "ClearStat: "; laser.ClearStat(1);
  std::cout << "StartDataStorage: "; laser.StartDataStorage();
  std::cout << "StopDataStorage: "; laser.StopDataStorage();
  std::cout << "InitDataStorage: "; laser.InitDataStorage();
  std::vector<double> value4;
  std::cout << "OutputDataStorage: "; laser.OutputDataStorage(1, value4);
  std::string value5;
  std::cout << "DataStorageStatus: "; laser.DataStorageStatus(value5);
  std::cout << "SetSamplingRate: "; laser.SetSamplingRate(0);
  std::cout << "SetSamplingRate: "; laser.SetSamplingRate(4);
  std::cout << "SetAveraging: "; laser.SetAveraging(1, 4);
  return 0;
}

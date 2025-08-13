/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <iostream>
#include "Keyence.h"

int main()
{
  Keyence laser("/dev/ttyUSB0");

  double value = 0;
  //std::cout << "MeasurementValueOutput: \n"; laser.MeasurementValueOutput(1, value); std::cout << "value" << value << std::endl;
  std::cout << "MeasurementValueOutput 1: \n"; laser.MeasurementValueOutput(2, value); std::cout << "value" << value << std::endl;
  
  usleep(5000);

  std::cout << "MeasurementValueOutput 2: \n"; laser.MeasurementValueOutput(2, value); std::cout << "value" << value << std::endl;
  std::cout << "MeasurementValueOutput 3: \n"; laser.MeasurementValueOutput(2, value); std::cout << "value" << value << std::endl;
  //std::cout << "PanelLock: \n"; laser.PanelLock(1);
  //std::cout << "PanelLock: \n"; laser.PanelLock(1);
  //std::cout << "SetSamplingRate: \n"; laser.SetSamplingRate(0);
  //std::cout << "SetSamplingRate: \n"; laser.SetSamplingRate(4);
  std::cout << "SetAveraging: \n"; laser.SetAveraging(1, 4);

  /*
  std::cout << "Timing: "; laser.Timing(1, 0);
  std::cout << "Timing: "; laser.Timing(1, 1);
  std::cout << "AutoZero: "; laser.AutoZero(1, 0);
  std::cout << "AutoZero: "; laser.AutoZero(1, 1);
  */
  //std::cout << "Reset: \n"; laser.Reset(1);
  //  int value2= 0;
  //std::cout << "ProgramCheck: \n"; laser.ProgramCheck(value2); std::cout << "value2 = " << value2 << std::endl;
  //std::cout << "ProgramChange: \n"; laser.ProgramChange(3);
  //std::cout << "ProgramCheck: \n"; laser.ProgramCheck(value2); std::cout << "value2 = " << value2 << std::endl;
  /*
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
  */

  return 0;
}

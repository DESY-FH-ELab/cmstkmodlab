/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef USE_FAKEIO
#include "LeyboldGraphixOneFake.h"
typedef LeyboldGraphixOneFake LeyboldGraphixOne_t;
#include "LeyboldGraphixThreeFake.h"
typedef LeyboldGraphixThreeFake LeyboldGraphixThree_t;
#else
#include "LeyboldGraphixOne.h"
typedef LeyboldGraphixOne LeyboldGraphixOne_t;
#include "LeyboldGraphixThree.h"
typedef LeyboldGraphixThree LeyboldGraphixThree_t;
#endif

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  std::string ioPort = "/dev/ttyUSB0";
  if (argc==2) ioPort = argv[1];

  std::cout << "test on " << ioPort << std::endl;

  {
    LeyboldGraphixOne_t leybold(ioPort.c_str());

    std::cout << "version:                 " << leybold.GetVersion() << std::endl;
    std::cout << "serial number:           " << leybold.GetSerialNumber() << std::endl;
    std::cout << "item number:             " << leybold.GetItemNumber() << std::endl;

    std::cout << "pressure:                " << leybold.GetPressure() << std::endl;
  }

  /*
  {
    LeyboldGraphixThree_t leybold("/dev/ttyUSB0");

    std::cout << "version:                 " << leybold.GetVersion() << std::endl;
    std::cout << "serial number:           " << leybold.GetSerialNumber() << std::endl;
    std::cout << "item number:             " << leybold.GetItemNumber() << std::endl;

    leybold.SetDisplayUnit(VLeyboldGraphix::DisplayUnit_mbar);
    VLeyboldGraphix::DisplayUnit unit = leybold.GetDisplayUnit();
    std::cout << "display unit:            " << leybold.GetDisplayUnitName(unit) << std::endl << std::endl;

    std::cout << "number of channels:      " << leybold.GetNumberOfChannels() << std::endl << std::endl;

    leybold.SetSensorName(1, "SYS");
    leybold.SetSensorName(2, "P1");
    leybold.SetSensorName(3, "P2");

    leybold.SetSetPointChannelAssignment(1, VLeyboldGraphix::SetPointChannelOff);
    leybold.SetSetPointChannelAssignment(2, VLeyboldGraphix::SetPointChannelOff);
    leybold.SetSetPointChannelAssignment(3, VLeyboldGraphix::SetPointChannelOff);
    leybold.SetSetPointChannelAssignment(4, VLeyboldGraphix::SetPointChannelOff);
    leybold.SetSetPointChannelAssignment(5, VLeyboldGraphix::SetPointChannelOff);
    leybold.SetSetPointChannelAssignment(6, VLeyboldGraphix::SetPointChannelOff);

    for (int i=1;i<4;++i) {

      if (leybold.GetSensorDetectionMode(i)!=VLeyboldGraphix::SensorDetectionAuto) {
        leybold.SetSensorDetectionMode(i, VLeyboldGraphix::SensorDetectionAuto);
      }

      VLeyboldGraphix::SensorType type = leybold.GetSensorType(i);

      if (type==VLeyboldGraphix::SensorType_NOSEN) continue;

      if (type==VLeyboldGraphix::SensorType_TTRx) {
        leybold.SetSensorType(i, VLeyboldGraphix::SensorType_TTR91N);
      }

      // leybold.SetSensorType(i, VLeyboldGraphix::SensorType_TTRx);

      std::cout << "sensor " << i << std::endl;

      std::cout << "  detection mode:        " << leybold.GetSensorDetectionMode(i) << std::endl;
      std::cout << "  type:                  " << leybold.GetSensorTypeName(i) << std::endl;
      leybold.SetSensorName(i, std::string("SENSOR")+std::to_string(i));
      std::cout << "  name:                  " << leybold.GetSensorName(i) << std::endl;
      VLeyboldGraphix::SensorStatus status = leybold.GetSensorStatus(i);
      std::cout << "  status:                " << leybold.GetSensorStatusText(status) << std::endl;
      std::cout << "  pressure:              " << leybold.GetPressure(i) << std::endl;

      std::cout << std::endl;
    }
  }
  */
  
  return 0;
}

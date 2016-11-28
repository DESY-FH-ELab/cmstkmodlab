#include <iostream>

#ifdef USE_FAKEIO
#include "LeyboldGraphixThreeFake.h"
typedef LeyboldGraphixThreeFake LeyboldGraphixThree_t;
#else
#include "LeyboldGraphixThree.h"
typedef LeyboldGraphixThree LeyboldGraphixThree_t;
#endif

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  LeyboldGraphixThree_t gt("/dev/ttyUSB0");

  std::cout << "version:                 " << gt.GetVersion() << std::endl;
  std::cout << "serial number:           " << gt.GetSerialNumber() << std::endl;
  std::cout << "item number:             " << gt.GetItemNumber() << std::endl;
  gt.SetDisplayUnit(VLeyboldGraphixThree::DisplayUnit_mbar);
  VLeyboldGraphixThree::DisplayUnit unit = gt.GetDisplayUnit();
  std::cout << "display unit:            " << gt.GetDisplayUnitName(unit) << std::endl << std::endl;

  std::cout << "number of channels:      " << gt.GetNumberOfChannels() << std::endl << std::endl;

  for (int i=1;i<4;++i) {

    gt.SetSensorDetectionMode(i, VLeyboldGraphixThree::SensorDetectionAuto);

    VLeyboldGraphixThree::SensorType type = gt.GetSensorType(i);
    if (type==VLeyboldGraphixThree::SensorType_NOSEN) continue;

    if (type==VLeyboldGraphixThree::SensorType_TTRx) gt.SetSensorType(i, VLeyboldGraphixThree::SensorType_TTR91N);

    std::cout << "sensor " << i << std::endl;

    std::cout << "  detection mode:        " << gt.GetSensorDetectionMode(i) << std::endl;
    std::cout << "  type:                  " << gt.GetSensorTypeName(i) << std::endl;
    gt.SetSensorName(i, std::string("SENSOR")+std::to_string(i));
    std::cout << "  name:                  " << gt.GetSensorName(i) << std::endl;
    VLeyboldGraphixThree::SensorStatus status = gt.GetSensorStatus(i);
    std::cout << "  status:                " << gt.GetSensorStatusText(status) << std::endl;
    std::cout << "  pressure:              " << gt.GetPressure(i) << std::endl;

    std::cout << std::endl;
  }

  std::cout << std::endl;

  gt.SetSetPointChannelAssignment(1, VLeyboldGraphixThree::SetPointChannel1);
  gt.SetSetPointChannelAssignment(2, VLeyboldGraphixThree::SetPointChannel1);
  gt.SetSetPointChannelAssignment(3, VLeyboldGraphixThree::SetPointChannel1);
  gt.SetSetPointChannelAssignment(4, VLeyboldGraphixThree::SetPointChannel1);
  gt.SetSetPointChannelAssignment(5, VLeyboldGraphixThree::SetPointChannel2);
  gt.SetSetPointChannelAssignment(6, VLeyboldGraphixThree::SetPointChannel3);

  for (int i=1;i<7;++i) {

    int setPointAssignment = gt.GetSetPointChannelAssignment(i);

    if (setPointAssignment==0) continue;

    std::cout << "set point " << i << std::endl;

    gt.SetSetPointOnPressure(i, 700);
    gt.SetSetPointOffPressure(i, 800);

    std::cout << "  assignment:            " << gt.GetSetPointChannelAssignment(i) << std::endl;
    std::cout << "  on pressure:           " << gt.GetSetPointOnPressure(i) << std::endl;
    std::cout << "  off pressure:          " << gt.GetSetPointOffPressure(i) << std::endl;
    std::cout << "  status:                " << gt.GetSetPointStatus(i) << std::endl;

    std::cout << std::endl;
  }

  return 0;
}

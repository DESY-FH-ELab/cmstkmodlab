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
    std::cout << "sensor " << i << std::endl;

    gt.SetSensorDetectionMode(i, VLeyboldGraphixThree::SensorDetectionManual);

    std::cout << "  detection mode:        " << gt.GetSensorDetectionMode(i) << std::endl;
    std::cout << "  type:                  " << gt.GetSensorType(i) << std::endl;
    gt.SetSensorName(i, std::string("SENSOR")+std::to_string(i));
    std::cout << "  name:                  " << gt.GetSensorName(i) << std::endl;
    VLeyboldGraphixThree::SensorStatus status = gt.GetSensorStatus(i);
    std::cout << "  status:                " << gt.GetSensorStatusText(status) << std::endl;
    std::cout << "  pressure:              " << gt.GetPressure(i) << std::endl;

    std::cout << std::endl;
  }

  return 0;
}

#ifndef _LEYBOLDGRAPHIXTHREE_H_
#define _LEYBOLDGRAPHIXTHREE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VLeyboldGraphixThree.h"
#include "LeyboldComHandler.h"

class LeyboldGraphixThree : public VLeyboldGraphixThree
{
 public:

  LeyboldGraphixThree( const ioport_t );
  ~LeyboldGraphixThree();

  bool DeviceAvailable() const;

  std::string GetVersion() const;
  int GetSerialNumber() const;
  int GetItemNumber() const;

  int GetNumberOfChannels() const;

  std::string GetSensorType(int sensor) const;

  std::string GetSensorName(int sensor) const;
  void SetSensorName(int sensor, const std::string& name);

  SensorStatus GetSensorStatus(int sensor) const;

  double GetPressure(int sensor) const;

  DisplayUnit GetDisplayUnit() const;
  void SetDisplayUnit(DisplayUnit);

 protected:

  void SendCommand(std::string& command) const;
  bool ReceiveData(std::string& buffer) const;
  void StripBuffer(char*) const;
  void DeviceInit();

  static constexpr const char Separator = 0x3B;
  static constexpr const char EOT       = 0x04;
  static constexpr const char SI        = 0x0F;
  static constexpr const char SO        = 0x0E;
  static constexpr const char ACK       = 0x06;
  static constexpr const char NACK      = 0x15;

  char GetChecksum(const std::string& buffer) const;

  LeyboldComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif // _LEYBOLDGRAPHIXTHREE_H_

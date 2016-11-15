#include <cstring>
#include <cmath>
#include <sstream>

#include "LeyboldGraphixThree.h"

LeyboldGraphixThree::LeyboldGraphixThree( const ioport_t ioPort )
  :VLeyboldGraphixThree(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new LeyboldComHandler( ioPort );
  DeviceInit();
}

LeyboldGraphixThree::~LeyboldGraphixThree()
{
  delete comHandler_;
}

std::string LeyboldGraphixThree::GetVersion() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "1";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}

int LeyboldGraphixThree::GetSerialNumber() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "2";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return std::atoi(buffer.c_str());
}

int LeyboldGraphixThree::GetItemNumber() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "3";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return std::atoi(buffer.c_str());
}

int LeyboldGraphixThree::GetNumberOfChannels() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "8";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return std::atoi(buffer.c_str());
}

std::string LeyboldGraphixThree::GetSensorType(int sensor) const
{
  if (sensor<1 || sensor>3) return std::string("out of range");

  std::string command;

  command += SI;
  command += std::to_string(sensor);
  command += Separator;
  command += "4";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}

std::string LeyboldGraphixThree::GetSensorName(int sensor) const
{
  if (sensor<1 || sensor>3) return std::string("out of range");

  std::string command;

  command += SI;
  command += std::to_string(sensor);
  command += Separator;
  command += "5";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}
void LeyboldGraphixThree::SetSensorName(int sensor, const std::string& name)
{
  if (sensor<1 || sensor>3) return;

  std::string command;

  command += SO;
  command += std::to_string(sensor);
  command += Separator;
  command += "5";
  command += Separator;
  command += name;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

LeyboldGraphixThree::SensorStatus LeyboldGraphixThree::GetSensorStatus(int sensor) const
{
  if (sensor<1 || sensor>3) return SensorStatus_nosen;

  std::string command;

  command += SI;
  command += std::to_string(sensor);
  command += Separator;
  command += "24";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  std::map<std::string,SensorStatus>::const_iterator itFind = sensorStatusText_.find(buffer);
  if (itFind!=sensorStatusText_.end()) {
    return itFind->second;
  }

  return SensorStatus_nosen;
}

double LeyboldGraphixThree::GetPressure(int sensor) const
{
  if (sensor<1 || sensor>3) return -1;

  std::string command;

  command += SI;
  command += std::to_string(sensor);
  command += Separator;
  command += "29";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  if (buffer=="....") return -1.0;

  return std::atof(buffer.c_str());
}

LeyboldGraphixThree::DisplayUnit LeyboldGraphixThree::GetDisplayUnit() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "4";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  std::map<std::string,DisplayUnit>::const_iterator itFind = displayNameUnits_.find(buffer);
  if (itFind!=displayNameUnits_.end()) {
    return itFind->second;
  }

  return DisplayUnit_unknown;
}

void LeyboldGraphixThree::SetDisplayUnit(LeyboldGraphixThree::DisplayUnit unit)
{
  std::string name;

  std::map<DisplayUnit,std::string>::const_iterator itFind = displayUnitNames_.find(unit);
  if (itFind!=displayUnitNames_.end()) {
    name = itFind->second;
  } else {
    return;
  }

  std::string command;

  command += SO;
  command += "5";
  command += Separator;
  command += "4";
  command += Separator;
  command += name;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

bool LeyboldGraphixThree::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

void LeyboldGraphixThree::SendCommand(std::string& command) const
{
  command += " ";
  char crc = GetChecksum(command);
  command += crc;
  command += EOT;

  // std::cout << command.length() << " |" << command.c_str() << "|" << std::endl;

  comHandler_->SendCommand(command.c_str());
}

bool LeyboldGraphixThree::ReceiveData(std::string& buffer) const
{
  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  buffer = buf;

  size_t idxEOT = buffer.find(EOT);
  char crc = buffer[idxEOT-1];

  buffer.resize(idxEOT-1);
  bool isACK = (buffer[0]==ACK);
  buffer.erase(0, 1);

  return isACK;
}

void LeyboldGraphixThree::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
    if(buffer[c]=='\n' || buffer[c]=='\r') {
      buffer[c] = 0;
      break;
    }
  }
}

char LeyboldGraphixThree::GetChecksum(const std::string& buffer) const
{
  int sum = 0;

  for (int i=0;i<buffer.length();++i) {
    sum += buffer[i];
  }

  char CRC = 255 - (sum%256);

  return CRC;
}

void LeyboldGraphixThree::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    int itemNumber = GetItemNumber();

    if (itemNumber==230682) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }
}

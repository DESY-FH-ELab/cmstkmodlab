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

  sensorStatus_[0] = SensorStatus_unknown;
  sensorStatus_[1] = SensorStatus_unknown;
  sensorStatus_[2] = SensorStatus_unknown;
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

VLeyboldGraphixThree::SensorDetectionMode LeyboldGraphixThree::GetSensorDetectionMode(int sensor) const
{
  std::string command;

  command += SI;
  command += std::to_string(sensor);
  command += Separator;
  command += "2";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  if (buffer=="Auto") {
    return SensorDetectionAuto;
  } else {
    return SensorDetectionManual;
  }
}

void LeyboldGraphixThree::SetSensorDetectionMode(int sensor, VLeyboldGraphixThree::SensorDetectionMode mode)
{
  std::string command;

  command += SO;
  command += std::to_string(sensor);
  command += Separator;
  command += "2";
  command += Separator;

  if (mode==SensorDetectionAuto) {
    command += "Auto";
  } else {
    command += "Manual";
  }

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
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

void LeyboldGraphixThree::SetSensorType(int sensor, std::string type)
{
  if (sensor<1 || sensor>3) return;

  std::string command;

  command += SO;
  command += std::to_string(sensor);
  command += Separator;
  command += "4";
  command += Separator;
  command += type;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
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
    sensorStatus_[sensor-1] = itFind->second;
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

  if (sensorStatus_[sensor-1]!=SensorStatus_ok) {
    // std::cout << "sensor " << sensor << " not OK" << std::endl;
    return -1;
  }

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

VLeyboldGraphixThree::SetPointChannel LeyboldGraphixThree::GetSetPointChannelAssignment(int sp) const
{
  if (sp<1 || sp>6) return SetPointChannelOff;

  std::string command;

  command += SI;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 1);

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  if (buffer=="Off") {
    return SetPointChannelOff;
  } else if (buffer=="1") {
    return SetPointChannel1;
  } else if (buffer=="2") {
    return SetPointChannel2;
  } else if (buffer=="3") {
    return SetPointChannel3;
  }

  return SetPointChannelOff;
}

void LeyboldGraphixThree::SetSetPointChannelAssignment(int sp, VLeyboldGraphixThree::SetPointChannel channel)
{
  if (sp<1 || sp>6) return;

  std::string command;

  command += SO;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 1);
  command += Separator;

  if (channel==SetPointChannel1) {
    command += "1";
  } else if (channel==SetPointChannel2) {
    command += "2";
  } else if(channel==SetPointChannel3) {
    command += "3";
  } else {
    command += "Off";
  }

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

double LeyboldGraphixThree::GetSetPointOnPressure(int sp) const
{
  if (sp<1 || sp>6) return -1;

  std::string command;

  command += SI;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 2);

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return std::atof(buffer.c_str());
}

void LeyboldGraphixThree::SetSetPointOnPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;

  std::string command;

  command += SO;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 2);
  command += Separator;
  command += std::to_string(pressure);

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

double LeyboldGraphixThree::GetSetPointOffPressure(int sp) const
{
  if (sp<1 || sp>6) return -1;

  std::string command;

  command += SI;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 3);

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return std::atof(buffer.c_str());
}

void LeyboldGraphixThree::SetSetPointOffPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;

  std::string command;

  command += SI;
  command += SO;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 3);
  command += Separator;
  command += std::to_string(pressure);

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

bool LeyboldGraphixThree::GetSetPointStatus(int sp) const
{
  if (sp<1 || sp>6) return false;

  std::string command;

  command += SI;
  command += "4";
  command += Separator;
  command += std::to_string((sp-1)*4 + 4);

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  if (buffer=="On") {
    return true;
  }

  return false;
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

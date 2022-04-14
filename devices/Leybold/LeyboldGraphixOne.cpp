/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <cstring>
#include <cmath>
#include <sstream>

#include "LeyboldGraphixOne.h"

LeyboldGraphixOne::LeyboldGraphixOne( const ioport_t ioPort )
  :VLeyboldGraphixOne(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new LeyboldComHandler( ioPort );
  DeviceInit();

  sensorStatus_ = SensorStatus_unknown;
}

LeyboldGraphixOne::~LeyboldGraphixOne()
{
  delete comHandler_;
}

std::string LeyboldGraphixOne::GetVersion() const
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

int LeyboldGraphixOne::GetSerialNumber() const
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

std::string LeyboldGraphixOne::GetItemNumber() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "3";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}

int LeyboldGraphixOne::GetNumberOfChannels() const
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

VLeyboldGraphix::SensorDetectionMode LeyboldGraphixOne::GetSensorDetectionMode() const
{
  std::string command;

  command += SI;
  command += "1";
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

void LeyboldGraphixOne::SetSensorDetectionMode(VLeyboldGraphix::SensorDetectionMode mode)
{
  std::string command;

  command += SO;
  command += "1";
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

std::string LeyboldGraphixOne::GetSensorTypeName() const
{
  std::string command;

  command += SI;
  command += "1";
  command += Separator;
  command += "4";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}

void LeyboldGraphixOne::SetSensorTypeName(std::string type)
{
  std::string command;

  command += SO;
  command += "1";
  command += Separator;
  command += "4";
  command += Separator;
  command += type;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

std::string LeyboldGraphixOne::GetSensorName() const
{
  std::string command;

  command += SI;
  command += "1";
  command += Separator;
  command += "5";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}
void LeyboldGraphixOne::SetSensorName(const std::string& name)
{
  std::string command;

  command += SO;
  command += "1";
  command += Separator;
  command += "5";
  command += Separator;
  command += name;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

VLeyboldGraphix::SensorStatus LeyboldGraphixOne::GetSensorStatus() const
{
  std::string command;

  command += SI;
  command += "1";
  command += Separator;
  command += "24";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  sensorStatus_ = GetSensorStatusByStatusText(buffer);
  
  return sensorStatus_;
}

double LeyboldGraphixOne::GetPressure() const
{
  if (GetSensorStatus()!=SensorStatus_ok) {
    std::cout << "sensor not OK" << std::endl;
    return 1000.;
  }
  
  std::string command;

  command += SI;
  command += "1";
  command += Separator;
  command += "29";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return std::atof(buffer.c_str());
}

VLeyboldGraphix::DisplayUnit LeyboldGraphixOne::GetDisplayUnit() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "4";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return GetDisplayUnitByUnitText(buffer);
}

void LeyboldGraphixOne::SetDisplayUnit(VLeyboldGraphix::DisplayUnit unit)
{
  std::string name = GetDisplayUnitName(unit);

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

VLeyboldGraphix::SetPointChannel LeyboldGraphixOne::GetSetPointChannelAssignment(int sp) const
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

void LeyboldGraphixOne::SetSetPointChannelAssignment(int sp, VLeyboldGraphix::SetPointChannel channel)
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

double LeyboldGraphixOne::GetSetPointOnPressure(int sp) const
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

void LeyboldGraphixOne::SetSetPointOnPressure(int sp, double pressure)
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

double LeyboldGraphixOne::GetSetPointOffPressure(int sp) const
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

void LeyboldGraphixOne::SetSetPointOffPressure(int sp, double pressure)
{
  if (sp<1 || sp>6) return;

  std::string command;

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

bool LeyboldGraphixOne::GetSetPointStatus(int sp) const
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

std::string LeyboldGraphixOne::GetDate() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "21";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}

void LeyboldGraphixOne::SetDate(const std::string& date)
{
  std::string command;

  command += SO;
  command += "5";
  command += Separator;
  command += "21";
  command += Separator;
  command += date;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

std::string LeyboldGraphixOne::GetTime() const
{
  std::string command;

  command += SI;
  command += "5";
  command += Separator;
  command += "20";

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);

  return buffer;
}

void LeyboldGraphixOne::SetTime(const std::string& time)
{
  std::string command;

  command += SO;
  command += "5";
  command += Separator;
  command += "20";
  command += Separator;
  command += time;

  SendCommand(command);

  std::string buffer;
  bool isACK = ReceiveData(buffer);
}

bool LeyboldGraphixOne::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

void LeyboldGraphixOne::SendCommand(std::string& command) const
{
  command += " ";
  char crc = GetChecksum(command);
  command += crc;
  command += EOT;

  /*
  std::cout << "LeyboldGraphixOne::SendCommand "
	    << command.length()
	    << " |"
	    << command.c_str()
	    << "|" << std::endl;
  */
  
  comHandler_->SendCommand(command.c_str());

  usleep(50);
}

bool LeyboldGraphixOne::ReceiveData(std::string& buffer) const
{
  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  buffer = buf;

  bool isACK = false;

  if (buffer[0]==ACK) isACK = true;
  buffer.erase(0, 1);

  if (!isACK) {
    std::cout << "*** NACK *** " << buffer[0] << std::endl;
    return isACK;
  }

  size_t idxEOT = buffer.find(EOT);
  if (idxEOT==std::string::npos) {
    return false;
  }

  char crc = buffer[idxEOT-1];
  buffer.resize(idxEOT-1);

  // std::cout << " ->  |" << buffer << "|" << std::endl;

  return isACK;
}

void LeyboldGraphixOne::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
    if(buffer[c]=='\n' || buffer[c]=='\r') {
      buffer[c] = 0;
      break;
    }
  }
}

char LeyboldGraphixOne::GetChecksum(const std::string& buffer) const
{
  int sum = 0;

  for (int i=0;i<buffer.length();++i) {
    sum += buffer[i];
  }

  char CRC = 255 - (sum%256);

  return CRC;
}

void LeyboldGraphixOne::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    std::string itemNumber = GetItemNumber();

    if (itemNumber=="230680V01") {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }
}

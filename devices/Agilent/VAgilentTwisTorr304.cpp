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

#include <iostream>
#include <sstream>
#include <iomanip>

#include "VAgilentTwisTorr304.h"

///
///
///
VAgilentTwisTorr304::VAgilentTwisTorr304(const ioport_t ioPort)
{

}

const std::string VAgilentTwisTorr304::GetPumpStatusText(VAgilentTwisTorr304::StatusCode status) const
{
  switch (status) {
  case Stop:
    return "Stop";
    break;
  case WaitingInterlock:
    return "Waiting for interlock";
    break;
  case Starting:
    return "Starting";
    break;
  case AutoTuning:
    return "Auto tuning";
    break;
  case Braking:
    return "Braking";
    break;
  case Normal:
    return "Normal";
        break;
  case Fail:
    return "Failure";
    break;
  default:
    return "Unknown status";
  }
}

const std::string VAgilentTwisTorr304::GetPumpStatusText() const
{
  VAgilentTwisTorr304::StatusCode status = GetPumpStatus();
  return GetPumpStatusText(status);
}

void VAgilentTwisTorr304::MakeReadCommand(std::string& command, unsigned int window) const
{
  command = AgilentSTX;
  command += 0x80;

  char win[4];
  sprintf(win, "%03d", window);
  command += win;

  command += AgilentRD;
  command += AgilentETX;

  char crc0, crc1;

  GetChecksum(command, crc0, crc1);

  command += crc1;
  command += crc0;
}

void VAgilentTwisTorr304::MakeWriteCommand(std::string& command, unsigned int window, bool data) const
{
  command = AgilentSTX;
  command += 0x80;

  char win[4];
  sprintf(win, "%03d", window);
  command += win;

  command += AgilentWR;

  if (data) {
    command += AgilentON;
  } else {
    command += AgilentOFF;
  }

  command += AgilentETX;

  char crc0, crc1;

  GetChecksum(command, crc0, crc1);

  command += crc1;
  command += crc0;
}

void VAgilentTwisTorr304::MakeWriteCommand(std::string& command, unsigned int window, int data) const
{
  command = AgilentSTX;
  command += 0x80;

  char win[4];
  sprintf(win, "%03d", window);
  command += win;

  command += AgilentWR;

  char number[7];
  sprintf(number, "%06d", data);
  command += number;

  command += AgilentETX;

  char crc0, crc1;

  GetChecksum(command, crc0, crc1);

  command += crc1;
  command += crc0;
}

void VAgilentTwisTorr304::MakeWriteCommand(std::string& command, unsigned int window, float data, const char* format) const
{
  command = AgilentSTX;
  command += 0x80;

  char win[4];
  sprintf(win, "%03d", window);
  command += win;

  command += AgilentWR;

  char number[7];
  sprintf(number, format, data);
  command += number;

  command += AgilentETX;

  char crc0, crc1;

  GetChecksum(command, crc0, crc1);

  command += crc1;
  command += crc0;
}

void VAgilentTwisTorr304::MakeWriteCommand(std::string& command, unsigned int window, std::string& data) const
{
  command = AgilentSTX;
  command += 0x80;

  char win[4];
  sprintf(win, "%03d", window);
  command += win;

  command += AgilentWR;

  char text[11];
  if (data.length()>10) {
  	sprintf(text, "%10s", data.substr(0, 10).c_str());
  } else {
  	sprintf(text, "%10s", data.c_str());
  }
  command += text;

  command += AgilentETX;

  char crc0, crc1;

  GetChecksum(command, crc0, crc1);

  command += crc1;
  command += crc0;
}

bool VAgilentTwisTorr304::GetBooleanValue(std::string& reply) const
{
  std::string valuestring = reply.substr(6, 1);

  std::string hexvalue;

  GetCommandAsHex(hexvalue, valuestring);
  std::cout << "GetBooleanValue: " << hexvalue << ": " << std::stoi(valuestring) << std::endl;

  return std::stoi(valuestring);
}

int VAgilentTwisTorr304::GetIntegerValue(std::string& reply) const
{
  std::string valuestring = reply.substr(6, 6);

  std::string hexvalue;

  GetCommandAsHex(hexvalue, valuestring);
  std::cout << "GetIntegerValue: " << hexvalue << ": " << std::stoi(valuestring) << std::endl;

  return std::stoi(valuestring);
}

float VAgilentTwisTorr304::GetFloatValue(std::string& reply) const
{
  std::string valuestring = reply.substr(6, 6);

  std::string hexvalue;

  GetCommandAsHex(hexvalue, valuestring);
  std::cout << "GetFloatValue: " << hexvalue << ": " << std::stof(valuestring) << std::endl;

  return std::stof(valuestring);
}

std::string VAgilentTwisTorr304::GetStringValue(std::string& reply) const
{
  std::string valuestring = reply.substr(6, 10);

  std::string hexvalue;

  GetCommandAsHex(hexvalue, valuestring);
  std::cout << "GetStringValue: " << hexvalue << ": " << valuestring << std::endl;
  
  return valuestring;
}

void VAgilentTwisTorr304::GetChecksum(const std::string& buffer, char& crc0, char& crc1) const
{
  unsigned int crc = 0;

  auto i = buffer.cbegin();

  while (i!=buffer.cend()) {
    ++i;
    if (*i=='\0') break;
    crc ^= (0xff&(unsigned int)*i);
  }

  std::stringstream ss;

  ss << std::hex << std::uppercase << (0xff&crc);
  crc1 = ss.get();
  crc0 = ss.get();
}

void VAgilentTwisTorr304::GetCommandAsHex(std::string& hexcommand, const std::string& command) const
{
  auto i = command.cbegin();

  std::stringstream ss;
  ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (0xff&(unsigned int)*i);
  while (i!=command.cend()) {
    ++i;
    if (*i=='\0') break;
    ss << ", 0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (0xff&(unsigned int)*i);
  }

  std::cout << "GetCommandAsHex: " << command << ": " << hexcommand << std::endl;

  hexcommand = ss.str();
}

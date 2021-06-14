/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <chrono>

#include "KeithleyDAQ6510.h"

///
///
///
KeithleyDAQ6510::KeithleyDAQ6510(ioport_t port)
    : VKeithleyDAQ6510(port)
{
  std::cout << "KeithleyDAQ6510::KeithleyDAQ6510(ioport_t port)" << std::endl;

  comHandler_ = new KeithleyUSBTMCComHandler(port);

  deviceChannelsSet_ = false;

  DeviceInit();
}

KeithleyDAQ6510::~KeithleyDAQ6510( )
{
  std::cout << "KeithleyDAQ6510::~KeithleyDAQ6510()" << std::endl;

  delete comHandler_;
}

void KeithleyDAQ6510::ShutDown()
{
  // reset the device
  comHandler_->SendCommand("*RST");

  // clear event registers and queues
  comHandler_->SendCommand("*CLS");

  comHandler_->CloseIoPort();
}

void KeithleyDAQ6510::SetTime(int year, int month, int day,
                              int hour, int minute, int second)
{
  std::stringstream ss;

  ss << ":SYST:TIME ";
  ss << year << ", ";
  ss << month << ", ";
  ss << day << ", ";
  ss << hour << ", ";
  ss << minute << ", ";
  ss << second;

  comHandler_->SendCommand(ss.str().c_str());
}

void KeithleyDAQ6510::SetChannelMode(unsigned int card, unsigned int channel,
		ChannelMode_t mode)
{
  if (card<1 || card>2) return;
  if (channel<1 || channel>10) return;

  std::stringstream ss;
  ss << "(@";
  ss << card * 100 + channel;
  ss << ")";

  if (mode==FourWireRTD_PT100) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN FRTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:FOUR PT100, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==ThreeWireRTD_PT100) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TRTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:THR PT100, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==TwoWireRTD_PT100) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN RTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:TWO PT100, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==FourWireRTD_PT385) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN FRTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:FOUR PT385, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==ThreeWireRTD_PT385) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TRTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:THR PT385, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==TwoWireRTD_PT385) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN RTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:TWO PT385, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==FourWireRTD_PT3916) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN FRTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:FOUR PT3916, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==ThreeWireRTD_PT3916) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TRTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:THR PT3916, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==TwoWireRTD_PT3916) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN RTD, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:RTD:TWO PT3916, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==Thermistor_2252) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN THER, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:THER 2252, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==Thermistor_5000) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN THER, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:THER 5000, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==Thermistor_10000) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN THER, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:THER 10000, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());

  } else if (mode==TCouple_B) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE B, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_E) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE E, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_J) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE J, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_K) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE K, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_N) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE N, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_R) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE R, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_S) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE S, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  } else if (mode==TCouple_T) {
    comHandler_->SendCommand(std::string("SENS:FUNC 'TEMP', ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TRAN TC, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:TC:TYPE T, ") + ss.str());
    comHandler_->SendCommand(std::string("SENS:TEMP:UNIT CELS, ") + ss.str());
    comHandler_->SendCommand(std::string("TEMP:TC:RJUN:SIM 30, ") + ss.str());

  }

  channelModes_[card-1][channel-1] = mode;
}

void KeithleyDAQ6510::ActivateChannel(unsigned int card, unsigned int channel,
                                      bool active)
{
  if (card<1 || card>2) return;
  if (channel<1 || channel>10) return;
  activeChannels_[card-1][channel-1] = active;

  sensorScanCountMap_.clear();

  unsigned int scanCount = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[0][channel-1]) {
      sensorScanCountMap_[scanCount] = 100 + channel;
      scanCount++;
    }
  }
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[1][channel-1]) {
      sensorScanCountMap_[scanCount] = 200 + channel;
      scanCount++;
    }
  }
  
  deviceChannelsSet_ = false;
}

bool KeithleyDAQ6510::GetScanStatus() const
{
  char buffer[1000];
  std::string buf;

  comHandler_->SendCommand(":ROUT:SCAN:STAT?");
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  buf = buffer;
  // std::cout << buf << std::endl;

  return true;
}

void KeithleyDAQ6510::Scan()
{
  char buffer[1000];
  std::string buf;

  comHandler_->SendCommand("TRAC:CLE");

  comHandler_->SendCommand("INIT");
  if (!deviceChannelsSet_) DeviceSetChannels();
    
}

void KeithleyDAQ6510::GetScanData(reading_t & data)
{
  char buffer[2048];
  std::string buf;

  std::stringstream ss;
  ss << ":TRAC:DATA? 1, ";
  ss << GetActiveChannelCount();
  ss << ", 'defbuffer1', CHAN, READ, REL";

  comHandler_->SendCommand(ss.str().c_str());
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  buf = buffer;
  std::cout << buf << std::endl;

  std::vector<std::string> tokens(0);
  Tokenize(buf, tokens, ",");

  unsigned int scanCount = 0;
  for (std::vector<std::string>::iterator it=tokens.begin();
       it!=tokens.end();
       ++it) {

    unsigned int sensor;
    if (*it=="Rear") {
      sensor = GetSensorFromScanCount(scanCount);
      ++it;
    } else {
      sensor = std::atoi(it->c_str()); ++it;
    }

    // unsigned int sensor = GetSensorFromScanCount(scanCount);

    double temperature = std::atof(it->c_str()); ++it;
    double relTime = std::atof(it->c_str());

    data.push_back(std::tuple<unsigned int,double,double>(sensor,temperature,relTime));

    scanCount++;
  }
}

unsigned int KeithleyDAQ6510::GetSensorFromScanCount(unsigned int scanCount)
{
  auto search = sensorScanCountMap_.find(scanCount);
  if (search != sensorScanCountMap_.end()) return search->second;
  return 0;
}

float KeithleyDAQ6510::GetScanDuration() const
{
  unsigned int channelCount = GetActiveChannelCount();

  return (channelCount+1) * 0.5;
}

void KeithleyDAQ6510::DeviceSetChannels()
{
  std::stringstream ss;
  unsigned int count;

  // build rout:scan command
  ss << ":ROUT:SCAN:CRE (@";

  count = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[0][channel-1]) count++;
  }
  if (count) {
    ss << CreateChannelString(1, activeChannels_[0]);
  }

  count = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[1][channel-1]) count++;
  }
  if (count) {
    ss << ",";
    ss << CreateChannelString(2, activeChannels_[1]);
  }

  ss << ")";

  std::cout << ss.str() << std::endl;

  comHandler_->SendCommand(ss.str().c_str());

  deviceChannelsSet_ = true;
}

void KeithleyDAQ6510::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {

    std::cout << "void KeithleyDAQ6510::DeviceInit()" << std::endl;

    char buffer[1000];
    std::string buf;

    // get device identifyer
    comHandler_->SendCommand("*IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("KEITHLEY INSTRUMENTS,MODEL DAQ6510", 0)!=0) return;

    // reset the device
    comHandler_->SendCommand("*RST");

    // clear event registers and queues
    comHandler_->SendCommand("*CLS");

    // setting the time seems to mess up communication over usbtmc
    /*
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);
    SetTime(local_tm.tm_year + 1900,
            local_tm.tm_mon + 1,
            local_tm.tm_mday,
            local_tm.tm_hour,
            local_tm.tm_min,
            local_tm.tm_sec);

    comHandler_->SendCommand(":SYST:TIME? 1\n");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    */

    // open all routings
    // comHandler_->SendCommand("ROUT:OPEN:ALL");

    // set ascii format precision
    // comHandler_->SendCommand(":FORM:ASC:PREC 6");

    comHandler_->SendCommand(":SYST:CARD1:IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("7700,20Ch Mux w/CJC", 0)==0) {
      availableCards_[0] = true;
      for (unsigned int channel = 1;channel<=10;++channel) {
        availableChannels_[0][channel-1] = true;
        activeChannels_[0][channel-1] = false;
        channelModes_[0][channel-1] = FourWireRTD_PT100;
      }

      /*
      comHandler_->SendCommand("SENS:FUNC 'TEMP', (@101:110)");
      comHandler_->SendCommand("SENS:TEMP:TRAN FRTD, (@101:110)");
      comHandler_->SendCommand("SENS:TEMP:RTD:FOUR PT100, (@101:110)");
      comHandler_->SendCommand("SENS:TEMP:UNIT CELS, (@101:110)");
			*/

      comHandler_->SendCommand("SENS:FUNC 'VOLT', (@101:120)");

      // set channel delay to 0.1 s
      comHandler_->SendCommand("ROUT:DEL 0.1, (@101:110)");
    }

    comHandler_->SendCommand(":SYST:CARD2:IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("7700,20Ch Mux w/CJC", 0)==0) {
      availableCards_[1] = true;
      for (unsigned int channel = 1;channel<=10;++channel) {
        availableChannels_[1][channel-1] = true;
        activeChannels_[1][channel-1] = false;
        channelModes_[1][channel-1] = FourWireRTD_PT100;
     }

      /*
      comHandler_->SendCommand("SENS:FUNC 'TEMP', (@201:220)");
      comHandler_->SendCommand("SENS:TEMP:TRAN FRTD, (@201:220)");
      comHandler_->SendCommand("SENS:TEMP:RTD:FOUR PT100, (@201:220)");
      comHandler_->SendCommand("SENS:TEMP:UNIT CELS, (@201:220)");
			*/

      comHandler_->SendCommand("SENS:FUNC 'VOLT', (@201:220)");

      // set channel delay to 0.1 s
      comHandler_->SendCommand("ROUT:DEL 0.1, (@201:210)");
    }
    
    isDeviceAvailable_ = true;
  }
}

void KeithleyDAQ6510::StripBuffer(char* buffer) const
{
  for (unsigned int c=0;c<std::strlen(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}

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
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <chrono>

#ifndef __DEBUG
// #define __DEBUG 1
#endif

#include "KeithleyDAQ6510.h"

///
///
///
KeithleyDAQ6510::KeithleyDAQ6510(ioport_t port)
    : VKeithleyDAQ6510(port)
{
#ifdef __DEBUG
  std::cout << "KeithleyDAQ6510::KeithleyDAQ6510(ioport_t port)" << std::endl;
#endif

  comHandler_ = new KeithleyUSBTMCComHandler(port);

  deviceChannelsSet_ = false;

  DeviceInit();
}

KeithleyDAQ6510::~KeithleyDAQ6510( )
{
#ifdef __DEBUG
  std::cout << "KeithleyDAQ6510::~KeithleyDAQ6510()" << std::endl;
#endif

  delete comHandler_;
}

void KeithleyDAQ6510::ShutDown()
{
#ifdef __DEBUG
  std::cout << "KeithleyDAQ6510::ShutDown()" << std::endl;
#endif

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

  if (channelModes_[card-1][channel-1]==mode) return;

  std::stringstream ss;
  ss << "(@";
  ss << card * 100 + channel;
  ss << ")";

  std::stringstream ssc;

  if (mode==FourWireRTD_PT100) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN FRTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:FOUR PT100, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==ThreeWireRTD_PT100) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TRTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:THR PT100, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TwoWireRTD_PT100) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN RTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:TWO PT100, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==FourWireRTD_PT385) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN FRTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:FOUR PT385, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==ThreeWireRTD_PT385) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TRTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:THR PT385, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TwoWireRTD_PT385) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN RTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:TWO PT385, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==FourWireRTD_PT3916) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN FRTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:FOUR PT3916, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==ThreeWireRTD_PT3916) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TRTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:THR PT3916, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TwoWireRTD_PT3916) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN RTD, " << ss.str();
    ssc << ";:SENS:TEMP:RTD:TWO PT3916, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==Thermistor_2252) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN THER, " << ss.str();
    ssc << ";:SENS:TEMP:THER 2252, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==Thermistor_5000) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN THER, " << ss.str();
    ssc << ";:SENS:TEMP:THER 5000, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==Thermistor_10000) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN THER, " << ss.str();
    ssc << ";:SENS:TEMP:THER 10000, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_B) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE B, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_E) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE E, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_J) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE J, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_K) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE K, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_N) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE N, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_R) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE R, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_S) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE S, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  } else if (mode==TCouple_T) {
    ssc << ":SENS:FUNC 'TEMP', " << ss.str();
    ssc << ";:SENS:TEMP:TRAN TC, " << ss.str();
    ssc << ";:SENS:TEMP:TC:TYPE T, " << ss.str();
    ssc << ";:SENS:TEMP:TC:RJUN:SIM 30, " << ss.str();
    ssc << ";:SENS:TEMP:UNIT CELS, " << ss.str();

  }

  comHandler_->SendCommand(ssc.str());

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
#ifdef __DEBUG
  std::cout << "void KeithleyDAQ6510::GetScanStatus() const " << buf << std::endl;
#endif

  return true;
}

void KeithleyDAQ6510::Scan()
{
  /*
  char buffer[1000];
  std::string buf;
    
  // get device identifyer
  comHandler_->SendCommand("*IDN?");
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  buf = buffer;
  std::cout << buf << std::endl;
  */

  if (GetActiveChannelCount()==0) return;
  
  if (!deviceChannelsSet_) DeviceSetChannels();
    
  comHandler_->SendCommand(":ROUT:SCAN:MODE ALL");
  //comHandler_->SendCommand(":ABOR");
  //comHandler_->SendCommand(":TRAC:FILL:MODE ONCE");
  comHandler_->SendCommand(":TRAC:CLE 'defbuffer1'");
  comHandler_->SendCommand(":INIT:IMM");
}

void KeithleyDAQ6510::GetScanData(reading_t & data)
{
  char buffer[2048];
  std::string buf;

  if (GetActiveChannelCount()==0) return;

  std::stringstream ss;
  ss << ":TRAC:DATA? 1, ";
  ss << GetActiveChannelCount();
  // ss << ", 'defbuffer1', CHAN, READ, REL";
  ss << ", 'defbuffer1', CHAN, READ";

  comHandler_->SendCommand(ss.str().c_str());
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  buf = buffer;
#ifdef __DEBUG
  std::cout << "void KeithleyDAQ6510::GetScanData(reading_t & data) " << buf << std::endl;
#endif

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

    double temperature = std::atof(it->c_str());

    /*
    ++it;
    double relTime = std::atof(it->c_str());
    */
    
    //data.push_back(std::tuple<unsigned int,double,double>(sensor,temperature,relTime));
    data.push_back(std::tuple<unsigned int,double,double>(sensor,temperature,0.0));
    
    scanCount++;
  }

  // clear buffer
  comHandler_->SendCommand(":TRAC:CLE 'defbuffer1'");

  // open all routings
  comHandler_->SendCommand(":ROUT:OPEN:ALL");
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
  unsigned int count1, count2;

  // build rout:scan command
  ss << ":ROUT:SCAN:CRE (@";

  count1 = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[0][channel-1]) count1++;
  }
  if (count1) {
    ss << CreateChannelString(1, activeChannels_[0]);
  }

  count2 = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[1][channel-1]) count2++;
  }
  if (count2) {
    if (count1) {
      ss << ",";
    }
    ss << CreateChannelString(2, activeChannels_[1]);
  }
  
  ss << ")";

#ifdef __DEBUG
  std::cout << "void KeithleyDAQ6510::DeviceSetChannels() " << ss.str() << std::endl;
#endif

  comHandler_->SendCommand(ss.str().c_str());

  deviceChannelsSet_ = true;
}

void KeithleyDAQ6510::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {

#ifdef __DEBUG
    std::cout << "void KeithleyDAQ6510::DeviceInit()" << std::endl;
#endif

    char buffer[1000];
    std::string buf;

    // get device identifyer
    comHandler_->SendCommand("*IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

#ifdef __DEBUG
    std::cout << "*IDN? " << buf << std::endl;
#endif

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

#ifdef __DEBUG
    std::cout << ":SYST:CARD1:IDN? " << buf << std::endl;
#endif

    if (buf.find("7700,20Ch Mux w/CJC", 0)==0) {
      availableCards_[0] = true;
      for (unsigned int channel = 1;channel<=10;++channel) {
        availableChannels_[0][channel-1] = true;
        activeChannels_[0][channel-1] = false;
        channelModes_[0][channel-1] = FourWireRTD_PT100;
      }

      std::stringstream ss;
      ss << ":SENS:FUNC 'TEMP', (@101:110)";
      ss << ";:SENS:TEMP:TRAN FRTD, (@101:110)";
      ss << ";:SENS:TEMP:RTD:FOUR PT100, (@101:110)";
      ss << ";:SENS:TEMP:UNIT CELS, (@101:110)";
      ss << ";:ROUT:CHAN:DEL 0.1, (@101:110)"; // set channel delay to 0.1 s
      comHandler_->SendCommand(ss.str());
    }

    comHandler_->SendCommand(":SYST:CARD2:IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

#ifdef __DEBUG
    std::cout << ":SYST:CARD2:IDN? " << buf << std::endl;
#endif

    if (buf.find("7700,20Ch Mux w/CJC", 0)==0) {
      availableCards_[1] = true;
      for (unsigned int channel = 1;channel<=10;++channel) {
        availableChannels_[1][channel-1] = true;
        activeChannels_[1][channel-1] = false;
        channelModes_[1][channel-1] = FourWireRTD_PT100;
      }

      std::stringstream ss;
      ss << ":SENS:FUNC 'TEMP', (@201:210)";
      ss << ";:SENS:TEMP:TRAN FRTD, (@201:210)";
      ss << ";:SENS:TEMP:RTD:FOUR PT100, (@201:210)";
      ss << ";:SENS:TEMP:UNIT CELS, (@201:210)";
      ss << ";:ROUT:CHAN:DEL 0.1, (@201:210)"; // set channel delay to 0.1 s
      comHandler_->SendCommand(ss.str());
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

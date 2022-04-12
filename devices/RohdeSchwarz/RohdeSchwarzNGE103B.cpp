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

#include "RohdeSchwarzNGE103B.h"

RohdeSchwarzNGE103B::RohdeSchwarzNGE103B( const ioport_t ioPort )
  :VRohdeSchwarzNGE103B(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new NGE103BComHandler( ioPort );
  DeviceInit();
}

RohdeSchwarzNGE103B::~RohdeSchwarzNGE103B()
{
  delete comHandler_;
}

bool RohdeSchwarzNGE103B::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

void RohdeSchwarzNGE103B::GetIdentification(std::string& id) const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("*IDN?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    id = buffer;

    while (!OperationCompleted()) { usleep(10); }

  } else {
    id = "Unknown";
  }
}

void RohdeSchwarzNGE103B::SelectChannel(unsigned int ch)
{
  if (DeviceAvailable()) {
    char cmd[100];
    sprintf(cmd, "INST:SEL OUT%d", ch);
    comHandler_->SendCommand(cmd);

    while (!OperationCompleted()) { usleep(10); } 
  }

  usleep(10000);
}

unsigned int RohdeSchwarzNGE103B::SelectedChannel() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("INST:NSEL?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); }

    return std::atoi(buffer);
  }
  return 0;
}

void RohdeSchwarzNGE103B::SetVoltage(float v)
{
  if (v<MinVoltage || v>MaxVoltage) return;

  if (DeviceAvailable()) {
    char cmd[100];
    sprintf(cmd, "VOLT %f", v);
    comHandler_->SendCommand(cmd);

    while (!OperationCompleted()) { usleep(10); } 
  }
}

float RohdeSchwarzNGE103B::GetVoltage() const 
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("VOLT?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); } 


    return std::atof(buffer);
  }

  return -1;
}

float RohdeSchwarzNGE103B::MeasureVoltage() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("MEAS:VOLT?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); } 

    return std::atof(buffer);
  }

  return -1;
}

void RohdeSchwarzNGE103B::SetEasyRampDuration(float d)
{
  if (d<MinEasyRampDuration || d>MaxEasyRampDuration) return;

  if (DeviceAvailable()) {
    char cmd[100];
    sprintf(cmd, "VOLT:RAMP:DUR %.2f", d);
    comHandler_->SendCommand(cmd);

    while (!OperationCompleted()) { usleep(10); }
  }
}

float RohdeSchwarzNGE103B::GetEasyRampDuration() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("VOLT:RAMP:DUR?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); }


    return std::atof(buffer);
  }

  return -1;
}

void RohdeSchwarzNGE103B::SetEasyRampState(bool s)
{
  if (DeviceAvailable()) {
    char cmd[100];
    sprintf(cmd, "VOLT:RAMP %i", s);
    comHandler_->SendCommand(cmd);

    while (!OperationCompleted()) { usleep(10); }
  }
}

bool RohdeSchwarzNGE103B::GetEasyRampState() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("VOLT:RAMP?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); }


    return std::atoi(buffer);
  }

  return -1;
}

void RohdeSchwarzNGE103B::SetCurrent(float i)
{
  if (i<MinCurrent || i>MaxCurrent) return;

  if (DeviceAvailable()) {
    char cmd[100];
    sprintf(cmd, "CURR %f", i);
    comHandler_->SendCommand(cmd);

    while (!OperationCompleted()) { usleep(10); } 
  }
}

float RohdeSchwarzNGE103B::GetCurrent() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("CURR?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); } 


    return std::atof(buffer);
  }

  return -1;
}

float RohdeSchwarzNGE103B::MeasureCurrent() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("MEAS:CURR?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); } 

    return std::atof(buffer);
  }

  return -1;
}

float RohdeSchwarzNGE103B::MeasurePower() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("MEAS:POW?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); }

    return std::atof(buffer);
  }

  return -1;
}

void RohdeSchwarzNGE103B::SetOutputState(bool s)
{
  if (DeviceAvailable()) {
    char cmd[100];
    sprintf(cmd, "OUTP:STAT %i", s);
    comHandler_->SendCommand(cmd);

    while (!OperationCompleted()) { usleep(10); } 
  }

  usleep(10000);
}

bool RohdeSchwarzNGE103B::GetOutputState() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("OUTP:STAT?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); } 

    return std::atoi(buffer);
  }

  return false;
}

unsigned int RohdeSchwarzNGE103B::GetOutputMode() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("OUTP:MODE?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    while (!OperationCompleted()) { usleep(10); } 

    std::string buf = buffer;

    if (buf.find("OFF", 0)==0) return RohdeSchwarzNGE103B_OFF;
    if (buf.find("CV", 0)==0) return RohdeSchwarzNGE103B_CV;
    if (buf.find("CC", 0)==0) return RohdeSchwarzNGE103B_CC;

    return RohdeSchwarzNGE103B_OFF;
  }

  return RohdeSchwarzNGE103B_OFF;
}

void RohdeSchwarzNGE103B::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}

void RohdeSchwarzNGE103B::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    comHandler_->SendCommand("*IDN?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    std::string buf = buffer;
    
    if (buf.find("Rohde&Schwarz,NGE103B", 0)==0) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }
}

bool RohdeSchwarzNGE103B::OperationCompleted() const
{
  if (DeviceAvailable()) {
    comHandler_->SendCommand("*OPC?");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    return std::atoi(buffer);
  }
  
  return true;
}

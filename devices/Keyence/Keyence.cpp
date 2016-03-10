#include <unistd.h>

#include <cmath>
#include <sstream>
#include <iostream>

#include "Keyence.h"

// #define LSTEPDEBUG 1

Keyence::Keyence( const ioport_t ioPort )
  :VKeyence(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new KeyenceComHandler( ioPort );
  DeviceInit();
}

Keyence::~Keyence()
{
  delete comHandler_;
}

bool Keyence::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

// low level debugging methods
void Keyence::SendCommand(const std::string & command)
{
#ifdef LSTEPDEBUG
  std::cout << "SendCommand: " << command << std::endl;
#endif
  std::cout << command << std::endl;
  comHandler_->SendCommand(command.c_str());
}

void Keyence::ReceiveString(std::string & buffer)
{
  usleep(1000);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);
  buffer = buf;
}

//FIX ME!
void Keyence::ModeChange()
{
}

void Keyence::MeasurementValueOutput(int out, double value)
{
  GetValue("M",out,value);
}

void Keyence::Timing(int out, int status)
{
  SetValue("T", status, out);
}

void Keyence::AutoZero(int out, bool status)
{
  if(status){
    SetValue("V", out);
  }else{
    SetValue("W", out);
  }
}

void Keyence::Reset(int out)
{
  SetValue("VR,", out);
}

void Keyence::PanelLock(int status)
{
  SetValue("KL,", status);
}

void Keyence::ProgramChange(int prog_number)
{
  SetValue("PW,",prog_number);
}

void Keyence::ProgramCheck(int value)
{
  GetValue("PR", value);
}

void Keyence::StatResultOutput(int out, std::string value)
{
  GetValue("DO,", out, value);
}

void Keyence::ClearStat(int out)
{
  SetValue("DQ,", out);
}

void Keyence::StartDataStorage()
{
  SendCommand("AS");
  //string response; ReceiveString(response);
  //if(input == response){return 0;}
  //else{return 1;}
}

void Keyence::StopDataStorage()
{
  SendCommand("AP");
  //string response; ReceiveString(response);
}

void Keyence::InitDataStorage()
{
  SendCommand("AQ");
  //string response; ReceiveString(response);
}

void Keyence::OutputDataStorage(int out, std::vector<double> values)
{
  GetValue("AO,",out,values);
}

//FIX ME!
void Keyence::DataStorageStatus(std::string value)
{
  GetValue("AN",value);
}

void Keyence::SetSamplingRate(int mode)
{
  /*
    mode = 0 -> 20 microsec
    mode = 1 -> 50 microsec
    mode = 2 -> 100 microsec
    mode = 3 -> 200 microsec
    mode = 4 -> 500 microsec
    mode = 5 -> 1000 microsec
  */
  SetValue("SW,CA,",mode);
}

void Keyence::SetAveraging(int out, int mode)
{
  /*
    mode = 0 -> average over 1
    mode = 1 -> 4
    mode = 2 -> 16
    mode = 3 -> 64
    mode = 4 -> 256
    mode = 5 -> 1024
    mode = 6 -> 4096
    mode = 7 -> 16384
    mode = 8 -> 65536
    mode = 9 -> 262144
  */
  SetValue("SW,OC,",out,"0",mode);
}

void Keyence::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\r') {
      buffer[c] = '\0';
      break;
    }
  }
}

//FIX ME!!! still for LStepExpress settings
void Keyence::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {

    isDeviceAvailable_ = true;

    char buffer[1000];
    std::string buf;

    comHandler_->SendCommand("ver"); // read version

    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if (buf.find("PE43 1.00.01", 0)!=0) {
      isDeviceAvailable_ = false;
      return;
    }

    comHandler_->SendCommand("iver"); // read internal version
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if (buf.find("E2015.02.27-3012", 0)!=0) {
      isDeviceAvailable_ = false;
      return;
    }

  }
}

//FIX ME!
bool Keyence::GetStatus()
{
  std::string value;
  this->GetValue("status", value);

  if (value.compare("OK...")==0) return true;
  return false;
}

//FIX ME!
int Keyence::GetError()
{
  int value;
  this->GetValue("err", value);
  return value;
}

//FIX ME!
void Keyence::Reset()
{
  this->SendCommand("!Reset");
}

//FIX ME!
void Keyence::ValidConfig()
{
  this->SendCommand("!validconfig");
}

//FIX ME!
void Keyence::ValidParameter()
{
  this->SendCommand("!validpar");
}

//FIX ME!
void Keyence::ConfirmErrorRectification()
{
  this->SendCommand("!quit");
}

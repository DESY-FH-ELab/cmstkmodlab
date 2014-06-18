#include <cmath>
#include <sstream>

#include "Hameg8143.h"

Hameg8143::Hameg8143( const ioport_t ioPort )
  :VHameg8143(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new HO820ComHandler( ioPort );
  DeviceInit();
}

Hameg8143::~Hameg8143()
{
  delete comHandler_;
}

bool Hameg8143::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

unsigned int Hameg8143::GetStatus() const
{
  comHandler_->SendCommand("STA");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::istringstream iss(buffer);
  std::string temp;
  unsigned int status = 0;
  while (iss >> temp) {
	  if (temp=="OP0") status |= hmOP0;
	  if (temp=="OP1") status |= hmOP1;
	  if (temp=="CV1") status |= hmCV1;
	  if (temp=="CC1") status |= hmCC1;
	  if (temp=="CV2") status |= hmCV2;
	  if (temp=="CC2") status |= hmCC2;
	  if (temp=="RM1") status |= hmRM1;
	  if (temp=="RM0") status |= hmRM0;
  }

  return status;
}

bool Hameg8143::SetRemoteMode(bool remote) const
{
  if (remote) {
    comHandler_->SendCommand("RM1");
  } else {
    comHandler_->SendCommand("RM0");
  }

  return true;
}

bool Hameg8143::SetMixedMode(bool mixed) const
{
  if (mixed) {
    comHandler_->SendCommand("MX1");
  } else {
    comHandler_->SendCommand("MX0");
  }

  return true;
}
 
bool Hameg8143::SetVoltage(int channel, float voltage) const
{
  if (channel==0 || channel>2) return false;
  if (voltage<0 || voltage>30) return false;

  std::stringstream theCommand;
  theCommand << "SU" << channel << ":";
  std::cout.precision(2);
  theCommand << std::fixed << voltage;

  comHandler_->SendCommand( theCommand.str().c_str() );

  return true;
}
  
float Hameg8143::GetSetVoltage(int channel) const
{
  if (channel==0 || channel>2) return -99;
  
  if (channel==1) {
    comHandler_->SendCommand("RU1");
  } else {
    comHandler_->SendCommand("RU2");
  }

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  std::string buf = buffer;
  buf = buf.substr(buf.find(':')+1);
  buf = buf.substr(0, buf.find('V'));

  float voltage = std::atof(buf.c_str());
  return voltage;
}
  
float Hameg8143::GetVoltage(int channel) const
{
  if (channel==0 || channel>2) return -99;
  
  if (channel==1) {
    comHandler_->SendCommand("MU1");
  } else {
    comHandler_->SendCommand("MU2");
  }

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  std::string buf = buffer;
  buf = buf.substr(buf.find(':')+1);
  buf = buf.substr(0, buf.find('V'));

  float voltage = std::atof(buf.c_str());
  return voltage;
}

bool Hameg8143::SetCurrent(int channel, float current) const
{
  if (channel==0 || channel>2) return false;
  if (current<0 || current>2) return false;

  std::stringstream theCommand;
  theCommand << "SI" << channel << ":";
  std::cout.precision(3);
  theCommand << std::fixed << current;

  comHandler_->SendCommand( theCommand.str().c_str() );

  return true;
}
  
float Hameg8143::GetSetCurrent(int channel) const
{
  if (channel==0 || channel>2) return -99;
  
  if (channel==1) {
    comHandler_->SendCommand("RI1");
  } else {
    comHandler_->SendCommand("RI2");
  }

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  std::string buf = buffer;
  buf = buf.substr(buf.find(':')+1);
  buf = buf.substr(0, buf.find('A'));

  float current = std::atof(buf.c_str());
  return current;
}

float Hameg8143::GetCurrent(int channel) const
{
  if (channel==0 || channel>2) return -99;
  
  if (channel==1) {
    comHandler_->SendCommand("MI1");
  } else {
    comHandler_->SendCommand("MI2");
  }

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  std::string buf = buffer;
  buf = buf.substr(buf.find(':')+1);
  buf = buf.substr(0, buf.find('A'));

  float current = std::atof(buf.c_str());
  return current;
}
 
bool Hameg8143::SetTrackingVoltage(float voltage) const
{
  if (voltage<0 || voltage>30) return false;

  std::stringstream theCommand;
  theCommand << "TRU:";
  std::cout.precision(2);
  theCommand << std::fixed << voltage;

  comHandler_->SendCommand( theCommand.str().c_str() );

  return true;
}

bool Hameg8143::SetTrackingCurrent(float current) const
{
  if (current<0 || current>2) return false;

  std::stringstream theCommand;
  theCommand << "TRI:";
  std::cout.precision(3);
  theCommand << std::fixed << current;

  comHandler_->SendCommand( theCommand.str().c_str() );

  return true;
}

bool Hameg8143::SwitchOutputOn() const
{
  comHandler_->SendCommand("OP1");

  return true;
}

bool Hameg8143::SwitchOutputOff() const
{
  comHandler_->SendCommand("OP0");

  return true;
}

void Hameg8143::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}

void Hameg8143::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    comHandler_->SendCommand("ID?");

    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    std::string buf = buffer;
    
    if (buf.find("HAMEG Instruments,HM8143", 0)==0) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }
}

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

bool LeyboldGraphixThree::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

void LeyboldGraphixThree::SendCommand(std::string& command) const
{
  char crc = GetChecksum(command);
  command += crc;
  command += EOT;

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

void LeyboldGraphixThree::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    /*
    comHandler_->SendCommand("#1v");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    std::string buf = buffer;
    if (buf.find("1v SMCI36_RS485", 0)==0) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  */
  }
}

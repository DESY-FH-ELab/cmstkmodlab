#include <cmath>
#include <sstream>

#include "NanotecSMCI36.h"

NanotecSMCI36::NanotecSMCI36( const ioport_t ioPort )
  :VNanotecSMCI36(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new NanotecComHandler( ioPort );
  DeviceInit();
}

NanotecSMCI36::~NanotecSMCI36()
{
  delete comHandler_;
}

bool NanotecSMCI36::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

std::string NanotecSMCI36::GetFirmwareVersion() const
{
  comHandler_->SendCommand("v");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  return std::string(buffer);
}

void NanotecSMCI36::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\n' || buffer[c]=='\r') {
      buffer[c] = 0;
      break;
    }
  }
}

void NanotecSMCI36::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    comHandler_->SendCommand("v");

    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    std::string buf = buffer;
    
    if (buf.find("001v SMCI36", 0)==0) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }
}

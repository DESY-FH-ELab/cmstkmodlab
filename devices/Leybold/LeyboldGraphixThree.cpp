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

bool LeyboldGraphixThree::DeviceAvailable() const
{
  return isDeviceAvailable_;
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

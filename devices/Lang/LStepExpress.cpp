#include <cmath>
#include <sstream>

#include "LStepExpress.h"

LStepExpress::LStepExpress( const ioport_t ioPort )
  :VLStepExpress(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new LStepExpressComHandler( ioPort );
  DeviceInit();
}

LStepExpress::~LStepExpress()
{
  delete comHandler_;
}

bool LStepExpress::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

unsigned int LStepExpress::GetStatus() const
{
  /*
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
  */

  return 0;
}

void LStepExpress::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}

void LStepExpress::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    comHandler_->SendCommand("?ver"); // read version
    comHandler_->SendCommand("?readsn"); // read serial number

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

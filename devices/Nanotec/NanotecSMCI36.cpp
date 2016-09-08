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

int NanotecSMCI36::GetStatus() const
{
  comHandler_->SendCommand("$");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  return std::atoi(buffer);
}

void NanotecSMCI36::SetMotorType(int type)
{
  if (type < smciStepper || type > smciBLDCEncoder) return;

  char command[20];
  sprintf(command, ":CL_motor_type%d", type);

  comHandler_->SendCommand(command);
}

int NanotecSMCI36::GetMotorType() const
{
  comHandler_->SendCommand(":CL_motor_type");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  return std::atoi(buffer);
}

void NanotecSMCI36::SetPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;

  char command[20];
  sprintf(command, "i%d", current);

  comHandler_->SendCommand(command);
}

int NanotecSMCI36::GetPhaseCurrent() const
{
  comHandler_->SendCommand("Zi");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  return std::atoi(buffer);
}

void NanotecSMCI36::SetStandStillPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;

  char command[20];
  sprintf(command, "r%d", current);

  comHandler_->SendCommand(command);
}

int NanotecSMCI36::GetStandStillPhaseCurrent() const
{
  comHandler_->SendCommand("Zr");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  return std::atoi(buffer);
}

void NanotecSMCI36::SetStepMode(int mode)
{
  if (mode != smci01MicroStepsPerFullStep &&
      mode != smci02MicroStepsPerFullStep &&
      mode != smci04MicroStepsPerFullStep &&
      mode != smci05MicroStepsPerFullStep &&
      mode != smci08MicroStepsPerFullStep &&
      mode != smci10MicroStepsPerFullStep &&
      mode != smci16MicroStepsPerFullStep &&
      mode != smci32MicroStepsPerFullStep &&
      mode != smci64MicroStepsPerFullStep &&
      mode != smciFeedRateMode &&
      mode != smciAdaptiveStepMode) return;

  char command[20];
  sprintf(command, "g%d", mode);

  comHandler_->SendCommand(command);
}

int NanotecSMCI36::GetStepMode() const
{
  comHandler_->SendCommand("Zg");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  return std::atoi(buffer);
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

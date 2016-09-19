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
  comHandler_->SendCommand("#1v");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find("1v")+3);

  return ret;
}

int NanotecSMCI36::GetStatus() const
{
  comHandler_->SendCommand("#1$");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find("1$")+2);

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMotorType(int type)
{
  if (type < smciStepper || type > smciBLDCEncoder) return;

  char command[20];
  sprintf(command, "#1:CL_motor_type%d", type);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMotorType() const
{
  comHandler_->SendCommand("#1:CL_motor_type");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:CL_motor_type"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;

  char command[20];
  sprintf(command, "#1i%d", current);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetPhaseCurrent() const
{
  comHandler_->SendCommand("#1Zi");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zi"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetStandStillPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;

  char command[20];
  sprintf(command, "#1r%d", current);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetStandStillPhaseCurrent() const
{
  comHandler_->SendCommand("#1Zr");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zr"));

  return std::atoi(ret.c_str());
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
  sprintf(command, "#1g%d", mode);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetStepMode() const
{
  comHandler_->SendCommand("#1Zg");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zg"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMotorID(int ID)
{
  char command[20];
  sprintf(command, "#1:mt%d", ID);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMotorID() const
{
  comHandler_->SendCommand("#1:mt");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:mt"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetErrorCorrectionMode(int mode)
{
  if (mode < smciErrCorrectionOff || mode > smciErrCorrectionDuringTravel) return;
  char command[20];
  sprintf(command, "#1U%d", mode);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetErrorCorrectionMode() const
{
  comHandler_->SendCommand("#1ZU");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1ZU"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetEncoderDirection(int direction)
{
  if (direction < 0 || direction > 1) return;
  char command[20];
  sprintf(command, "#1q%d", direction);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetEncoderDirection() const
{
  comHandler_->SendCommand("#1Zq");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zq"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetSwingOutTime(int time)
{
  if (time<0 || time > 255) return;
  char command[20];
  sprintf(command, "#1O%d", time);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetSwingOutTime() const
{
  comHandler_->SendCommand("#1ZO");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1ZO"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMaxEncoderDeviation(int deviation)
{
  if (deviation < 0 || deviation > 255) return;
  char command[20];
  sprintf(command, "#1X%d", deviation);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMaxEncoderDeviation() const
{
  comHandler_->SendCommand("#1ZX");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1ZX"));

  return std::atoi(ret.c_str());
}

int NanotecSMCI36::GetPosition() const
{
  comHandler_->SendCommand("#1C");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1C"));

  return std::atoi(ret.c_str());
}

int NanotecSMCI36::GetEncoderPosition() const
{
  comHandler_->SendCommand("#1I");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1I"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::ResetPositionError()
{
  comHandler_->SendCommand("#1D");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

void NanotecSMCI36::ResetPositionError(int position)
{
  char command[20];
  sprintf(command, "#1D%d", position);

  comHandler_->SendCommand(command);
}

void NanotecSMCI36::SetInputPinFunction(int pin, int function)
{
  if (pin<1 || pin>6) return;
  if (function<smciIPinUserDefined || function>smciIClockDirectionMode2) return;

  char command[20];
  sprintf(command, "#1:port_in_%c%d", 'a'+pin-1, function);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetInputPinFunction(int pin) const
{
  if (pin<1 || pin>6) return smciIInvalid;

  char command[20];
  sprintf(command, "#1:port_in_%c", 'a'+pin-1);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:port_in_X"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetOutputPinFunction(int pin, int function)
{
  if (pin<1 || pin>3) return;
  if (function<smciOPinUserDefined || function>smciOError) return;

  char command[20];
  sprintf(command, "#1:port_out_%c%d", 'a'+pin-1, function);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetOutputPinFunction(int pin) const
{
  if (pin<1 || pin>3) return smciOInvalid;

  char command[20];
  sprintf(command, "#1:port_out_%c", 'a'+pin-1);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:port_out_X"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetReversePolarityMask(int mask)
{
  char command[20];
  sprintf(command, "#1h%d", mask);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetReversePolarityMask() const
{
  comHandler_->SendCommand("#1Zh");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zh"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetRampMode(int ramp)
{
  if (ramp < smciTrapezoidalRamp || ramp > smciJerkFreeRamp) return;
  char command[20];
  sprintf(command, "#1:ramp_mode%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetRampMode() const
{
  comHandler_->SendCommand("#1:ramp_mode");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:ramp_mode"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetQuickstopRamp(int ramp)
{
  if (ramp < 0 || ramp > 8000) return;
  char command[20];
  sprintf(command, "#1H%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetQuickstopRamp() const
{
  comHandler_->SendCommand("#1ZH");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1ZH"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetQuickstopRampHzPerSecond(int ramp)
{
  if (ramp < 0 || ramp > 3000000) return;
  char command[20];
  sprintf(command, "#1:decelquick%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetQuickstopRampHzPerSecond() const
{
  comHandler_->SendCommand("#1:decelquick");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:decelquick"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetAccelerationRamp(int ramp)
{
  if (ramp < 1 || ramp > 65535) return;
  char command[20];
  sprintf(command, "#1b%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetAccelerationRamp() const
{
  comHandler_->SendCommand("#1Zb");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zb"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetAccelerationRampHzPerSecond(int ramp)
{
  if (ramp < 1 || ramp > 3000000) return;
  char command[20];
  sprintf(command, "#1:accel%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetAccelerationRampHzPerSecond() const
{
  comHandler_->SendCommand("#1:accel");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:accel"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDecelerationRamp(int ramp)
{
  if (ramp < 0 || ramp > 65535) return;
  char command[20];
  sprintf(command, "#1B%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetDecelerationRamp() const
{
  comHandler_->SendCommand("#1ZB");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1ZB"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDecelerationRampHzPerSecond(int ramp)
{
  if (ramp < 0 || ramp > 3000000) return;
  char command[20];
  sprintf(command, "#1:decel%d", ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetDecelerationRampHzPerSecond() const
{
  comHandler_->SendCommand("#1:decel");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1:decel"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetPositioningMode(int mode)
{
  if (mode < smciRelativePositioning || mode >= smciMaxPositioningMode) return;
  char command[20];
  sprintf(command, "#1p%d", mode);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetPositioningMode() const
{
  comHandler_->SendCommand("#1Zp");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zp"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetTravelDistance(int distance)
{
  char command[20];
  sprintf(command, "#1s%d", distance);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetTravelDistance() const
{
  comHandler_->SendCommand("#1Zs");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zs"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDirection(bool direction)
{
  char command[20];
  sprintf(command, "#1d%d", (int)direction);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

bool NanotecSMCI36::GetDirection() const
{
  comHandler_->SendCommand("#1Zd");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zd"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMinimumFrequency(int frequency)
{
  if (frequency < 1 || frequency > 160000) return;
  char command[20];
  sprintf(command, "#1u%d", frequency);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMinimumFrequency() const
{
  comHandler_->SendCommand("#1Zu");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zu"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMaximumFrequency(int frequency)
{
  if (frequency < 1 || frequency > 1000000) return;
  char command[20];
  sprintf(command, "#1o%d", frequency);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMaximumFrequency() const
{
  comHandler_->SendCommand("#1Zo");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zo"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMaximumFrequency2(int frequency)
{
  if (frequency < 1 || frequency > 1000000) return;
  char command[20];
  sprintf(command, "#1n%d", frequency);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMaximumFrequency2() const
{
  comHandler_->SendCommand("#1Zn");

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, strlen("1Zn"));

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::Start()
{
  comHandler_->SendCommand("#1A");
}

void NanotecSMCI36::Stop(bool quickstop)
{
  char command[20];
  sprintf(command, "#1S%d", (int)!quickstop);

  comHandler_->SendCommand(command);
}

void NanotecSMCI36::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
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
  }

  isDeviceAvailable_ = true;
}

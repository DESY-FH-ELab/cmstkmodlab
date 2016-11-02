#include <cstring>
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
  char command[20];
  sprintf(command, "#%dv", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find("v")+2);

  return ret;
}

int NanotecSMCI36::GetStatus() const
{
  char command[20];
  sprintf(command, "#%d$", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('$')+1);

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMotorType(int type)
{
  if (type < smciStepper || type > smciBLDCEncoder) return;

  char command[20];
  sprintf(command, "#%d:CL_motor_type%d", driveAddress_, type);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMotorType() const
{
  char command[20];
  sprintf(command, "#%d:CL_motor_type", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find("type")+4);

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;

  char command[20];
  sprintf(command, "#%di%d", driveAddress_, current);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetPhaseCurrent() const
{
  char command[20];
  sprintf(command, "#%dZi", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('i')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetStandStillPhaseCurrent(int current)
{
  if (current < 0 || current >100) return;

  char command[20];
  sprintf(command, "#%dr%d", driveAddress_, current);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetStandStillPhaseCurrent() const
{
  char command[20];
  sprintf(command, "#%dZr", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('r')+1);

  // std::cout << ret << std::endl;

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
  sprintf(command, "#%dg%d", driveAddress_, mode);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetStepMode() const
{
  char command[20];
  sprintf(command, "#%dZg", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('g')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDriveAddress(int address)
{
  driveAddress_ = address;

  char command[20];
  sprintf(command, "m%d", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetDriveAddress()
{
  comHandler_->SendCommand("#*m");
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('m')+1);

  // std::cout << ret << std::endl;

  driveAddress_ = std::atoi(ret.c_str());

  return driveAddress_;
}

void NanotecSMCI36::SetMotorID(int ID)
{
  char command[20];
  sprintf(command, "#%d:mt%d", driveAddress_, ID);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMotorID() const
{
  char command[20];
  sprintf(command, "#%d:mt", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('t')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetErrorCorrectionMode(int mode)
{
  if (mode < smciErrCorrectionOff || mode > smciErrCorrectionDuringTravel) return;

  char command[20];
  sprintf(command, "#%dU%d", driveAddress_, mode);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetErrorCorrectionMode() const
{
  char command[20];
  sprintf(command, "#%dZU", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('U')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetEncoderDirection(bool direction)
{
  char command[20];
  sprintf(command, "#%dq%d", driveAddress_, (int)direction);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

bool NanotecSMCI36::GetEncoderDirection() const
{
  char command[20];
  sprintf(command, "#%dZq", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('q')+1);

  // std::cout << ret << std::endl;
  
  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetSwingOutTime(int time)
{
  if (time<0 || time > 255) return;

  char command[20];
  sprintf(command, "#%dO%d", driveAddress_, time);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetSwingOutTime() const
{
  char command[20];
  sprintf(command, "#%dZO", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('O')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMaxEncoderDeviation(int deviation)
{
  if (deviation < 0 || deviation > 255) return;

  char command[20];
  sprintf(command, "#%dX%d", driveAddress_, deviation);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMaxEncoderDeviation() const
{
  char command[20];
  sprintf(command, "#%dZX", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('X')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

int NanotecSMCI36::GetPosition() const
{
  char command[20];
  sprintf(command, "#%dC", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('C')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

int NanotecSMCI36::GetEncoderPosition() const
{
  char command[20];
  sprintf(command, "#%dI", driveAddress_);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('I')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::ResetPositionError(int position)
{
  char command[20];
  sprintf(command, "#%dD%d", driveAddress_, position);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

void NanotecSMCI36::SetInputPinFunction(int pin, int function)
{
  if (pin<1 || pin>6) return;
  if (function<smciIPinUserDefined || function>smciIClockDirectionMode2) return;

  char command[20];
  sprintf(command, "#%d:port_in_%c%d", driveAddress_, 'a'+pin-1, function);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetInputPinFunction(int pin) const
{
  if (pin<1 || pin>6) return smciIInvalid;

  char command[20];
  sprintf(command, "#%d:port_in_%c", driveAddress_, 'a'+pin-1);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find("in_")+4);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetOutputPinFunction(int pin, int function)
{
  if (pin<1 || pin>3) return;
  if (function<smciOPinUserDefined || function>smciOError) return;

  char command[20];
  sprintf(command, "#%d:port_out_%c%d", driveAddress_, 'a'+pin-1, function);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetOutputPinFunction(int pin) const
{
  if (pin<1 || pin>3) return smciOInvalid;

  char command[20];
  sprintf(command, "#%d:port_out_%c", driveAddress_, 'a'+pin-1);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find("out_")+5);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetIOMask(unsigned int mask)
{
  char command[20];
  sprintf(command, "#%dL%d", driveAddress_, mask);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

unsigned int NanotecSMCI36::GetIOMask() const
{
  char command[20];
  sprintf(command, "#%dZL", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('L')+1);

  // std::cout << ret << std::endl;

  return std::atoll(ret.c_str());
}

void NanotecSMCI36::SetReversePolarityMask(unsigned int mask)
{
  char command[20];
  sprintf(command, "#%dh%d", driveAddress_, mask);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

unsigned int NanotecSMCI36::GetReversePolarityMask() const
{
  char command[20];
  sprintf(command, "#%dZh", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('h')+1);

  // std::cout << ret << std::endl;

  return std::atoll(ret.c_str());
}

void NanotecSMCI36::SetIO(unsigned int mask)
{
  char command[20];
  sprintf(command, "#%dY%d", driveAddress_, mask);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

unsigned int NanotecSMCI36::GetIO() const
{
  char command[20];
  sprintf(command, "#%dZY", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('Y')+1);

  // std::cout << ret << std::endl;

  return std::atoll(ret.c_str());
}

void NanotecSMCI36::SetRampMode(int ramp)
{
  if (ramp < smciTrapezoidalRamp || ramp > smciJerkFreeRamp) return;

  char command[20];
  sprintf(command, "#%d:ramp_mode%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetRampMode() const
{
  char command[20];
  sprintf(command, "#%d:ramp_mode", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('e')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetQuickstopRamp(int ramp)
{
  if (ramp < 0 || ramp > 8000) return;

  char command[20];
  sprintf(command, "#%dH%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetQuickstopRamp() const
{
  char command[20];
  sprintf(command, "#%dZH", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('H')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetQuickstopRampHzPerSecond(int ramp)
{
  if (ramp < 0 || ramp > 3000000) return;

  char command[20];
  sprintf(command, "#%d:decelquick%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetQuickstopRampHzPerSecond() const
{
  char command[20];
  sprintf(command, "#%d:decelquick", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('k')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetAccelerationRamp(int ramp)
{
  if (ramp < 1 || ramp > 65535) return;

  char command[20];
  sprintf(command, "#%db%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetAccelerationRamp() const
{
  char command[20];
  sprintf(command, "#%dZb", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('b')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetAccelerationRampHzPerSecond(int ramp)
{
  if (ramp < 1 || ramp > 3000000) return;

  char command[20];
  sprintf(command, "#%d:accel%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetAccelerationRampHzPerSecond() const
{
  char command[20];
  sprintf(command, "#%d:accel", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('l')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDecelerationRamp(int ramp)
{
  if (ramp < 0 || ramp > 65535) return;

  char command[20];
  sprintf(command, "#%dB%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetDecelerationRamp() const
{
  char command[20];
  sprintf(command, "#%dZB", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('B')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDecelerationRampHzPerSecond(int ramp)
{
  if (ramp < 0 || ramp > 3000000) return;

  char command[20];
  sprintf(command, "#%d:decel%d", driveAddress_, ramp);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetDecelerationRampHzPerSecond() const
{
  char command[20];
  sprintf(command, "#%d:decel", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('l')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetPositioningMode(int mode)
{
  if (mode < smciRelativePositioning || mode >= smciMaxPositioningMode) return;

  char command[20];
  sprintf(command, "#%dp%d", driveAddress_, mode);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetPositioningMode() const
{
  char command[20];
  sprintf(command, "#%dZp", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('p')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetTravelDistance(int distance)
{
  char command[20];
  sprintf(command, "#%ds%d", driveAddress_, distance);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetTravelDistance() const
{
  char command[20];
  sprintf(command, "#%dZs", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('s')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetDirection(bool direction)
{
  char command[20];
  sprintf(command, "#%dd%d", driveAddress_, (int)direction);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

bool NanotecSMCI36::GetDirection() const
{
  char command[20];
  sprintf(command, "#%dZd", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('d')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMinimumFrequency(int frequency)
{
  if (frequency < 1 || frequency > 160000) return;

  char command[20];
  sprintf(command, "#%du%d", driveAddress_, frequency);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMinimumFrequency() const
{
  char command[20];
  sprintf(command, "#%dZu", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('u')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMaximumFrequency(int frequency)
{
  if (frequency < 1 || frequency > 1000000) return;

  char command[20];
  sprintf(command, "#%do%d", driveAddress_, frequency);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMaximumFrequency() const
{
  char command[20];
  sprintf(command, "#%dZo", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('o')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::SetMaximumFrequency2(int frequency)
{
  if (frequency < 1 || frequency > 1000000) return;

  char command[20];
  sprintf(command, "#%dn%d", driveAddress_, frequency);

  comHandler_->SendCommand(command);
  char buffer[1000];
  comHandler_->ReceiveString(buffer);
}

int NanotecSMCI36::GetMaximumFrequency2() const
{
  char command[20];
  sprintf(command, "#%dZn", driveAddress_);

  comHandler_->SendCommand(command);

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  std::string ret = buffer;
  ret.erase(0, ret.find('n')+1);

  // std::cout << ret << std::endl;

  return std::atoi(ret.c_str());
}

void NanotecSMCI36::Start()
{
  char command[20];
  sprintf(command, "#%dA", driveAddress_);

  comHandler_->SendCommand(command);
}

void NanotecSMCI36::Stop(bool quickstop)
{
  char command[20];
  sprintf(command, "#%dS%d", driveAddress_, (int)!quickstop);

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
    
    // get the drive address
    comHandler_->SendCommand("#*m");
    char buffer[1000];
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);

    std::string buf = buffer;
    buf.resize(buf.find('m'));

    driveAddress_ = std::atoi(buf.c_str());

    char command[20];
    sprintf(command, "#%dv", driveAddress_);

    comHandler_->SendCommand(command);
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    sprintf(buffer, "%dv SMCI36_RS485", driveAddress_);
    if (buf.find(buffer, 0)==0) {
      isDeviceAvailable_ = true;
    } else {
      isDeviceAvailable_ = false;
    }
  }

  isDeviceAvailable_ = true;
}

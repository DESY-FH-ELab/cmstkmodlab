#include <iostream>

#ifdef USE_FAKEIO
#include "NanotecSMCI36Fake.h"
typedef NanotecSMCI36Fake NanotecSMCI36_t;
#else
#include "NanotecSMCI36.h"
typedef NanotecSMCI36 NanotecSMCI36_t;
#endif

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  /*
  int fid = open( "/dev/tty.SLAB_USBtoUART", O_RDWR | O_NOCTTY | O_NDELAY );

  struct termios m_termiosInitial;
  struct termios m_termios;

  // get and save current ioport settings for later restoring
  tcgetattr(fid, &m_termiosInitial);

  // clear new settions structure
  bzero(&m_termios, sizeof(m_termios));

  // set input/output baud rate
  cfsetispeed(&m_termios, B115200);
  cfsetospeed(&m_termios, B115200);

  // enable the receiver and disable modem control signals
  m_termios.c_cflag |= CREAD;
  m_termios.c_cflag |= CLOCAL;

  // set 8 bits per character, no parity, 1 stop bit (8N1)
  m_termios.c_cflag &= ~PARENB;
  m_termios.c_cflag &= ~CSTOPB;
  m_termios.c_cflag &= ~CSIZE;
  m_termios.c_cflag |= (CLOCAL | CREAD | CS8);
  m_termios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  m_termios.c_oflag &= ~OPOST;
  m_termios.c_cc[VMIN] = 0;
  m_termios.c_cc[VTIME] = 10;

  // commit changes
  tcflush(fid, TCIOFLUSH);

  if (tcsetattr(fid, TCSAFLUSH, &m_termios) != 0)
    return 0;

  int modeLines = 0;

  // put card into exclusive mode
  if (ioctl(fid, TIOCEXCL, &modeLines) == -1)
    return 0;

  std::string command = "#1v";
  char buffer[1000];
  char singleCharacter = 0;

  for ( unsigned int i = 0; i < command.length(); i++ ) {

    // scan command string character wise & write
    singleCharacter = command[i];
    write( fid, &singleCharacter, 1 );
  }
  command = '\r';
  write( fid, command.c_str(), 1);

  usleep(10000);

  read( fid, buffer, 1024 );

  std::cout << buffer << std::endl;

  close(fid);

  std::cout << "test" << std::endl;
*/
  NanotecSMCI36_t smci36("/dev/tty.SLAB_USBtoUART");

  std::cout << "firmware version:          " << smci36.GetFirmwareVersion() << std::endl;
  std::cout << "status:                    0x" << std::hex << smci36.GetStatus() << std::dec << std::endl;

  //smci36.SetMotorType(VNanotecSMCI36::smciStepper);
  //usleep(100000);
  //smci36.SetRampMode(VNanotecSMCI36::smciTrapezoidalRamp);
  //smci36.SetRampMode(VNanotecSMCI36::smciJerkFreeRamp);
  //smci36.SetPositioningMode(VNanotecSMCI36::smciRelativePositioning);
  //smci36.SetTravelDistance(10000);
  //usleep(100000);

  std::cout << "motor type:                " << smci36.GetMotorType() << std::endl;
  std::cout << "phase current:             " << smci36.GetPhaseCurrent() << std::endl;
  std::cout << "standstill phase current:  " << smci36.GetStandStillPhaseCurrent() << std::endl;
  std::cout << "step mode:                 " << smci36.GetStepMode() << std::endl;
  std::cout << "ramp mode:                 " << smci36.GetRampMode() << std::endl;

  std::cout << "motor ID:                  " << smci36.GetMotorID() << std::endl;
  std::cout << "error correction mode:     " << smci36.GetErrorCorrectionMode() << std::endl;
  std::cout << "encoder direction:         " << smci36.GetEncoderDirection() << std::endl;
  std::cout << "swing out time:            " << smci36.GetSwingOutTime() << std::endl;
  std::cout << "maximum encoder deviation: " << smci36.GetMaxEncoderDeviation() << std::endl;
  std::cout << "position:                  " << smci36.GetPosition() << std::endl;
  std::cout << "encoder position:          " << smci36.GetEncoderPosition() << std::endl;

  std::cout << "input pin function:        ";
  for (int p=1;p<=6;++p) std::cout << smci36.GetInputPinFunction(p) << " ";
  std::cout << std::endl;

  std::cout << "output pin function:       ";
  for (int p=1;p<=3;++p) std::cout << smci36.GetOutputPinFunction(p) << " ";
  std::cout << std::endl;

  std::cout << "pin polarity mask:         0x" << std::hex << smci36.GetReversePolarityMask() << std::dec << std::endl;

  std::cout << "quickstop ramp:            " << smci36.GetQuickstopRamp() << std::endl;
  std::cout << "quickstop ramp [Hz/s]:     " << smci36.GetQuickstopRampHzPerSecond() << std::endl;
  std::cout << "acceleration ramp:         " << smci36.GetAccelerationRamp() << std::endl;
  std::cout << "acceleration ramp [Hz/s]:  " << smci36.GetAccelerationRampHzPerSecond() << std::endl;
  std::cout << "deceleration ramp:         " << smci36.GetDecelerationRamp() << std::endl;
  std::cout << "deceleration ramp [Hz/s]:  " << smci36.GetDecelerationRampHzPerSecond() << std::endl;

  std::cout << "positioning mode:          " << smci36.GetPositioningMode() << std::endl;
  std::cout << "travel distance:           " << smci36.GetTravelDistance() << std::endl;
  std::cout << "minimum frequency:         " << smci36.GetMinimumFrequency() << std::endl;
  std::cout << "maximum frequency:         " << smci36.GetMaximumFrequency() << std::endl;
  std::cout << "maximum frequency 2:       " << smci36.GetMaximumFrequency2() << std::endl;

  //smci36.Start();


  //smci36.Stop();

  /*
  std::cout << "quickstop ramp:            " << smci36.GetQuickstopRamp() << std::endl;
  std::cout << "quickstop ramp [Hz/s]:     " << smci36.GetQuickstopRampHzPerSecond() << std::endl;
  std::cout << "acceleration ramp:         " << smci36.GetAccelerationRamp() << std::endl;
  std::cout << "acceleration ramp [Hz/s]:  " << smci36.GetAccelerationRampHzPerSecond() << std::endl;
  std::cout << "deceleration ramp:         " << smci36.GetDecelerationRamp() << std::endl;
  std::cout << "deceleration ramp [Hz/s]:  " << smci36.GetDecelerationRampHzPerSecond() << std::endl;

  std::cout << "positioning mode:          " << smci36.GetPositioningMode() << std::endl;
  std::cout << "travel distance:           " << smci36.GetTravelDistance() << std::endl;
  std::cout << "minimum frequency:         " << smci36.GetMinimumFrequency() << std::endl;
  std::cout << "maximum frequency:         " << smci36.GetMaximumFrequency() << std::endl;
  std::cout << "maximum frequency 2:       " << smci36.GetMaximumFrequency2() << std::endl;
  */

  return 0;
}

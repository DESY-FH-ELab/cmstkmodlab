/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//         Modeled on cmstkmodlab/devices/Conrad/ConradCommunication.cpp       //
//                          Last Updated August 20, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "VellemanCommunication.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

//! Default constructor (parametrized)
VellemanCommunication::VellemanCommunication(const std::string& comPort)
  : m_comPort(comPort),
    m_ioPort(-1) 
{
}

//! Default destructor (virtual)
VellemanCommunication::~VellemanCommunication()
{
  // For instance, if someone forgot to close connection . . .
  if (m_ioPort != -1) 
    finish();
}

//! Initialize Velleman IO communication
bool VellemanCommunication::initialize()
{
  if (m_ioPort != -1) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): I/0 port for relay card is already open, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }

  // open io port (read/write | no terminal control | no DCD line check i.e. no waiting for carrier detection)
  m_ioPort = open(m_comPort.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (m_ioPort < 0) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Unsuccessful in opening I/O port for relay card, ERRNO %i \n", comPort().c_str(), errno);
    // NOTE (ALL ERRORS): use Linux command "errno <number>" to find errno meaning
    return false;
  }

  // get and save current io port settings for use restoring later
  tcgetattr(m_ioPort, &m_termiosInitial);

  // clear structure for new settings
  bzero(&m_termios, sizeof(m_termios));

  // set input/output baud rate 
  cfsetispeed(&m_termios, B19200);
  cfsetospeed(&m_termios, B19200);

  // enable the receiver and disable modem control signals
  m_termios.c_cflag |= CREAD | CLOCAL;

  // disable flow control of RTS/CTS hardware
  m_termios.c_cflag &= ~CRTSCTS;

  // set 8 bits per character (byte), no parity, 1 stop bit (8-N-1)
  m_termios.c_cflag |= CS8;
  m_termios.c_cflag &= ~PARENB;
  m_termios.c_cflag &= ~CSTOPB;

  // disable canonical mode and related settings
  m_termios.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);

  // turn off software flow control and received data pre-processing
  m_termios.c_iflag &= ~(IXON | IXOFF | IXANY);
  m_termios.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

  // turn off sent data pre-processing
  m_termios.c_oflag &= ~(OPOST | ONLCR);

  // set processing wait time
  m_termios.c_cc[VMIN] = 0;
  m_termios.c_cc[VTIME] = 10; // wait 1s (10ds) or until data received before return

  // send data received/not read and and writte/not transmitted to Velleman
  tcflush(m_ioPort, TCIOFLUSH);

  // save & check configuring for errors
  if (tcsetattr(m_ioPort, TCSAFLUSH, &m_termios) != 0) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Unsuccessful in configuring relay card connection, ERRNO %i \n", comPort().c_str(), errno);
    return false;
  }

  int modeLines = 0;

  // put Velleman into exclusive mode
  if (ioctl(m_ioPort, TIOCGEXCL, &modeLines) == -1) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Failure to put card port into exclusive mode, ERRNO %i \n", comPort().c_str(), errno);
    return false;
  }

  return true;
}


//! Finish and reset IO port
void VellemanCommunication::finish()
{
  if (m_ioPort == -1)
    return;

  // immediately restore original comPort settings
  tcsetattr(m_ioPort, TCSANOW, &m_termiosInitial);

  // close io port
  close(m_ioPort);
  m_ioPort = -1;
  
}

//! Send instructions to Velleman relay card
bool VellemanCommunication::sendCommand(unsigned char cmd, unsigned char mask, unsigned char param1, unsigned char param2) const
{ 
  if (m_ioPort == -1) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): I/0 port for relay card is closed and must be opened before attempting to send commands, ERRNO %i\n", comPort().c_str(), errno);
    return false;
  }

  // packet of information sent to card determined from VM 8090 Protocol Manual
  unsigned char buffer[7];
  buffer[0] = STX;
  buffer[1] = cmd;
  buffer[2] = mask;
  buffer[3] = param1;
  buffer[4] = param2;
  buffer[5] = ~(STX + cmd + mask + param1 + param2) + 0x01;
  buffer[6] = ETX;
  int wroteBytes = write(m_ioPort, buffer, 7);

  // wait until the output has been transmitted to card
  tcdrain(m_ioPort);

  if (wroteBytes != 7) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Could send only %d of 7 bytes, ERRNO %i \n", comPort().c_str(), wroteBytes, errno);
    return false;
  }

  // give the card some time (100 ms)
  usleep(100000);

  return true;
  
}

//! Receive answer from Velleman
bool VellemanCommunication::receiveAnswer(unsigned char* cmd, unsigned char* mask, unsigned char* param1, unsigned char* param2) const
{
  unsigned char buffer[7] = {0};

  int readBytes = read(m_ioPort, buffer, 7);
  if (readBytes == -1) {
    // NOTE: Error message currently commented because it is uninformative in situations when
    //       there is not a status change after a command is called. Uncomment to allow this
    //       error message to appear.
    // fprintf(stderr, "[Velleman] ERROR (port: %s): Reading from relay card failed, ERRNO %i \n", comPort().c_str(), errno);
    return false;
  }

  // check that all 7 bytes were received
  if (readBytes != 7) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Could receive only %d of 7 bytes, ERRNO %i \n", comPort().c_str(), readBytes, errno);
    return false;
  }

  // check for correct start (first) and end (last) of text bytes
  if (STX != buffer[0]) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): Start of text byte is %x but should be %x, ERRNO %i", comPort().c_str(), buffer[0], STX, errno);
    return false; 
  }
  
  if (ETX != buffer[6]) {
    fprintf(stderr, "[Velleman] ERROR (port: %s): End of text byte is %x but should be %x, ERRNO %i", comPort().c_str(), buffer[6], ETX, errno);
    return false; 
  }

  // check for correct checksum byte (byte 6/7)
  unsigned char sum = 0x00;
  for (int i = 0; i < 5; i++)
    sum += buffer[i];
  unsigned char checksum = ~((unsigned char) sum) + 0x01;
  
  if (checksum != buffer[5]) {
    fprintf(stderr, "[Velleman] ERROR (port: %s ): Checksum is %x but should be %x, ERRNO %i \n", comPort().c_str(), (unsigned int) buffer[5], (unsigned int) checksum, errno);
    return false;
  }

  // set parameter pointers to values received from Velleman relay card
  *cmd = buffer[1];
  *mask = buffer[2];
  *param1 = buffer[3];
  *param2 = buffer[4];

  return true;
}

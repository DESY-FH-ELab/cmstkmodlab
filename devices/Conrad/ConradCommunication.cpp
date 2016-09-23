#include "ConradCommunication.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <iostream>

//! Default constructor
ConradCommunication::ConradCommunication(const char* comPort)
  : m_comPort(comPort),
    m_ioPort(-1)
{
}


//! Default destructor
ConradCommunication::~ConradCommunication()
{
  // See, if someone forgot to close...
  if (m_ioPort != -1)
    finish();
}

//! Initialize Conrad IO communication
bool ConradCommunication::initialize()
{
    
    assert(m_ioPort == -1);
    
  // open io port (read/write | no term control | no DCD line check)
  m_ioPort = open(m_comPort, O_RDWR | O_NOCTTY  | O_NDELAY);
  if (m_ioPort == -1)
    return false;

  // get and save current ioport settings for later restoring
  tcgetattr(m_ioPort, &m_termiosInitial);

  // clear new settions structure
  bzero(&m_termios, sizeof(m_termios));

  // set input/output baud rate
  cfsetispeed(&m_termios, B19200);
  cfsetospeed(&m_termios, B19200);

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
  tcflush(m_ioPort, TCIOFLUSH);

  if (tcsetattr(m_ioPort, TCSAFLUSH, &m_termios) != 0)
    return false;

  int modeLines = 0;

  // put card into exclusive mode
  if (ioctl(m_ioPort, TIOCEXCL, &modeLines) == -1)
    return false;

  return true;
}

//! Finish & reset IO port
void ConradCommunication::finish()
{
  if (m_ioPort == -1)
    return;

  // restore old com port settings
  tcsetattr(m_ioPort, TCSANOW, &m_termiosInitial);

  // close io port
  close(m_ioPort);
  m_ioPort = -1;
}

//! Send instructions to Conrad
bool ConradCommunication::sendCommand(unsigned char command, unsigned char address, unsigned char data) const
{
  assert(m_ioPort != -1);

  unsigned char buffer[4];
  buffer[0] = command;
  buffer[1] = address;
  buffer[2] = data;
  buffer[3] = command ^ address ^ data;
  int wroteBytes = write(m_ioPort, buffer, 4);

  // wait until the output has been transmitted
  tcdrain(m_ioPort);

  if (wroteBytes != 4) {
    fprintf(stderr, "[Conrad] ERROR: Could send only %d of 4 bytes\n", wroteBytes);
    return false;
  }

  // give the card some time (100 ms)
  usleep(100000);

  return true;
}

//! Receive answer from Conrad
bool ConradCommunication::receiveAnswer(unsigned char* answer, unsigned char* address, unsigned char* data) const
{
  unsigned char buffer[4] = { 0 };

  int readBytes = read(m_ioPort, buffer, 4);
  if (readBytes == -1) {
    fprintf(stderr, "[Conrad] ERROR: Reading from card failed\n");
    return false;
  }

  if (readBytes != 4) {
    fprintf(stderr, "[Conrad] ERROR: Could receive only %d of 4 bytes\n", readBytes);
    return false;
  }

  int checksum = buffer[0] ^ buffer[1] ^ buffer[2];
  if (checksum != buffer[3]) {
    fprintf(stderr, "[Conrad] ERROR: XOR checksum is %x but should be %x\n", checksum, buffer[3]);
    return false;
  }

  *answer = buffer[0];
  *address = buffer[1];
  *data = buffer[2];

  return true;
}

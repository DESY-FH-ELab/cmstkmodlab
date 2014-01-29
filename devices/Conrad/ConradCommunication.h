#ifndef ConradCommunication_h
#define ConradCommunication_h

#include <termios.h>

/// \brief Class handling raw IO communication with Conrad relais card
class ConradCommunication {

public:

  ConradCommunication(const char* comPort);
  ~ConradCommunication();

  bool initialize();
  void finish();

  bool sendCommand(unsigned char command, unsigned char address, unsigned char data) const;
  bool receiveAnswer(unsigned char* answer, unsigned char* address, unsigned char* data) const;

private:
  const char* m_comPort;
  int m_ioPort;

  struct termios m_termiosInitial;
  struct termios m_termios;
};

#endif

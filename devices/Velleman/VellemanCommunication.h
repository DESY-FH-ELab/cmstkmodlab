/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//          Modeled on cmstkmodlab/devices/Conrad/ConradCommunication.h        //
//                           Last Updated August 6, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VellemanCommunication_h
#define VellemanCommunication_h

#include <string>
#include <termios.h>

// Packet delimiters for Velleman Relay Card
#define STX 0x04
#define ETX 0x0f

// Useful commands for Velleman Relay Card
#define CMD_SWITCH_RELAY_ON 0x11
#define CMD_SWITCH_RELAY_OFF 0x12
#define CMD_TOGGLE_RELAY 0x14
#define CMD_QUERY_RELAY_STATUS 0x18
#define CMD_RELAY_STATUS 0x51
#define CMD_RESET_FACTORY_DEFAULTS 0x66 // DEFAULTS = toggle mode; timer delay of 5s


// \brief Class handling raw IO communication with Velleman Relay Card
class VellemanCommunication {

public:

  VellemanCommunication(const std::string& comPort);
  ~VellemanCommunication();

  bool initialize();
  void finish();

  bool sendCommand(unsigned char cmd, unsigned char mask, unsigned char param1, unsigned char param2) const;
  bool receiveAnswer(unsigned char* cmd, unsigned char* mask, unsigned char* param1, unsigned char* param2) const;

  const std::string& comPort() const { return m_comPort; }

  
private:

  const std::string m_comPort;
  int m_ioPort;

  struct termios m_termiosInitial; 
  struct termios m_termios;
 
};

#endif // ends #ifndef VellemanCommunication_h

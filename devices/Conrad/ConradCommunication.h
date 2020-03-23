/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ConradCommunication_h
#define ConradCommunication_h

#include <string>
#include <termios.h>

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Conrad
 *  @{
 */

class ConradCommunication
{
public:

  ConradCommunication(const std::string& comPort);
  ~ConradCommunication();

  bool initialize();
  void finish();

  bool sendCommand(unsigned char command, unsigned char address, unsigned char data) const;
  bool receiveAnswer(unsigned char* answer, unsigned char* address, unsigned char* data) const;

  const std::string& comPort() const { return m_comPort; }

private:

  const std::string m_comPort;
  int m_ioPort;

  struct termios m_termiosInitial;
  struct termios m_termios;
};

/** @} */

/** @} */

#endif

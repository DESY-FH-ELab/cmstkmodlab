/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _AgilentTwisTorr304ComHandler_H_
#define _AgilentTwisTorr304ComHandler_H_

#include <termios.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Agilent
 *  @{
 */

#define COM1 "/dev/ttyS0"
#define COM2 "/dev/ttyS1"
#define COM3 "/dev/ttyS2"
#define COM4 "/dev/ttyS3"

#define ttyS0 "/dev/ttyS0"
#define ttyS1 "/dev/ttyS1"
#define ttyS2 "/dev/ttyS2"
#define ttyS3 "/dev/ttyS3"
#define ttyS4 "/dev/ttyS4"
#define ttyS5 "/dev/ttyS5"

#define ttyACM0 "/dev/ttyACM0"

typedef const char* ioport_t;
typedef struct termios termios_t;

class AgilentTwisTorr304ComHandler
{
 public:
  
  //! Constructor.
  AgilentTwisTorr304ComHandler(ioport_t);

  //! Destructor.
  ~AgilentTwisTorr304ComHandler();

  //! Default bitwise copy constructor.
  AgilentTwisTorr304ComHandler(const AgilentTwisTorr304ComHandler&);

  void SendCommand(const char*);
  void ReceiveString(char*);

  void flush( void);

  static constexpr int ComHandlerDelay = 1000;

 private:

  void OpenIoPort() noexcept(false);
  void InitializeIoPort();
  void RestoreIoPort();
  void CloseIoPort();
  void SendFeedString();

  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

/** @} */

/** @} */

#endif // _AgilentTwisTorr304ComHandler_H_

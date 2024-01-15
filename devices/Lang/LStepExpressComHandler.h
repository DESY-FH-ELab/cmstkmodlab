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

#ifndef _LSTEPEXPRESSCOMHANDLER_H_
#define _LSTEPEXPRESSCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Lang
 *  @{
 */

typedef struct termios termios_t;

class LStepExpressComHandler {

 public:

  //! Constructor.
  LStepExpressComHandler(const std::string&);

  //! Destructor.
  ~LStepExpressComHandler();

  //! Default bitwise copy constructor.
  LStepExpressComHandler( const LStepExpressComHandler & );

  const std::string& ioPort() const { return fIoPort; }

  void SendCommand( const char* );
  std::string ReceiveString();

  bool DeviceAvailable();

 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  bool fDeviceAvailable;
  int fIoPortFileDescriptor;

  const std::string fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

/** @} */

/** @} */

#endif




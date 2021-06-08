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

#include <string.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "AgilentTwisTorr304ComHandler.h"

#include "AgilentTwisTorr304.h"

///
///
///
AgilentTwisTorr304::AgilentTwisTorr304( const ioport_t ioPort )
 : VAgilentTwisTorr304(ioPort),
   uDelay_(100)
{
  comHandler_ = new AgilentTwisTorr304ComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();
}

VAgilentTwisTorr304::StatusCode AgilentTwisTorr304::GetPumpStatus() const
{
  std::string command, hexcommand;

  MakeReadCommand(command, VAgilentTwisTorr304::PumpStatus);

  GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  comHandler_->SendCommand(command.c_str());

  usleep(uDelay_);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  std::string reply = buf;

  GetCommandAsHex(hexcommand, reply);
  std::cout << hexcommand << std::endl;

  return static_cast<VAgilentTwisTorr304::StatusCode>(GetIntegerValue(reply));
}

unsigned int AgilentTwisTorr304::GetErrorCode() const
{
  std::string command, hexcommand;

  MakeReadCommand(command, VAgilentTwisTorr304::ErrorCode);

  GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  comHandler_->SendCommand(command.c_str());

  usleep(uDelay_);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  std::string reply = buf;

  GetCommandAsHex(hexcommand, reply);
  std::cout << hexcommand << std::endl;

  return GetIntegerValue(reply);
}

bool AgilentTwisTorr304::GetPumpState() const
{
  std::string command, hexcommand;

  MakeReadCommand(command, VAgilentTwisTorr304::StartStop);

  GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  comHandler_->SendCommand(command.c_str());

  usleep(uDelay_);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  std::string reply = buf;

  GetCommandAsHex(hexcommand, reply);
  std::cout << hexcommand << std::endl;

  return GetBooleanValue(reply);
}

void AgilentTwisTorr304::SwitchPumpOn()
{
  std::string command, hexcommand;

  MakeWriteCommand(command, VAgilentTwisTorr304::StartStop, true);

  GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  comHandler_->SendCommand(command.c_str());

  usleep(uDelay_);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  std::string reply = buf;

  GetCommandAsHex(hexcommand, reply);
  std::cout << hexcommand << std::endl;
}

void AgilentTwisTorr304::SwitchPumpOff()
{
  std::string command, hexcommand;

  MakeWriteCommand(command, VAgilentTwisTorr304::StartStop, false);

  GetCommandAsHex(hexcommand, command);
  std::cout << hexcommand << std::endl;

  comHandler_->SendCommand(command.c_str());

  usleep(uDelay_);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);

  std::string reply = buf;

  GetCommandAsHex(hexcommand, reply);
  std::cout << hexcommand << std::endl;
}

///
/// strip trailing newlines & stuff
/// from machine answer
///
void AgilentTwisTorr304::StripBuffer( char* buffer ) const
{
#ifdef __AgilentTwisTorr304_DEBUG
  std::cout << "[AgilentTwisTorr304::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[AgilentTwisTorr304::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
  std::cout << " > " << buffer << std::endl;
#endif

  for( unsigned int c = 0; c < strlen( buffer ); ++c ) {
    if( '\n' == buffer[c] || '\r' == buffer[c] ) {
      buffer[c] = 0;
      break;
    }
  }
}

///
/// read back software version
/// to check communication with device
///
void AgilentTwisTorr304::Device_Init()
{
#ifdef __AgilentTwisTorr304_DEBUG
  std::cout << "[AgilentTwisTorr304::Device_Init] -- DEBUG: Called." << std::endl;
#endif

  isCommunication_ = false;

  char buffer[1000];
  
  comHandler_->SendCommand("#STr");
  usleep( uDelay_ );

  comHandler_->ReceiveString( buffer );

  usleep( uDelay_ );
  StripBuffer( buffer );
  std::string temp(buffer);
  // std::cout<<"status = "<<buffer<<std::endl;
  if (temp.compare(0, 2, "ST")!=0) {
    std::cerr << " [AgilentTwisTorr304::Device_Init] ** ERROR: Device communication problem." << std::endl;
    return;
  }

  isCommunication_ = true;
}

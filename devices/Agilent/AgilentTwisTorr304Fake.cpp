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

#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "AgilentTwisTorr304Fake.h"

///
///
///
AgilentTwisTorr304Fake::AgilentTwisTorr304Fake(const ioport_t ioPort)
 : VAgilentTwisTorr304(ioPort)
{
  pumpState_ = false;
  pumpStateChanged_ = std::chrono::system_clock::now() - std::chrono::hours(1);
}

VAgilentTwisTorr304::StatusCode AgilentTwisTorr304Fake::GetPumpStatus() const
{
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  int msecs = std::chrono::duration_cast<std::chrono::milliseconds>(now - pumpStateChanged_).count();

  if (!pumpState_) {
    if (msecs>20000) {
      return Stop;
    } else {
      return Braking;
    }
  } else {
    if (msecs>20000) {
      return Normal;
    } else {
      return Starting;
    }
  }
}

unsigned int AgilentTwisTorr304Fake::GetErrorCode() const
{
  return 0;
}

bool AgilentTwisTorr304Fake::GetPumpState() const
{
  return pumpState_;
}

void AgilentTwisTorr304Fake::SwitchPumpOn()
{
  if (!pumpState_) {
    pumpState_ = true;
    pumpStateChanged_ = std::chrono::system_clock::now();
  }
}

void AgilentTwisTorr304Fake::SwitchPumpOff()
{
  if (pumpState_) {
    pumpState_ = false;
    pumpStateChanged_ = std::chrono::system_clock::now();
  }
}

///
///
///
void AgilentTwisTorr304Fake::Device_Init()
{
  
}

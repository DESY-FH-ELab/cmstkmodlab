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

#ifndef _AgilentTwisTorr304Fake_H_
#define _AgilentTwisTorr304Fake_H_

#include <chrono>

#include "VAgilentTwisTorr304.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Agilent
 *  @{
 */

class AgilentTwisTorr304Fake : public VAgilentTwisTorr304
{
 public:

  AgilentTwisTorr304Fake(ioport_t);
 
  StatusCode GetPumpStatus() const;
  unsigned int GetErrorCode() const;

  bool GetPumpState() const;
  void SwitchPumpOn();
  void SwitchPumpOff();

 private:

  mutable bool pumpState_;
  std::chrono::system_clock::time_point pumpStateChanged_;

  void Device_Init(void);
};

/** @} */

/** @} */

#endif

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _AgilentTwisTorr304_H_
#define _AgilentTwisTorr304_H_

#include "VAgilentTwisTorr304.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Agilent
 *  @{
 */

class AgilentTwisTorr304ComHandler;

class AgilentTwisTorr304 : public VAgilentTwisTorr304
{
 public:

  AgilentTwisTorr304(ioport_t);

  bool IsCommunication() const { return isCommunication_; }

  StatusCode GetPumpStatus() const;
  unsigned int GetErrorCode() const;

  bool GetPumpState() const;
  void SwitchPumpOn();
  void SwitchPumpOff();

 private:

  const int uDelay_;
  void StripBuffer(char*) const;

  void Device_Init();
  AgilentTwisTorr304ComHandler* comHandler_;
  bool isCommunication_;
};

/** @} */

/** @} */

#endif // _AgilentTwisTorr304_H_

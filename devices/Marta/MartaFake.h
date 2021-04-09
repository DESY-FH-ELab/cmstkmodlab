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

#ifndef _MARTAFAKE_H_
#define _MARTAFAKE_H_

#include "VMarta.h"

/** @addtogroup devices
 *  @{
 */


/** @addtogroup Marta
 *  @{
 */

class MartaFake : public VMarta
{
 public:

  MartaFake( ipaddress_t );
  MartaFake( ipaddress_t, port_t );
  ~MartaFake();

 public:

  void ReadRegisters(int, int, uint16_t*) {};
  void WriteRegisters(int, int, const uint16_t*) {};

  bool DeviceAvailable() const { return true; }

  private:

};
 
/** @} */

/** @} */

#endif

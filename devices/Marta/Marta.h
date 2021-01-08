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

#ifndef _MARTA_H_
#define _MARTA_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VMarta.h"
#include "MartaComHandler.h"

/** @addtogroup devices
 *  @{
 */


/** @addtogroup Marta
 *  @{
 */

class Marta : public VMarta
{
public:

  Marta( ipaddress_t );
  Marta( ipaddress_t, port_t );
  ~Marta();

public:

  void ReadRegisters(int addr, int nb, uint16_t *dest);
  void WriteRegisters(int addr, int nb, const uint16_t *src);
  
  bool DeviceAvailable() const;
  
 private:

  MartaComHandler* comHandler_;
  bool isDeviceAvailable_;
};
 
/** @} */

/** @} */

#endif

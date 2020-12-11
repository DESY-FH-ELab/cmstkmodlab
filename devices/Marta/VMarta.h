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

#ifndef _VMARTA_H_
#define _VMARTA_H_

#include <string>

typedef const char* ipaddress_t;
typedef unsigned int port_t;

/** @addtogroup devices
 *  @{
 */


/** @addtogroup Marta
 *  @{
 */

class VMarta
{
public:

  VMarta( ipaddress_t, port_t );
  virtual ~VMarta();

public:

  virtual void ReadRegisters(int addr, int nb, uint16_t *dest) = 0;
  
  virtual bool DeviceAvailable() const = 0;

  float ToFloatABCD(uint16_t *reg);
  float ToFloatBADC(uint16_t *reg);
  float ToFloatCDAB(uint16_t *reg);
  float ToFloatDCBA(uint16_t *reg);
};
 
/** @} */

/** @} */

#endif // _VMARTA_H_

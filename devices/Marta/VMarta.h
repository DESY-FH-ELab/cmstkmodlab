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

  VMarta( ipaddress_t );
  VMarta( ipaddress_t, port_t );
  virtual ~VMarta();

public:

  virtual void ReadRegisters(int, int, uint16_t*) = 0;
  virtual void WriteRegisters(int, int, const uint16_t*) = 0;
  
  virtual bool DeviceAvailable() const = 0;

  float ToFloatABCD(uint16_t *reg);
  float ToFloatBADC(uint16_t *reg);
  float ToFloatCDAB(uint16_t *reg);
  float ToFloatDCBA(uint16_t *reg);

  void FromFloatABCD(float f, uint16_t *reg);
  void FromFloatDCBA(float f, uint16_t *reg);
  void FromFloatBADC(float f, uint16_t *reg);
  void FromFloatCDAB(float f, uint16_t *reg);
};
 
/** @} */

/** @} */

#endif // _VMARTA_H_

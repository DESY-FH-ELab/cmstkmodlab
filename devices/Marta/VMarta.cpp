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
#include <arpa/inet.h>
#include <byteswap.h>

#include "VMarta.h"

VMarta::VMarta( ipaddress_t )
{

}

VMarta::VMarta( ipaddress_t, port_t )
{

}

VMarta::~VMarta()
{

}

float VMarta::ToFloatABCD(uint16_t *reg)
{
  float f;
  uint32_t i;

  i = ntohl(((uint32_t)reg[0] << 16) + reg[1]);
  memcpy(&f, &i, sizeof(float));
  
  return f;
}

float VMarta::ToFloatBADC(uint16_t *reg)
{
  float f;
  uint32_t i;
  
  i = ntohl((uint32_t)(bswap_16(reg[0]) << 16) + bswap_16(reg[1]));
  memcpy(&f, &i, sizeof(float));
  
  return f;
}

float VMarta::ToFloatCDAB(uint16_t *reg)
{
  float f;
  uint32_t i;

  i = ntohl(((uint32_t)reg[1] << 16) + reg[0]);
  memcpy(&f, &i, sizeof(float));
  
  return f;
}

float VMarta::ToFloatDCBA(uint16_t *reg)
{
  float f;
  uint32_t i;

  i = ntohl(bswap_32((((uint32_t)reg[0]) << 16) + reg[1]));
  memcpy(&f, &i, sizeof(float));
  
  return f;
}

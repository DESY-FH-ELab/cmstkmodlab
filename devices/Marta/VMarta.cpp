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

#ifndef __Darwin__
#include <byteswap.h>
#else
#if !defined(bswap_16)
#  warning "Fallback on C functions for bswap_16"
static inline uint16_t bswap_16(uint16_t x)
{
	return (x >> 8) | (x << 8);
}
#endif
#if !defined(bswap_32)
#  warning "Fallback on C functions for bswap_32"
static inline uint32_t bswap_32(uint32_t x)
{
	return (bswap_16(x & 0xffff) << 16) | (bswap_16(x >> 16));
}
#endif

#endif

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

void VMarta::FromFloatABCD(float f, uint16_t *reg)
{
  uint32_t i;

  memcpy(&i, &f, sizeof(uint32_t));
  i = htonl(i);
  reg[0] = (uint16_t)(i >> 16);
  reg[1] = (uint16_t)i;
}

void VMarta::FromFloatDCBA(float f, uint16_t *reg)
{
  uint32_t i;

  memcpy(&i, &f, sizeof(uint32_t));
  i = bswap_32(htonl(i));
  reg[0] = (uint16_t)(i >> 16);
  reg[1] = (uint16_t)i;
}

void VMarta::FromFloatBADC(float f, uint16_t *reg)
{
  uint32_t i;

  memcpy(&i, &f, sizeof(uint32_t));
  i = htonl(i);
  reg[0] = (uint16_t)bswap_16(i >> 16);
  reg[1] = (uint16_t)bswap_16(i & 0xFFFF);
}

void VMarta::FromFloatCDAB(float f, uint16_t *reg)
{
  uint32_t i;

  memcpy(&i, &f, sizeof(uint32_t));
  i = htonl(i);
  reg[0] = (uint16_t)i;
  reg[1] = (uint16_t)(i >> 16);
}

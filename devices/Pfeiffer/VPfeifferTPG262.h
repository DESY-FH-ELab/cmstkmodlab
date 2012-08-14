#ifndef __VPFEIFFERTPG262_H
#define __VPFEIFFERTPG262_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

typedef const char* ioport_t;

class VPfeifferTPG262
{
 public:

  VPfeifferTPG262( ioport_t );

  virtual bool IsCommunication( void ) const = 0;
  virtual int GetErrorStatus( void ) const = 0;
  virtual float GetPressure1( void ) const = 0;
  virtual float GetPressure2( void ) const = 0;
};

#endif

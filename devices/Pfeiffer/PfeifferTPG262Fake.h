#ifndef __PFEIFFERTPG262FAKE_H
#define __PFEIFFERTPG262FAKE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "VPfeifferTPG262.h"

class PfeifferTPG262Fake : public VPfeifferTPG262
{
 public:

  PfeifferTPG262Fake( ioport_t );

  virtual bool IsCommunication( void ) const { return true; }
  virtual int GetErrorStatus( void ) const { return 0x0; }
  virtual float GetPressure1( void ) const {return 1.0e-3; }
  virtual float GetPressure2( void ) const {return 1.0e-3; }
};

#endif

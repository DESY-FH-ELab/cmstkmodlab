#ifndef __PFEIFFERTPG262FAKE_H
#define __PFEIFFERTPG262FAKE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "VPfeifferTPG262.h"

typedef const char* ioport_t;
typedef std::pair<int,int> stat_t;

class PfeifferTPG262Fake : public VPfeifferTPG262
{
 public:

  PfeifferTPG262Fake( ioport_t );

  virtual bool IsCommunication( void ) const { return true; }
  virtual int GetErrorStatus( void ) const;
  virtual bool GetPressure1(reading_t & reading);
  virtual bool GetPressure2(reading_t & reading);
  virtual bool GetPressures(reading_t & reading1, reading_t & reading2);
};

#endif

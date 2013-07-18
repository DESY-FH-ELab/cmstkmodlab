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
  virtual int GetErrorStatus( void ) const { return 0x0; }
  virtual int GetPressure1(float & pressure) { return 0x0; }
  virtual int GetPressure2(float & pressure) { return 0x0; }
  virtual stat_t GetPressure(float & pressure1, float & pressure2);
  virtual stat_t Readout( float & pressure1, float & pressure2 ) const {return stat_t (0x0,0x0);}
  virtual void SendETX (void) const {;}
  virtual void Reset (void) const {;}
  virtual int KeyLock( void ) const{ return 0x0; }
  virtual void SetContinuous( void ) const{;}
  virtual int GuageIdentification (void) const {return 0x0;}

};

#endif

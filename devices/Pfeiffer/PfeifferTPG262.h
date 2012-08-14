#ifndef __PFEIFFERTPG262_H
#define __PFEIFFERTPG262_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "VPfeifferTPG262.h"

class TPG262ComHandler;

class PfeifferTPG262 : public VPfeifferTPG262
{
 public:

  PfeifferTPG262( ioport_t );
  ~PfeifferTPG262();

  virtual bool IsCommunication( void ) const { return isCommunication_; }
  virtual int GetErrorStatus( void ) const;
  virtual float GetPressure1( void ) const;
  virtual float GetPressure2( void ) const;

 private:

  void StripBuffer( char* ) const;
  void Device_Init( void );
  TPG262ComHandler* comHandler_;
  bool isCommunication_;
};

#endif

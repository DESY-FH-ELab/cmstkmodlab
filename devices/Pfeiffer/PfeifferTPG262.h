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
  virtual bool GetPressure1(reading_t & reading);
  virtual bool GetPressure2(reading_t & reading);
  virtual bool GetPressures(reading_t & reading1, reading_t & reading2);

  /*
  virtual void SendETX (void) const;
  virtual void Reset (void) const;
  virtual int KeyLock( void ) const;
  virtual void SetContinuous( void ) const;
  virtual stat_t Readout( float & pressure1, float & pressure2 ) const;
  virtual int GuageIdentification (void) const;
*/

 private:

  void StripBuffer( char* ) const;
  void Device_Init( void );
  int SetPressureUnit(void) const;
  bool SetBRate(void);
  TPG262ComHandler* comHandler_;
  bool isCommunication_;
};

#endif

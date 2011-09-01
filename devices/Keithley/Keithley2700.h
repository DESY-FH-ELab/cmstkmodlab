#ifndef __KEITHLEY2700_H
#define __KEITHLEY2700_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "VKeithley2700.h"

#include "KMMComHandler.h"

// delay time constants (usec)
// delay for 1 channel scan -- delay for 10 channel scan
#define __DELAY_MIN 1400000 // 1400000
#define __DELAY_MAX 7000000

class Keithley2700 : public VKeithley2700
{
 public:
  Keithley2700( ioport_t );

  void SetActiveChannels( string );
  void AddActiveChannels( string );
  void DisableActiveChannels( string );
  const reading_t Scan( void );
  void Dump( void ) const;
  bool IsScanOk( void );

 private:
  KMMComHandler* comHandler_;
  bool isDebug_;
  bool isScanOk_;
  unsigned int uSecDelay_;
  

  void Device_SetChannels( void ) const;
  void Device_Init( void ) const;
  void CalculateDelay( void );
};

#endif

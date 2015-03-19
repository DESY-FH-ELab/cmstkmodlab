
#ifndef __KEITHLEY2700_H
#define __KEITHLEY2700_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "KMMComHandler.h"

// the number of channels available to the device,
// for range checking purposes
#define __RANGE_MIN 0
#define __RANGE_MAX 9

// delay time constants (usec)
// delay for 1 channel scan -- delay for 10 channel scan
#define __DELAY_MIN 1700000 // 1400000
#define __DELAY_MAX 3200000


typedef std::vector<std::pair<unsigned int, double> > reading_t;
typedef std::vector<unsigned int> channels_t;
typedef std::pair<unsigned int, unsigned int> range_t;

class Keithley2700 {

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
  channels_t enabledChannels_;
  bool isDebug_;
  bool isScanOk_;
  unsigned int uSecDelay_;
  

  void Device_SetChannels( void ) const;
  void Device_Init( void ) const;
  const channels_t ParseChannelString( const string& ) const;
  void Tokenize( const std::string&, std::vector<std::string>&, const std::string& ) const;
  const unsigned int EvaluateChannelToken( const std::string& ) const;
  const range_t EvaluateRangeToken( const std::string& ) const;
  const void CalculateDelay( void );

};


#endif

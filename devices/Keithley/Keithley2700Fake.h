#ifndef __KEITHLEY2700FAKE_H
#define __KEITHLEY2700FAKE_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "VKeithley2700.h"

class Keithley2700Fake : public VKeithley2700
{
 public:
  Keithley2700Fake( ioport_t );

  void SetActiveChannels( std::string );
  void AddActiveChannels( std::string );
  void DisableActiveChannels( std::string );
  const reading_t Scan( void );
  void Dump( void ) const;
  bool IsScanOk( void );

protected:

  std::vector<int> activeChannels_;
};

#endif

#ifndef __VKEITHLEY2700_H
#define __VKEITHLEY2700_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

// the number of channels available to the device,
// for range checking purposes
#define __RANGE_MIN 0
#define __RANGE_MAX 9

typedef std::vector<std::pair<unsigned int, double> > reading_t;
typedef std::vector<unsigned int> channels_t;
typedef std::pair<unsigned int, unsigned int> range_t;

typedef const char* ioport_t;

class VKeithley2700
{
 public:
  VKeithley2700( ioport_t );
  virtual ~VKeithley2700();
  
  virtual void SetActiveChannels( std::string ) = 0;
  virtual void AddActiveChannels( std::string ) = 0;
  virtual void DisableActiveChannels( std::string ) = 0;
  virtual const reading_t Scan( void ) = 0;
  virtual void Dump( void ) const = 0;
  virtual bool IsScanOk( void ) = 0;

 protected:

  const channels_t ParseChannelString( const std::string& ) const;
  void Tokenize( const std::string&, std::vector<std::string>&, const std::string& ) const;
  unsigned int EvaluateChannelToken( const std::string& ) const;
  const range_t EvaluateRangeToken( const std::string& ) const;

  channels_t enabledChannels_;
};

#endif

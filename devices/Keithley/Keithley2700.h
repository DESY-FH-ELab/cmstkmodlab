#ifndef __KEITHLEY2700_H
#define __KEITHLEY2700_H

#include "VKeithley2700.h"

#include "KMMComHandler.h"

class Keithley2700 : public VKeithley2700
{
 public:
  Keithley2700( ioport_t );

  void SetActiveChannels( std::string );
  void SetActiveChannels( channels_t );
  void AddActiveChannels( std::string );
  void DisableActiveChannels( std::string );
  const reading_t Scan( void );
  void Dump( void ) const;
  bool IsScanOk( void ) { return isScanOk_; }
  void Reset();

  // delay time constants (usec)
  // delay for 1 channel scan -- delay for 10 channel scan
  static constexpr int DelayMin = 1700000;
  static constexpr int DelayMax = 7000000;

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

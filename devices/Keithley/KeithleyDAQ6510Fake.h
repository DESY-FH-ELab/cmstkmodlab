/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __KEITHLEYDAQ6510FAKE_H
#define __KEITHLEYDAQ6510FAKE_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <random>

#include "VKeithleyDAQ6510.h"

class KeithleyDAQ6510Fake : public VKeithleyDAQ6510
{
 public:
  KeithleyDAQ6510Fake( ioport_t );

  bool DeviceAvailable() const { return true; }

  void SetTime(int /* year */, int /* month */, int /* day */,
               int /* hour */, int /* minute */, int /* second */) { }

  void SetChannelMode(unsigned int card, unsigned int channel,
  		ChannelMode_t mode);

  void ActivateChannel(unsigned int card, unsigned int channel,
                       bool active);

  bool GetScanStatus() const { return true; }
  void Scan();
  void GetScanData(reading_t & data);

  /*
  void SetActiveChannels( std::string );
  void SetActiveChannels( channels_t );
  void AddActiveChannels( std::string );
  void DisableActiveChannels( std::string );
  const reading_t Scan( void );
  void Dump( void ) const;
  bool IsScanOk( void );
  void Reset() { }
  */
  
  float GetScanDuration() const;

protected:

  std::random_device randomDevice_{};
  std::mt19937 randomGenerator_{randomDevice_()};
  std::normal_distribution<> normalDistribution_{0,0.025};

  reading_t data_;
};

#endif

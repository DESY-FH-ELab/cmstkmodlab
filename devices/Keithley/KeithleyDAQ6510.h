/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __KEITHLEYDAQ6510_H
#define __KEITHLEYDAQ6510_H

#include "VKeithleyDAQ6510.h"

#include "KeithleyUSBTMCComHandler.h"

/************************************************
 * USB Vendor ID:  0x05E6
 * USB Product ID: 0x6510
 ************************************************/
class KeithleyDAQ6510 : public VKeithleyDAQ6510
{
 public:
  KeithleyDAQ6510( ioport_t );

  bool DeviceAvailable() const { return isDeviceAvailable_; }

  void ActivateChannel(unsigned int card, unsigned int channel,
                       bool active);

  /*
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
  */
  
 private:

  KeithleyUSBTMCComHandler* comHandler_;
  bool isDeviceAvailable_;
  
  void StripBuffer(char*) const;
  void DeviceInit();
  void DeviceSetChannels();

  /*
  bool isDebug_;
  bool isScanOk_;
  unsigned int uSecDelay_;
  
  void CalculateDelay( void );
  */
};

#endif

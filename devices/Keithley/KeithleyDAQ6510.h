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
  ~KeithleyDAQ6510( );

  bool DeviceAvailable() const { return isDeviceAvailable_; }

  void SetTime(int year, int month, int day,
               int hour, int minute, int second);

  void SetChannelMode(unsigned int card, unsigned int channel,
  		ChannelMode_t mode);

  void ActivateChannel(unsigned int card, unsigned int channel,
                       bool active);

  bool GetScanStatus() const;
  void Scan();
  void GetScanData(reading_t & data);

  void ShutDown();
  
  float GetScanDuration() const;

 private:

  KeithleyUSBTMCComHandler* comHandler_;
  bool isDeviceAvailable_;
  
  void StripBuffer(char*) const;
  void DeviceInit();

  bool deviceChannelsSet_;
  void DeviceSetChannels();

  std::map<unsigned int, unsigned int> sensorScanCountMap_;
  unsigned int GetSensorFromScanCount(unsigned int scanCount);
};

#endif

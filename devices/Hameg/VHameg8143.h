/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __VHAMEG8143_H
#define __VHAMEG8143_H

typedef const char* ioport_t;

class VHameg8143 {

 public:

  enum StatusBits {
    hmOP0 = 0x01,
    hmOP1 = 0x02,
    hmCV1 = 0x04,
    hmCC1 = 0x08,
    hmCV2 = 0x10,
    hmCC2 = 0x20,
    hmRM1 = 0x40,
    hmRM0 = 0x80
  };

  VHameg8143( const ioport_t );
  virtual ~VHameg8143();

 public:

  virtual bool DeviceAvailable() const = 0;

  virtual unsigned int GetStatus() const = 0;
  virtual bool SetRemoteMode(bool remote = true) const = 0;
  virtual bool SetMixedMode(bool mixed = true) const = 0;
  virtual bool SetVoltage(int channel, float voltage) const = 0;
  virtual bool SetTrackingVoltage(float voltage) const = 0;
  virtual float GetSetVoltage(int channel) const = 0;
  virtual float GetVoltage(int channel) const = 0;
  virtual bool SetCurrent(int channel, float current) const = 0;
  virtual bool SetTrackingCurrent(float current) const = 0;
  virtual float GetSetCurrent(int channel) const = 0;
  virtual float GetCurrent(int channel) const = 0;

  virtual bool SwitchOutputOn() const = 0;
  virtual bool SwitchOutputOff() const = 0;
};

#endif

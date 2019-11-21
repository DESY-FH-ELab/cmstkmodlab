/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ROHDESCHWARZNGE103B_H
#define __ROHDESCHWARZNGE103B_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VRohdeSchwarzNGE103B.h"
#include "NGE103BComHandler.h"

class RohdeSchwarzNGE103B : public VRohdeSchwarzNGE103B
{
public:

  RohdeSchwarzNGE103B( const ioport_t );
  ~RohdeSchwarzNGE103B();

public:

  bool DeviceAvailable() const;

  void GetIdentification(std::string& id) const;

  void SelectChannel(unsigned int ch);
  unsigned int SelectedChannel() const;

  void SetVoltage(float v);
  float GetVoltage() const;
  float MeasureVoltage() const;

  void SetCurrent(float i) ;
  float GetCurrent() const;
  float MeasureCurrent() const;

  float MeasurePower() const;

  void SetOutputState(bool s);
  bool GetOutputState() const;

  unsigned int GetOutputMode() const;

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  bool OperationCompleted() const;

  NGE103BComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif

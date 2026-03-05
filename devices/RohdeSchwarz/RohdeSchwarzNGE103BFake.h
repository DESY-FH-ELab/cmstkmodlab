/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ROHDESCHWARZNGE103BFAKE_H
#define __ROHDESCHWARZNGE103BFAKE_H

#include <array>

#include "VRohdeSchwarzNGE103B.h"

/** @addtogroup devices
 *  @{
 */


/** @addtogroup RohdeSchwarz
 *  @{
 */

class RohdeSchwarzNGE103BFake : public VRohdeSchwarzNGE103B
{
 public:

  RohdeSchwarzNGE103BFake( const ioport_t );
  ~RohdeSchwarzNGE103BFake();

 public:

  bool DeviceAvailable() const { return true; }

  void GetIdentification(std::string& id) const;

  void SelectChannel(unsigned int ch);
  unsigned int SelectedChannel() const;

  void SetVoltage(float v);
  float GetVoltage() const;
  float MeasureVoltage() const;

  void SetEasyRampDuration(float d);
  float GetEasyRampDuration() const;
  void SetEasyRampState(bool s);
  bool GetEasyRampState() const;

  void SetCurrent(float i) ;
  float GetCurrent() const;
  float MeasureCurrent() const;

  float MeasurePower() const;

  void SetOutputState(bool s);
  bool GetOutputState() const;

  unsigned int GetOutputMode() const;

  private:

  unsigned int selectedChannel_;
  std::array<float,3> voltage_;
  std::array<float,3> current_;
  std::array<bool,3> outputState_;
  std::array<float,3> easyRampDuration_;
  std::array<bool,3> easyRampState_;
};
 
/** @} */

/** @} */

#endif

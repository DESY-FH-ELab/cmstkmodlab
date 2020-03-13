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

#ifndef _LEYBOLDGRAPHIXONEFAKE_H_
#define _LEYBOLDGRAPHIXONEFAKE_H_

#include "LeyboldGraphixThreeFake.h"

class LeyboldGraphixOneFake : public LeyboldGraphixThreeFake
{
 public:

  LeyboldGraphixOneFake( const ioport_t );
  ~LeyboldGraphixOneFake();

  int GetNumberOfChannels() const;

  SensorDetectionMode GetSensorDetectionMode() const;
  void SetSensorDetectionMode(SensorDetectionMode mode);

  std::string GetSensorTypeName() const;
  void SetSensorTypeName(std::string type);

  std::string GetSensorName() const;
  void SetSensorName(const std::string& name);

  SensorStatus GetSensorStatus() const;

  double GetPressure() const;

 protected:

  using LeyboldGraphixThreeFake::GetSensorDetectionMode;
  using LeyboldGraphixThreeFake::SetSensorDetectionMode;
  using LeyboldGraphixThreeFake::GetSensorTypeName;
  using LeyboldGraphixThreeFake::SetSensorTypeName;
  using LeyboldGraphixThreeFake::GetSensorName;
  using LeyboldGraphixThreeFake::SetSensorName;
  using LeyboldGraphixThreeFake::GetSensorStatus;
  using LeyboldGraphixThreeFake::GetPressure;
};

#endif // _LEYBOLDGRAPHIXONEFAKE_H_

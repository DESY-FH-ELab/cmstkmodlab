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

#ifndef _LEYBOLDGRAPHIXONE_H_
#define _LEYBOLDGRAPHIXONE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>
#include <array>

#include "LeyboldGraphixThree.h"

class LeyboldGraphixOne : public LeyboldGraphixThree
{
 public:

  LeyboldGraphixOne( const ioport_t );
  ~LeyboldGraphixOne();

  SensorDetectionMode GetSensorDetectionMode() const;
  void SetSensorDetectionMode(SensorDetectionMode mode);

  std::string GetSensorTypeName() const;
  void SetSensorTypeName(std::string type);

  std::string GetSensorName() const;
  void SetSensorName(const std::string& name);

  SensorStatus GetSensorStatus() const;

  double GetPressure() const;

 protected:

  using LeyboldGraphixThree::GetSensorDetectionMode;
  using LeyboldGraphixThree::SetSensorDetectionMode;
  using LeyboldGraphixThree::GetSensorTypeName;
  using LeyboldGraphixThree::SetSensorTypeName;
  using LeyboldGraphixThree::GetSensorName;
  using LeyboldGraphixThree::SetSensorName;
  using LeyboldGraphixThree::GetSensorStatus;
  using LeyboldGraphixThree::GetPressure;
};

#endif // _LEYBOLDGRAPHIXONE_H_

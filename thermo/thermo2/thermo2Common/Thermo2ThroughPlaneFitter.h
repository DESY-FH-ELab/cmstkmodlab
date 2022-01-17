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

#ifndef Thermo2ThroughPlaneFitter_H
#define Thermo2ThroughPlaneFitter_H

#include <vector>

class Thermo2ThroughPlaneFitter
{
public:

  explicit Thermo2ThroughPlaneFitter();

  bool fit(std::vector<std::pair<double,double> > &values, unsigned int order,
      double & p0, double & p1);
};

#endif // Thermo2ThroughPlaneFitter_H

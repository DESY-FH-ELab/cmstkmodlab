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

#include <unistd.h>

#include <iostream>
#include <cmath>

#include "Thermo2ThroughPlaneFitter.h"

int main()
{
  std::vector<std::pair<double,double> > values;

  std::vector<double> temp { 33.513, 33.6431, 33.5369, 33.4525, 33.3548, 33.3 };
  // std::vector<double> temp { 15.7696, 15.858, 15.8441, 15.9938, 16.0728, 16.093 };

  values.push_back(std::pair<double,double>(-60.0, temp[0]));
  values.push_back(std::pair<double,double>(-50.0, temp[1]));
  values.push_back(std::pair<double,double>(-40.0, temp[2]));
  values.push_back(std::pair<double,double>(-30.0, temp[3]));
  values.push_back(std::pair<double,double>(-20.0, temp[4]));
  values.push_back(std::pair<double,double>(-10.0, temp[5]));

  for (std::vector<std::pair<double,double> >::iterator it = values.begin();
       it!=values.end();
       ++it) {
    std::cout << it->first << "\t" << it->second << std::endl;
  }

  double p0, p1;
  Thermo2ThroughPlaneFitter fitter;
  fitter.fit(values, 2, p0, p1);

  std::cout << p0 << "\t" << p1 << std::endl;

  double gradient = 1000. * p1; // [degC/m]
  double area = 30.0 * 30.0;    // [mm^2]
  double k = 394.0;             // [W/m/K]

  std::cout << "nominal power: " << 4.7 * 0.8 * 0.8 << std::endl;

  double power = gradient * k * area * 1e-6;

  std::cout << "measured power: " << power << std::endl;


  return 0;
}

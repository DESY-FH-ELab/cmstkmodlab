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

#ifndef __VROHDESCHWARZNGE103B_H
#define __VROHDESCHWARZNGE103B_H

#include <string>

typedef const char* ioport_t;

class VRohdeSchwarzNGE103B {

 public:

  VRohdeSchwarzNGE103B( const ioport_t );
  virtual ~VRohdeSchwarzNGE103B();

 public:

  virtual bool DeviceAvailable() const = 0;

  virtual void GetIdentification(std::string& id) const = 0;
};

#endif // __VROHDESCHWARZNGE103B_H

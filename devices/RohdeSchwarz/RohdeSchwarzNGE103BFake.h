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

#ifndef __ROHDESCHWARZNGE103BFAKE_H
#define __ROHDESCHWARZNGE103BFAKE_H

#include "VRohdeSchwarzNGE103B.h"

class RohdeSchwarzNGE103BFake : public VRohdeSchwarzNGE103B
{
 public:

  RohdeSchwarzNGE103BFake( const ioport_t );
  ~RohdeSchwarzNGE103BFake();

 public:

  bool DeviceAvailable() const { return true; }

  void GetIdentification(std::string& id) const;

  private:

};

#endif

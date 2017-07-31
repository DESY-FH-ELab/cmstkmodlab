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

#ifndef VConradController_h
#define VConradController_h

#include <vector>

typedef const char* ioport_t;

/// \brief Class handling readout of Conrad multimeter
class VConradController {

public:

  VConradController( const ioport_t );
  virtual ~VConradController();

  virtual bool initialize() = 0;

  virtual std::vector<bool> queryStatus() const = 0;
  virtual bool setChannel(unsigned channel, bool value) const = 0;
  virtual bool setSingleChannel(unsigned channel, bool value) const = 0;
};

#endif

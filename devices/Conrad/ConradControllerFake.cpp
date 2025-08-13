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

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "ConradControllerFake.h"

//! Default constructor
ConradControllerFake::ConradControllerFake(const std::string& ioPort)
  : VConradController(ioPort)
{
  for (int c=0;c<8;++c) status_[c] = false;
}

//! Default destructor
ConradControllerFake::~ConradControllerFake()
{

}

//! Initialize Conrad IO communication
bool ConradControllerFake::initialize()
{
  return true;
}

//! Query channel status (returns 8 items reflecting the channel state, on/off, 0 items if it failed)
std::vector<bool> ConradControllerFake::queryStatus() const
{
  std::vector<bool> result(status_, status_ + 8);
  return result;
}

//! Set a specific channel on or off, not touching any other channel status
bool ConradControllerFake::setChannel(unsigned channel, bool value) const
{
  assert(channel <= 8);

  status_[channel] = value;

  return true;
}

//! Set a specific channel on or off, and switch _all_ other channels to off
bool ConradControllerFake::setSingleChannel(unsigned channel, bool value) const
{
  assert(channel <= 8);

  for (int c=0;c<8;++c) status_[c] = false;
  status_[channel] = value;

  return true;
}

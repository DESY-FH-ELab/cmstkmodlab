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

#ifndef ConradControllerFake_h
#define ConradControllerFake_h

#include <vector>

#include "VConradController.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Conrad
 *  @{
 */

class ConradControllerFake : public VConradController
{
public:

  ConradControllerFake(const std::string&);
  virtual ~ConradControllerFake();

  bool initialize();

  std::vector<bool> queryStatus() const;
  bool setChannel(unsigned channel, bool value) const;
  bool setSingleChannel(unsigned channel, bool value) const;

private:

  mutable bool status_[8];
};

/** @} */

/** @} */

#endif

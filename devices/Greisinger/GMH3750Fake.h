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

#ifndef _GMH3750Fake_H_
#define _GMH3750Fake_H_

#include "VGMH3750.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Greisinger
 *  @{
 */

class GMH3750Fake : public VGMH3750
{
 public:
  
  //! Constructor.
  GMH3750Fake( ioport_t );

  //! Destructor.
  ~GMH3750Fake();

  bool Read(double & temperature);
};

/** @} */

/** @} */

#endif

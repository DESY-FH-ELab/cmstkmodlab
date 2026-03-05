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

#ifndef _GMH3750_H_
#define _GMH3750_H_

#include "GMH3750ComHandler.h"

#include "VGMH3750.h"

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Greisinger
 *  @{
 */

class GMH3750 : public VGMH3750
{
 public:
  
  //! Constructor.
  GMH3750( ioport_t );

  //! Destructor.
  ~GMH3750();

  bool Read(double & temperature);

 private:

  GMH3750ComHandler* comHandler_;
};

/** @} */

/** @} */

#endif

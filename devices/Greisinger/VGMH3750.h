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

#ifndef _VGMH3750_H_
#define _VGMH3750_H_

/** @addtogroup devices
 *  @{
 */

/** @addtogroup Greisinger
 *  @{
 */

typedef const char* ioport_t;

class VGMH3750
{
 public:

  //! Constructor.
  VGMH3750( ioport_t );

  //! Destructor.
  virtual ~VGMH3750();

  virtual bool Read(double & temperature) = 0;
};

/** @} */

/** @} */

#endif

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

#include "VEOS550D.h"

VEOS550D::VEOS550D(const char* port) {

}

VEOS550D::~VEOS550D() {

}

/// Returns the current index of given value in list, or -1 if not found.
int VEOS550D::indexOf(const VEOS550D::OptionList &list, const std::string &value) {

  int i = list.size() - 1;
  // loop until found or smaller than 0
  while ( list[i] != value && i >= 0 )
    --i;

  return i;
}

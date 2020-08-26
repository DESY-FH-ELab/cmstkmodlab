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

#ifndef VALUEHISTORY_H
#define VALUEHISTORY_H

#include <iostream>

#include "Fifo.h"

template <class T> class ValueHistory : public Fifo<T>
{
public:

  ValueHistory(int updateInterval = 0, int historySpan = 0)
    : Fifo<T>(historySpan/updateInterval+1),
      updateInterval_(updateInterval),
      historySpan_(historySpan) {

  }

  const T & secondsAgo(int seconds) const {
    if (seconds>historySpan_) return this->last();
    if (this->size()*updateInterval_<seconds) return this->last();
    return this->at(seconds/updateInterval_);
  }

protected:

  int updateInterval_;
  int historySpan_;
};

#endif // VALUEHISTORY_H

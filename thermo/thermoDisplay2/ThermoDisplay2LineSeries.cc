/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>

#include <QtNetwork>

#include <nqlogger.h>

#include "ThermoDisplay2LineSeries.h"

ThermoDisplay2LineSeries::ThermoDisplay2LineSeries()
  : QLineSeries(),
    initialized_(false),
    enabled_(false),
    minX_(0), maxX_(0),
    minY_(0), maxY_(0),
    lastX_(0), lastY_(0)
{

}

void ThermoDisplay2LineSeries::setEnabled(bool enabled)
{
  if (enabled_ && !enabled) {
    clear();
    resetInitialized();
  }

  enabled_ = enabled;
 }

void ThermoDisplay2LineSeries::append(qreal x, qreal y)
{
  if (!enabled_) return;

  if (lastX_==x && lastY_==y) return;

  if (!initialized_) {
    minX_ = std::numeric_limits<qreal>::max();
    maxX_ = -std::numeric_limits<qreal>::max();
    minY_ = std::numeric_limits<qreal>::max();
    maxY_ = -std::numeric_limits<qreal>::max();
    initialized_ = true;
  }

  minX_ = std::min(minX_, x);
  maxX_ = std::max(maxX_, x);

  minY_ = std::min(minY_, y);
  maxY_ = std::max(maxY_, y);

  lastX_ = x;
  lastY_ = y;

  QLineSeries::append(x, y);
}

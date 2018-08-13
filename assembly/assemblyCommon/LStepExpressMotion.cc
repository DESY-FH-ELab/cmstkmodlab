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

#include <QApplication>

#include <nqlogger.h>

#include "LStepExpressMotion.h"

LStepExpressMotion::LStepExpressMotion(const std::vector<double> & values, bool mode)
    : mode_(mode),
      x_(values[0]),
      y_(values[1]),
      z_(values[2]),
      a_(values[3])
{
}

LStepExpressMotion::LStepExpressMotion(const double x, const double y, const double z, const double a, const bool mode)
    : mode_(mode),
      x_(x),
      y_(y),
      z_(z),
      a_(a)
{
}

LStepExpressMotion::LStepExpressMotion(const unsigned int axis, const double value, const bool mode)
    : mode_(mode),
      x_(0.0),
      y_(0.0),
      z_(0.0),
      a_(0.0)
{
    switch(axis)
    {
      case 0: x_ = value; break;
      case 1: y_ = value; break;
      case 2: z_ = value; break;
      case 3: a_ = value; break;
    }
}

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

#ifndef LSTEPEXPRESSMOTION_H
#define LSTEPEXPRESSMOTION_H

#include <vector>

#include "LStepExpressModel.h"

class LStepExpressMotion
{
public:

    LStepExpressMotion(std::vector<double> & values, bool mode);
    LStepExpressMotion(double x, double y, double z, double a, bool mode);
    LStepExpressMotion(unsigned int axis, double value, bool mode);

    bool getMode() const { return mode_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getZ() const { return z_; }
    double getA() const { return a_; }

protected:

    bool mode_;
    double x_, y_, z_, a_;
};

#endif // LSTEPEXPRESSMOTION_H

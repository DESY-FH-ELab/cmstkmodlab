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

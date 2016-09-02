#include <QApplication>

#include <nqlogger.h>

#include "LStepExpressMotion.h"

LStepExpressMotion::LStepExpressMotion(std::vector<double> & values, bool mode)
    : mode_(mode),
      x_(values[0]),
      y_(values[1]),
      z_(values[2]),
      a_(values[3])
{

}

LStepExpressMotion::LStepExpressMotion(double x, double y, double z, double a, bool mode)
    : mode_(mode),
      x_(x),
      y_(y),
      z_(z),
      a_(a)
{

}

LStepExpressMotion::LStepExpressMotion(unsigned int axis, double value, bool mode)
    : mode_(mode),
      x_(0.0),
      y_(0.0),
      z_(0.0),
      a_(0.0)
{
    switch (axis) {
    case 0: x_ = value; break;
    case 1: y_ = value; break;
    case 2: z_ = value; break;
    case 3: a_ = value; break;
    }
}

#include <QApplication>

#include <nqlogger.h>

#include "LStepExpressMotionManager.h"

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

LStepExpressMotionManager::LStepExpressMotionManager(LStepExpressModel* model, QObject *parent)
    : QObject(parent),
      model_(model),
      inMotion_(false)
{
    connect(model_, SIGNAL(motionStarted()),
            this, SLOT(motionStarted()));

    connect(model_, SIGNAL(motionFinished()),
            this, SLOT(motionFinished()));
}

void LStepExpressMotionManager::start()
{

}

void LStepExpressMotionManager::moveRelative(std::vector<double> & values)
{
    motions_.enqueue(LStepExpressMotion(values, false));
}

void LStepExpressMotionManager::moveRelative(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, false));
}

void LStepExpressMotionManager::moveRelative(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, false));
}

void LStepExpressMotionManager::moveAbsolute(std::vector<double> & values)
{
    motions_.enqueue(LStepExpressMotion(values, true));
}

void LStepExpressMotionManager::moveAbsolute(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, true));
}

void LStepExpressMotionManager::moveAbsolute(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, true));
}

void LStepExpressMotionManager::motionStarted()
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "motionStarted()";
    inMotion_ = true;
}

void LStepExpressMotionManager::motionFinished()
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "motionFinished()";
    inMotion_ = false;
}

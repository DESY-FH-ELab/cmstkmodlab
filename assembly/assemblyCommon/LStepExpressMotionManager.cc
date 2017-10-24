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

#include <LStepExpressMotionManager.h>
#include <nqlogger.h>
#include <Log.h>

LStepExpressMotionManager::LStepExpressMotionManager(LStepExpressModel* model, QObject *parent) :
  QObject(parent),
  model_(model),
  model_connected_(false),
  inMotion_(false)
{
    if(!model_)
    {
      Log::KILL("LStepExpressMotionManager::LStepExpressMotionManager -- pointer to LStepExpressModel initialized to NULL");
    }

    this->connect_model();
}

LStepExpressMotionManager::~LStepExpressMotionManager()
{
}

void LStepExpressMotionManager::connect_model()
{
    if(!model_){ return; }

    if(model_connected_ == false)
    {
      connect   (model_, SIGNAL(motionStarted()) , this, SLOT( start_motion()));
      connect   (model_, SIGNAL(motionFinished()), this, SLOT(finish_motion()));

      connect   (this  , SIGNAL(signalMoveAbsolute(double, double, double, double)),
                 model_, SLOT  (      moveAbsolute(double, double, double, double)));

      connect   (this  , SIGNAL(signalMoveRelative(double, double, double, double)),
                 model_, SLOT  (      moveRelative(double, double, double, double)));

      model_connected_ = true;
    }

    return;
}

void LStepExpressMotionManager::disconnect_model()
{
    if(!model_){ return; }

    if(model_connected_ == true)
    {
      disconnect(model_, SIGNAL(motionStarted()) , this, SLOT( start_motion()));
      disconnect(model_, SIGNAL(motionFinished()), this, SLOT(finish_motion()));

      disconnect(this  , SIGNAL(signalMoveAbsolute(double, double, double, double)),
                 model_, SLOT  (      moveAbsolute(double, double, double, double)));

      disconnect(this  , SIGNAL(signalMoveRelative(double, double, double, double)),
                 model_, SLOT  (      moveRelative(double, double, double, double)));

      model_connected_ = false;
    }

    return;
}

void LStepExpressMotionManager::myMoveToThread(QThread* thread)
{
    model_->moveToThread(thread);
    this  ->moveToThread(thread);
}

void LStepExpressMotionManager::run()
{
    if(inMotion_){ return; }

    if(motions_.empty())
    {
      emit motion_finished();
    }

    LStepExpressMotion motion = motions_.dequeue();

    inMotion_ = true;

    if(motion.getMode() == true)
    {
      NQLog("LStepExpressMotionManager::run") << "emitting signal \"signalMoveAbsolute("
        <<   "x=" << motion.getX()
        << ", y=" << motion.getY()
        << ", z=" << motion.getZ()
        << ", a=" << motion.getA()
        << ")\"";

      emit signalMoveAbsolute(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    }
    else
    {
      NQLog("LStepExpressMotionManager::run") << "emitting signal \"signalMoveRelative("
        <<   "x=" << motion.getX()
        << ", y=" << motion.getY()
        << ", z=" << motion.getZ()
        << ", a=" << motion.getA()
        << ")\"";

      emit signalMoveRelative(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    }

    return;
}

void LStepExpressMotionManager::appendMotion(const LStepExpressMotion& motion)
{
    motions_.enqueue(motion);
    run();
}

void LStepExpressMotionManager::appendMotions(const QQueue<LStepExpressMotion>& motions)
{
    motions_.append(motions);
    run();
}

void LStepExpressMotionManager::moveRelative(const std::vector<double>& values)
{
    motions_.enqueue(LStepExpressMotion(values, false));
    run();
}

void LStepExpressMotionManager::moveRelative(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, false));
    run();
}

void LStepExpressMotionManager::moveRelative(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, false));
    run();
}

void LStepExpressMotionManager::moveAbsolute(std::vector<double> & values)
{
    motions_.enqueue(LStepExpressMotion(values, true));
    run();
}

void LStepExpressMotionManager::moveAbsolute(double x, double y, double z, double a)
{
    NQLog("LStepExpressMotionManager") << "moveAbsolute";

    motions_.enqueue(LStepExpressMotion(x, y, z, a, true));

    NQLog("LStepExpressMotionManager") << "motionsEnquee x "<<  x  <<" y "<< y<<" z "<< z << " a "<< a;

    this->run();

    NQLog("LStepExpressMotionManager") << "run Done";
}

void LStepExpressMotionManager::moveAbsolute(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, true));
    this->run();
}

void LStepExpressMotionManager::start_motion()
{
    inMotion_ = true;
}

void LStepExpressMotionManager::finish_motion()
{
    inMotion_ = false;

    this->run();
}

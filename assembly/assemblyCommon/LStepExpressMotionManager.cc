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

LStepExpressMotionManager::LStepExpressMotionManager(LStepExpressModel* model, QObject *parent) :
  QObject(parent),
  model_(model),
  model_connected_(false),
  inMotion_(false)
{
    if(model_ == false)
    {
      NQLog("LStepExpressMotionManager", NQLog::Fatal) << "initialization error"
         << ": null pointer to LStepExpressModel object, exiting constructor";

      return;
    }

    this->connect_model();
}

LStepExpressMotionManager::~LStepExpressMotionManager()
{
}

void LStepExpressMotionManager::connect_model()
{
    if(model_ == false){ return; }

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
    if(model_ == false){ return; }

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

    if(motions_.empty()){ return; }

    const bool axes_ready = (
         this->AxisIsReady(0)
      && this->AxisIsReady(1)
      && this->AxisIsReady(2)
      && this->AxisIsReady(3)
    );

    if(axes_ready == false)
    {
      NQLog("LStepExpressMotionManager", NQLog::Critical) << "run"
         << ": at least one axis in invalid state, motion request rejected";

      this->finish_motion();

      return;
    }

    LStepExpressMotion motion = motions_.dequeue();

    inMotion_ = true;

    if(motion.getMode() == true)
    {
      NQLog("LStepExpressMotionManager", NQLog::Spam) << "run: emitting signal \"signalMoveAbsolute("
        <<   "x=" << motion.getX()
        << ", y=" << motion.getY()
        << ", z=" << motion.getZ()
        << ", a=" << motion.getA()
        << ")\"";

      emit signalMoveAbsolute(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    }
    else
    {
      NQLog("LStepExpressMotionManager", NQLog::Spam) << "run: emitting signal \"signalMoveAbsolute("
        <<   "x=" << motion.getX()
        << ", y=" << motion.getY()
        << ", z=" << motion.getZ()
        << ", a=" << motion.getA()
        << ")\"";

      emit signalMoveRelative(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    }

    return;
}

bool LStepExpressMotionManager::AxisIsReady(const int axis) const
{
  const bool axis_ready = (model_->getAxisStatusText(axis) == "@");

  if(axis_ready == false)
  {
    NQLog("LStepExpressMotionManager", NQLog::Critical) << "AxisIsReady"
       << ": invalid state (" << model_->getAxisStatusText(axis)
       << ") for axis #" << axis << " of motion stage, axis not ready";
  }

  return axis_ready;
}

void LStepExpressMotionManager::appendMotion(const LStepExpressMotion& motion)
{
    motions_.enqueue(motion);
    this->run();
}

void LStepExpressMotionManager::appendMotions(const QQueue<LStepExpressMotion>& motions)
{
    motions_.append(motions);
    this->run();
}

void LStepExpressMotionManager::moveRelative(const std::vector<double>& values)
{
    motions_.enqueue(LStepExpressMotion(values, false));
    this->run();
}

void LStepExpressMotionManager::moveRelative(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, false));
    this->run();
}

void LStepExpressMotionManager::moveRelative(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, false));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const std::vector<double>& values)
{
    motions_.enqueue(LStepExpressMotion(values, true));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, true));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, true));
    this->run();
}

void LStepExpressMotionManager::motionStarted()
{
    inMotion_ = true;
}

void LStepExpressMotionManager::motionFinished()
{
    inMotion_ = false;
    this->run();
}

void LStepExpressMotionManager::start_motion()
{
    this->motionStarted();
}

void LStepExpressMotionManager::finish_motion()
{
    inMotion_ = false;

    NQLog("LStepExpressMotionManager", NQLog::Spam) << "finish_motion"
       << ": emitting signal \"motion_finished\"";

    emit motion_finished();
}

void LStepExpressMotionManager::read_position3D()
{
  const double x = this->get_position_X();
  const double y = this->get_position_Y();
  const double z = this->get_position_Z();

  emit position3D(x, y, z);
}

void LStepExpressMotionManager::read_position4D()
{
  const double x = this->get_position_X();
  const double y = this->get_position_Y();
  const double z = this->get_position_Z();
  const double a = this->get_position_A();

  emit position4D(x, y, z, a);
}

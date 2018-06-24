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
    if(model_ == nullptr){ return; }

    if(model_connected_ == false)
    {
      connect   (model_, SIGNAL(motionStarted()) , this, SLOT(motionStarted()));
      connect   (model_, SIGNAL(motionFinished()), this, SLOT(motionFinished()));

      connect   (this  , SIGNAL(signalMoveAbsolute(double, double, double, double)),
                 model_, SLOT  (      moveAbsolute(double, double, double, double)));

      connect   (this  , SIGNAL(signalMoveRelative(double, double, double, double)),
                 model_, SLOT  (      moveRelative(double, double, double, double)));

      NQLog("LStepExpressMotionManager", NQLog::Spam) << "connect_model"
         << ": manager connected to LStepExpressModel";

      model_connected_ = true;
    }

    return;
}

void LStepExpressMotionManager::disconnect_model()
{
    if(model_ == false){ return; }

    if(model_connected_ == true)
    {
      disconnect(model_, SIGNAL(motionStarted()) , this, SLOT(motionStarted()));
      disconnect(model_, SIGNAL(motionFinished()), this, SLOT(motionFinished()));

      disconnect(this  , SIGNAL(signalMoveAbsolute(double, double, double, double)),
                 model_, SLOT  (      moveAbsolute(double, double, double, double)));

      disconnect(this  , SIGNAL(signalMoveRelative(double, double, double, double)),
                 model_, SLOT  (      moveRelative(double, double, double, double)));

      NQLog("LStepExpressMotionManager", NQLog::Spam) << "disconnect_model"
         << ": manager disconnected from LStepExpressModel";

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
      NQLog("LStepExpressMotionManager", NQLog::Spam) << "run"
         << ": emitting signal \"motion_finished\"";

      emit motion_finished();

      return;
    }

    // check on axes ----
    this->AxisIsReady(0);
    this->AxisIsReady(1);
    this->AxisIsReady(2);
    this->AxisIsReady(3);
    // ------------------

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
      NQLog("LStepExpressMotionManager", NQLog::Spam) << "run: emitting signal \"signalMoveRelative("
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
    NQLog("LStepExpressMotionManager", NQLog::Warning) << "AxisIsReady"
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

void LStepExpressMotionManager::read_position3D()
{
  const double x = this->get_position_X();
  const double y = this->get_position_Y();
  const double z = this->get_position_Z();

  NQLog("LStepExpressMotionManager", NQLog::Spam) << "read_position3D"
     << ": emitting signal \"position4D"
     << "(x=" << x << ", y=" << y << ", z=" << z << ")\"";

  emit position3D(x, y, z);
}

void LStepExpressMotionManager::read_position4D()
{
  const double x = this->get_position_X();
  const double y = this->get_position_Y();
  const double z = this->get_position_Z();
  const double a = this->get_position_A();

  NQLog("LStepExpressMotionManager", NQLog::Spam) << "read_position4D"
     << ": emitting signal \"position4D"
     << "(x=" << x << ", y=" << y << ", z=" << z << ", a=" << a << ")\"";

  emit position4D(x, y, z, a);
}

void LStepExpressMotionManager::clear_motion_queue()
{
  if(inMotion_)
  {
    NQLog("LStepExpressMotionManager", NQLog::Warning) << "clear_motion_queue"
       << ": motion stage is moving, no action taken";

    return;
  }

  motions_.clear();

  NQLog("LStepExpressMotionManager", NQLog::Message) << "clear_motion_queue"
     << ": motion queue cleared";

  return;
}

void LStepExpressMotionManager::emergency_stop()
{
  if(model_->getDeviceState() == READY)
  {
    model_->emergencyStop();
  }
  else
  {
    NQLog("LStepExpressMotionManager", NQLog::Warning) << "emergency_stop"
       << ": LStepExpressMotionManager not in READY state, no action taken";
  }

  return;
}

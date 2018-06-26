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
  use_smart_move_(false),
  inMotion_(false),
  timer_(nullptr)
{
    if(model_ == false)
    {
      NQLog("LStepExpressMotionManager", NQLog::Fatal) << "initialization error"
         << ": null pointer to LStepExpressModel object, exiting constructor";

      return;
    }

    timer_ = new QTimer(this);
    timer_->setSingleShot(true);

    this->connect_model();
}

LStepExpressMotionManager::~LStepExpressMotionManager()
{
}

void LStepExpressMotionManager::wait()
{
  if(timer_ != nullptr)
  {
    NQLog("LStepExpressMotionManager", NQLog::Spam) << "wait"
       << ": timer to time-out in " << motion_interval_sec_ << " msec";

    timer_->start(motion_interval_sec_);
  }
}

void LStepExpressMotionManager::connect_model()
{
    if(model_ == nullptr){ return; }

    if(model_connected_ == false)
    {
      connect   (model_, SIGNAL(motionStarted()) , this, SLOT(motionStarted()));

      connect   (model_, SIGNAL(motionFinished()), this, SLOT(wait()));
      connect   (timer_, SIGNAL(timeout()), this, SLOT(finish_motion()));

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

      disconnect(model_, SIGNAL(motionFinished()), this, SLOT(wait()));
      disconnect(timer_, SIGNAL(timeout()), this, SLOT(finish_motion()));

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

void LStepExpressMotionManager::moveRelative(const double dx, const double dy, const double dz, const double da)
{
    if(use_smart_move_)
    {
      if(dz > 0.)
      {
        motions_.enqueue(LStepExpressMotion( 0,  0, dz,  0, false));
        motions_.enqueue(LStepExpressMotion(dx, dy,  0, da, false));
      }
      else
      {
        motions_.enqueue(LStepExpressMotion(dx, dy,  0, da, false));

        std::vector<double> dz_vec;
        {
          double abs_dz_resid = fabs(dz);

          while((abs_dz_resid >  0.1) && (dz_vec.size() <  2)){ abs_dz_resid -=  0.1; dz_vec.emplace_back(- 0.1); }
          while((abs_dz_resid >  0.2) && (dz_vec.size() <  6)){ abs_dz_resid -=  0.2; dz_vec.emplace_back(- 0.2); }
          while((abs_dz_resid >  1.0) && (dz_vec.size() < 10)){ abs_dz_resid -=  1.0; dz_vec.emplace_back(- 1.0); }
          while((abs_dz_resid > 15.0) && (dz_vec.size() < 13)){ abs_dz_resid -= 15.0; dz_vec.emplace_back(-15.0); }

          if(abs_dz_resid > 0.){ dz_vec.emplace_back(-abs_dz_resid); }
        }

        // reversed loop on dz movements
        for(unsigned i_dz = dz_vec.size(); i_dz-- > 0; )
        {
          motions_.enqueue(LStepExpressMotion(0, 0, dz_vec.at(i_dz), 0, false));
        }
      }
    }
    else
    {
      motions_.enqueue(LStepExpressMotion(dx, dy, dz, da, false));
    }

    this->run();
}

void LStepExpressMotionManager::moveRelative(const unsigned int axis, const double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, false));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const std::vector<double>& values)
{
    motions_.enqueue(LStepExpressMotion(values, true));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const double x, const double y, const double z, const double a)
{
    const double dx = (x - this->get_position_X());
    const double dy = (y - this->get_position_Y());
    const double dz = (z - this->get_position_Z());
    const double da = (a - this->get_position_A());

    this->moveRelative(dx, dy, dz, da);

    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const unsigned int axis, const double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, true));
    this->run();
}

void LStepExpressMotionManager::motionStarted()
{
  inMotion_ = true;
}

void LStepExpressMotionManager::finish_motion()
{
  NQLog("LStepExpressMotionManager", NQLog::Spam) << "finish_motion"
     << ": setting \"inMotion=false\" and calling run() method";

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

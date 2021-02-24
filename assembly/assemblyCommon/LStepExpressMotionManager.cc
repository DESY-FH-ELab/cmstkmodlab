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
#include <AssemblyUtilities.h>

#include <ApplicationConfig.h>
#include <nqlogger.h>

#include <unistd.h>

LStepExpressMotionManager::LStepExpressMotionManager(LStepExpressModel* model, QObject* parent)
 : QObject(parent)
 , model_(model)
 , model_connected_(false)
 , inMotion_(false)
{
  qRegisterMetaType<LStepExpressMotion>("LStepExpressMotion");
  qRegisterMetaType<QQueue<LStepExpressMotion> >("QQueue<LStepExpressMotion>");

  ApplicationConfig* config = ApplicationConfig::instance(); //Access config file parameters
  if(not config)
  {
    NQLog("LStepExpressMotionManager", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[LStepExpressMotionManager]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
  }

  x_lowerBound_ = config->getValue<double>("MotionStageLowerBound_X", -150.);
  x_upperBound_ = config->getValue<double>("MotionStageUpperBound_X",  150.);
  y_lowerBound_ = config->getValue<double>("MotionStageLowerBound_Y", -150.);
  y_upperBound_ = config->getValue<double>("MotionStageUpperBound_Y",  150.);
  z_lowerBound_ = config->getValue<double>("MotionStageLowerBound_Z", -150.);
  z_upperBound_ = config->getValue<double>("MotionStageUpperBound_Z",  150.);
  a_lowerBound_ = config->getValue<double>("MotionStageLowerBound_A", -180.);
  a_upperBound_ = config->getValue<double>("MotionStageUpperBound_A",  180.);

  if(model_ == nullptr)
  {
    NQLog("LStepExpressMotionManager", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressModel object, exiting constructor";

    assembly::kill_application("[LStepExpressMotionManager]", "Invalid (NULL) pointer to LStepExpressModel object, closing application");
  }

  connect(this, SIGNAL(emergencyStop_request()), this, SLOT(emergency_stop()));

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
    connect(model_, SIGNAL(motionStarted()) , this, SLOT(motionStarted()));

    connect(model_, SIGNAL(motionFinished()), this, SLOT(finish_motion()));

    connect(this  , SIGNAL(signalMoveAbsolute(double, double, double, double)),
            model_, SLOT(moveAbsolute(double, double, double, double)));

    connect(this  , SIGNAL(signalMoveRelative(double, double, double, double)),
            model_, SLOT(moveRelative(double, double, double, double)));

    NQLog("LStepExpressMotionManager", NQLog::Spam) << "connect_model"
       << ": manager connected to LStepExpressModel";

    model_connected_ = true;
  }

  return;
}

void LStepExpressMotionManager::disconnect_model()
{
  if(model_ == nullptr){ return; }

  if(model_connected_ == true)
  {
    disconnect(model_, SIGNAL(motionStarted()) , this, SLOT(motionStarted()));

    disconnect(model_, SIGNAL(motionFinished()), this, SLOT(finish_motion()));

    disconnect(this  , SIGNAL(signalMoveAbsolute(double, double, double, double)),
               model_, SLOT(moveAbsolute(double, double, double, double)));

    disconnect(this  , SIGNAL(signalMoveRelative(double, double, double, double)),
               model_, SLOT(moveRelative(double, double, double, double)));

    NQLog("LStepExpressMotionManager", NQLog::Spam) << "disconnect_model"
       << ": manager disconnected from LStepExpressModel";

    model_connected_ = false;
  }

  return;
}

void LStepExpressMotionManager::myMoveToThread(QThread* thread)
{
    model_->moveToThread(thread);
    this->moveToThread(thread);
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
    if(values.size() != 4){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, vector of X/Y/Z/A distances has invalid size (" << values.size() << " != 4)";
      return;
    }

    //Checks on MS boundaries
    const double x1(this->get_position_X() + values[0]);
    if(x1 < x_lowerBound_ || x1 > x_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in X:" << x1;
      return;
    }

    const double y1(this->get_position_Y() + values[1]);
    if(y1 < y_lowerBound_ || y1 > y_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in Y:" << y1;
      return;
    }

    const double z1(this->get_position_Z() + values[2]);
    if(z1 < z_lowerBound_ || z1 > z_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in Z:" << z1;
      return;
    }

    const double a1(this->get_position_A() + values[3]);
    if(a1 < a_lowerBound_ || a1 > a_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in A:" << a1;
      return;
    }

    //Queue movement
    motions_.enqueue(LStepExpressMotion(values, false));
    this->run();
}

void LStepExpressMotionManager::moveRelative(const double dx, const double dy, const double dz, const double da)
{
    //Checks on MS boundaries
    const double x1(this->get_position_X() + dx);
    if(x1 < x_lowerBound_ || x1 > x_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in X:" << x1;
      return;
    }

    const double y1(this->get_position_Y() + dy);
    if(y1 < y_lowerBound_ || y1 > y_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in Y:" << y1;
      return;
    }

    const double z1(this->get_position_Z() + dz);
    if(z1 < z_lowerBound_ || z1 > z_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in Z:" << z1;
      return;
    }

    const double a1(this->get_position_A() + da);
    if(a1 < a_lowerBound_ || a1 > a_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in A:" << a1;
      return;
    }

    //Queue movement
    motions_.enqueue(LStepExpressMotion(dx, dy, dz, da, false));
    this->run();
}

void LStepExpressMotionManager::moveRelative(const unsigned int axis, const double value)
{
    //Checks on MS boundaries
    if(axis == 0){
      const double x1(this->get_position_X() + value);
      if(x1 < x_lowerBound_ || x1 > x_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in X:" << x1;
        return;
      }
    }
    else if(axis == 1){
      const double y1(this->get_position_Y() + value);
      if(y1 < y_lowerBound_ || y1 > y_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in Y:" << y1;
        return;
      }
    }
    else if(axis == 2){
      const double z1(this->get_position_Z() + value);
      if(z1 < z_lowerBound_ || z1 > z_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in Z:" << z1;
        return;
      }
    }
    else if(axis == 3){
      const double a1(this->get_position_A() + value);
      if(a1 < a_lowerBound_ || a1 > a_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, Motion Stage would exceed its boundary in A:" << a1;
        return;
      }
    }
    else {
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Relative movement not executed, invalid axis index:" << axis;
      return;
    }

    //Queue movement
    motions_.enqueue(LStepExpressMotion(axis, value, false));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const std::vector<double>& values)
{
    if(values.size() != 4){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, vector of X/Y/Z/A positions has invalid size (" << values.size() << " != 4)";
      return;
    }

    //Checks on MS boundaries
    const double x1(values[0]);
    if(x1 < x_lowerBound_ || x1 > x_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in X:" << x1;
      return;
    }

    const double y1(values[1]);
    if(y1 < y_lowerBound_ || y1 > y_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in Y:" << y1;
      return;
    }

    const double z1(values[2]);
    if(z1 < z_lowerBound_ || z1 > z_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in Z:" << z1;
      return;
    }

    const double a1(values[3]);
    if(a1 < a_lowerBound_ || a1 > a_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in A:" << a1;
      return;
    }

    //Queue movement
    motions_.enqueue(LStepExpressMotion(values, true));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const double x, const double y, const double z, const double a)
{
    //Checks on MS boundaries
    if(x < x_lowerBound_ || x > x_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in X:" << x;
      return;
    }

    if(y < y_lowerBound_ || y > y_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in Y:" << y;
      return;
    }

    if(z < z_lowerBound_ || z > z_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in Z:" << z;
      return;
    }

    if(a < a_lowerBound_ || a > a_upperBound_){
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in A:" << a;
      return;
    }

    //Queue movement
    motions_.enqueue(LStepExpressMotion(x, y, z, a, true));
    this->run();
}

void LStepExpressMotionManager::moveAbsolute(const unsigned int axis, const double value)
{
    //Checks on MS boundaries
    if(axis == 0){
      if(value < x_lowerBound_ || value > x_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in X:" << value;
        return;
      }
    }
    else if(axis == 1){
      if(value < y_lowerBound_ || value > y_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in Y:" << value;
        return;
      }
    }
    else if(axis == 2){
      if(value < z_lowerBound_ || value > z_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in Z:" << value;
        return;
      }
    }
    else if(axis == 3){
      if(value < a_lowerBound_ || value > a_upperBound_){
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage would exceed its boundary in A:" << value;
        return;
      }
    }
    else {
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, invalid axis index:" << axis;
      return;
    }

    //Queue movement
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

  const auto mots = motions_;

  motions_.clear();

  for(const auto& i_mot : mots)
  {
    NQLog("LStepExpressMotionManager", NQLog::Spam) << "clear_motion_queue"
       << ": removed LStepExpressMotion"
       << " (x=" << i_mot.getX()
       << ", y=" << i_mot.getY()
       << ", z=" << i_mot.getZ()
       << ", a=" << i_mot.getA()
       << ")[absolute=" << i_mot.getMode() << "]";
  }

  NQLog("LStepExpressMotionManager", NQLog::Message) << "clear_motion_queue"
     << ": motion queue cleared (removed " << mots.size() << " motions)";

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

double LStepExpressMotionManager::get_position(const int axis) const
{
  //
  // position validation:
  //  * read position only after ensuring model is not updating internally,
  //    the motion stage is not moving and the list of positions has a valid format
  //
  // output:
  //  * it is not safe to return if any of the conditions above is not satisfied
  //  * this means the 'while' shoud not be broken or shortcut (a priori, it might never finish)
  //  * after a certain number of attempts, explicit warnings are printed
  //
  uint tries(0);

  while(model()->isUpdating() || model()->isInMotion() || inMotion_ || (this->model()->getPositions().size() != 4))
  {
    ++tries;

    if(tries > 10)
    {
      NQLog("LStepExpressMotionManager", NQLog::Warning) << "get_position(" << axis << ")"
         << ": after try #" << tries << ", call for LStepExpressMotionManager::emergency_stop()";

      NQLog("LStepExpressMotionManager", NQLog::Spam) << "get_position(" << axis << ")"
         << ": emitting signal \"emergencyStop_request\"";

      emit emergencyStop_request();
    }
    else if(tries > 5)
    {
      if(model()->isUpdating())
      {
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "get_position(" << axis << ")"
           << ": cannot return motion stage position [try #" << tries << "] because LStepExpressModel is still updating";
      }
      else if(model()->isInMotion())
      {
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "get_position(" << axis << ")"
           << ": cannot return motion stage position [try #" << tries << "] because LStepExpressModel is still in motion";
      }
      else if(inMotion_)
      {
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "get_position(" << axis << ")"
           << ": cannot return motion stage position [try #" << tries << "] because LStepExpressMotionManager is still in motion";
      }
      else if(this->model()->getPositions().size() != 4)
      {
    	//FIXME -- debugging recurrent MS instability (returning incorrect pos vector) //Related to angle... ?
    	std::cout<<"this->model()->getPositions().size() = "<<this->model()->getPositions().size()<<std::endl;
    	for(int i=0; i<this->model()->getPositions().size(); i++)
    	{
            std::cout<<"this->model()->getPositions().at("<<i<<") = "<<this->model()->getPositions().at(i)<<std::endl;
        }
        //Additional debug messages -- other vectors still have correct size ?
        for(int i=0; i<4; i++)
    	{
            // check axes status
            if(this->AxisIsReady(i)) {continue;}

            std::cout<<"this->model()->getVelocity("<<i<<") = "<<this->model()->getVelocity(i)<<std::endl;
            std::cout<<"this->model()->getAccelerationJerk("<<i<<") = "<<this->model()->getAccelerationJerk(i)<<std::endl;
            std::cout<<"this->model()->getDecelerationJerk("<<i<<") = "<<this->model()->getDecelerationJerk(i)<<std::endl;
            std::cout<<"this->model()->getAcceleration("<<i<<") = "<<this->model()->getAcceleration(i)<<std::endl;
            std::cout<<"this->model()->getDeceleration("<<i<<") = "<<this->model()->getDeceleration(i)<<std::endl;
            std::cout<<"this->model()->getPosition("<<i<<") = "<<this->model()->getPosition(i)<<std::endl;
        }

        NQLog("LStepExpressMotionManager", NQLog::Warning) << "get_position(" << axis << ")"
           << ": cannot return motion stage position [try #" << tries << "] because positions vector has invalid size ("
           << this->model()->getPositions().size() << ")";

        for(int iaxis=0; iaxis<this->model()->getPositions().size(); iaxis++) //Additional printout info: list all available/correct positions
        {
            NQLog("LStepExpressMotionManager", NQLog::Warning) << "Pos: " << this->model()->getPosition(iaxis);
        }

      }
    }

    usleep(model()->updateInterval() * 400); // from ms to us, times 0.4
  }

  return this->model()->getPosition(axis);
}

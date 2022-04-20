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

  x_lowerBound_ = config->getDefaultValue<double>("main", "MotionStageLowerBound_X", -150.);
  x_upperBound_ = config->getDefaultValue<double>("main", "MotionStageUpperBound_X",  150.);
  y_lowerBound_ = config->getDefaultValue<double>("main", "MotionStageLowerBound_Y", -150.);
  y_upperBound_ = config->getDefaultValue<double>("main", "MotionStageUpperBound_Y",  150.);
  z_lowerBound_ = config->getDefaultValue<double>("main", "MotionStageLowerBound_Z", -150.);
  z_upperBound_ = config->getDefaultValue<double>("main", "MotionStageUpperBound_Z",  150.);
  a_lowerBound_ = config->getDefaultValue<double>("main", "MotionStageLowerBound_A", -180.);
  a_upperBound_ = config->getDefaultValue<double>("main", "MotionStageUpperBound_A",  180.);

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

    LStepExpressMotion motion = motions_.dequeue(); //Returns first (head) movement from the queue

    inMotion_ = true;

    if(motion.getMode() == true)
    {
      NQLog("LStepExpressMotionManager", NQLog::Spam) << "run: emitting signal \"signalDeactivate()";
      emit signalDeactivate();
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
      
      NQLog("LStepExpressMotionManager", NQLog::Spam) << "run: emitting signal \"signalDeactivate()";
      emit signalDeactivate();
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

    //Queue movement //Changed
    //motions_.enqueue(LStepExpressMotion(values, false));

    //-- Set priorities for XYA and Z movements
    this->set_movements_priorities_XYZA(values[0], values[1], values[2], values[3], false);

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

    //Queue movement //Changed
    //motions_.enqueue(LStepExpressMotion(dx, dy, dz, da, false));

    //-- Set priorities for XYA and Z movements
    this->set_movements_priorities_XYZA(dx, dy, dz, da, false);

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

    //Queue movement //Changed
    //motions_.enqueue(LStepExpressMotion(values, true));

    //-- Set priorities for XYA and Z movements
    this->set_movements_priorities_XYZA(values[0], values[1], values[2], values[3], true);

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

    //Queue movement //Changed
    //motions_.enqueue(LStepExpressMotion(x, y, z, a, true));

    //-- Set priorities for XYA and Z movements
    this->set_movements_priorities_XYZA(x, y, z, a, true);

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
  
  emit signalReactivate();
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
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "get_position(" << axis << ")"
           << ": cannot return motion stage position [try #" << tries << "] because positions vector has invalid size ("
           << this->model()->getPositions().size() << ")";

        for(unsigned int iaxis=0; iaxis<this->model()->getPositions().size(); iaxis++) //Additional printout info: list all available/correct positions
        {
            NQLog("LStepExpressMotionManager", NQLog::Warning) << "Pos: " << this->model()->getPosition(iaxis);
        }

        //-- Recurrent MS instability (returning incorrect pos vector) //Seems like the angle does not get returned, why and can it be fixed ?
        //-- Whenever the "positions vector has invalid size" error appears and the MS stops responding, the MS should be restarted (manually via the 'Restart Motion Stage' button in the 'HW Controllers' tab, or done automatically here)
        emit restartMotionStage_request();
      }
    }

    usleep(model()->updateInterval() * 400); // from ms to us, times 0.4
  }

  return this->model()->getPosition(axis);
}


//-- Function inspired from AssemblySmartMotionManager::smartMotions_relative()
//-- Prioritize XYA and Z movements depending on their directions, to avoid crashes of the motion stage
void LStepExpressMotionManager::set_movements_priorities_XYZA(const double x, const double y, const double z, const double a, const bool is_absolute_movements)
{
    double dx = x;
    double dy = y;
    double dz = z;
    double da = a;

    if(inMotion_ && is_absolute_movements)
    {
        NQLog("LStepExpressMotionManager", NQLog::Warning) << "ERROR ! Absolute movement not executed, Motion Stage is still moving - calculation of smart relative motion not possible.";
        return;

    }
    else if(is_absolute_movements)
    {
	//-- Convert absolute to relative movements //Simpler to use a single convention (because e.g. moving to "0" has different meaning in abs/rel)
        dx = (x - this->get_position_X());
        dy = (y - this->get_position_Y());
        dz = (z - this->get_position_Z());
        da = (a - this->get_position_A());
    }

    const bool move_xya = ((std::fabs(dx) > std::numeric_limits<double>::epsilon()) || (std::fabs(dy) > std::numeric_limits<double>::epsilon()) || (std::fabs(da) > std::numeric_limits<double>::epsilon()));
    const bool move_z = ((std::fabs(dz) > std::numeric_limits<double>::epsilon()));

    NQLog("LStepExpressMotionManager", NQLog::Message) << "Requested relative motion in xya: " << (move_xya ? "yes" : "no") << "  - Motion in z: " << (move_z ? "yes" : "no");
        
    
    if(!move_xya && !move_z) {return;} //No movement
    else if(move_xya && !move_z) {motions_.enqueue(LStepExpressMotion(dx, dy, 0, da, false));} //Only XYA movement
    else if(!move_xya && move_z) {motions_.enqueue(LStepExpressMotion(0, 0, dz, 0, false));} //Only Z movement
    else if(dz > 0.) //Positive z-movement <-> apply first
    {
      motions_.enqueue(LStepExpressMotion(0, 0, dz, 0, false));
      motions_.enqueue(LStepExpressMotion(dx, dy, 0, da, false));
    }
    else if(dz < 0.) //Negative z-movement <-> apply second
    {
      motions_.enqueue(LStepExpressMotion(dx, dy, 0, da, false));
      motions_.enqueue(LStepExpressMotion(0, 0, dz, 0, false));
    }

    return;
}

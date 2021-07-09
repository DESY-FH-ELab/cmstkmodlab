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

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//            Fourth Vacuum Line Capability Added by Elise Hinkle              //
//                       Last Modified October 7, 2019                         //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AlignmentCheck.h>
#include <AssemblyUtilities.h>

#include <string>
#include <QMessageBox>

#include <AssemblyObjectAligner.h>
#include <AssemblyParameters.h>
#include <AssemblyUtilities.h>

#include <cmath>



// AssemblyAssembly::AssemblyAssembly(const LStepExpressMotionManager* const motion, const ConradManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent) // CONRAD
AlignmentCheck::AlignmentCheck(const LStepExpressMotionManager* const motion, const VellemanManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent) // VELLEMAN

  : QObject(parent)

 , motion_(motion)
 , vacuum_(vacuum)
 , smart_motion_(smart_motion)

 , vacuum_pickup_(0)
 , vacuum_spacer_(0)
 , vacuum_basepl_(0)
 , vacuum_stage_(0)

 , pickup1_Z_(0.)
 , pickup2_Z_(0.)

 , use_smartMove_(false)

 , PSPToPSSPosition_isRegistered_(false)
 , PSPToPSSPosition_X_(0.)
 , PSPToPSSPosition_Y_(0.)
 , PSPToPSSPosition_Z_(0.)
 , PSPToPSSPosition_A_(0.)
{
  // validate pointers to controllers
  this->motion();
  this->vacuum();

  // indices of vacuum lines
  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
  }

  vacuum_pickup_ = config->getValue<int>("Vacuum_PickupTool");
  vacuum_spacer_ = config->getValue<int>("Vacuum_Spacers");
  vacuum_basepl_ = config->getValue<int>("Vacuum_Baseplate");
  vacuum_stage_ = config->getValue<int>("Vacuum_Stage");

  // absolute Z-position of motion stage for pickup of object after gluing
  // (1: PSs to Spacers, 2: PSs+Spacers to PSp)
  pickup1_Z_ = config->getValue<double>("AssemblyAssembly_pickup1_Z");
  pickup2_Z_ = config->getValue<double>("AssemblyAssembly_pickup2_Z");
}

const LStepExpressMotionManager* AlignmentCheck::motion() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "motion"
       << ": pointer to LStepExpressMotionManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("pointer to LStepExpressMotionManager is NULL, aborting"));
  }

  return motion_;
}

// *****LEAVE METHOD UNCOMMENTED FOR USE WITH CONRAD RELAY CARD***** // 
/* const ConradManager* AssemblyAssembly::vacuum() const
{
  if(vacuum_ == nullptr) // NOTE: CHANGED FROM motion_ TO vacuum_
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "vacuum"
       << ": pointer to ConradManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("pointer to ConradManager is NULL, aborting"));
  }

  return vacuum_;
}
*/

// *****LEAVE METHOD UNCOMMENTED FOR USE WITH VELLEMAN RELAY CARD***** // 
/**/
const VellemanManager* AlignmentCheck::vacuum() const
{
  if(vacuum_ == nullptr) // THIS WAS ORIGINALLY MOTION_ BUT I THINK THAT MAY HAVE BEEN A COPY/PASTE ERROR ABOVE
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "vacuum"
       << ": pointer to VellemanManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("pointer to VellemanManager is NULL, aborting"));
  }

  return vacuum_;
}
/**/

AssemblyParameters* AlignmentCheck::parameters() const
{
  AssemblyParameters* const params = AssemblyParameters::instance();

  if(params == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "parameters"
       << ": pointer to AssemblyParameters is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("pointer to AssemblyParameters is NULL, aborting"));
  }

  return params;
}

void AlignmentCheck::use_smartMove(const int state)
{
  if(state == 2)
  {
    if(smart_motion_ == nullptr)
    {
      NQLog("AssemblyAssembly", NQLog::Warning) << "use_smartMove(" << state << ")"
         << ": attempting to enable \"smartMove\" mode, but pointer to AssemblySmartMotionManager is invalid (NULL), \"smartMove\" mode will stay disabled";

      use_smartMove_ = false;
    }
    else
    {
      NQLog("AssemblyAssembly", NQLog::Message) << "use_smartMove(" << state << ")"
         << ": \"smartMove\" mode switched ON";

      use_smartMove_ = true;
    }
  }
  else if(state == 0)
  {
    NQLog("AssemblyAssembly", NQLog::Message) << "use_smartMove(" << state << ")"
       << ": \"smartMove\" mode switched OFF";

    use_smartMove_ = false;
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Warning) << "use_smartMove(" << state << ")"
       << ": invalid argument (state=" << state << "), no action taken (smartMove=" << use_smartMove_ << ")";
  }

  return;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToSensorMarkerPreAlignment -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AlignmentCheck::GoToSensorMarkerPreAlignment_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoToSensorMarkerPreAlignment_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoToSensorMarkerPreAlignment_finish"
       << ": emitting signal \"GoToSensorMarkerPreAlignment_finished\"";

    emit GoToSensorMarkerPreAlignment_finished();

    return;
  }

  const double x0 = this->parameters()->get("RefPointSensor_X");
  const double y0 = this->parameters()->get("RefPointSensor_Y");
  const double z0 = this->parameters()->get("RefPointSensor_Z");
  const double a0 = this->parameters()->get("RefPointSensor_A");

  connect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToSensorMarkerPreAlignment_start"
     << ": emitting signal \"move_absolute_request(" << x0 << ", " << y0 << ", " << z0 << ", " << a0 << ")\"";

  emit move_absolute_request(x0, y0, z0, a0);
}

void AlignmentCheck::GoToSensorMarkerPreAlignment_finish()
{
  disconnect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToSensorMarkerPreAlignment_finish"
     << ": emitting signal \"GoToSensorMarkerPreAlignment_finished\"";

  emit GoToSensorMarkerPreAlignment_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoToSensorMarkerPreAlignment_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
// GoFromSensorMarkerToPickupXY ---> GoFromSensorMarkertoPSSLL -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AlignmentCheck::GoFromSensorMarkerToPSSLL_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromSensorMarkerToPickupXY_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_finish"
       << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

    emit GoFromSensorMarkerToPSSLL_finished();

    return;
  }

  const double dx0 = this->parameters()->get("FromPSPRefPointToPSSRefPoint_dX");
  const double dy0 = 0.0;
  const double dz0 = this->parameters()->get("FromPSPRefPointToPSSRefPoint_dZ");
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPSSLL_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPSSLL__start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AlignmentCheck::GoFromSensorMarkerToPSSLL_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPSSLL_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPSSLL_finish"
     << ": emitting signal \"GoFromSensorMarkerToPSSLL_finished\"";

  emit GoFromSensorMarkerToPSSLL_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromSensorMarkerToPSSLL_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------


// GoFromPSSLLToPSSTL -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AlignmentCheck::GoFromPSSLLToPSSTL_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromPSSLLToPSSTL_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSLLTOPSSTL_finish"
       << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

    emit GoFromPSSLLToPSSTL_finished();

    return;
  }

  const double dx0 = this->parameters()->get("FromPSSRefPointToPSSTLRefPoint_dX");
  const double dy0 = 0.0;
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromPSSLLToPSSTL_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSLLToPSSTL_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AlignmentCheck::GoFromPSSLLToPSSTL_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromPSSLLToPSSTL_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromToPSSLLToPSSTL_finish"
     << ": emitting signal \"GoFromPSSLLToPSSTL_finished\"";

  emit GoFromPSSLLToPSSTL_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromPSSLLToPSSTL_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------


// GoFromPSSTLToPSPTL -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AlignmentCheck::GoFromPSSTLToPSPTL_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromPSSTLToPSPTL_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSTLToPSPTL_finish"
       << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

    emit GoFromPSSTLToPSPTL_finished();

    return;
  }

  const double dx0 = this->parameters()->get("FromPSSTLToPSPTL_dX");
  const double dy0 = 0.0;
  const double dz0 = this->parameters()->get("FromPSSTLToPSPTL_dZ");
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromPSSTLToPSPTL_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSTLToPSPTL_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AlignmentCheck::GoFromPSSTLToPSPTL_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromPSSTLToPSPTL_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromToPSTLLToPSPTL_finish"
     << ": emitting signal \"GoFromPSSTLToPSPTL_finished\"";

  emit GoFromPSSTLToPSPTL_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromPSSTLToPSPTL_finish"
     << ": assembly-step completed";
}

// ----------------------------------------------------------------------------------------------------
// RegisterPSPToPSSPosition ---------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AlignmentCheck::RegisterPSPToPSSPosition_start()
{
  PSPToPSSPosition_X_ = motion_->get_position_X();
  PSPToPSSPosition_Y_ = motion_->get_position_Y();
  PSPToPSSPosition_Z_ = motion_->get_position_Z();
  PSPToPSSPosition_A_ = motion_->get_position_A();

  PSPToPSSPosition_isRegistered_ = true;

  NQLog("AssemblyAssembly", NQLog::Message) << "RegisterPSPToPSSPosition_start"
     << ": registered position (x=" << PSPToPSSPosition_X_ << ", y=" << PSPToPSSPosition_Y_ << ", z=" << PSPToPSSPosition_Z_ << ", a=" << PSPToPSSPosition_A_ << ")";

  connect(this, SIGNAL(PSPToPSSPosition_registered()), this, SLOT(RegisterPSPToPSSPosition_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "RegisterPSPToPSSPosition_start"
     << ": emitting signal \"PSPToPSSPosition_registered\"";

  emit PSPToPSSPosition_Registered();
}

void AlignmentCheck::RegisterPSPToPSSPosition_finish()
{
  disconnect(this, SIGNAL(PSPToPSSPosition_registered()), this, SLOT(RegisterPSPToPSSPosition_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "RegisterPSPToPSSPosition_finish"
     << ": emitting signal \"RegisterPSPToPSSPosition_finished\"";

  emit RegisterPSPToPSSPosition_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "RegisterPSPToPSSPosition_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// --------------------------- Image processing stuff below this line ---------------------------------

// ----------------------------------------------------------------------------------------------------



AlignmentCheck::AlignmentCheck(const LStepExpressMotionManager* const motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyObjectAligner", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    assembly::kill_application(tr("[AssemblyObjectAligner]"), tr("Null pointer to LStepExpressMotionManager. Aborting Application.")\
);
  }

  qRegisterMetaType<AlignmentCheck::Configuration>("AlignmentCheck::Configuration");

  configuration_.reset();

  this->reset();

  connect(this, SIGNAL(nextAlignmentStep(double, double, double)), this, SLOT(run_alignment(double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_alignment_step()));

  NQLog("AlignmentCheck", NQLog::Debug) << "constructed";
}

void AlignmentCheck::Configuration::reset()
{
  object_deltaX = -999.;
  object_deltaY = -999.;

  target_angle = -999.;

  only_measure_angle = true;

  complete_at_position1 = true;

  use_autofocusing = true;

  angle_max_dontIter = -999.;
  angle_max_complete = -999.;

  PatRecOne_configuration.reset();
  PatRecTwo_configuration.reset();
}

bool AlignmentCheck::Configuration::is_valid() const
{
  if(object_deltaX == -999.){ return false; }
  if(object_deltaY == -999.){ return false; }

  if(only_measure_angle == false)
  {
    if(fabs(target_angle) > 180.){ return false; }

    if(angle_max_dontIter <= 0.)
    {
      return false;
    }
    else if(angle_max_dontIter > 1.0)
    {
      NQLog("AlignmentCheck::Configuration", NQLog::Warning) << "is_valid"
         << ": unexpected value for max. angle for 1-rotation (" << angle_max_dontIter << ")";
    }

    if(angle_max_complete <= 0.)
    {
      return false;
    }
    else if(angle_max_complete > 0.1)
    {
      NQLog("AlignmentCheck::Configuration", NQLog::Warning) << "is_valid"
         << ": unexpected value for max. angle to validate alignment (" << angle_max_complete << ")";
    }
  }

  if(PatRecOne_configuration.is_valid() == false){ return false; }
  if(PatRecTwo_configuration.is_valid() == false){ return false; }

  return true;
}

void AlignmentCheck::reset()
{
  alignment_step_ = 0;

  posi_x1_ = 0.;
  posi_y1_ = 0.;
  posi_x2_ = 0.;
  posi_y2_ = 0.;
  posi_x3_ = 0.;
  posi_y3_ = 0.;
  posi_x4_ = 0.;
  posi_y4_ = 0.;

  obj_angle_deg_ = 0.;

  return;
}

void AlignmentCheck::update_configuration(const AlignmentCheck::Configuration& conf)
{
  if(conf.is_valid() == false)
  {
    NQLog("AlignmentCheck", NQLog::Critical) << "update_configuration"
       << ": invalid input AlignmentCheck::Configuration object, no action taken";

    return;
  }

  configuration_ = conf;

  NQLog("AlignmentCheck", NQLog::Spam) << "update_configuration"
     << ": emitting signal \"configuration_updated\"";

  emit configuration_updated();
}


void AlignmentCheck::redirect_image(const cv::Mat& img)
{
  if(alignment_step_ == 2)
  {
    NQLog("AlignmentCheck", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecOne\"";

    emit image_PatRecOne(img);
  }
  else if(alignment_step_ == 5)
  {
    NQLog("AlignmentCheck", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecTwo\"";

    emit image_PatRecTwo(img);
  }

  return;
}

void AlignmentCheck::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AlignmentCheck", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AlignmentCheck already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, \
double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = true;

    NQLog("AlignmentCheck", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AlignmentCheck enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, doubl\
e, double, double)));

    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = false;

    NQLog("AlignmentCheck", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for Alignmentdisabled";
  }

  return;
}

void AlignmentCheck::move_relative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative_request(x, y, z, a);
}

void AlignmentCheck::complete_motion()
{
  this->disconnect_motion_manager();

  NQLog("AlignmentCheck", NQLog::Spam) << "complete_motion"
     << ": emitting signal \"motion_completed\"";

  emit motion_completed();
}

void AlignmentCheck::launch_next_alignment_step()
{
  NQLog("AlignmentCheck", NQLog::Spam) << "launch_next_alignment_step"
     << ": emitting signal \"nextAlignmentStep(0, 0, 0)\"";

  emit nextAlignmentStep(0.0, 0.0, 0.0);
}

void AlignmentCheck::execute()
{
  alignment_step_ = 0;

  this->run_alignment(0., 0., 0.);
}

void AlignmentCheck::run_alignment(const double patrec_dX, const double patrec_dY, const double patrec_angle)
{
  
  // Step #0: request image on marker-1                                                                                             
  if(alignment_step_ == 0)
  {
    NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    if(this->configuration().use_autofocusing)
    {
      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++alignment_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++alignment_step_;

      emit image_request();
    }
  }
  // Step #1: PatRec on marker-1                                                                                                     
  else if(alignment_step_ == 1)
    {
    NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++alignment_step_;

    emit PatRec_request(this->configuration().PatRecOne_configuration);
  }

  // Step #2: Report back the x,y position of the image
  else if(alignment_step_ == 2)
    {
    // marker-1: position of PatRec best-match                                                                                       
    posi_x1_ = motion_manager_->get_position_X() + patrec_dX;
    posi_y1_ = motion_manager_->get_position_Y() + patrec_dY;
    const double dX = this->parameters()->get("");
    const double dY = this->parameters()->get("")

    ++alignment_step_;

    this->move_relative(dX, dY, 0.0, 0.0);
      
    }  



}
























}

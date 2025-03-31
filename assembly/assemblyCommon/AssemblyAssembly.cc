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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyAssembly.h>
#include <AssemblyUtilities.h>

#include <string>

#include <QMessageBox>

AssemblyAssembly::AssemblyAssembly(const LStepExpressMotionManager* const motion, const RelayCardManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent)
 : QObject(parent)

 , motion_(motion)
 , vacuum_(vacuum)
 , smart_motion_(smart_motion)
 , config_(nullptr)

 , vacuum_pickup_(0)
 , vacuum_spacer_(0)
 , vacuum_basepl_(0)
 , vacuum_sub_(0)

 , pickup1_Z_(0.)
 , pickup2_Z_(0.)

 , use_smartMove_(false)
 , in_action_(false)

 , PSSPlusSpacersToMaPSAPosition_isRegistered_(false)
 , PSSPlusSpacersToMaPSAPosition_X_(0.)
 , PSSPlusSpacersToMaPSAPosition_Y_(0.)
 , PSSPlusSpacersToMaPSAPosition_Z_(0.)
 , PSSPlusSpacersToMaPSAPosition_A_(0.)
{
  // validate pointers to controllers
  this->motion();
  this->vacuum();

  // indices of vacuum lines
  config_ = ApplicationConfig::instance();
  if(config_ == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
  }

  vacuum_pickup_ = config_->getValue<int>("main", "Vacuum_PickupTool");
  vacuum_spacer_ = config_->getValue<int>("main", "Vacuum_Spacers");
  vacuum_basepl_ = config_->getValue<int>("main", "Vacuum_Baseplate");
  vacuum_sub_ = config_->getValue<int>("main", "Vacuum_Subassembly");

  // absolute Z-position of motion stage for pickup of object after gluing
  // (1: PSs to Spacers, 2: PSs+Spacers to MaPSA)
  pickup1_Z_ = config_->getValue<double>("main", "AssemblyAssembly_pickup1_Z");
  pickup2_Z_ = config_->getValue<double>("main", "AssemblyAssembly_pickup2_Z");
}

const LStepExpressMotionManager* AssemblyAssembly::motion() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "motion"
       << ": pointer to LStepExpressMotionManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("pointer to LStepExpressMotionManager is NULL, aborting"));
  }

  return motion_;
}

const RelayCardManager* AssemblyAssembly::vacuum() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "vacuum"
       << ": pointer to RelayCardManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblyAssembly]"), tr("pointer to RelayCardManager is NULL, aborting"));
  }

  return vacuum_;
}

void AssemblyAssembly::use_smartMove(const int state)
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
void AssemblyAssembly::GoToSensorMarkerPreAlignment_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "GoToSensorMarkerPreAlignment_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double x0 = config_->getValue<double>("parameters", "RefPointSensor_X");
  const double y0 = config_->getValue<double>("parameters", "RefPointSensor_Y");
  const double z0 = config_->getValue<double>("parameters", "RefPointSensor_Z");
  const double a0 = config_->getValue<double>("parameters", "RefPointSensor_A");

  connect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToSensorMarkerPreAlignment_start"
     << ": emitting signal \"move_absolute_request(" << x0 << ", " << y0 << ", " << z0 << ", " << a0 << ")\"";

  emit move_absolute_request(x0, y0, z0, a0);
}

void AssemblyAssembly::GoToSensorMarkerPreAlignment_finish()
{
  disconnect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToSensorMarkerPreAlignment_finish"
     << ": emitting signal \"GoToSensorMarkerPreAlignment_finished\"";

  emit GoToSensorMarkerPreAlignment_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoToSensorMarkerPreAlignment_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to sensor's reference marker (pre-alignment)]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumPickupTool -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "EnableVacuumPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumPickupTool_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_pickup_ << ")\"";

  emit vacuum_ON_request(vacuum_pickup_);
}

void AssemblyAssembly::EnableVacuumPickupTool_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumPickupTool_finish"
     << ": emitting signal \"EnableVacuumPickupTool_finished\"";

  emit EnableVacuumPickupTool_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable pickup tool vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumPickupTool ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "DisableVacuumPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumPickupTool_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_pickup_ << ")\"";

  emit vacuum_OFF_request(vacuum_pickup_);
}

void AssemblyAssembly::DisableVacuumPickupTool_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumPickupTool_finish"
     << ": emitting signal \"DisableVacuumPickupTool_finished\"";

  emit DisableVacuumPickupTool_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable pickup tool vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "EnableVacuumSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumSpacers_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_spacer_ << ")\"";

  emit vacuum_ON_request(vacuum_spacer_);
}

void AssemblyAssembly::EnableVacuumSpacers_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumSpacers_finish"
     << ": emitting signal \"EnableVacuumSpacers_finished\"";

  emit EnableVacuumSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable spacers vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumSpacers -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "DisableVacuumSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumSpacers_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_spacer_ << ")\"";

  emit vacuum_OFF_request(vacuum_spacer_);
}

void AssemblyAssembly::DisableVacuumSpacers_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumSpacers_finish"
     << ": emitting signal \"DisableVacuumSpacers_finished\"";

  emit DisableVacuumSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable spacers vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumBaseplate ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "EnableVacuumBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumBaseplate_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumBaseplate_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_basepl_ << ")\"";

  emit vacuum_ON_request(vacuum_basepl_);
}

void AssemblyAssembly::EnableVacuumBaseplate_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumBaseplate_finish"
     << ": emitting signal \"EnableVacuumBaseplate_finished\"";

  emit EnableVacuumBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable baseplate vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumBaseplate -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "DisableVacuumBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumBaseplate_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumBaseplate_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_basepl_ << ")\"";

  emit vacuum_OFF_request(vacuum_basepl_);
}

void AssemblyAssembly::DisableVacuumBaseplate_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumBaseplate_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumBaseplate_finish"
     << ": emitting signal \"DisableVacuumBaseplate_finished\"";

  emit DisableVacuumBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable baseplate vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumSubassembly ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumSubassembly_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "EnableVacuumSubassembly_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSubassembly_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSubassembly_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSubassembly_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumSubassembly_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_sub_ << ")\"";

  emit vacuum_ON_request(vacuum_sub_);
}

void AssemblyAssembly::EnableVacuumSubassembly_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSubassembly_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSubassembly_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSubassembly_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumSubassembly_finish"
     << ": emitting signal \"EnableVacuumSubassembly_finished\"";

  emit EnableVacuumSubassembly_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumSubassembly_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable subassembly vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumSubassembly -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumSubassembly_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "DisableVacuumSubassembly_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSubassembly_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSubassembly_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSubassembly_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumSubassembly_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_sub_ << ")\"";

  emit vacuum_OFF_request(vacuum_sub_);
}

void AssemblyAssembly::DisableVacuumSubassembly_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSubassembly_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSubassembly_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSubassembly_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumSubassembly_finish"
     << ": emitting signal \"DisableVacuumSubassembly_finished\"";

  emit DisableVacuumSubassembly_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumSubassembly_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable subassembly vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoFromSensorMarkerToPickupXY -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoFromSensorMarkerToPickupXY_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "GoFromSensorMarkerToPickupXY_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dX");
  const double dy0 = config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoFromSensorMarkerToPickupXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_finish"
     << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

  emit GoFromSensorMarkerToPickupXY_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromSensorMarkerToPickupXY_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move from sensor's reference marker to pickup XY position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPickupToolOntoPSS -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerPickupToolOntoPSS_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "LowerPickupToolOntoPSS_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;
    const double dz0 = config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ");
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoPSS_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPickupToolOntoPSS_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerPickupToolOntoPSS_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerPickupToolOntoPSS_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPickupToolOntoPSS_finish"
       << ": emitting signal \"LowerPickupToolOntoPSS_finished\"";

    emit LowerPickupToolOntoPSS_finished();

    return;
  }
}

void AssemblyAssembly::LowerPickupToolOntoPSS_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoPSS_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPickupToolOntoPSS_finish"
     << ": emitting signal \"LowerPickupToolOntoPSS_finished\"";

  emit LowerPickupToolOntoPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerPickupToolOntoPSS_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower pickup tool onto PS-s]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupPSS ------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::PickupPSS_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "PickupPSS_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "PickupPSS_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_finish"
       << ": emitting signal \"PickupPSS_finished\"";

    emit PickupPSS_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupPSS_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_finish"
     << ": emitting signal \"PickupPSS_finished\"";

  emit PickupPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupPSS_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Pick up PS-s]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToXYAPositionToGluePSSToSpacers ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoToXYAPositionToGluePSSToSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "GoToXYAPositionToGluePSSToSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 =
     config_->getValue<double>("parameters", "PlatformRefPointCalibrationSpacers_X")
   + config_->getValue<double>("parameters", "FromPlatformRefPointCalibrationSpacersToSpacerEdge_dX")
   + config_->getValue<double>("parameters", "FromSpacerEdgeToPSSRefPoint_dX")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dX")
   - motion_->get_position_X();

  const double dy0 =
     config_->getValue<double>("parameters", "PlatformRefPointCalibrationSpacers_Y")
   + config_->getValue<double>("parameters", "FromPlatformRefPointCalibrationSpacersToSpacerEdge_dY")
   + config_->getValue<double>("parameters", "FromSpacerEdgeToPSSRefPoint_dY")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dY")
   - motion_->get_position_Y();

  const double dz0 = 0.0;

  const double da0 =
     config_->getValue<double>("parameters", "PlatformRefPointCalibrationSpacers_A")
   - motion_->get_position_A();

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGluePSSToSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoToXYAPositionToGluePSSToSpacers_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGluePSSToSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_finish"
     << ": emitting signal \"GoToXYAPositionToGluePSSToSpacers_finished\"";

  emit GoToXYAPositionToGluePSSToSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoToXYAPositionToGluePSSToSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to XYA position for gluing PS-s to spacers]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSOntoSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerPSSOntoSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "LowerPSSOntoSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
      - config_->getValue<double>("parameters", "Depth_SpacerSlots")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSOntoSpacers_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSOntoSpacers_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerPSSOntoSpacers_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerPSSOntoSpacers_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSOntoSpacers_finish"
       << ": emitting signal \"LowerPSSOntoSpacers_finished\"";

    emit LowerPSSOntoSpacers_finished();

    return;
  }
}

void AssemblyAssembly::LowerPSSOntoSpacers_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSOntoSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSOntoSpacers_finish"
     << ": emitting signal \"LowerPSSOntoSpacers_finished\"";

  emit LowerPSSOntoSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerPSSOntoSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower PS-s onto spacers]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// ApplyPSPToPSSXYOffset ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::ApplyPSPToPSSXYOffset_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "ApplyPSPToPSSXYOffset_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dX");
  const double dy0 = config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(ApplyPSPToPSSXYOffset_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "ApplyPSPToPSSXYOffset_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::ApplyPSPToPSSXYOffset_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(ApplyPSPToPSSXYOffset_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "ApplyPSPToPSSXYOffset_finish"
     << ": emitting signal \"ApplyPSPToPSSXYOffset_finished\"";

  emit ApplyPSPToPSSXYOffset_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "ApplyPSPToPSSXYOffset_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move by PS-s/PS-p relative XY offset]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// RegisterPSSPlusSpacersToMaPSAPosition --------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::RegisterPSSPlusSpacersToMaPSAPosition_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "RegisterPSSPlusSpacersToMaPSAPosition_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  PSSPlusSpacersToMaPSAPosition_X_ = motion_->get_position_X();
  PSSPlusSpacersToMaPSAPosition_Y_ = motion_->get_position_Y();
  PSSPlusSpacersToMaPSAPosition_Z_ = motion_->get_position_Z();
  PSSPlusSpacersToMaPSAPosition_A_ = motion_->get_position_A();

  PSSPlusSpacersToMaPSAPosition_isRegistered_ = true;

  NQLog("AssemblyAssembly", NQLog::Message) << "RegisterPSSPlusSpacersToMaPSAPosition_start"
     << ": registered position (x=" << PSSPlusSpacersToMaPSAPosition_X_ << ", y=" << PSSPlusSpacersToMaPSAPosition_Y_
     << ", z=" << PSSPlusSpacersToMaPSAPosition_Z_ << ", a=" << PSSPlusSpacersToMaPSAPosition_A_ << ")";

  connect(this, SIGNAL(PSSPlusSpacersToMaPSAPosition_registered()), this, SLOT(RegisterPSSPlusSpacersToMaPSAPosition_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "RegisterPSSPlusSpacersToMaPSAPosition_start"
     << ": emitting signal \"PSSPlusSpacersToMaPSAPosition_registered\"";

  emit PSSPlusSpacersToMaPSAPosition_registered();
}

void AssemblyAssembly::RegisterPSSPlusSpacersToMaPSAPosition_finish()
{
  disconnect(this, SIGNAL(PSSPlusSpacersToMaPSAPosition_registered()), this, SLOT(RegisterPSSPlusSpacersToMaPSAPosition_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "RegisterPSSPlusSpacersToMaPSAPosition_finish"
     << ": emitting signal \"RegisterPSSPlusSpacersToMaPSAPosition_finished\"";

  emit RegisterPSSPlusSpacersToMaPSAPosition_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "RegisterPSSPlusSpacersToMaPSAPosition_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Register {PS-s + spacers} Z position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY -----------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = config_->getValue<double>("parameters", "FromPSSPlusSpacersToMaPSAPositionToGluingStage_dX");
    const double dy0 = config_->getValue<double>("parameters", "FromPSSPlusSpacersToMaPSAPositionToGluingStage_dY");
    const double dz0 = 0.0;
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish"
       << ": emitting signal \"GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished\"";

    emit GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished();

    return;
  }
}

void AssemblyAssembly::GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish"
     << ": emitting signal \"GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished\"";

  emit GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move from {PS-s + spacers --> MaPSA} position to gluing stage reference XY position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSPlusSpacersOntoGluingStage -----------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerPSSPlusSpacersOntoGluingStage_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "LowerPSSPlusSpacersOntoGluingStage_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "CameraFocusOnGluingStage_Z")
      + config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoGluingStage_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSPlusSpacersOntoGluingStage_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerPSSPlusSpacersOntoGluingStage_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerPSSPlusSpacersOntoGluingStage_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSPlusSpacersOntoGluingStage_finish"
       << ": emitting signal \"LowerPSSPlusSpacersOntoGluingStage_finished\"";

    emit LowerPSSPlusSpacersOntoGluingStage_finished();

    return;
  }
}

void AssemblyAssembly::LowerPSSPlusSpacersOntoGluingStage_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoGluingStage_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSPlusSpacersOntoGluingStage_finish"
     << ": emitting signal \"LowerPSSPlusSpacersOntoGluingStage_finished\"";

  emit LowerPSSPlusSpacersOntoGluingStage_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerPSSPlusSpacersOntoGluingStage_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower {PS-s + spacers} onto gluing stage]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// ReturnToPSSPlusSpacersToMaPSAPosition --------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::ReturnToPSSPlusSpacersToMaPSAPosition_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(PSSPlusSpacersToMaPSAPosition_isRegistered_ == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": \"PSS+Spacers To MaPSA\" position not registered, but mandatory for this step (see button in \"Assembly\" tab)";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::ReturnToPSSPlusSpacersToMaPSAPosition_start -- \"PSS+Spacers To MaPSA\" position not registered, but mandatory for this step (see button in \"Assembly\" tab)"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
       << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

    emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = PSSPlusSpacersToMaPSAPosition_X_ - motion_->get_position_X();
    const double dy0 = PSSPlusSpacersToMaPSAPosition_Y_ - motion_->get_position_Y();
    const double dz0 = PSSPlusSpacersToMaPSAPosition_Z_ - motion_->get_position_Z();
    const double da0 = 0.;

    if(dz0 <= 0.)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
         << ": invalid (non-positive) value for vertical upward movement (dz=" << dz0 << "), no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
         << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

      emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

      return;
    }

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(ReturnToPSSPlusSpacersToMaPSAPosition_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "ReturnToPSSPlusSpacersToMaPSAPosition_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::ReturnToPSSPlusSpacersToMaPSAPosition_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
       << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

    emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

    return;
  }
}

void AssemblyAssembly::ReturnToPSSPlusSpacersToMaPSAPosition_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(ReturnToPSSPlusSpacersToMaPSAPosition_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
     << ": emitting signal \"ReturnToPSSPlusSpacersToMaPSAPosition_finished\"";

  emit ReturnToPSSPlusSpacersToMaPSAPosition_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "ReturnToPSSPlusSpacersToMaPSAPosition_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move {PS-s + spacers} from gluing stage to motion stage for gluing to MaPSA]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSPlusSpacersOntoMaPSA -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerPSSPlusSpacersOntoMaPSA_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "LowerPSSPlusSpacersOntoMaPSA_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_MPA");

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoMaPSA_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSPlusSpacersOntoMaPSA_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerPSSPlusSpacersOntoMaPSA_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerPSSPlusSpacersOntoMaPSA_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSPlusSpacersOntoMaPSA_finish"
       << ": emitting signal \"LowerPSSPlusSpacersOntoMaPSA_finished\"";

    emit LowerPSSPlusSpacersOntoMaPSA_finished();

    return;
  }
}

void AssemblyAssembly::LowerPSSPlusSpacersOntoMaPSA_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSPlusSpacersOntoMaPSA_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSPlusSpacersOntoMaPSA_finish"
     << ": emitting signal \"LowerPSSPlusSpacersOntoMaPSA_finished\"";

  emit LowerPSSPlusSpacersOntoMaPSA_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerPSSPlusSpacersOntoMaPSA_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower {PS-s + spacers} onto MaPSA]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupPSSPlusSpacers -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::PickupPSSPlusSpacers_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "PickupPSSPlusSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "PickupPSSPlusSpacers_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSSPlusSpacers_finish"
       << ": emitting signal \"PickupPSSPlusSpacers_finished\"";

    emit PickupPSSPlusSpacers_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSSPlusSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSSPlusSpacers_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupPSSPlusSpacers_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSSPlusSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSSPlusSpacers_finish"
     << ": emitting signal \"PickupPSSPlusSpacers_finished\"";

  emit PickupPSSPlusSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupPSSPlusSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Pickup {PS-s + spacers}]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LiftUpPickupTool -----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LiftUpPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "LiftUpPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup2_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LiftUpPickupTool_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #2 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "LiftUpPickupTool_finish"
       << ": emitting signal \"LiftUpPickupTool_finished\"";

    emit LiftUpPickupTool_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(LiftUpPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "LiftUpPickupTool_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::LiftUpPickupTool_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(LiftUpPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "LiftUpPickupTool_finish"
     << ": emitting signal \"LiftUpPickupTool_finished\"";

  emit LiftUpPickupTool_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LiftUpPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lift up pickup tool]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupSensorAssembly -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::PickupSensorAssembly_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "PickupSensorAssembly_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "PickupSensorAssembly_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "PickupSensorAssembly_finish"
       << ": emitting signal \"PickupSensorAssembly_finished\"";

    emit PickupSensorAssembly_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupSensorAssembly_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupSensorAssembly_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupSensorAssembly_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupSensorAssembly_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupSensorAssembly_finish"
     << ": emitting signal \"PickupSensorAssembly_finished\"";

  emit PickupSensorAssembly_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupSensorAssembly_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Pickup {PS-s + spacers + MaPSA}]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToXYAPositionToGlueSensorAssemblyToBaseplate -----------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoToXYAPositionToGlueSensorAssemblyToBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 =
     config_->getValue<double>("parameters", "PlatformRefPointCalibrationBaseplate_X")
   + config_->getValue<double>("parameters", "FromPlatformRefPointCalibrationBaseplateToPSPEdge_dX")
   + config_->getValue<double>("parameters", "FromPSPEdgeToPSPRefPoint_dX")
   + config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dX")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dX")
   - motion_->get_position_X();

  const double dy0 =
     config_->getValue<double>("parameters", "PlatformRefPointCalibrationBaseplate_Y")
   + config_->getValue<double>("parameters", "FromPlatformRefPointCalibrationBaseplateToPSPEdge_dY")
   + config_->getValue<double>("parameters", "FromPSPEdgeToPSPRefPoint_dY")
   + config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dY")
   + config_->getValue<double>("parameters", "FromSensorRefPointToSensorPickup_dY")
   - motion_->get_position_Y();

  const double dz0 = 0.0;

  const double da0 =
     config_->getValue<double>("parameters", "PlatformRefPointCalibrationBaseplate_A")
   - motion_->get_position_A();

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish()));

  in_action_ = true;

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish"
     << ": emitting signal \"GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished\"";

  emit GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to XYA position for gluing of {PS-s + spacers + MaPSA} to baseplate]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerSensorAssemblyOntoBaseplate -------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerSensorAssemblyOntoBaseplate_start()
{
  if(in_action_){

    NQLog("AssemblyAssembly", NQLog::Warning) << "LowerSensorAssemblyOntoBaseplate_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
      + config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ")
      + config_->getValue<double>("parameters", "Thickness_PSS")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Spacer")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_MPA")
      + config_->getValue<double>("parameters", "Thickness_PSP")
      + config_->getValue<double>("parameters", "Thickness_GlueLayer")
      + config_->getValue<double>("parameters", "Thickness_Baseplate")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSensorAssemblyOntoBaseplate_finish()));

    in_action_ = true;

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSensorAssemblyOntoBaseplate_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerSensorAssemblyOntoBaseplate_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerSensorAssemblyOntoBaseplate_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSensorAssemblyOntoBaseplate_finish"
       << ": emitting signal \"LowerSensorAssemblyOntoBaseplate_finished\"";

    emit LowerSensorAssemblyOntoBaseplate_finished();

    return;
  }
}

void AssemblyAssembly::LowerSensorAssemblyOntoBaseplate_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSensorAssemblyOntoBaseplate_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSensorAssemblyOntoBaseplate_finish"
     << ": emitting signal \"LowerSensorAssemblyOntoBaseplate_finished\"";

  emit LowerSensorAssemblyOntoBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerSensorAssemblyOntoBaseplate_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower {PS-s + spacers + MaPSA} onto baseplate]");
}
// ----------------------------------------------------------------------------------------------------

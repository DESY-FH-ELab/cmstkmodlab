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

#include <AssemblyAssembly.h>
#include <AssemblyUtilities.h>

#include <string>

#include <QMessageBox>

// AssemblyAssembly::AssemblyAssembly(const LStepExpressMotionManager* const motion, const ConradManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent) // CONRAD
AssemblyAssembly::AssemblyAssembly(const LStepExpressMotionManager* const motion, const VellemanManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent) // VELLEMAN

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
const VellemanManager* AssemblyAssembly::vacuum() const
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

AssemblyParameters* AssemblyAssembly::parameters() const
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

void AssemblyAssembly::GoToSensorMarkerPreAlignment_finish()
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
// EnableVacuumPickupTool -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumPickupTool_start()
{
  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumPickupTool_finish"
     << ": emitting signal \"EnableVacuumPickupTool_finished\"";

  emit EnableVacuumPickupTool_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumPickupTool_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumPickupTool ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumPickupTool_start()
{
  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumPickupTool_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumPickupTool_finish"
     << ": emitting signal \"DisableVacuumPickupTool_finished\"";

  emit DisableVacuumPickupTool_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumPickupTool_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumSpacers_start()
{
  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumSpacers_finish"
     << ": emitting signal \"EnableVacuumSpacers_finished\"";

  emit EnableVacuumSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumSpacers_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumSpacers -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumSpacers_start()
{
  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumSpacers_finish"
     << ": emitting signal \"DisableVacuumSpacers_finished\"";

  emit DisableVacuumSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumSpacers_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumBaseplate ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumBaseplate_start()
{
  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumBaseplate_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumBaseplate_finish"
     << ": emitting signal \"EnableVacuumBaseplate_finished\"";

  emit EnableVacuumBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumBaseplate_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumBaseplate -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumBaseplate_start()
{
  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumBaseplate_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumBaseplate_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumBaseplate_finish"
     << ": emitting signal \"DisableVacuumBaseplate_finished\"";

  emit DisableVacuumBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumBaseplate_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumStage ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::EnableVacuumStage_start()
{
  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumStage_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumStage_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumStage_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumStage_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_stage_ << ")\"";

  emit vacuum_ON_request(vacuum_stage_);
}

void AssemblyAssembly::EnableVacuumStage_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumStage_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumStage_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumStage_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "EnableVacuumStage_finish"
     << ": emitting signal \"EnableVacuumStage_finished\"";

  emit EnableVacuumStage_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "EnableVacuumStage_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumStage -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::DisableVacuumStage_start()
{
  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumStage_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumStage_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumStage_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumStage_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_stage_ << ")\"";

  emit vacuum_OFF_request(vacuum_stage_);
}

void AssemblyAssembly::DisableVacuumStage_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumStage_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumStage_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumStage_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "DisableVacuumStage_finish"
     << ": emitting signal \"DisableVacuumStage_finished\"";

  emit DisableVacuumStage_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "DisableVacuumStage_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------
// GoFromSensorMarkerToPickupXY -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoFromSensorMarkerToPickupXY_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromSensorMarkerToPickupXY_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_finish"
       << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

    emit GoFromSensorMarkerToPickupXY_finished();

    return;
  }

  const double dx0 = this->parameters()->get("FromSensorRefPointToSensorPickup_dX");
  const double dy0 = this->parameters()->get("FromSensorRefPointToSensorPickup_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoFromSensorMarkerToPickupXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_finish"
     << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

  emit GoFromSensorMarkerToPickupXY_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromSensorMarkerToPickupXY_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPickupToolOntoPSS -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerPickupToolOntoPSS_start()
{
  if(use_smartMove_)
  {
    const bool valid_params = this->parameters()->update();

    if(valid_params == false)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "LowerPickupToolOntoPSS_start"
         << ": failed to update content of AssemblyParameters, no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPickupToolOntoPSS_finish"
         << ": emitting signal \"LowerPickupToolOntoPSS_finished\"";

      emit LowerPickupToolOntoPSS_finished();

      return;
    }

    const double dx0 = 0.0;
    const double dy0 = 0.0;
    const double dz0 = this->parameters()->get("FromCameraBestFocusToPickupHeight_dZ");
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoPSS_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPickupToolOntoPSS_finish"
     << ": emitting signal \"LowerPickupToolOntoPSS_finished\"";

  emit LowerPickupToolOntoPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerPickupToolOntoPSS_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupPSS ------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::PickupPSS_start()
{
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

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupPSS_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_finish"
     << ": emitting signal \"PickupPSS_finished\"";

  emit PickupPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupPSS_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToXYAPositionToGluePSSToSpacers ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoToXYAPositionToGluePSSToSpacers_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoToXYAPositionToGluePSSToSpacers_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_finish"
       << ": emitting signal \"GoToXYAPositionToGluePSSToSpacers_finished\"";

    emit GoToXYAPositionToGluePSSToSpacers_finished();

    return;
  }

  const double dx0 =
     this->parameters()->get("PlatformRefPointCalibrationSpacers_X")
   + this->parameters()->get("FromPlatformRefPointCalibrationSpacersToSpacerEdge_dX")
   + this->parameters()->get("FromSpacerEdgeToPSSRefPoint_dX")
   + this->parameters()->get("FromSensorRefPointToSensorPickup_dX")
   - motion_->get_position_X();

  const double dy0 =
     this->parameters()->get("PlatformRefPointCalibrationSpacers_Y")
   + this->parameters()->get("FromPlatformRefPointCalibrationSpacersToSpacerEdge_dY")
   + this->parameters()->get("FromSpacerEdgeToPSSRefPoint_dY")
   + this->parameters()->get("FromSensorRefPointToSensorPickup_dY")
   - motion_->get_position_Y();

  const double dz0 = 0.0;

  const double da0 =
     this->parameters()->get("PlatformRefPointCalibrationSpacers_A")
   - motion_->get_position_A();

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGluePSSToSpacers_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoToXYAPositionToGluePSSToSpacers_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGluePSSToSpacers_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGluePSSToSpacers_finish"
     << ": emitting signal \"GoToXYAPositionToGluePSSToSpacers_finished\"";

  emit GoToXYAPositionToGluePSSToSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoToXYAPositionToGluePSSToSpacers_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPSSOntoSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerPSSOntoSpacers_start()
{
  if(use_smartMove_)
  {
    const bool valid_params = this->parameters()->update();

    if(valid_params == false)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "LowerPSSOntoSpacers_start"
         << ": failed to update content of AssemblyParameters, no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSOntoSpacers_finish"
         << ": emitting signal \"LowerPSSOntoSpacers_finished\"";

      emit LowerPSSOntoSpacers_finished();

      return;
    }

    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        this->parameters()->get("CameraFocusOnAssemblyStage_Z")
      - this->parameters()->get("Depth_SpacerSlots")
      + this->parameters()->get("Thickness_Spacer")
      + this->parameters()->get("FromCameraBestFocusToPickupHeight_dZ")
      + this->parameters()->get("Thickness_PSS")
      + this->parameters()->get("Thickness_GlueLayer")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPSSOntoSpacers_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPSSOntoSpacers_finish"
     << ": emitting signal \"LowerPSSOntoSpacers_finished\"";

  emit LowerPSSOntoSpacers_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerPSSOntoSpacers_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// ApplyPSPToPSSXYOffset ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::ApplyPSPToPSSXYOffset_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "ApplyPSPToPSSXYOffset_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "ApplyPSPToPSSXYOffset_finish"
       << ": emitting signal \"ApplyPSPToPSSXYOffset_finished\"";

    emit ApplyPSPToPSSXYOffset_finished();

    return;
  }

  const double dx0 = this->parameters()->get("FromPSPRefPointToPSSRefPoint_dX");
  const double dy0 = this->parameters()->get("FromPSPRefPointToPSSRefPoint_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(ApplyPSPToPSSXYOffset_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "ApplyPSPToPSSXYOffset_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::ApplyPSPToPSSXYOffset_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(ApplyPSPToPSSXYOffset_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "ApplyPSPToPSSXYOffset_finish"
     << ": emitting signal \"ApplyPSPToPSSXYOffset_finished\"";

  emit ApplyPSPToPSSXYOffset_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "ApplyPSPToPSSXYOffset_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// RegisterPSPToPSSPosition ---------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::RegisterPSPToPSSPosition_start()
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

  emit PSPToPSSPosition_registered();
}

void AssemblyAssembly::RegisterPSPToPSSPosition_finish()
{
  disconnect(this, SIGNAL(PSPToPSSPosition_registered()), this, SLOT(RegisterPSPToPSSPosition_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "RegisterPSPToPSSPosition_finish"
     << ": emitting signal \"RegisterPSPToPSSPosition_finished\"";

  emit RegisterPSPToPSSPosition_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "RegisterPSPToPSSPosition_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoFromPSPToPSSPosToGluingStageRefPointXY -----------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoFromPSPToPSSPosToGluingStageRefPointXY_start()
{
  if(use_smartMove_)
  {
    const bool valid_params = this->parameters()->update();

    if(valid_params == false)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromPSPToPSSPosToGluingStageRefPointXY_start"
         << ": failed to update content of AssemblyParameters, no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSPToPSSPosToGluingStageRefPointXY_finish"
         << ": emitting signal \"GoFromPSPToPSSPosToGluingStageRefPointXY_finished\"";

      emit GoFromPSPToPSSPosToGluingStageRefPointXY_finished();

      return;
    }

    const double dx0 = this->parameters()->get("FromPSPToPSSPosToGluingStage_dX");
    const double dy0 = this->parameters()->get("FromPSPToPSSPosToGluingStage_dY");
    const double dz0 = 0.0;
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(GoFromPSPToPSSPosToGluingStageRefPointXY_finish()));

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSPToPSSPosToGluingStageRefPointXY_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoFromPSPToPSSPosToGluingStageRefPointXY_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::GoFromPSPToPSSPosToGluingStageRefPointXY_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSPToPSSPosToGluingStageRefPointXY_finish"
       << ": emitting signal \"GoFromPSPToPSSPosToGluingStageRefPointXY_finished\"";

    emit GoFromPSPToPSSPosToGluingStageRefPointXY_finished();

    return;
  }
}

void AssemblyAssembly::GoFromPSPToPSSPosToGluingStageRefPointXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(GoFromPSPToPSSPosToGluingStageRefPointXY_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromPSPToPSSPosToGluingStageRefPointXY_finish"
     << ": emitting signal \"GoFromPSPToPSSPosToGluingStageRefPointXY_finished\"";

  emit GoFromPSPToPSSPosToGluingStageRefPointXY_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoFromPSPToPSSPosToGluingStageRefPointXY_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerSpacersAndPSSOntoGluingStage ------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerSpacersAndPSSOntoGluingStage_start()
{
  if(use_smartMove_)
  {
    const bool valid_params = this->parameters()->update();

    if(valid_params == false)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "LowerSpacersAndPSSOntoGluingStage_start"
         << ": failed to update content of AssemblyParameters, no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoGluingStage_finish"
         << ": emitting signal \"LowerSpacersAndPSSOntoGluingStage_finished\"";

      emit LowerSpacersAndPSSOntoGluingStage_finished();

      return;
    }

    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        this->parameters()->get("CameraFocusOnGluingStage_Z")
      + this->parameters()->get("FromCameraBestFocusToPickupHeight_dZ")
      + this->parameters()->get("Thickness_PSS")
      + this->parameters()->get("Thickness_GlueLayer")
      + this->parameters()->get("Thickness_Spacer")
      + this->parameters()->get("Thickness_GlueLayer")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSpacersAndPSSOntoGluingStage_finish()));

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoGluingStage_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerSpacersAndPSSOntoGluingStage_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerSpacersAndPSSOntoGluingStage_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoGluingStage_finish"
       << ": emitting signal \"LowerSpacersAndPSSOntoGluingStage_finished\"";

    emit LowerSpacersAndPSSOntoGluingStage_finished();

    return;
  }
}

void AssemblyAssembly::LowerSpacersAndPSSOntoGluingStage_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSpacersAndPSSOntoGluingStage_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoGluingStage_finish"
     << ": emitting signal \"LowerSpacersAndPSSOntoGluingStage_finished\"";

  emit LowerSpacersAndPSSOntoGluingStage_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerSpacersAndPSSOntoGluingStage_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// ReturnToPSPToPSSPosition ---------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::ReturnToPSPToPSSPosition_start()
{
  if(PSPToPSSPosition_isRegistered_ == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "ReturnToPSPToPSSPosition_start"
       << ": PSP-To-PSS Position was not registered, which is mandatory for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::ReturnToPSPToPSSPosition_start -- PSP-To-PSS Position was not registered, which is mandatory for this step (see dedicated button in \"Assembly\" tab)"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSPToPSSPosition_finish"
       << ": emitting signal \"ReturnToPSPToPSSPosition_finished\"";

    emit ReturnToPSPToPSSPosition_finished();

    return;
  }

  if(use_smartMove_)
  {
    const double dx0 = PSPToPSSPosition_X_ - motion_->get_position_X();
    const double dy0 = PSPToPSSPosition_Y_ - motion_->get_position_Y();
    const double dz0 = PSPToPSSPosition_Z_ - motion_->get_position_Z();
    const double da0 = 0.;

    if(dz0 <= 0.)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "ReturnToPSPToPSSPosition_start"
         << ": invalid (non-positive) value for vertical upward movement (dz=" << dz0 << "), no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSPToPSSPosition_finish"
         << ": emitting signal \"ReturnToPSPToPSSPosition_finished\"";

      emit ReturnToPSPToPSSPosition_finished();

      return;
    }

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(ReturnToPSPToPSSPosition_finish()));

    NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSPToPSSPosition_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "ReturnToPSPToPSSPosition_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::ReturnToPSPToPSSPosition_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSPToPSSPosition_finish"
       << ": emitting signal \"ReturnToPSPToPSSPosition_finished\"";

    emit ReturnToPSPToPSSPosition_finished();

    return;
  }
}

void AssemblyAssembly::ReturnToPSPToPSSPosition_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(ReturnToPSPToPSSPosition_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "ReturnToPSPToPSSPosition_finish"
     << ": emitting signal \"ReturnToPSPToPSSPosition_finished\"";

  emit ReturnToPSPToPSSPosition_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "ReturnToPSPToPSSPosition_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerSpacersAndPSSOntoPSP --------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerSpacersAndPSSOntoPSP_start()
{
  if(use_smartMove_)
  {
    const bool valid_params = this->parameters()->update();

    if(valid_params == false)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "LowerSpacersAndPSSOntoPSP_start"
         << ": failed to update content of AssemblyParameters, no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoPSP_finish"
         << ": emitting signal \"LowerSpacersAndPSSOntoPSP_finished\"";

      emit LowerSpacersAndPSSOntoPSP_finished();

      return;
    }

    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        this->parameters()->get("FromCameraBestFocusToPickupHeight_dZ")
      + this->parameters()->get("Thickness_PSS")
      + this->parameters()->get("Thickness_GlueLayer")
      + this->parameters()->get("Thickness_Spacer")
      + this->parameters()->get("Thickness_GlueLayer");

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSpacersAndPSSOntoPSP_finish()));

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoPSP_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "LowerSpacersAndPSSOntoPSP_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblyAssembly::LowerSpacersAndPSSOntoPSP_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoPSP_finish"
       << ": emitting signal \"LowerSpacersAndPSSOntoPSP_finished\"";

    emit LowerSpacersAndPSSOntoPSP_finished();

    return;
  }
}

void AssemblyAssembly::LowerSpacersAndPSSOntoPSP_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSpacersAndPSSOntoPSP_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSpacersAndPSSOntoPSP_finish"
     << ": emitting signal \"LowerSpacersAndPSSOntoPSP_finished\"";

  emit LowerSpacersAndPSSOntoPSP_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerSpacersAndPSSOntoPSP_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupSpacersAndPSS --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::PickupSpacersAndPSS_start()
{
  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "PickupSpacersAndPSS_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "PickupSpacersAndPSS_finish"
       << ": emitting signal \"PickupSpacersAndPSS_finished\"";

    emit PickupSpacersAndPSS_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupSpacersAndPSS_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupSpacersAndPSS_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupSpacersAndPSS_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupSpacersAndPSS_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupSpacersAndPSS_finish"
     << ": emitting signal \"PickupSpacersAndPSS_finished\"";

  emit PickupSpacersAndPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupSpacersAndPSS_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LiftUpPickupTool -----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LiftUpPickupTool_start()
{
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

  NQLog("AssemblyAssembly", NQLog::Spam) << "LiftUpPickupTool_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::LiftUpPickupTool_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(LiftUpPickupTool_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "LiftUpPickupTool_finish"
     << ": emitting signal \"LiftUpPickupTool_finished\"";

  emit LiftUpPickupTool_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LiftUpPickupTool_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupPSPAndPSS ------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::PickupPSPAndPSS_start()
{
  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "PickupPSPAndPSS_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSPAndPSS_finish"
       << ": emitting signal \"PickupPSPAndPSS_finished\"";

    emit PickupPSPAndPSS_finished();

    return;
  }

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSPAndPSS_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSPAndPSS_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupPSPAndPSS_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSPAndPSS_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSPAndPSS_finish"
     << ": emitting signal \"PickupPSPAndPSS_finished\"";

  emit PickupPSPAndPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupPSPAndPSS_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoToXYAPositionToGlueSensorAssemblyToBaseplate -----------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoToXYAPositionToGlueSensorAssemblyToBaseplate_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Critical) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish"
       << ": emitting signal \"GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished\"";

    emit GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished();

    return;
  }

  const double dx0 =
     this->parameters()->get("PlatformRefPointCalibrationBaseplate_X")
   + this->parameters()->get("FromPlatformRefPointCalibrationBaseplateToPSPEdge_dX")
   + this->parameters()->get("FromPSPEdgeToPSPRefPoint_dX")
   + this->parameters()->get("FromPSPRefPointToPSSRefPoint_dX")
   + this->parameters()->get("FromSensorRefPointToSensorPickup_dX")
   - motion_->get_position_X();

  const double dy0 =
     this->parameters()->get("PlatformRefPointCalibrationBaseplate_Y")
   + this->parameters()->get("FromPlatformRefPointCalibrationBaseplateToPSPEdge_dY")
   + this->parameters()->get("FromPSPEdgeToPSPRefPoint_dY")
   + this->parameters()->get("FromPSPRefPointToPSSRefPoint_dY")
   + this->parameters()->get("FromSensorRefPointToSensorPickup_dY")
   - motion_->get_position_Y();

  const double dz0 = 0.0;

  const double da0 =
     this->parameters()->get("PlatformRefPointCalibrationBaseplate_A")
   - motion_->get_position_A();

  connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish"
     << ": emitting signal \"GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished\"";

  emit GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "GoToXYAPositionToGlueSensorAssemblyToBaseplate_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerSensorAssemblyOntoBaseplate -------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::LowerSensorAssemblyOntoBaseplate_start()
{
  if(use_smartMove_)
  {
    const bool valid_params = this->parameters()->update();

    if(valid_params == false)
    {
      NQLog("AssemblyAssembly", NQLog::Critical) << "LowerSensorAssemblyOntoBaseplate_start"
         << ": failed to update content of AssemblyParameters, no action taken";

      NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSensorAssemblyOntoBaseplate_finish"
         << ": emitting signal \"LowerSensorAssemblyOntoBaseplate_finished\"";

      emit LowerSensorAssemblyOntoBaseplate_finished();

      return;
    }

    const double dx0 = 0.0;
    const double dy0 = 0.0;

    const double dz0 =
        this->parameters()->get("CameraFocusOnAssemblyStage_Z")
      + this->parameters()->get("FromCameraBestFocusToPickupHeight_dZ")
      + this->parameters()->get("Thickness_PSS")
      + this->parameters()->get("Thickness_GlueLayer")
      + this->parameters()->get("Thickness_Spacer")
      + this->parameters()->get("Thickness_GlueLayer")
      + this->parameters()->get("Thickness_PSP")
      + this->parameters()->get("Thickness_GlueLayer")
      + this->parameters()->get("Thickness_Baseplate")
      - motion_->get_position_Z();

    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerSensorAssemblyOntoBaseplate_finish()));

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

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerSensorAssemblyOntoBaseplate_finish"
     << ": emitting signal \"LowerSensorAssemblyOntoBaseplate_finished\"";

  emit LowerSensorAssemblyOntoBaseplate_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "LowerSensorAssemblyOntoBaseplate_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

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

AssemblyAssembly::AssemblyAssembly(const LStepExpressMotionManager* const motion, const ConradManager* const vacuum, QObject* parent)
 : QObject(parent)

 , motion_(motion)
 , vacuum_(vacuum)

 , vacuum_pickup_(0)
 , vacuum_spacer_(0)
 , vacuum_basepl_(0)
{
  // validate pointers to controllers
  this->motion();
  this->vacuum();

  // indices of vacuum lines
  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyVacuumWidget", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    return;
  }

  vacuum_pickup_ = config->getValue<int>("Vacuum_PickupTool");
  vacuum_spacer_ = config->getValue<int>("Vacuum_Spacers");
  vacuum_basepl_ = config->getValue<int>("Vacuum_Baseplate");
}

const LStepExpressMotionManager* AssemblyAssembly::motion() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "motion"
       << ": pointer to LStepExpressMotionManager is NULL, exiting constructor";

    QMessageBox::critical(0
     , tr("[AssemblyAssembly]")
     , tr("pointer to LStepExpressMotionManager is NULL, aborting")
     , QMessageBox::Abort
    );
  }

  return motion_;
}

const ConradManager* AssemblyAssembly::vacuum() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "vacuum"
       << ": pointer to ConradManager is NULL, exiting constructor";

    QMessageBox::critical(0
     , tr("[AssemblyAssembly]")
     , tr("pointer to ConradManager is NULL, aborting")
     , QMessageBox::Abort
    );
  }

  return vacuum_;
}

AssemblyParameters* AssemblyAssembly::parameters() const
{
  AssemblyParameters* const params = AssemblyParameters::instance();

  if(params == nullptr)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "parameters"
       << ": pointer to AssemblyParameters is NULL, exiting constructor";

    QMessageBox::critical(0
     , tr("[AssemblyAssembly]")
     , tr("pointer to AssemblyParameters is NULL, aborting")
     , QMessageBox::Abort
    );

    throw; // must abort
  }

  return params;
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
    NQLog("AssemblyAssembly", NQLog::Fatal) << "GoToSensorMarkerPreAlignment_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    return;
  }

  connect(this, SIGNAL(move_absolute(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  const double x0 = this->parameters()->get("refPointSensor_X");
  const double y0 = this->parameters()->get("refPointSensor_Y");
  const double z0 = this->parameters()->get("refPointSensor_Z");
  const double a0 = this->parameters()->get("refPointSensor_A");

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToSensorMarkerPreAlignment_start"
     << ": emitting signal \"move_absolute(" << x0 << ", " << y0 << ", " << z0 << ", " << a0 << ")\"";

  emit move_absolute(x0, y0, z0, a0);
}

void AssemblyAssembly::GoToSensorMarkerPreAlignment_finish()
{
  disconnect(this, SIGNAL(move_absolute(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
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
// GoFromSensorMarkerToPickupXY -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblyAssembly::GoFromSensorMarkerToPickupXY_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "GoFromSensorMarkerToPickupXY_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    return;
  }

  connect(this, SIGNAL(move_relative(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  const double dx0 = this->parameters()->get("marker_to_pickup_dX");
  const double dy0 = this->parameters()->get("marker_to_pickup_dY");
  const double dz0 = 0.0;
  const double da0 = 0.0;

  NQLog("AssemblyAssembly", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_start"
     << ": emitting signal \"move_relative(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::GoFromSensorMarkerToPickupXY_finish()
{
  disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
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
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "LowerPickupToolOntoPSS_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    return;
  }

  connect(this, SIGNAL(move_relative(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(LowerPickupToolOntoPSS_finish()));

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (this->parameters()->get("pickupOnRotStage_Z") + this->parameters()->get("thickness_PSS")) - motion_->get_position_Z() /* !! */ + 1.000;
  const double da0 = 0.0;

  NQLog("AssemblyAssembly", NQLog::Spam) << "LowerPickupToolOntoPSS_start"
     << ": emitting signal \"move_relative(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::LowerPickupToolOntoPSS_finish()
{
  disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(LowerPickupToolOntoPSS_finish()));

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
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "PickupPSS_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    return;
  }

  connect(this, SIGNAL(move_relative(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));

  const double dx0 =  0.0;
  const double dy0 =  0.0;
  const double dz0 = 80.0;
  const double da0 =  0.0;

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_start"
     << ": emitting signal \"move_relative(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative(dx0, dy0, dz0, da0);
}

void AssemblyAssembly::PickupPSS_finish()
{
  disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupPSS_finish()));

  NQLog("AssemblyAssembly", NQLog::Spam) << "PickupPSS_finish"
     << ": emitting signal \"PickupPSS_finished\"";

  emit PickupPSS_finished();

  NQLog("AssemblyAssembly", NQLog::Message) << "PickupPSS_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

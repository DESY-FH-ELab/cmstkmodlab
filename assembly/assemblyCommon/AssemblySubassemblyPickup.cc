/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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
#include <AssemblyUtilities.h>

#include <AssemblySubassemblyPickup.h>

AssemblySubassemblyPickup::AssemblySubassemblyPickup(const LStepExpressMotionManager* const motion, const RelayCardManager* const vacuum, const AssemblySmartMotionManager* const smart_motion, QObject* parent)
 : QObject(parent)

  , motion_(motion)
  , vacuum_(vacuum)
  , smart_motion_(smart_motion)
  , config_(nullptr)

  , vacuum_pickup_(0)
  , vacuum_spacer_(0)
  , vacuum_basepl_(0)

  , use_smartMove_(false)
  , in_action_(false)
{
    // validate pointers to controllers
    this->motion();
    this->vacuum();

    // indices of vacuum lines
    config_ = ApplicationConfig::instance();
    if(config_ == nullptr)
    {
      NQLog("AssemblySubassemblyPickup", NQLog::Fatal)
         << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

      assembly::kill_application(tr("[AssemblySubassemblyPickup]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
    }

    vacuum_pickup_ = config_->getValue<int>("main", "Vacuum_PickupTool");
    vacuum_spacer_ = config_->getValue<int>("main", "Vacuum_Spacers");
    vacuum_basepl_ = config_->getValue<int>("main", "Vacuum_Baseplate");

    alreadyClicked_LowerPickupToolOntoSubassembly = false;

    // absolute Z-position of motion stage for pickup of object after gluing
    // (1: PSs to Spacers)
    pickup1_Z_ = config_->getValue<double>("main", "AssemblyAssembly_pickup1_Z");

  this->reset();

  NQLog("AssemblySubassemblyPickup", NQLog::Debug) << "constructed";
}

AssemblySubassemblyPickup::~AssemblySubassemblyPickup()
{
}

const LStepExpressMotionManager* AssemblySubassemblyPickup::motion() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Fatal) << "motion"
       << ": pointer to LStepExpressMotionManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblySubassemblyPickup]"), tr("pointer to LStepExpressMotionManager is NULL, aborting"));
  }

  return motion_;
}

const RelayCardManager* AssemblySubassemblyPickup::vacuum() const
{
  if(motion_ == nullptr)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Fatal) << "vacuum"
       << ": pointer to RelayCardManager is NULL, exiting constructor";

    assembly::kill_application(tr("[AssemblySubassemblyPickup]"), tr("pointer to RelayCardManager is NULL, aborting"));
  }

  return vacuum_;
}

void AssemblySubassemblyPickup::use_smartMove(const int state)
{
  if(state == 2)
  {
    if(smart_motion_ == nullptr)
    {
      NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "use_smartMove(" << state << ")"
         << ": attempting to enable \"smartMove\" mode, but pointer to AssemblySmartMotionManager is invalid (NULL), \"smartMove\" mode will stay disabled";

      use_smartMove_ = false;
    }
    else
    {
      NQLog("AssemblySubassemblyPickup", NQLog::Message) << "use_smartMove(" << state << ")"
         << ": \"smartMove\" mode switched ON";

      use_smartMove_ = true;
    }
  }
  else if(state == 0)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Message) << "use_smartMove(" << state << ")"
       << ": \"smartMove\" mode switched OFF";

    use_smartMove_ = false;
  }
  else
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "use_smartMove(" << state << ")"
       << ": invalid argument (state=" << state << "), no action taken (smartMove=" << use_smartMove_ << ")";
  }

  return;
}
// ----------------------------------------------------------------------------------------------------

void AssemblySubassemblyPickup::reset()
{

}

// ----------------------------------------------------------------------------------------------------
// GoToSensorMarkerPreAlignment -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::GoToSensorMarkerPreAlignment_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "GoToSensorMarkerPreAlignment_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double x0 =
     config_->getValue<double>("parameters", "RefPointPlatform_X")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToSpacerEdge_dX")
   + config_->getValue<double>("parameters", "FromSpacerEdgeToPSSRefPoint_dX");

  const double y0 =
     config_->getValue<double>("parameters", "RefPointPlatform_Y")
   + config_->getValue<double>("parameters", "FromRefPointPlatformToSpacerEdge_dY")
   + config_->getValue<double>("parameters", "FromSpacerEdgeToPSSRefPoint_dY");

  const double z0 =
     config_->getValue<double>("parameters", "CameraFocusOnAssemblyStage_Z")
   + config_->getValue<double>("parameters", "Thickness_SubassemblyJig")
   + config_->getValue<double>("parameters", "Thickness_Spacer")
   + config_->getValue<double>("parameters", "Thickness_GlueLayer")
   + config_->getValue<double>("parameters", "Thickness_PSS");

  const double a0 = config_->getValue<double>("parameters", "RefPointPlatform_A");

  connect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  connect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  in_action_ = true;

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "GoToSensorMarkerPreAlignment_start"
     << ": emitting signal \"move_absolute_request(" << x0 << ", " << y0 << ", " << z0 << ", " << a0 << ")\"";

  emit move_absolute_request(x0, y0, z0, a0);
}

void AssemblySubassemblyPickup::GoToSensorMarkerPreAlignment_finish()
{
  disconnect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_, SLOT(moveAbsolute(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoToSensorMarkerPreAlignment_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "GoToSensorMarkerPreAlignment_finish"
     << ": emitting signal \"GoToSensorMarkerPreAlignment_finished\"";

  emit GoToSensorMarkerPreAlignment_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "GoToSensorMarkerPreAlignment_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move to sensor's reference marker (pre-alignment)]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumSpacers --------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::EnableVacuumSpacers_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "EnableVacuumSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "EnableVacuumSpacers_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_spacer_ << ")\"";

  emit vacuum_ON_request(vacuum_spacer_);
}

void AssemblySubassemblyPickup::EnableVacuumSpacers_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "EnableVacuumSpacers_finish"
     << ": emitting signal \"EnableVacuumSpacers_finished\"";

  emit EnableVacuumSpacers_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "EnableVacuumSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable spacers vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// switchToAlignmentTab_PSS ------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::switchToAlignmentTab_PSS()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "switchToAlignmentTab_PSS"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  in_action_ = true;

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "switchToAlignmentTab_PSS"
    << ": emitting signal \"switchToAlignmentTab_PSS_request\"";

  emit switchToAlignmentTab_PSS_request(); //Will auto-switch to 'Alignment' sub-tab, and select PSS mode

  in_action_ = false;

  return;
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// GoFromSensorMarkerToPickupXY -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::GoFromSensorMarkerToPickupXY_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "GoFromSensorMarkerToPickupXY_start"
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

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblySubassemblyPickup::GoFromSensorMarkerToPickupXY_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
  disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(GoFromSensorMarkerToPickupXY_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "GoFromSensorMarkerToPickupXY_finish"
     << ": emitting signal \"GoFromSensorMarkerToPickupXY_finished\"";

  emit GoFromSensorMarkerToPickupXY_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "GoFromSensorMarkerToPickupXY_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Move from sensor's reference marker to pickup XY position]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// LowerPickupToolOntoSubassembly ---------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::LowerPickupToolOntoSubassembly_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "LowerPickupToolOntoSubassembly_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  if(alreadyClicked_LowerPickupToolOntoSubassembly)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "LowerPickupToolOntoSubassembly_start"
        << ": this button was pressed more than once already";

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("WARNING: this button was already clicked at least once. Do you want to proceed?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }
  }
  alreadyClicked_LowerPickupToolOntoSubassembly = true; //Remember that this button already got clicked (repeating this action involuntarily can cause crash)

  if(use_smartMove_)
  {
    const double dx0 = 0.0;
    const double dy0 = 0.0;
    const double dz0 = config_->getValue<double>("parameters", "FromCameraBestFocusToPickupHeight_dZ");
    const double da0 = 0.0;

    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoSubassembly_finish()));

    in_action_ = true;

    NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "LowerPickupToolOntoSubassembly_start"
       << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

    emit move_relative_request(dx0, dy0, dz0, da0);
  }
  else
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Critical) << "LowerPickupToolOntoSubassembly_start"
       << ": please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step";

    QMessageBox msgBox;
    msgBox.setText(tr("AssemblySubassemblyPickup::LowerPickupToolOntoSubassembly_start -- please enable \"smartMove\" mode (tick box in top-left corner of Assembly tab), required for this step"));
    msgBox.exec();

    NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "LowerPickupToolOntoSubassembly_finish"
       << ": emitting signal \"LowerPickupToolOntoSubassembly_finished\"";

    emit LowerPickupToolOntoSubassembly_finished();

    return;
  }
}

void AssemblySubassemblyPickup::LowerPickupToolOntoSubassembly_finish()
{
  disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
  disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(LowerPickupToolOntoSubassembly_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "LowerPickupToolOntoSubassembly_finish"
     << ": emitting signal \"LowerPickupToolOntoSubassembly_finished\"";

  emit LowerPickupToolOntoSubassembly_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "LowerPickupToolOntoSubassembly_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Lower pickup tool onto Subassembly]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// EnableVacuumPickupTool -----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::EnableVacuumPickupTool_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "EnableVacuumPickupTool_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

  in_action_ = true;

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "EnableVacuumPickupTool_start"
     << ": emitting signal \"vacuum_ON_request(" << vacuum_pickup_ << ")\"";

  emit vacuum_ON_request(vacuum_pickup_);
}

void AssemblySubassemblyPickup::EnableVacuumPickupTool_finish()
{
  disconnect(this, SIGNAL(vacuum_ON_request(int)), this->vacuum(), SLOT(enableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_enabled()), this, SLOT(EnableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled()), this, SLOT(EnableVacuumPickupTool_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error  ()), this, SLOT(EnableVacuumPickupTool_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "EnableVacuumPickupTool_finish"
     << ": emitting signal \"EnableVacuumPickupTool_finished\"";

  emit EnableVacuumPickupTool_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "EnableVacuumPickupTool_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Enable pickup tool vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// DisableVacuumSpacers -------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::DisableVacuumSpacers_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "DisableVacuumSpacers_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  connect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  connect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  connect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

  in_action_ = true;

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "DisableVacuumSpacers_start"
     << ": emitting signal \"vacuum_OFF_request(" << vacuum_spacer_ << ")\"";

  emit vacuum_OFF_request(vacuum_spacer_);
}

void AssemblySubassemblyPickup::DisableVacuumSpacers_finish()
{
  disconnect(this, SIGNAL(vacuum_OFF_request(int)), this->vacuum(), SLOT(disableVacuum(int)));

  disconnect(this->vacuum(), SIGNAL(vacuum_disabled()), this, SLOT(DisableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_toggled ()), this, SLOT(DisableVacuumSpacers_finish()));
  disconnect(this->vacuum(), SIGNAL(vacuum_error   ()), this, SLOT(DisableVacuumSpacers_finish()));

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "DisableVacuumSpacers_finish"
     << ": emitting signal \"DisableVacuumSpacers_finished\"";

  emit DisableVacuumSpacers_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "DisableVacuumSpacers_finish"
     << ": assembly-step completed";

  emit DBLogMessage("== Assembly step completed : [Disable spacers vacuum]");
}
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// PickupSubassembly ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
void AssemblySubassemblyPickup::PickupSubassembly_start()
{
  if(in_action_){

    NQLog("AssemblySubassemblyPickup", NQLog::Warning) << "PickupSubassembly_start"
       << ": logic error, an assembly step is still in progress, will not take further action";

    return;
  }

  const double dx0 = 0.0;
  const double dy0 = 0.0;
  const double dz0 = (pickup1_Z_ - motion_->get_position_Z());
  const double da0 = 0.0;

  if(dz0 <= 0.)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Critical) << "PickupSubassembly_start"
       << ": invalid (non-positive) value for vertical upward movement for pickup #1 (dz=" << dz0 << "), no action taken";

    NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "PickupSubassembly_finish"
       << ": emitting signal \"PickupSubassembly_finished\"";

    emit PickupSubassembly_finished();

    return;
  }

  if(use_smartMove_) {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    connect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupSubassembly_finish()));
  } else {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupSubassembly_finish()));
  }

  in_action_ = true;

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "PickupSubassembly_start"
     << ": emitting signal \"move_relative_request(" << dx0 << ", " << dy0 << ", " << dz0 << ", " << da0 << ")\"";

  emit move_relative_request(dx0, dy0, dz0, da0);
}

void AssemblySubassemblyPickup::PickupSubassembly_finish()
{
  if(use_smartMove_) {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), smart_motion_, SLOT(move_relative(double, double, double, double)));
    disconnect(smart_motion_, SIGNAL(motion_completed()), this, SLOT(PickupSubassembly_finish()));
  } else {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_, SIGNAL(motion_finished()), this, SLOT(PickupSubassembly_finish()));
  }

  if(in_action_){ in_action_ = false; }

  NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "PickupSubassembly_finish"
     << ": emitting signal \"PickupSubassembly_finished\"";

  emit PickupSubassembly_finished();

  NQLog("AssemblySubassemblyPickup", NQLog::Message) << "PickupSubassembly_finish"
     << ": assembly-step completed";
}
// ----------------------------------------------------------------------------------------------------

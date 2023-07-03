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

#include <AssemblySubassemblyPickup.h>

AssemblySubassemblyPickup::AssemblySubassemblyPickup(const LStepExpressMotionManager* motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Critical) << "input error"
       << ": null pointer to LStepExpressMotionManager object, initialization stopped";

    return;
  }

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    return;
  }


  this->reset();

  NQLog("AssemblySubassemblyPickup", NQLog::Debug) << "constructed";
}

AssemblySubassemblyPickup::~AssemblySubassemblyPickup()
{
}

void AssemblySubassemblyPickup::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AssemblySubassemblyPickup", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for multi-pickup test already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(setup_next_step()));

    motion_manager_enabled_ = true;

    NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for multi-pickup test enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(setup_next_step()));

    motion_manager_enabled_ = false;

    NQLog("AssemblySubassemblyPickup", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for multi-pickup test disabled";
  }

  return;
}

void AssemblySubassemblyPickup::reset()
{

  this->disconnect_motion_manager();
}

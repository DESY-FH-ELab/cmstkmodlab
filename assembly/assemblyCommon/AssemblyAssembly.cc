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
{
  // validate pointers to controllers
  this->motion();
  this->vacuum();
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
     << ": assembly-step completed";

  return;
}
// ----------------------------------------------------------------------------------------------------

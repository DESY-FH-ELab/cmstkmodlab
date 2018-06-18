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

AssemblyAssembly::AssemblyAssembly(QObject* parent)
 : QObject(parent)
{
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

void AssemblyAssembly::GoToPSSPreAlignment_start()
{
  const bool valid_params = this->parameters()->update();

  if(valid_params == false)
  {
    NQLog("AssemblyAssembly", NQLog::Fatal) << "GoToPSSPreAlignment_start"
       << ": failed to update content of AssemblyParameters, no action taken";

    return;
  }

//  connect(this, SIGNAL(move_absolute(double, double, double, double)), motion_manager, SLOT(moveAbsolute(double, double, double, double)));
//  connect(motion_manager, SIGNAL(motion_finished()), this, SLOT(GoToPSSPreAlignment_finish()));
//
//  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToPSSPreAlignment_start"
//     << ": emitting signal \"move_absolute(" << x << ", " << y << ", " << z << ", " << a << ")\"";

  emit move_absolute(0., 0., 0., 0.);
}

void AssemblyAssembly::GoToPSSPreAlignment_finish()
{
//  disconnect(this, SIGNAL(move_absolute(double, double, double, double)), motion_manager, SLOT(moveAbsolute(double, double, double, double)));
//  disconnect(motion_manager, SIGNAL(motion_finished()), this, SLOT(GoToPSSPreAlignment_finish()));
//
//  NQLog("AssemblyAssembly", NQLog::Spam) << "GoToPSSPreAlignment_finish"
//     << ": assembly-step completed";

  return;
}

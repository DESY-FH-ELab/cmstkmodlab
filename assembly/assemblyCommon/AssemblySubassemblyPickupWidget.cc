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

#include <AssemblySubassemblyPickupWidget.h>

#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include <QVBoxLayout>
#include <QStringList>

AssemblySubassemblyPickupWidget::AssemblySubassemblyPickupWidget(const LStepExpressMotionManager* const motion_manager, QWidget* parent) :
  QWidget(parent),

  motion_manager_(motion_manager)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblySubassemblyPickupWidget", NQLog::Critical)
       << "input error: null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  uint pickup_step_N = 0;

  // step: Place MaPSA on Assembly Platform
  {
    ++pickup_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->text()->setText("Place PSS+spacers subassembly on assembly platform with the spacers in their spacer pockets");
    layout->addWidget(tmp_wid);
  }
  // ----------

  // step: Go To Measurement Position on MaPSA
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Go To Measurement Position on PSS");
    layout->addWidget(tmp_wid);

    //tmp_wid->connect_action(assembly, SLOT(GoToSensorMarkerPreAlignment_start()), SIGNAL(GoToSensorMarkerPreAlignment_finished()));
  }
  // ----------


  layout->addStretch(1);
}

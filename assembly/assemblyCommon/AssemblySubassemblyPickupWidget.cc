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

AssemblySubassemblyPickupWidget::AssemblySubassemblyPickupWidget(const QObject* const subassembly_pickup, QWidget* parent)
  : QWidget(parent)

  , smartMove_checkbox_(nullptr)
{
  if(subassembly_pickup == nullptr)
  {
      NQLog("AssemblyAssemblyView", NQLog::Fatal) << "AssemblyAssemblyView(" << subassembly_pickup << ", " << parent << ")"
      << ": null pointer to QObject --> GUI layout will not be created";

      return;
  }

  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  //// Assembly Options ------------------------------
  QHBoxLayout* opts_lay = new QHBoxLayout;
  layout->addLayout(opts_lay);

  smartMove_checkbox_ = new QCheckBox(tr("Use SmartMove"));

  opts_lay->addWidget(smartMove_checkbox_);

  connect(smartMove_checkbox_, SIGNAL(stateChanged(int)), subassembly_pickup, SLOT(use_smartMove(int)));

  smartMove_checkbox_->setChecked(true);
  //// -----------------------------------------------

  QVBoxLayout* sub_pick_lay = new QVBoxLayout;
  layout->addLayout(sub_pick_lay);

  uint pickup_step_N = 0;

  // step: Place PSS+Spacers Subassembly on Assembly Platform
  {
    ++pickup_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->text()->setText("Place PSS+spacers subassembly on assembly platform with the spacers aligned in their spacer pockets");
    sub_pick_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go To Measurement Position on PS-s
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Go To Measurement Position on PS-s - Spacer height considered");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(GoToSensorMarkerPreAlignment_start()), SIGNAL(GoToSensorMarkerPreAlignment_finished()));
  }
  // ----------

  // step: Enable vacuum on spacers
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Enable vacuum on spacers");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(EnableVacuumSpacers_start()), SIGNAL(EnableVacuumSpacers_finished()));
  }
  // ----------

  // step: Align PSS to motion stage
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Align PS-s to Motion Stage (Go to \"Alignment\" Tab and select \"PS-s Sensor\")");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(switchToAlignmentTab_PSS()), SIGNAL(switchToAlignmentTab_PSS_request()));
  }
  // ----------

  // step: Go from sensor marker to pickup XY
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Go From Sensor Marker Ref-Point to Pickup XY");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(GoFromSensorMarkerToPickupXY_start()), SIGNAL(GoFromSensorMarkerToPickupXY_finished()));
  }
  // ----------

  // step: Lower Pickup Tool onto Subassembly
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PSS+spacers subassembly");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(LowerPickupToolOntoSubassembly_start()), SIGNAL(LowerPickupToolOntoSubassembly_finished()));
  }
  // ----------

  // step: Enable Vacuum on Pickup Tool
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Enable Vacuum on Pickup-Tool");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(EnableVacuumPickupTool_start()), SIGNAL(EnableVacuumPickupTool_finished()));
  }
  // ----------

  // step: Disable vacuum on Spacers
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Disable Vacuum on Spacers");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(DisableVacuumSpacers_start()), SIGNAL(DisableVacuumSpacers_finished()));
  }
  // ----------

  // step: Pick Up Subassembly
  {
    ++pickup_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(pickup_step_N));
    tmp_wid->button()->setText("Pick Up PSS+spacers subassembly");
    sub_pick_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(subassembly_pickup, SLOT(PickupSubassembly_start()), SIGNAL(PickupSubassembly_finished()));
  }
  // ----------

  sub_pick_lay->addStretch(1);
}

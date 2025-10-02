/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBox>
#include <QLabel>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyAssemblyView.h>
#include <AssemblyAssemblyActionWidget.h>
#include <AssemblyAssemblyTextWidget.h>

AssemblyAssemblyView::AssemblyAssemblyView(const QObject* const assembly, QWidget* parent)
 : QWidget(parent)
 , smartMove_checkbox_(nullptr)
 , wid_PSSAlignm_(nullptr)
 , wid_PSSToSpacers_(nullptr)
 , wid_PSSToMaPSA_(nullptr)
 , wid_PSToBasep_(nullptr)
{
  if(assembly == nullptr)
  {
    NQLog("AssemblyAssemblyView", NQLog::Fatal) << "AssemblyAssemblyView(" << assembly << ", " << parent << ")"
       << ": null pointer to QObject --> GUI layout will not be created";

    return;
  }

  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  //// Assembly Options --------------------------------
  QHBoxLayout* opts_lay = new QHBoxLayout;
  layout->addLayout(opts_lay);

  smartMove_checkbox_ = new QCheckBox(tr("Use SmartMove"));

  opts_lay->addWidget(smartMove_checkbox_);

  connect(smartMove_checkbox_, SIGNAL(stateChanged(int)), assembly, SLOT(use_smartMove(int)));

  smartMove_checkbox_->setChecked(true);
  //// -------------------------------------------------

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  uint assembly_step_N(0);

  //// PS-s Alignment and Pick-up --------------------
  wid_PSSAlignm_ = new QWidget;

  toolbox->addItem(wid_PSSAlignm_, tr("[1] PS-s Alignment and Pickup"));

  QVBoxLayout* PSSAlignm_lay = new QVBoxLayout;
  wid_PSSAlignm_->setLayout(PSSAlignm_lay);

  // step: Place PS-s on Assembly Platform
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Place PS-s on Assembly Platform");
    PSSAlignm_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Vacuum on PS-s
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Enable Vacuum on PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumBaseplate_start()), SIGNAL(EnableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Go To Measurement Position on PS-s
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go To Measurement Position on PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToSensorMarkerPreAlignment_start()), SIGNAL(GoToSensorMarkerPreAlignment_finished()));
  }
  // ----------

  // step: Align PS-s to Motion Stage
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Align PS-s to Motion Stage (Go to \"Alignment\" Tab and select \"PS-s Sensor\")");
    PSSAlignm_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go From Sensor Marker Ref-Point to Pickup XY
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go From Sensor Marker Ref-Point to Pickup XY");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoFromSensorMarkerToPickupXY_start()), SIGNAL(GoFromSensorMarkerToPickupXY_finished()));
  }
  // ----------

  // step: Lower Pickup-Tool onto PS-s
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerPickupToolOntoPSS_start()), SIGNAL(LowerPickupToolOntoPSS_finished()));
  }
  // ----------

  // step: Enable Vacuum on Pickup-Tool
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Enable Vacuum on Pickup-Tool");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumPickupTool_start()), SIGNAL(EnableVacuumPickupTool_finished()));
  }
  // ----------

  // step: Disable Vacuum on PS-s
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Disable Vacuum on PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumBaseplate_start()), SIGNAL(DisableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Pick Up PS-s
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Pick Up PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(PickupPSS_start()), SIGNAL(PickupPSS_finished()));
  }
  // ----------

  PSSAlignm_lay->addStretch(1);
  //// -----------------------------------------------

  //// PS-s To Spacers -------------------------------
  wid_PSSToSpacers_ = new QWidget;

  toolbox->addItem(wid_PSSToSpacers_, tr("[2] PS-s onto Spacers"));

  QVBoxLayout* PSSToSpacers_lay = new QVBoxLayout;
  wid_PSSToSpacers_->setLayout(PSSToSpacers_lay);

  // step: Dispense Glue on Spacers and Place them on Assembly Platform
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Dispense Glue on Spacers and Place them on Assembly Platform");
    PSSToSpacers_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go To XYA Position To Glue PS-s to Spacers
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go To XYA Position To Glue PS-s to Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToXYAPositionToGluePSSToSpacers_start()), SIGNAL(GoToXYAPositionToGluePSSToSpacers_finished()));
  }
  // ----------

  // step: Enable Vacuum on Spacers
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Enable Vacuum on Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumSpacers_start()), SIGNAL(EnableVacuumSpacers_finished()));
  }
  // ----------

  // step: Lower PS-s onto Spacers
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Lower PS-s onto Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerPSSOntoSpacers_start()), SIGNAL(LowerPSSOntoSpacers_finished()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Wait for Glue To Cure (approx. 20 min)");
    PSSToSpacers_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Spacers Vacuum
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Disable Vacuum on Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumSpacers_start()), SIGNAL(DisableVacuumSpacers_finished()));
  }
  // ----------

  // step: Pick Up Spacers+PS-s
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Pick Up \"PS-s + Spacers\"");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(PickupPSSPlusSpacers_start()), SIGNAL(PickupPSSPlusSpacers_finished()));
  }
  // ----------

  PSSToSpacers_lay->addStretch(1);
  //// -----------------------------------------------

  //// PS-s To MaPSA ---------------------------------
  wid_PSSToMaPSA_ = new QWidget;

  toolbox->addItem(wid_PSSToMaPSA_, tr("[3] PS-p Alignment and Sensor-To-Sensor Assembly"));

  QVBoxLayout* PSSToMaPSA_lay = new QVBoxLayout;
  wid_PSSToMaPSA_->setLayout(PSSToMaPSA_lay);

  // step: Place PS-p on Assembly Platform
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Place MaPSA on Assembly Platform");
    PSSToMaPSA_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Enable Vacuum on MaPSA");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumBaseplate_start()), SIGNAL(EnableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Go To Measurement Position on PS-p
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go To Measurement Position on PS-p");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToSensorMarkerPreAlignment_start()), SIGNAL(GoToSensorMarkerPreAlignment_finished()));
  }
  // ----------

  // step: Align PS-p to Motion Stage
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Align PS-p to Motion Stage (Go to \"Alignment\" Tab and select \"PS-p Sensor\")");
    PSSToMaPSA_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go From Sensor Marker Ref-Point to Pickup XY
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go From Sensor Marker Ref-Point to Pickup XY");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoFromSensorMarkerToPickupXY_start()), SIGNAL(GoFromSensorMarkerToPickupXY_finished()));
  }
  // ----------

  // step: Apply "PS-p To PS-s" XY Offset
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Apply \"PS-p To PS-s\" XY Offset");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(ApplyPSPToPSSXYOffset_start()), SIGNAL(ApplyPSPToPSSXYOffset_finished()));
  }
  // ----------

  // step: Register "PS-s to MaPSA" XYZA Position
  // (position that will be used as starting point to lower "PS-s + Spacers" on MaPSA;
  //  the height corresponds to the best-focus height on PS-p surface, as resulting from the PS-p alignment,
  //  if the latter step completed successfully and was executed with auto-focusing enabled)
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Register \"PS-s to MaPSA\" XYZA Position");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(RegisterPSSPlusSpacersToMaPSAPosition_start()), SIGNAL(RegisterPSSPlusSpacersToMaPSAPosition_finished()));
  }
  // ----------

  // step: Go From "PS-s to MaPSA" Position to Gluing Stage (XY) Ref-Point
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go From \"PS-s To MaPSA\" Position to Gluing Stage (XY) Ref-Point");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start()), SIGNAL(GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished()));
  }
  // ----------

  // step: Lower Pickup-Tool onto Gluing Stage
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Lower Pickup-Tool onto Gluing Stage");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerPSSPlusSpacersOntoGluingStage_start()), SIGNAL(LowerPSSPlusSpacersOntoGluingStage_finished()));
  }
  // ----------

  // step: Return To "PS-s to MaPSA" XYZA Position
  // (step prior to lowering pickup tool; camera height must correspond to best-focus height on PS-p surface)
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Return To \"PS-s to MaPSA\" Position (XYZA)");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(ReturnToPSSPlusSpacersToMaPSAPosition_start()), SIGNAL(ReturnToPSSPlusSpacersToMaPSAPosition_finished()));
  }
  // ----------

  // step: Lower Pickup-Tool onto MaPSA
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Lower Pickup-Tool onto MaPSA");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerPSSPlusSpacersOntoMaPSA_start()), SIGNAL(LowerPSSPlusSpacersOntoMaPSA_finished()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Wait for Glue To Cure (approx. 20 min)");
    PSSToMaPSA_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Disable Vacuum on MaPSA");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumBaseplate_start()), SIGNAL(DisableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Pick Up Sensor Assembly ("MaPSA + Spacers + PS-s")
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Pick Up Sensor Assembly (\"PS-s + Spacers + MaPSA\")");
    PSSToMaPSA_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(PickupSensorAssembly_start()), SIGNAL(PickupSensorAssembly_finished()));
  }
  // ----------

  PSSToMaPSA_lay->addStretch(1);
  //// -----------------------------------------------

  //// Sensor Assembly to Baseplate ------------------
  wid_PSToBasep_ = new QWidget;

  toolbox->addItem(wid_PSToBasep_, tr("[4] Sensor Assembly to Baseplate"));

  QVBoxLayout* PSToBasep_lay = new QVBoxLayout;
  wid_PSToBasep_->setLayout(PSToBasep_lay);

  // step: Dispense Glue on Baseplate and Place it on Assembly Platform
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Dispense Glue on Baseplate and Place it on Assembly Platform");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Vacuum on Baseplate
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Enable Vacuum on Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumBaseplate_start()), SIGNAL(EnableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Go To XYA Position To Glue Sensor Assembly To Baseplate
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Go To XYA Position To Glue Sensor Assembly To Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToXYAPositionToGlueSensorAssemblyToBaseplate_start()), SIGNAL(GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished()));
  }
  // ----------

  // step: Lower Sensor Assembly onto Baseplate
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Lower Sensor Assembly onto Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerSensorAssemblyOntoBaseplate_start()), SIGNAL(LowerSensorAssemblyOntoBaseplate_finished()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Wait for Glue To Cure (approx. 20 min)");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Vacuum on Pickup-Tool
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Disable Vacuum on Pickup-Tool");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumPickupTool_start()), SIGNAL(DisableVacuumPickupTool_finished()));
  }
  // ----------

  // step: Lift Up Pickup-Tool
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Lift Up Pickup-Tool");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LiftUpPickupTool_start()), SIGNAL(LiftUpPickupTool_finished()));
  }
  // ----------

  // step: Remove Baseplate Pins from Assembly Platform
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Remove Baseplate Pins from Assembly Platform");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Vacuum on Baseplate
  {
    ++assembly_step_N;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->button()->setText("Disable Vacuum on Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumBaseplate_start()), SIGNAL(DisableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Remove PS Module from Assembly Platform
  {
    ++assembly_step_N;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N));
    tmp_wid->text()->setText("Remove PS Module from Assembly Platform");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  PSToBasep_lay->addStretch(1);
  //// -----------------------------------------------
}

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyAssemblyView::display_infoTab()
{
    QMessageBox::information(this, tr("Information - Assembly"),
            tr("<p>There is no available information about the content of this tab yet.</p>"));

    return;
}

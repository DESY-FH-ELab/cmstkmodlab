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

#include <AssemblyAssemblyView.h>
#include <AssemblyUtilities.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBox>
#include <QLabel>

AssemblyAssemblyView::AssemblyAssemblyView(const AssemblyAssembly* const assembly, QWidget* parent)
 : QWidget(parent)

 , smartMove_checkbox_(nullptr)

 , wid_CalibRotStage_(nullptr)
 , wid_PSSAlignm_    (nullptr)
 , wid_PSSToSpacers_ (nullptr)
 , wid_PSSToPSP_     (nullptr)
 , wid_PSToBasep_    (nullptr)
{
  if(assembly == nullptr)
  {
    NQLog("AssemblyAssemblyView", NQLog::Fatal) << "initialization error"
       << ": null pointer to AssemblyAssembly object, exiting constructor";

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

  assembly_step_N_ = 0;

//  //// Calibration of Rotation Stage -----------------
//  wid_CalibRotStage_ = new QWidget;
//
//  toolbox->addItem(wid_CalibRotStage_, tr("[0] Calibration of Rotation Stage"));
//
//  QVBoxLayout* CalibRotStage_lay = new QVBoxLayout;
//  wid_CalibRotStage_->setLayout(CalibRotStage_lay);
//
//  CalibRotStage_lay->addWidget(new QLabel);
//
//  // 1 go to   ref-point on rotation stage
//  // 2 measure ref-point on rotation stage
//
//  CalibRotStage_lay->addStretch(1);
//  //// -----------------------------------------------

  //// PS-s Alignment and Pick-up --------------------
  wid_PSSAlignm_ = new QWidget;

  toolbox->addItem(wid_PSSAlignm_, tr("[1] PS-s Alignment and Pickup"));

  QVBoxLayout* PSSAlignm_lay = new QVBoxLayout;
  wid_PSSAlignm_->setLayout(PSSAlignm_lay);

  // step: Place PS-s on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Place PS-s on Assembly Platform");
    PSSAlignm_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Sensor/Baseplate Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumBaseplate_start()), SIGNAL(EnableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Go To Measurement Position on PS-s
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To Measurement Position on PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToSensorMarkerPreAlignment_start()), SIGNAL(GoToSensorMarkerPreAlignment_finished()));
  }
  // ----------

  // step: Align PS-s to Motion Stage
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Align PS-s to Motion Stage (Go to \"Alignment\" Tab)");
    PSSAlignm_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go From Sensor Marker Ref-Point to Pickup XY
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go From Sensor Marker Ref-Point to Pickup XY");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoFromSensorMarkerToPickupXY_start()), SIGNAL(GoFromSensorMarkerToPickupXY_finished()));
  }
  // ----------

  // step: Lower Pickup-Tool onto PS-s
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerPickupToolOntoPSS_start()), SIGNAL(LowerPickupToolOntoPSS_finished()));
  }
  // ----------

  // step: Enable Pickup-Tool Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Pickup-Tool Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumPickupTool_start()), SIGNAL(EnableVacuumPickupTool_finished()));
  }
  // ----------

  // step: Disable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Sensor/Baseplate Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumBaseplate_start()), SIGNAL(DisableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Pick Up PS-s
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
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

  // step: Dispense Glue on Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Dispense Glue on Spacers and Place them on Assembly Platform");
    PSSToSpacers_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Vacuum on Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Vacuum on Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumSpacers_start()), SIGNAL(EnableVacuumSpacers_finished()));
  }
  // ----------

  // step: Go To XYA Position To Glue PS-s to Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XYA Position To Glue PS-s to Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToXYAPositionToGluePSSToSpacers_start()), SIGNAL(GoToXYAPositionToGluePSSToSpacers_finished()));
  }
  // ----------

  // step: Lower PS-s onto Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower PS-s onto Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerPSSOntoSpacers_start()), SIGNAL(LowerPSSOntoSpacers_finished()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Wait for Glue To Cure (approx. 25 min)");
    PSSToSpacers_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Spacers Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Vacuum on Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumSpacers_start()), SIGNAL(DisableVacuumSpacers_finished()));
  }
  // ----------

  // step: Pick Up Spacers+PS-s
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Pick Up Spacers+PS-s");
    PSSToSpacers_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(PickupSpacersAndPSS_start()), SIGNAL(PickupSpacersAndPSS_finished()));
  }
  // ----------

  PSSToSpacers_lay->addStretch(1);
  //// -----------------------------------------------

  //// PS-s To PS-p ----------------------------------
  wid_PSSToPSP_ = new QWidget;

  toolbox->addItem(wid_PSSToPSP_, tr("[3] PS-p Alignment and Sensor-To-Sensor Assembly"));

  QVBoxLayout* PSSToPSP_lay = new QVBoxLayout;
  wid_PSSToPSP_->setLayout(PSSToPSP_lay);

  // step: Place PS-p on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Place PS-p on Assembly Platform");
    PSSToPSP_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Sensor/Baseplate Vacuum");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumBaseplate_start()), SIGNAL(EnableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Go To Measurement Position on PS-p
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To Measurement Position on PS-p");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToSensorMarkerPreAlignment_start()), SIGNAL(GoToSensorMarkerPreAlignment_finished()));
  }
  // ----------

  // step: Align PS-p to Motion Stage
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Align PS-p to Motion Stage (Go to \"Alignment\" Tab)");
    PSSToPSP_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go From Sensor Marker Ref-Point to Pickup XY
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go From Sensor Marker Ref-Point to Pickup XY");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoFromSensorMarkerToPickupXY_start()), SIGNAL(GoFromSensorMarkerToPickupXY_finished()));
  }
  // ----------

  // step: Apply "PS-p To PS-s" XY Offset
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Apply \"PS-p To PS-s\" XY Offset");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(ApplyPSPToPSSXYOffset_start()), SIGNAL(ApplyPSPToPSSXYOffset_finished()));
  }
  // ----------

  // step: Register "PS-p To PS-s" XYZA Position (before lowering pickup tool, camera focused on PS-p surface)
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Register \"PS-p To PS-s XYZA Position\"");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(RegisterPSPToPSSPosition_start()), SIGNAL(RegisterPSPToPSSPosition_finished()));
  }
  // ----------

  // step: Go From "PS-p To PS-s" Position to Gluing Stage (XY) Ref-Point
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go From \"PS-p To PS-s Position\" to Gluing Stage (XY) Ref-Point");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoFromPSPToPSSPosToGluingStageRefPoint_start()), SIGNAL(GoFromPSPToPSSPosToGluingStageRefPoint_finished()));
  }
  // ----------

  // step: Lower Pickup-Tool onto Gluing Stage
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Pickup-Tool onto Gluing Stage");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerSpacersAndPSSOntoGluingStage_start()), SIGNAL(LowerSpacersAndPSSOntoGluingStage_finished()));
  }
  // ----------

  // step: Return To "PS-p To PS-s" XYZA Position (before lowering pickup tool, camera focused on PS-p surface)
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Return To \"PS-p To PS-s XYZA Position\"");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(ReturnToPSPToPSSPosition_start()), SIGNAL(ReturnToPSPToPSSPosition_finished()));
  }
  // ----------

  // step: Lower Pickup-Tool onto PS-p
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PS-p");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerSpacersAndPSSOntoPSP_start()), SIGNAL(LowerSpacersAndPSSOntoPSP_finished()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Wait for Glue To Cure (approx. 25 min)");
    PSSToPSP_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Sensor/Baseplate Vacuum");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumBaseplate_start()), SIGNAL(DisableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Pick Up "PS-p + PS-s"
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Pick Up \"PS-p + PS-s\"");
    PSSToPSP_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(PickupPSPAndPSS_start()), SIGNAL(PickupPSPAndPSS_finished()));
  }
  // ----------

  PSSToPSP_lay->addStretch(1);
  //// -----------------------------------------------

  //// "PS-p + PS-s" to Baseplate --------------------
  wid_PSToBasep_ = new QWidget;

  toolbox->addItem(wid_PSToBasep_, tr("[4] Sensor-To-Sensor Assembly onto Baseplate"));

  QVBoxLayout* PSToBasep_lay = new QVBoxLayout;
  wid_PSToBasep_->setLayout(PSToBasep_lay);

  // step: Dispense Glue on Baseplate and Place it on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Dispense Glue on Baseplate and Place it on Assembly Platform");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Sensor/Baseplate Vacuum");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(EnableVacuumBaseplate_start()), SIGNAL(EnableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Go To XYA Position To Glue Sensor Assembly To Baseplate
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XYA Position To Glue Sensor Assembly To Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(GoToXYAPositionToGlueSensorAssemblyToBaseplate_start()), SIGNAL(GoToXYAPositionToGlueSensorAssemblyToBaseplate_finished()));
  }
  // ----------

  // step: Lower Sensor Assembly onto Baseplate
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Sensor Assembly onto Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LowerSensorAssemblyOntoBaseplate_start()), SIGNAL(LowerSensorAssemblyOntoBaseplate_finished()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Wait for Glue To Cure (approx. 25 min)");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Disable Pickup-Tool Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Pickup-Tool Vacuum");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumPickupTool_start()), SIGNAL(DisableVacuumPickupTool_finished()));
  }
  // ----------

  // step: Lift Up Pickup-Tool
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lift Up Pickup-Tool");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(LiftUpPickupTool_start()), SIGNAL(LiftUpPickupTool_finished()));
  }
  // ----------

  // step: Disable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Sensor/Baseplate Vacuum");
    PSToBasep_lay->addWidget(tmp_wid);

    tmp_wid->connect_action(assembly, SLOT(DisableVacuumBaseplate_start()), SIGNAL(DisableVacuumBaseplate_finished()));
  }
  // ----------

  // step: Remove PS Module from Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Remove PS Module from Assembly Platform");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  PSToBasep_lay->addStretch(1);
  //// -----------------------------------------------

}
// ====================================================================================================

AssemblyAssemblyActionWidget::AssemblyAssemblyActionWidget(QWidget* parent)
 : QWidget(parent)

 , layout_  (nullptr)

 , label_   (nullptr)
 , button_  (nullptr)
 , checkbox_(nullptr)

 , qobject_    (nullptr)
 , start_slot_ (nullptr)
 , stop_signal_(nullptr)
{
//  connect(this, SIGNAL(move_relative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));

  // --------------

  // layout
  layout_ = new QHBoxLayout;
  this->setLayout(layout_);

  label_ = new QLabel;
  label_->setStyleSheet(
    "QLabel { font-weight : bold; }"
  );

  button_ = new QPushButton;
  button_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  checkbox_ = new QCheckBox("Done");

  connect(checkbox_, SIGNAL(stateChanged(int)), this, SLOT(disable(int)));

  layout_->addWidget(label_    ,  2, Qt::AlignRight);
  layout_->addWidget(button_   , 40);
  layout_->addWidget(new QLabel, 48);
  layout_->addWidget(checkbox_ , 10);
  // --------------
}

void AssemblyAssemblyActionWidget::disable(const bool to_be_disabled)
{
  this->disable(int(to_be_disabled ? 2 : 0));
}

void AssemblyAssemblyActionWidget::disable(const int state)
{
  if(state == 2)
  {
    label_ ->setEnabled(false);
    button_->setEnabled(false);
  }
  else if(state == 0)
  {
    label_ ->setEnabled(true);
    button_->setEnabled(true);
  }

  return;
}

void AssemblyAssemblyActionWidget::connect_action(const QObject* qobject, const char* start_slot, const char* stop_signal)
{
  if(qobject_){ this->reset_action(); }

  if(qobject)
  {
    qobject_     = qobject;
    start_slot_  = start_slot;
    stop_signal_ = stop_signal;

    connect(this->button(), SIGNAL(clicked()), this, SLOT(start_action()));
  }
  else
  {
    NQLog("AssemblyAssemblyActionWidget", NQLog::Warning) << "connect_action"
       << ": invalid (NULL) input pointer to QObject, no action taken";
  }
}

void AssemblyAssemblyActionWidget::reset_action()
{
  qobject_     = nullptr;
  start_slot_  = nullptr;
  stop_signal_ = nullptr;
}

void AssemblyAssemblyActionWidget::start_action()
{
  if(qobject_)
  {
    connect(this, SIGNAL(action_request()), qobject_, start_slot_);

    connect(qobject_, stop_signal_, this, SLOT(disable_action()));

    emit action_request();
  }
  else
  {
    NQLog("AssemblyAssemblyActionWidget", NQLog::Warning) << "start_action"
       << ": invalid (NULL) pointer to QObject, no action taken";
  }
}

void AssemblyAssemblyActionWidget::disable_action()
{
  if(qobject_)
  {
    disconnect(this, SIGNAL(action_request()), qobject_, start_slot_);

    disconnect(qobject_, stop_signal_, this, SLOT(disable_action()));

    checkbox_->setChecked(true);
  }
  else
  {
    NQLog("AssemblyAssemblyActionWidget", NQLog::Warning) << "disable_action"
       << ": invalid (NULL) pointer to QObject, no action taken";
  }
}
// ====================================================================================================

AssemblyAssemblyTextWidget::AssemblyAssemblyTextWidget(QWidget* parent)
 : QWidget(parent)

 , layout_  (nullptr)
 , label_   (nullptr)
 , text_    (nullptr)
 , checkbox_(nullptr)
{
//  connect(this, SIGNAL(move_relative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));

  // --------------

  // layout
  layout_ = new QHBoxLayout;
  this->setLayout(layout_);

  label_ = new QLabel;
  label_->setStyleSheet(
    "QLabel { font-weight : bold; }"
  );

  text_ = new QLabel;
  text_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  checkbox_ = new QCheckBox("Done");

  connect(checkbox_, SIGNAL(stateChanged(int)), this, SLOT(disable(int)));

  layout_->addWidget(label_    ,  2, Qt::AlignRight);
  layout_->addWidget(text_     , 40);
  layout_->addWidget(new QLabel, 48);
  layout_->addWidget(checkbox_ , 10);
  // --------------
}

void AssemblyAssemblyTextWidget::disable(const bool to_be_disabled)
{
  this->disable(int(to_be_disabled ? 2 : 0));
}

void AssemblyAssemblyTextWidget::disable(const int state)
{
  if(state == 2)
  {
    label_->setEnabled(false);
    text_ ->setEnabled(false);
  }
  else if(state == 0)
  {
    label_->setEnabled(true);
    text_ ->setEnabled(true);
  }

  return;
}
// ====================================================================================================

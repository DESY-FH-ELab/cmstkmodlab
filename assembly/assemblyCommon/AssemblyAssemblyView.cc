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
  smartMove_checkbox_->setChecked(false);

  opts_lay->addWidget(smartMove_checkbox_);

  connect(smartMove_checkbox_, SIGNAL(stateChanged(int)), assembly, SLOT(use_smartMove(int)));
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

  //// PSP To Baseplate ------------------------------
  wid_PSSAlignm_ = new QWidget;

  toolbox->addItem(wid_PSSAlignm_, tr("[1] PS-s Alignment and Pickup"));

  QVBoxLayout* PSSAlignm_lay = new QVBoxLayout;
  wid_PSSAlignm_->setLayout(PSSAlignm_lay);

  // step: Place PSS on Assembly Platform
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(EnableVacuumBaseplate_start()));
//    connect(assembly, SIGNAL(EnableVacuumBaseplate_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Go To Measurement Position on PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To Measurement Position on PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToSensorMarkerPreAlignment_start()));
//    connect(assembly, SIGNAL(GoToSensorMarkerPreAlignment_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Align PSS to Motion Stage
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Align PS-s to Motion Stage");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoFromSensorMarkerToPickupXY_start()));
//    connect(assembly, SIGNAL(GoFromSensorMarkerToPickupXY_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Lower Pickup-Tool onto PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LowerPickupToolOntoPSS_start()));
//    connect(assembly, SIGNAL(LowerPickupToolOntoPSS_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Enable Pickup-Tool Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Pickup-Tool Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(EnableVacuumPickupTool_start()));
//    connect(assembly, SIGNAL(EnableVacuumPickupTool_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Disable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Sensor/Baseplate Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(DisableVacuumBaseplate_start()));
//    connect(assembly, SIGNAL(DisableVacuumBaseplate_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Pick Up PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Pick Up PS-s");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(PickupPSS_start()));
//    connect(assembly, SIGNAL(PickupPSS_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  PSSAlignm_lay->addStretch(1);
  //// -----------------------------------------------

  //// PSS To Spacers --------------------------------
  wid_PSSToSpacers_ = new QWidget;

  toolbox->addItem(wid_PSSToSpacers_, tr("[2] PS-s onto Spacers"));

  QVBoxLayout* PSSToSpacers_lay = new QVBoxLayout;
  wid_PSSToSpacers_->setLayout(PSSToSpacers_lay);

  // step: Place Spacers on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Place Spacers on Assembly Platform");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(EnableVacuumSpacers_start()));
//    connect(assembly, SIGNAL(EnableVacuumSpacers_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Go To XYZA Spacer Ref-Point
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XYZA Spacer Ref-Point");
    PSSToSpacers_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToSpacerRefPoint_start()));
//    connect(assembly, SIGNAL(GoToSpacerRefPoint_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Go To XY Position To Glue Spacers to PS-s
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XY Position To Glue Spacers to PS-s");
    PSSToSpacers_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoFromSpacerRefPointToSpacerGluingXYPosition_start()));
//    connect(assembly, SIGNAL(GoFromSpacerRefPointToSpacerGluingXYPosition_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Dispense Fast-Curing Glue on Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Dispense Fast-Curing Glue on Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Lower PSS onto Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower PS-s onto Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LowerPSSOntoSpacers_start()));
//    connect(assembly, SIGNAL(LowerPSSOntoSpacers_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Wait for Glue To Cure (approx. 10 min)");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(DisableVacuumSpacers_start()));
//    connect(assembly, SIGNAL(DisableVacuumSpacers_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Pick Up Spacers+PS-s
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Pick Up Spacers+PS-s");
    PSSToSpacers_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(PickupSpacersAndPSS_start()));
//    connect(assembly, SIGNAL(PickupSpacersAndPSS_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  PSSToSpacers_lay->addStretch(1);
  //// -----------------------------------------------

  //// PSP To PSS ------------------------------------
  wid_PSSToPSP_ = new QWidget;

  toolbox->addItem(wid_PSSToPSP_, tr("[3] PS-p Alignment and Sensor-To-Sensor Assembly"));

  QVBoxLayout* PSSToPSP_lay = new QVBoxLayout;
  wid_PSSToPSP_->setLayout(PSSToPSP_lay);

  // step: Place PSP on Assembly Platform
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(EnableVacuumBaseplate_start()));
//    connect(assembly, SIGNAL(EnableVacuumBaseplate_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Go To Measurement Position on PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To Measurement Position on PS-p");
    PSSToPSP_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToSensorMarkerPreAlignment_start()));
//    connect(assembly, SIGNAL(GoToSensorMarkerPreAlignment_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Align PSP to Motion Stage
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Align PS-p to Motion Stage");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoFromSensorMarkerToPickupXY_start()));
//    connect(assembly, SIGNAL(GoFromSensorMarkerToPickupXY_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Apply PSP-To-PSS XY Offset
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Apply PSP-To-PSS XY Offset");
    PSSToPSP_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(ApplyPSPToPSSXYOffset_start()));
//    connect(assembly, SIGNAL(ApplyPSPToPSSXYOffset_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  ////
  //// This is where the steps for the Gluing Stage will be introduced
  ////

  // step: Lift Up Pickup-Tool For Glue Dispensing on Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lift Up Pickup-Tool For Glue Dispensing on Spacers");
    PSSToPSP_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LiftUpPickupTool_start()));
//    connect(assembly, SIGNAL(LiftUpPickupTool_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Dispense Both Glues On Underside Of Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Dispense Both Glues On Underside Of Spacers");
    PSSToPSP_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Lower Pickup-Tool onto PSP
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PS-p");
    PSSToPSP_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LowerSpacersAndPSSOntoPSP_start()));
//    connect(assembly, SIGNAL(LowerSpacersAndPSSOntoPSP_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Wait for Glue To Cure (approx. 10 min)");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(DisableVacuumBaseplate_start()));
//    connect(assembly, SIGNAL(DisableVacuumBaseplate_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Pick Up PSP+PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Pick Up PS-p+PS-s");
    PSSToPSP_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(PickupPSPAndPSS_start()));
//    connect(assembly, SIGNAL(PickupPSPAndPSS_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  PSSToPSP_lay->addStretch(1);
  //// -----------------------------------------------

  //// PSP+PSS to Baseplate
  wid_PSToBasep_ = new QWidget;

  toolbox->addItem(wid_PSToBasep_, tr("[4] Sensor-To-Sensor Assembly onto Baseplate"));

  QVBoxLayout* PSToBasep_lay = new QVBoxLayout;
  wid_PSToBasep_->setLayout(PSToBasep_lay);

  // step: Place Baseplate on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Place Baseplate on Assembly Platform");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(EnableVacuumBaseplate_start()));
//    connect(assembly, SIGNAL(EnableVacuumBaseplate_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Go To XYZA Baseplate Ref-Point
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XYZA Baseplate Ref-Point");
    PSToBasep_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToBaseplateRefPoint_start()));
//    connect(assembly, SIGNAL(GoToBaseplateRefPoint_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Dispense Fast-Curing Glue on Baseplate
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Dispense Fast-Curing Glue on Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Go To XY Position To Glue Sensor Assembly To Baseplate
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XY Position To Glue Sensor Assembly To Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoFromBaseplateRefPointToBaseplateGluingXYPosition_start()));
//    connect(assembly, SIGNAL(GoFromBaseplateRefPointToBaseplateGluingXYPosition_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Lower Sensor Assembly onto Baseplate
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Sensor Assembly onto Baseplate");
    PSToBasep_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LowerSensorAssemblyOntoBaseplate_start()));
//    connect(assembly, SIGNAL(LowerSensorAssemblyOntoBaseplate_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Wait For Glue To Cure
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Wait for Glue To Cure (approx. 10 min)");
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

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(DisableVacuumPickupTool_start()));
//    connect(assembly, SIGNAL(DisableVacuumPickupTool_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Lift Up Pickup-Tool
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lift Up Pickup-Tool");
    PSToBasep_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LiftUpPickupTool_start()));
//    connect(assembly, SIGNAL(LiftUpPickupTool_finished()), tmp_wid, SLOT(disable()));
  }
  // ----------

  // step: Disable Sensor/Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Sensor/Baseplate Vacuum");
    PSToBasep_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(DisableVacuumBaseplate_start()));
//    connect(assembly, SIGNAL(DisableVacuumBaseplate_finished()), tmp_wid, SLOT(disable()));
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

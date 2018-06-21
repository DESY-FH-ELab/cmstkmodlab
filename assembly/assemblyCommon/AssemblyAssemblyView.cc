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

  toolbox->addItem(wid_PSSAlignm_, tr("[1] PSS Alignment and Pickup"));

  QVBoxLayout* PSSAlignm_lay = new QVBoxLayout;
  wid_PSSAlignm_->setLayout(PSSAlignm_lay);

  // step: Place PSS on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Place PSS on Assembly Platform");
    PSSAlignm_lay->addWidget(tmp_wid);
  }
  // ----------

  // step: Enable Sensor Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Sensor Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(EnableVacuumBaseplate_start()));
  }
  // ----------

  // step: Go To Measurement Position on PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To Measurement Position on PSS");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToSensorMarkerPreAlignment_start()));
  }
  // ----------

  // step: Align PSS to Motion Stage
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Align PSS to Motion Stage");
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
  }
  // ----------

  // step: Lower Pickup-Tool onto PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower Pickup-Tool onto PSS");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(LowerPickupToolOntoPSS_start()));
  }
  // ----------

  // step: Disable Baseplate Vacuum
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Disable Baseplate Vacuum");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(DisableVacuumBaseplate_start()));
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
  }
  // ----------

  // step: Pick Up PSS
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Pick Up PSS");
    PSSAlignm_lay->addWidget(tmp_wid);

    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(PickupPSS_start()));
  }
  // ----------

  PSSAlignm_lay->addStretch(1);
  //// -----------------------------------------------

  //// PSS To Spacers --------------------------------
  wid_PSSToSpacers_ = new QWidget;

  toolbox->addItem(wid_PSSToSpacers_, tr("[2] PSS onto Spacers"));

  QVBoxLayout* PSSToSpacers_lay = new QVBoxLayout;
  wid_PSSToSpacers_->setLayout(PSSToSpacers_lay);

  // step: Put Spacers on Assembly Platform
  {
    ++assembly_step_N_;

    AssemblyAssemblyTextWidget* tmp_wid = new AssemblyAssemblyTextWidget;
    tmp_wid->label()->setText(QString::number(assembly_step_N_));
    tmp_wid->text() ->setText("Put Spacers on Assembly Platform");
    PSSToSpacers_lay->addWidget(tmp_wid);

//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
  }
  // ----------

  // step: Enable Vacuum on Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Enable Vacuum on Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
  }
  // ----------

  // step: Go To XYA for Spacers Gluing
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Go To XYA for Spacers Gluing");
    PSSToSpacers_lay->addWidget(tmp_wid);

//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
  }
  // ----------

  // step: Lower PSS onto Spacers
  {
    ++assembly_step_N_;

    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
    tmp_wid->button()->setText("Lower PSS onto Spacers");
    PSSToSpacers_lay->addWidget(tmp_wid);

//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
  }
  // ----------

//!!  // step: wait for glue to cure
//!!  {
//!!    ++assembly_step_N_;
//!!
//!!    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
//!!    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
//!!    tmp_wid->button()->setText("Arrange assembly platform for PSS alignment");
//!!    PSSToSpacers_lay->addWidget(tmp_wid);
//!!
//!!//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
//!!  }
//!!  // ----------
//!!
//!!  // step: disable spacers vacuum
//!!  {
//!!    ++assembly_step_N_;
//!!
//!!    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
//!!    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
//!!    tmp_wid->button()->setText("Arrange assembly platform for PSS alignment");
//!!    PSSToSpacers_lay->addWidget(tmp_wid);
//!!
//!!//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
//!!  }
//!!  // ----------
//!!
//!!  // step: move up pickup tool (dZ=+50cm)
//!!  {
//!!    ++assembly_step_N_;
//!!
//!!    AssemblyAssemblyActionWidget* tmp_wid = new AssemblyAssemblyActionWidget;
//!!    tmp_wid->label() ->setText(QString::number(assembly_step_N_));
//!!    tmp_wid->button()->setText("Arrange assembly platform for PSS alignment");
//!!    PSSToSpacers_lay->addWidget(tmp_wid);
//!!
//!!//    connect(tmp_wid->button(), SIGNAL(clicked()), assembly, SLOT(GoToPSSPreAlignment()));
//!!  }
//!!  // ----------

  PSSToSpacers_lay->addStretch(1);
  //// -----------------------------------------------

  //// PSP To PSS ------------------------------------
  wid_PSSToPSP_ = new QWidget;

  toolbox->addItem(wid_PSSToPSP_, tr("[3] PSP Alignment and PSP+PSS Assembly"));

  QVBoxLayout* PSSToPSP_lay = new QVBoxLayout;
  wid_PSSToPSP_->setLayout(PSSToPSP_lay);

  PSSToPSP_lay->addWidget(new QLabel);

  PSSToPSP_lay->addStretch(1);
  //// -----------------------------------------------

  //// PSP+PSS to Baseplate
  wid_PSToBasep_ = new QWidget;

  toolbox->addItem(wid_PSToBasep_, tr("[4] PSP+PSS onto Baseplate"));

  QVBoxLayout* PSToBasep_lay = new QVBoxLayout;
  wid_PSToBasep_->setLayout(PSToBasep_lay);

  PSToBasep_lay->addWidget(new QLabel);

  PSToBasep_lay->addStretch(1);
  //// -----------------------------------------------

  //  1 arrange assembly platform for PSS alignment

  //  2 place PSS on assembly platform

  //  3 enable sensor vacuum

  //  4 go to measurement position on PSS

  //  5 align PSS to motion-stage X-axis

  //  6 pick up PSS

  //  7 put  spacers on platform

  //  8 lock spacers on platform (vacuum)

  //  9 move to XYA for spacers gluing

  // 10 put down PSS on to spacers

  // 11 wait for glue to cure

  // 12 disable spacers vacuum

  // 13 move up pickup tool (dZ=+50cm)

  // -----------------------------------------------
  // -----------------------------------------------

  // 14 arrange assembly platform for PSP alignment
  ++assembly_step_N_;
  // ----------

  // 15 place PSP on assembly platform
  ++assembly_step_N_;
  // ----------

  // 16 enable sensor vacuum
  ++assembly_step_N_;
  // ----------

  // 17 go to measurement position on PSP
  ++assembly_step_N_;
  // ----------

  // 18 align PSP to motion-stage X-axis
  ++assembly_step_N_;
  // ----------

  // 19 move up pickup tool (dZ=+50.00cm)
  ++assembly_step_N_;
  // ----------

  //// this is where the steps for the
  //// gluing stage will be introduced

  // 20 apply glue on underside of spacers
  ++assembly_step_N_;
  // ----------

  // 21 move to XY for PSS onto PSP
  ++assembly_step_N_;
  // ----------

  // 22 lower PSS onto PSP
  ++assembly_step_N_;
  // ----------

  // 23 wait for glue to cure
  ++assembly_step_N_;
  // ----------

  // 24 pick up PSP+PSS
  ++assembly_step_N_;
  // ----------

  // -----------------------------------------------
  // -----------------------------------------------

  // 25 move to XYA to place PSP+PSS onto baseplate
  ++assembly_step_N_;
  // ----------

  // 26 switch on baseplate vacuum (after dispensing glue on the baseplate and positioning it on the assembly platform)
  ++assembly_step_N_;
  // ----------

  // 27 lower PSP+PSS onto baseplate
  ++assembly_step_N_;
  // ----------

  // 28 wait for glue to cure
  ++assembly_step_N_;
  // ----------

  // 29 finish assembly procedure: release pickup vacuum, release baseplate vacuum, move up pickup tool (dZ=+50.00cm)
  ++assembly_step_N_;
  // ----------
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

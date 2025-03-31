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

#include <AssemblyParametersView.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QToolBox>
#include <QLabel>
#include <sstream>

AssemblyParametersView::AssemblyParametersView(QWidget* parent)
 : QWidget(parent)

 , dime_wid_(nullptr)
 , posi_wid_(nullptr)
 , dist_wid_(nullptr)
 , imag_wid_(nullptr)

 , paramIO_button_read_ (nullptr)
 , paramIO_button_write_(nullptr)

 , config_(nullptr)
{
  config_ = ApplicationConfig::instance();

  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  // Get information on assembly center to accommodate for different parameters required
  std::string assembly_center_str = QString::fromStdString(config_->getValue<std::string>("main", "assembly_center")).toUpper().toStdString();
  if(assembly_center_str == "FNAL") {
      assembly_center_ = assembly::Center::FNAL;
  } else if(assembly_center_str == "BROWN") {
      assembly_center_ = assembly::Center::BROWN;
  } else if(assembly_center_str == "DESY") {
      assembly_center_ = assembly::Center::DESY;
  } else {
      NQLog("AssemblyAssemblyV2", NQLog::Warning) << "Invalid assembly center provided: \"" << assembly_center_str << "\". Provide one of the following options: \"FNAL\", \"BROWN\", \"DESY\"";
  }

  //// Display parameter filename -------------------
  QHBoxLayout* filename_lay = new QHBoxLayout;
  layout->addLayout(filename_lay);

  std::string parameter_filename = config_->getValue<std::string>("main", "AssemblyParameters_file_path");
  QString parameter_filepath = QFileInfo(QString::fromStdString(parameter_filename)).absoluteDir().absolutePath();

  QWidget* fn_wid = new QWidget;
  QGridLayout* fn_lay = new QGridLayout;
  fn_wid->setLayout(fn_lay);

  std::string fn_par_name = "AssemblyParameters_file_path";
  map_lineEdit_[fn_par_name] = new QLineEdit(tr(""));
  map_lineEdit_[fn_par_name]->setEnabled(false);
  map_lineEdit_[fn_par_name]->setMinimumWidth(700);
  map_lineEdit_[fn_par_name]->setStyleSheet("color: black;");

  fn_lay->addWidget(new QLabel("Assembly parameters filename"), 0, 0, Qt::AlignLeft);
  fn_lay->addWidget(this->get(fn_par_name), 0, 1, Qt::AlignRight);

  fn_lay->setColumnStretch(0,50);
  fn_lay->setColumnStretch(1,50);

  filename_lay->addWidget(fn_wid);

  //// Start toolbox

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  int row_index(-1);

  std::string tmp_tag("");
  QString     tmp_des("");

  //// DIMENSIONS ----------
  dime_wid_ = new QWidget;

  toolbox->addItem(dime_wid_, tr("Dimensions of Assembly Components [mm]"));

  QGridLayout* dime_lay = new QGridLayout;
  dime_wid_->setLayout(dime_lay);

  // dimension: thickness of Baseplate
  ++row_index;

  tmp_tag = "Thickness_Baseplate";
  tmp_des = "Thickness of Baseplate (incl. kapton sheet) :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);

  // dimension: thickness of PS-s Sensor
  ++row_index;

  tmp_tag = "Thickness_PSS";
  tmp_des = "Thickness of PS-s Sensor :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);

  // dimension: thickness of PS-p Sensor
  ++row_index;

  tmp_tag = "Thickness_PSP";
  tmp_des = "Thickness of PS-p Sensor :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);

  // dimension: thickness of MPA attached to PS-p (including bump-bond)
  ++row_index;

  tmp_tag = "Thickness_MPA";
  tmp_des = "Thickness of MPA (incl. bump-bond) :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);

  // dimension: thickness of Spacers
  ++row_index;

  tmp_tag = "Thickness_Spacer";
  tmp_des = "Thickness of Spacers :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);

  // dimension: height of spacer slots
  ++row_index;

  tmp_tag = "Depth_SpacerSlots";
  tmp_des = "Depth of Spacer Slots :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);

  // dimension: thickness of Glue Layer
  ++row_index;

  tmp_tag = "Thickness_GlueLayer";
  tmp_des = "Thickness of Glue Layer :";

  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

  dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
  dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
  dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);


  if(assembly_center_ == assembly::Center::FNAL || assembly_center_ == assembly::Center::BROWN)
  {
    // dimension: thickness of SpacerClamp
    ++row_index;

    tmp_tag = "Thickness_SpacerClamp";
    tmp_des = "Thickness of SpacerClamp :";

    map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

    dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
    dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
    dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);
  }

  if(assembly_center == assembly::Center::DESY)
  {
    // dimension: thickness of SpacerClamp
    ++row_index;

    tmp_tag = "Thickness_SubassemblyJig";
    tmp_des = "Thickness of Subassembly Pickup Jig :";

    map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));

    dime_lay->addWidget(new QLabel(tmp_des) , row_index, 0, Qt::AlignLeft);
    dime_lay->addWidget(new QLabel(tr("dZ")), row_index, 5, Qt::AlignRight);
    dime_lay->addWidget(this->get(tmp_tag)  , row_index, 6, Qt::AlignRight);
  }

  //// ---------------------

  row_index = -1;

  //// POSITIONS -----------
  posi_wid_ = new QWidget;

  toolbox->addItem(posi_wid_, tr("Reference Positions [mm, deg]"));

  QGridLayout* posi_lay = new QGridLayout;
  posi_wid_->setLayout(posi_lay);

  // position: XYZA position to point camera to sensor marker-1 (using reference mark on platform to place sensor)
  ++row_index;

  tmp_tag = "RefPointSensor";
  tmp_des = "Sensor Marker-1 Ref-Point :";

  map_lineEdit_[tmp_tag+"_X"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_Y"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_Z"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_A"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tmp_des)    , row_index, 0, Qt::AlignLeft);
  posi_lay->addWidget(new QLabel(tr("X"))    , row_index, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_X"), row_index, 2, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("Y"))    , row_index, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_Y"), row_index, 4, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("Z"))    , row_index, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_Z"), row_index, 6, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("A"))    , row_index, 7, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_A"), row_index, 8, Qt::AlignRight);

  button_moveAbsRefPos1_  = new QPushButton(tr("Move To Abs. Position"));
  posi_lay->addWidget(button_moveAbsRefPos1_, row_index, 9, Qt::AlignRight);

  // position: XYZA position of ref-point on assembly platform for calibration of baseplate
  ++row_index;

  tmp_tag = "RefPointPlatform";
  tmp_des = "Platform Ref-Point :";

  map_lineEdit_[tmp_tag+"_X"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_Y"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_Z"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_A"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tmp_des)    , row_index, 0, Qt::AlignLeft);
  posi_lay->addWidget(new QLabel(tr("X"))    , row_index, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_X"), row_index, 2, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("Y"))    , row_index, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_Y"), row_index, 4, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("Z"))    , row_index, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_Z"), row_index, 6, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("A"))    , row_index, 7, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_A"), row_index, 8, Qt::AlignRight);

  button_moveAbsRefPos2_  = new QPushButton(tr("Move To Abs. Position"));
  posi_lay->addWidget(button_moveAbsRefPos2_, row_index, 9, Qt::AlignRight);

  // position: z-position where camera is focused on Assembly Stage surface
  ++row_index;

  tmp_tag = "CameraFocusOnAssemblyStage";
  tmp_des = "Camera Focused on Assembly Stage Surface :";

  map_lineEdit_[tmp_tag+"_X"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_Y"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_Z"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_A"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tmp_des)    , row_index, 0, Qt::AlignLeft);
  posi_lay->addWidget(new QLabel(tr("X"))    , row_index, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_X"), row_index, 2, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("Y"))    , row_index, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_Y"), row_index, 4, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("Z"))    , row_index, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_Z"), row_index, 6, Qt::AlignRight);
  posi_lay->addWidget(new QLabel(tr("A"))    , row_index, 7, Qt::AlignRight);
  posi_lay->addWidget(this->get(tmp_tag+"_A"), row_index, 8, Qt::AlignRight);

  button_moveAbsRefPos4_  = new QPushButton(tr("Move To Abs. Position"));
  posi_lay->addWidget(button_moveAbsRefPos4_, row_index, 9, Qt::AlignRight);


  if(assembly_center_ == assembly::Center::FNAL || assembly_center_ == assembly::Center::BROWN)
  {
    // position: z-position where camera is focused on Gluing Stage surface
    ++row_index;

    tmp_tag = "CameraFocusOnGluingStage";
    tmp_des = "Camera Focused on Gluing Stage Surface :";

    map_lineEdit_[tmp_tag+"_X"] = new QLineEdit(tr(""));
    map_lineEdit_[tmp_tag+"_Y"] = new QLineEdit(tr(""));
    map_lineEdit_[tmp_tag+"_Z"] = new QLineEdit(tr(""));
    map_lineEdit_[tmp_tag+"_A"] = new QLineEdit(tr(""));

    posi_lay->addWidget(new QLabel(tmp_des)    , row_index, 0, Qt::AlignLeft);
    posi_lay->addWidget(new QLabel(tr("X"))    , row_index, 1, Qt::AlignRight);
    posi_lay->addWidget(this->get(tmp_tag+"_X"), row_index, 2, Qt::AlignRight);
    posi_lay->addWidget(new QLabel(tr("Y"))    , row_index, 3, Qt::AlignRight);
    posi_lay->addWidget(this->get(tmp_tag+"_Y"), row_index, 4, Qt::AlignRight);
    posi_lay->addWidget(new QLabel(tr("Z"))    , row_index, 5, Qt::AlignRight);
    posi_lay->addWidget(this->get(tmp_tag+"_Z"), row_index, 6, Qt::AlignRight);
    posi_lay->addWidget(new QLabel(tr("A"))    , row_index, 7, Qt::AlignRight);
    posi_lay->addWidget(this->get(tmp_tag+"_A"), row_index, 8, Qt::AlignRight);

    button_moveAbsRefPos5_  = new QPushButton(tr("Move To Abs. Position")); //NB: make sure that the priority is correctly set between the XYA and Z movements (otherwise the robot arm may crash into the platform...)
    posi_lay->addWidget(button_moveAbsRefPos5_, row_index, 9, Qt::AlignRight);
    //// ---------------------

  }

  row_index = -1;

  //// DISTANCES -----------
  dist_wid_ = new QWidget;
  dist_wid_->setMinimumHeight(5);
  toolbox->addItem(dist_wid_, tr("Reference Distances [mm, deg]"));

  QGridLayout* dist_lay = new QGridLayout;
  dist_wid_->setLayout(dist_lay);

  // distance: angle between the camera frame and the XY reference frame of the motion stage
  ++row_index;

  tmp_tag = "AngleOfCameraFrameInRefFrame";
  tmp_des = "Angle of Camera Frame in the XY Motion Stage Ref-Frame :";

  map_lineEdit_[tmp_tag+"_dA"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dA"))    , row_index, 7, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dA"), row_index, 8, Qt::AlignRight);

  button_moveRelRefDist1_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist1_, row_index, 9, Qt::AlignLeft);

  // distance: from Sensor Ref-Point to Sensor Pickup Position
  ++row_index;

  tmp_tag = "FromSensorRefPointToSensorPickup";
  tmp_des = "From Sensor Ref-Point to Sensor Pickup Position :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist2_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist2_, row_index, 9, Qt::AlignLeft);

  // distance: shift from PS-p marker-1 Ref-Point to PS-s marker-1 Ref-Point
  ++row_index;

  tmp_tag = "FromPSPRefPointToPSSRefPoint";
  tmp_des = "From PS-p Ref-Point To PS-s Ref-Point :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist3_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist3_, row_index, 9, Qt::AlignLeft);

  // distance: from Spacer's Edge to PS-s Sensor Ref-Point
  ++row_index;

  tmp_tag = "FromSpacerEdgeToPSSRefPoint";
  tmp_des = "From Spacer's Edge to PS-s Ref-Point :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist4_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist4_, row_index, 9, Qt::AlignLeft);

  // distance: from Edge of PS-p Sensor to PS-p Ref-Point
  ++row_index;

  tmp_tag = "FromPSPEdgeToPSPRefPoint";
  tmp_des = "From Edge of PS-p Sensor to PS-p Ref-Point :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist5_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist5_, row_index, 9, Qt::AlignLeft);

  // distance: from PSS-To-PSP Position to PSS-To-GluingStage Position
  ++row_index;

  tmp_tag = "FromPSSPlusSpacersToMaPSAPositionToGluingStage";
  tmp_des = "From PSs-To-PSp Position to PSs-To-GluingStage Position :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist6_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist6_, row_index, 9, Qt::AlignLeft);

  // distance: from best-focus z-position to pickup z-position
  ++row_index;

  tmp_tag = "FromCameraBestFocusToPickupHeight";
  tmp_des = "From Best-Focus Height to Pickup Height :";

  map_lineEdit_[tmp_tag+"_dZ"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dZ"))    , row_index, 5, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dZ"), row_index, 6, Qt::AlignRight);

  button_moveRelRefDist7_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist7_, row_index, 9, Qt::AlignLeft);

  // distance: from ref-point on assembly platform (for spacers' calibration) to spacer's edge
  ++row_index;

  tmp_tag = "FromRefPointPlatformToSpacerEdge";
  tmp_des = "From Platform Ref-Point to Spacer's Edge :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist8_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist8_, row_index, 9, Qt::AlignLeft);

  // distance: from ref-point on assembly platform (for baseplate's calibration) to edge of PS-p sensor
  ++row_index;

  tmp_tag = "FromRefPointPlatformToPSPEdge";
  tmp_des = "From Platform Ref-Point to PS-p Edge :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist9_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist9_, row_index, 9, Qt::AlignLeft);

  // Add separator line

  ++row_index;
  QFrame* line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  dist_lay->addWidget(line, row_index, 0, 1, 10);

  // distance: from reference marker 1 ("top-left" marker when facing the platform) to reference marker 2 ("top-right")
  // Add buttons to perform +dX/-dX relative movements from one marker to the other, both for PSS and PSP
  // Contrary to other parameters, these values are read from the "assembly.cfg" file (hardcoded design values, not calibrated -- different for glass/silicon)
  ++row_index;
  tmp_tag = "AssemblyObjectAlignerView_PSP_deltaX";
  tmp_des = "From PSP marker 1 -> 2 (dX>0) :";
  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag), row_index, 2, Qt::AlignRight);
  button_moveRelRefDist10_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist10_, row_index, 9, Qt::AlignLeft);
  
  ++row_index;
  tmp_tag = "AssemblyObjectAlignerView_PSP_deltaX_neg";
  tmp_des = "From PSP marker 2 -> 1 (dX<0) :";
  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag), row_index, 2, Qt::AlignRight);
  button_moveRelRefDist11_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist11_, row_index, 9, Qt::AlignLeft);

  ++row_index;
  tmp_tag = "AssemblyObjectAlignerView_PSS_deltaX";
  tmp_des = "From PSS marker 1 -> 2 (dX>0) :";
  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag), row_index, 2, Qt::AlignRight);
  button_moveRelRefDist12_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist12_, row_index, 9, Qt::AlignLeft);

  ++row_index;
  tmp_tag = "AssemblyObjectAlignerView_PSS_deltaX_neg";
  tmp_des = "From PSS marker 2 -> 1 (dX<0) :";
  map_lineEdit_[tmp_tag] = new QLineEdit(tr(""));
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag), row_index, 2, Qt::AlignRight);
  button_moveRelRefDist13_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist13_, row_index, 9, Qt::AlignLeft);

  //// ---------------------

  copy_values();

  //// ---------------------

  layout->addStretch(1);

  QHBoxLayout* paramIO_lay = new QHBoxLayout;

  paramIO_button_read_  = new QPushButton(tr("Read From File"));

  paramIO_button_write_ = new QPushButton(tr("Write To File"));

  connect(paramIO_button_read_ , SIGNAL(clicked()), this, SLOT( read_parameters()));
  connect(paramIO_button_write_, SIGNAL(clicked()), this, SLOT(write_parameters()));

  paramIO_lay->addWidget(paramIO_button_read_ , 50);
  paramIO_lay->addWidget(paramIO_button_write_, 50);

  layout->addLayout(paramIO_lay);

  // Connections to motion actions
  connect(button_moveAbsRefPos1_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos1()));
  connect(button_moveAbsRefPos2_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos2()));
  connect(button_moveAbsRefPos4_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos4()));
  connect(this , SIGNAL(click_moveToAbsRefPos(int)), this, SLOT(askConfirmMoveToAbsRefPoint(int)));
  if(assembly_center_ == assembly::Center::FNAL || assembly_center_ == assembly::Center::BROWN)
  {
    connect(button_moveAbsRefPos5_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos5()));
  }

  connect(button_moveRelRefDist1_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist1()));
  connect(button_moveRelRefDist2_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist2()));
  connect(button_moveRelRefDist3_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist3()));
  connect(button_moveRelRefDist4_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist4()));
  connect(button_moveRelRefDist5_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist5()));
  connect(button_moveRelRefDist6_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist6()));
  connect(button_moveRelRefDist7_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist7()));
  connect(button_moveRelRefDist8_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist8()));
  connect(button_moveRelRefDist9_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist9()));
  connect(button_moveRelRefDist10_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist10()));
  connect(button_moveRelRefDist11_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist11()));
  connect(button_moveRelRefDist12_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist12()));
  connect(button_moveRelRefDist13_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist13()));
  connect(this , SIGNAL(click_moveByRelRefDist(int)), this, SLOT(askConfirmMoveByRelRefDist(int)));

  connect(config_, SIGNAL(valueChanged()), this, SLOT(copy_values()));
  connect(config_, SIGNAL(structureChanged()), this, SLOT(copy_values()));

  // Connections to text changes
  for(const auto& key : this->entries_map())
  {
    connect(this->get(key.first), SIGNAL(textEdited(const QString&)), this, SLOT(overwriteParameter(const QString&)));
  }
}

AssemblyParametersView::~AssemblyParametersView()
{
    //-- Disconnect slots
    disconnect(paramIO_button_read_ , SIGNAL(clicked()), this, SLOT( read_parameters()));
    disconnect(paramIO_button_write_, SIGNAL(clicked()), this, SLOT(write_parameters()));

    //Disconnections
    disconnect(button_moveAbsRefPos1_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos1()));
    disconnect(button_moveAbsRefPos2_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos2()));
    disconnect(button_moveAbsRefPos4_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos4()));
    disconnect(this , SIGNAL(click_moveToAbsRefPos(int)), this, SLOT(askConfirmMoveToAbsRefPoint(int)));
    if(assembly_center_ == assembly::Center::FNAL || assembly_center_ == assembly::Center::BROWN)
    {
      disconnect(button_moveAbsRefPos5_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos5()));
    }

    disconnect(button_moveRelRefDist1_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist1()));
    disconnect(button_moveRelRefDist2_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist2()));
    disconnect(button_moveRelRefDist3_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist3()));
    disconnect(button_moveRelRefDist4_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist4()));
    disconnect(button_moveRelRefDist5_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist5()));
    disconnect(button_moveRelRefDist6_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist6()));
    disconnect(button_moveRelRefDist7_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist7()));
    disconnect(button_moveRelRefDist8_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist8()));
    disconnect(button_moveRelRefDist9_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist9()));
    disconnect(button_moveRelRefDist10_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist10()));
    disconnect(button_moveRelRefDist11_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist11()));
    disconnect(button_moveRelRefDist12_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist12()));
    disconnect(button_moveRelRefDist13_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist13()));
    disconnect(this , SIGNAL(click_moveByRelRefDist(int)), this, SLOT(askConfirmMoveByRelRefDist(int)));
}

void AssemblyParametersView::read_parameters()
{
  auto input_file = config_->getValue<std::string>("main", "AssemblyParameters_file_path");
  auto input_path = QFileInfo(QString::fromStdString(input_file)).absoluteDir().absolutePath();

  const QString f_path = QFileDialog::getOpenFileName(this, tr("Read Parameters"), input_path, tr("*.cfg"));

  if(f_path.isNull())
  {
    NQLog("AssemblyParametersView", NQLog::Critical) << "read_parameters"
       << ": empty path to input file for assembly parameters, no further action taken";

    return;
  }

  NQLog("AssemblyParametersView", NQLog::Spam) << "read_parameters"
     << ": reading parameters from file: " << f_path;

  config_->setValue<std::string>("main", "AssemblyParameters_file_path", f_path.toStdString());

  config_->append(f_path.toStdString(), "parameters");
  this->copy_values();
}

void AssemblyParametersView::write_parameters()
{
  auto input_file = config_->getValue<std::string>("main", "AssemblyParameters_file_path");
  auto input_path = QFileInfo(QString::fromStdString(input_file)).absoluteDir().absolutePath();

  QString f_path = QFileDialog::getSaveFileName(this, tr("Write Parameters"), input_path, tr("*.cfg"));

  if(f_path.isNull())
  {
    NQLog("AssemblyParametersView", NQLog::Critical) << "write_parameters"
       << ": empty path to output file for assembly parameters, no further action taken";

    return;
  }

  if(QFileInfo(f_path).suffix().isEmpty() && !f_path.endsWith('.'))
  {
    f_path.append(QString(".cfg"));
    NQLog("AssemblyParametersView", NQLog::Spam) << "write_parameters"
       << ": adding suffix to file path, which is now: " << f_path;
  } else if(f_path.endsWith('.'))
  {
    f_path.append(QString("cfg"));
    NQLog("AssemblyParametersView", NQLog::Spam) << "write_parameters"
       << ": adding suffix to file path, which is now: " << f_path;
  }

  NQLog("AssemblyParametersView", NQLog::Spam) << "write_parameters"
     << ": calling configuration to save parameters as " << f_path;

  config_->setValue<std::string>("main", "AssemblyParameters_file_path", f_path.toStdString());

  config_->saveAs(f_path.toStdString(), "parameters");
}

bool AssemblyParametersView::has(const std::string& key) const
{
  return bool(map_lineEdit_.find(key) != map_lineEdit_.end());
}

QLineEdit* AssemblyParametersView::get(const std::string& key) const
{
  QLineEdit* ptr(nullptr);

  if(this->has(key) == false)
  {
    NQLog("AssemblyParametersView", NQLog::Fatal) << "get"
       << ": no QLineEdit object associated to parameter key \"" << key << "\", closing application";

    assembly::kill_application(tr("[AssemblyParametersView::get]"), tr("Failed to find QLineEdit for key: \"%1\"\n.").arg(QString(key.c_str())));
  }
  else
  {
    ptr = map_lineEdit_.at(key);
  }

  if(ptr == nullptr)
  {
    NQLog("AssemblyParametersView", NQLog::Fatal) << "get"
       << ": null pointer to QLineEdit associated to key \"" << key << "\", closing application";

    assembly::kill_application(tr("[AssemblyParametersView::get]"), tr("Null pointer to QLineEdit for key: \"%1\"\n.").arg(QString(key.c_str())));
  }

  return ptr;
}

void AssemblyParametersView::overwriteParameter(const QString& value)
{
  QLineEdit* ptr_qedit = qobject_cast<QLineEdit*>(sender()); //Get pointer address of QLineEdit that triggered the SIGNAL(textChanged)

  // Identify the sender an its key
  bool keyFound = false;
  for(const auto& key : this->entries_map())
  {
    if(key.first == "AssemblyParameters_file_path")
    {
      continue;
    }

    if(ptr_qedit == this->get(key.first))
    {
      keyFound = true;
      bool conversion_ok;
      double val = value.toDouble(&conversion_ok);
      if(conversion_ok)
      {
        NQLog("AssemblyParametersView", NQLog::Spam) << "overwriteParameter"
           << ": changing parameter " << key.first << " to " << val;
        ptr_qedit->setStyleSheet("");
        config_->setValue("parameters", key.first, value);
        ptr_qedit->setText(value);
      } else
      {
        NQLog("AssemblyParametersView", NQLog::Fatal) << "overwriteParameter"
           << ": changed parameter for " << key.first << " cannot be casted to double: " << value;
        ptr_qedit->setStyleSheet("border: 3px solid red");
      }
    }
  }

  if(!keyFound)
  {
    NQLog("AssemblyParametersView", NQLog::Fatal) << "overwriteParameter"
       << ": no parameter found for this LineEdit";
  }
}

std::map<std::string, std::string> AssemblyParametersView::entries_map() const
{
  std::map<std::string, std::string> map_str;

  for(const auto& key : map_lineEdit_)
  {
    QLineEdit* const ptr = this->get(key.first);

    map_str[key.first] = ptr->text().toUtf8().constData();
  }

  return map_str;
}

void AssemblyParametersView::transmit_entries()
{
  const std::map<std::string, std::string> map_str = this->entries_map();

  NQLog("AssemblyParametersView", NQLog::Spam) << "transmit_entries"
     << ": emitting signal \"entries\"";

  emit entries(map_str);
}

void AssemblyParametersView::copy_values()
{
  //-- Values read from parameters (<-> calibrations) file
  std::vector<std::string> exceptions;
  exceptions.push_back("AssemblyObjectAlignerView_PSP_deltaX");
  exceptions.push_back("AssemblyObjectAlignerView_PSP_deltaX_neg");
  exceptions.push_back("AssemblyObjectAlignerView_PSS_deltaX");
  exceptions.push_back("AssemblyObjectAlignerView_PSS_deltaX_neg");
  exceptions.push_back("AssemblyParameters_file_path");

  for(const auto& key : map_lineEdit_)
  {
    if (std::count(exceptions.begin(), exceptions.end(), key.first))
        continue;

    this->setText(key.first, config_->getValue<double>("parameters", key.first));
  }

  //-- Values read from config file
  //std::cout<<"===== AssemblyObjectAlignerView_PSS_deltaX = "<<config->getValue<std::string>(tmp)<<std::endl;
  std::string tmp(""); double d=0; std::stringstream ss;

  tmp = "AssemblyObjectAlignerView_PSP_deltaX";
  this->setText(tmp, config_->getValue<double>("main", tmp));
  this->setText(tmp+"_neg", -config_->getValue<double>("main", tmp));

  tmp = "AssemblyObjectAlignerView_PSS_deltaX";
  this->setText(tmp, config_->getValue<double>("main", tmp));
  this->setText(tmp+"_neg", -config_->getValue<double>("main", tmp));

  tmp = "AssemblyParameters_file_path";
  this->setText(tmp, config_->getValue<std::string>("main", tmp));

  return;
}

void AssemblyParametersView::setText(const std::string& key, const double val)
{
  QLineEdit* const ptr = this->get(key);

  std::stringstream strs;
  strs << val;

  ptr->setText(QString::fromStdString(strs.str()));

  return;
}

void AssemblyParametersView::setText(const std::string& key, const std::string val)
{
  QLineEdit* const ptr = this->get(key);

  std::stringstream strs;
  strs << val;

  ptr->setText(QString::fromStdString(strs.str()));

  return;
}

//Prompt y/n message box. If yes, move to selected ref point absolute position
void AssemblyParametersView::askConfirmMoveToAbsRefPoint(int refPoint)
{
    //Get the coordinates (in GUI) of the selected refPoint
    std::string tmp_tag("");
    switch(refPoint)
    {
        case 1: tmp_tag = "RefPointSensor";
            break;
        case 2: tmp_tag = "RefPointPlatform";
            break;
        case 4: tmp_tag = "CameraFocusOnAssemblyStage";
            break;
        case 5: tmp_tag = "CameraFocusOnGluingStage";
            break;
        default: return;
    }
    double x = this->get(tmp_tag+"_X")->text().toDouble();
    double y = this->get(tmp_tag+"_Y")->text().toDouble();
    double z = this->get(tmp_tag+"_Z")->text().toDouble();
    double a = this->get(tmp_tag+"_A")->text().toDouble();

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Move to absolute position"));
    msgBox.setText(QString("<p style=\"color:#dd4f0b\"><b>x=%1 / y=%2 / z=%3 / a=%4.</b></p>").arg(x).arg(y).arg(z).arg(a));
    msgBox.setInformativeText("Do you want to move to this absolute position?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }

    NQLog("AssemblyParametersView", NQLog::Spam) << "askConfirmMoveToAbsRefPoint"
       << ": request move to absolute ref. position " << refPoint << " (x="<<x<<" / y="<<y<<" / z="<<z<<" / a="<<a<<")";

    emit request_moveToAbsRefPosition(x,y,z,a);

    return;
}

//Prompt y/n message box. If yes, move by selected relative distance
//Possible improvement: highlight the row currently hovered by the mouse (to avoid clicking button corresponding to a different row)
void AssemblyParametersView::askConfirmMoveByRelRefDist(int refPoint)
{
    //Get the coordinates (in GUI) of the selected refPoint
    std::string tmp_tag("");
    switch(refPoint)
    {
        case 1: tmp_tag = "AngleOfCameraFrameInRefFrame";
            break;
        case 2: tmp_tag = "FromSensorRefPointToSensorPickup";
            break;
        case 3: tmp_tag = "FromPSPRefPointToPSSRefPoint";
            break;
        case 4: tmp_tag = "FromSpacerEdgeToPSSRefPoint";
            break;
        case 5: tmp_tag = "FromPSPEdgeToPSPRefPoint";
            break;
        case 6: tmp_tag = "FromPSSPlusSpacersToMaPSAPositionToGluingStage";
            break;
        case 7: tmp_tag = "FromCameraBestFocusToPickupHeight";
            break;
        case 8: tmp_tag = "FromRefPointPlatformToSpacerEdge";
            break;
        case 9: tmp_tag = "FromRefPointPlatformToPSPEdge";
            break;
        case 10: tmp_tag = "AssemblyObjectAlignerView_PSP_deltaX";
            break;
        case 11: tmp_tag = "AssemblyObjectAlignerView_PSP_deltaX_neg";
            break;
        case 12: tmp_tag = "AssemblyObjectAlignerView_PSS_deltaX";
            break;
        case 13: tmp_tag = "AssemblyObjectAlignerView_PSS_deltaX_neg";
            break;
        default: return;
    }

    //Get relative movements (not all 4 coordinates are available, depending on variable)
    double dx = 0., dy = 0., dz = 0., da = 0.;
    if(this->has(tmp_tag+"_dX") == true) {dx = this->get(tmp_tag+"_dX")->text().toDouble();}
    if(this->has(tmp_tag+"_dY") == true) {dy = this->get(tmp_tag+"_dY")->text().toDouble();}
    if(this->has(tmp_tag+"_dZ") == true) {dz = this->get(tmp_tag+"_dZ")->text().toDouble();}
    if(this->has(tmp_tag+"_dA") == true) {da = this->get(tmp_tag+"_dA")->text().toDouble();}

    //Different naming scheme for 'config file' values (full name, no suffix)
    if(this->has(tmp_tag) == true) {dx = this->get(tmp_tag)->text().toDouble();}

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Move by relative distance"));
    msgBox.setText(QString("<p style=\"color:#dd4f0b\"><b>dX=%1 / dY=%2 / dZ=%3 / dA=%4.</b></p>").arg(dx).arg(dy).arg(dz).arg(da));
    msgBox.setInformativeText("Do you want to apply this relative movement?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }

    NQLog("AssemblyParametersView", NQLog::Spam) << "askConfirmMoveByRelRefDist"
       << ": request move by relative ref. distance " << refPoint << " (dX="<<dx<<" / dY="<<dy<<" / dZ="<<dz<<" / dA="<<da<<")";

    emit request_moveByRelRefDistance(dx,dy,dz,da);

    return;
}

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ... . Use <br> for a newline in html ('\n' in plain text).
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyParametersView::display_infoTab()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Information - Parameters"));

    messageBox.setText(tr("<p>This tab stores reference values (dimensions, distances, etc.) necessary to the assembly procedure.<br>"
    "The values are read from a config file (whose path is hardcoded as option <i>AssemblyParameters_file_path</i> in the file <i>assembly/assembly.cfg)</i>, and may be edited interactively by the user in this interface (non-permanent changes).</p>"
    "<p style=color:orange>NB: make sure to read the proper config file, which differs for glass/silicon and different spacer thicknesses !</p>"
    "<p>[<b>Dimensions of Assembly Components</b>] -- Reference thickness values of assembly components, used to compute distances along the Z-axis and apply the corresponding movements during assembly.</p>"
    "<p>[<b>Reference Positions</b>] -- Absolute positions of several reference points <i>(related to assembly platform, spacer slots, gluing stage, ec.)</i> expressed in the reference frame of the motion stage (MS). These values must be determined manually as part of a periodic calibration procedure."
    "<ul>"
    "<li> <u>Sensor Marker-1 Ref Point</u>: position of the top-left fiducial marker of a sensor. This position is arbitrary and only used as a starting point for PatRec, which ultimately determines the exact marker position. The purpose of this constant is solely to provide a good starting point for the PatRec to run as fast as possible.</li>"
    "<li> <u>Baseplate Ref Point</u>: position of the inner edge of the right angle engraved on the bottom-left corner of the platform. This point provides all the necessary information about the positions of the platform in the MS reference frame, since all point-to-point distances on the platform are assumed to be known exactly.</li>"
    "<li> <u>Spacers Ref Point</u>: position of the same platform edge mentioned above, after a -90degrees rotation has been applied to the platform. The XYZ coordinates must be determined independently, while the angle is assumed to be shifted by exactly -90deg w.r.t. its initial value. <i>NB: the axis of rotation of the platform is not assumed to be known exactly, hence this position is measured rather than deduced from other quantities. [OBSOLETE with new assembly platform -- now identical to above]</i></li>"
    "</ul></p>"
    "<p>[<b>Reference Distances</b>] -- Distances between reference points.</p>"
    ));

    QSpacerItem* horizontalSpacer = new QSpacerItem(3000, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); //Use this to enlarge box width
    QGridLayout* layout = (QGridLayout*) messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    messageBox.exec();

    return;
}

//Dump all assembly parameters to DBlogfile (for archiving)
void AssemblyParametersView::Dump_UserValues_toDBlogfile(const QString& f_path)
{
    emit append_to_file_request(f_path);

    return;
}

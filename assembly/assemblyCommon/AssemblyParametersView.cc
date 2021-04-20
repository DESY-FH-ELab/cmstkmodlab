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

#include <AssemblyParametersView.h>
#include <AssemblyUtilities.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QToolBox>
#include <QLabel>

AssemblyParametersView::AssemblyParametersView(QWidget* parent)
 : QWidget(parent)

 , dime_wid_(nullptr)
 , posi_wid_(nullptr)
 , dist_wid_(nullptr)
 , imag_wid_(nullptr)

 , paramIO_button_read_ (nullptr)
 , paramIO_button_write_(nullptr)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

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

  tmp_tag = "PlatformRefPointCalibrationBaseplate";
  tmp_des = "[Calibration: Baseplate] Ref-Point :";

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

  // position: XYZA position of ref-point on assembly platform for calibration of spacers
  ++row_index;

  tmp_tag = "PlatformRefPointCalibrationSpacers";
  tmp_des = "[Calibration: Spacers] Ref-Point :";

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

  button_moveAbsRefPos3_  = new QPushButton(tr("Move To Abs. Position"));
  posi_lay->addWidget(button_moveAbsRefPos3_, row_index, 9, Qt::AlignRight);

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

  button_moveAbsRefPos5_  = new QPushButton(tr("Move To Abs. Position"));
  posi_lay->addWidget(button_moveAbsRefPos5_, row_index, 9, Qt::AlignRight);

  //// ---------------------

  row_index = -1;

  //// DISTANCES -----------
  dist_wid_ = new QWidget;

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
  map_lineEdit_[tmp_tag+"_dZ"] = new QLineEdit(tr(""));
  
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dZ"))    , row_index, 5, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dZ"), row_index, 6, Qt::AlignRight);

  // distance: shift from PSs Ref-Point to PSSTL Ref-Point
  ++row_index;

  tmp_tag = "FromPSSRefPointToPSSTLRefPoint";
  tmp_des = "From PSS Ref-Point To PSS TL Ref-Point :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dZ"] = new QLineEdit(tr(""));
  
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dZ"))    , row_index, 5, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dZ"), row_index, 6, Qt::AlignRight);


  // distance: shift from PSs Ref-Point to PSSTL Ref-Point
  ++row_index;

  tmp_tag = "FromPSSTLToPSPTL";
  tmp_des = "From PSSTL Fiducial To PSPTL Fiducial :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dZ"] = new QLineEdit(tr(""));
  
  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dZ"))    , row_index, 5, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dZ"), row_index, 6, Qt::AlignRight);

  


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

  tmp_tag = "FromPlatformRefPointCalibrationSpacersToSpacerEdge";
  tmp_des = "From Platform Ref-Point for Spacers-Calibration to Spacer's Edge :";

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

  tmp_tag = "FromPlatformRefPointCalibrationBaseplateToPSPEdge";
  tmp_des = "From Platform Ref-Point for Baseplate-Calibration to PS-p Edge :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  button_moveRelRefDist9_  = new QPushButton(tr("Apply Relative Movement"));
  dist_lay->addWidget(button_moveRelRefDist9_, row_index, 9, Qt::AlignLeft);

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

  //Connections
  connect(button_moveAbsRefPos1_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos1()));
  connect(button_moveAbsRefPos2_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos2()));
  connect(button_moveAbsRefPos3_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos3()));
  connect(button_moveAbsRefPos4_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos4()));
  connect(button_moveAbsRefPos5_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos5()));
  connect(this , SIGNAL(click_moveToAbsRefPos(int)), this, SLOT(askConfirmMoveToAbsRefPoint(int)));

  connect(button_moveRelRefDist1_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist1()));
  connect(button_moveRelRefDist2_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist2()));
  connect(button_moveRelRefDist3_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist3()));
  connect(button_moveRelRefDist4_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist4()));
  connect(button_moveRelRefDist5_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist5()));
  connect(button_moveRelRefDist6_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist6()));
  connect(button_moveRelRefDist7_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist7()));
  connect(button_moveRelRefDist8_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist8()));
  connect(button_moveRelRefDist9_ , SIGNAL(clicked()), this, SLOT(moveByRelRefDist9()));
  connect(this , SIGNAL(click_moveByRelRefDist(int)), this, SLOT(askConfirmMoveByRelRefDist(int)));
}

AssemblyParametersView::~AssemblyParametersView()
{
    //-- Disconnect slots
    disconnect(paramIO_button_read_ , SIGNAL(clicked()), this, SLOT( read_parameters()));
    disconnect(paramIO_button_write_, SIGNAL(clicked()), this, SLOT(write_parameters()));

    //Disconnections
    disconnect(button_moveAbsRefPos1_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos1()));
    disconnect(button_moveAbsRefPos2_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos2()));
    disconnect(button_moveAbsRefPos3_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos3()));
    disconnect(button_moveAbsRefPos4_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos4()));
    disconnect(button_moveAbsRefPos5_ , SIGNAL(clicked()), this, SLOT(moveToAbsRefPos5()));
    disconnect(this , SIGNAL(click_moveToAbsRefPos(int)), this, SLOT(askConfirmMoveToAbsRefPoint(int)));

    disconnect(button_moveRelRefDist1_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist1()));
    disconnect(button_moveRelRefDist2_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist2()));
    disconnect(button_moveRelRefDist3_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist3()));
    disconnect(button_moveRelRefDist4_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist4()));
    disconnect(button_moveRelRefDist5_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist5()));
    disconnect(button_moveRelRefDist6_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist6()));
    disconnect(button_moveRelRefDist7_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist7()));
    disconnect(button_moveRelRefDist8_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist8()));
    disconnect(button_moveRelRefDist9_ , SIGNAL(clicked()), this, SLOT(click_moveByRelRefDist9()));
    disconnect(this , SIGNAL(click_moveByRelRefDist(int)), this, SLOT(askConfirmMoveByRelRefDist(int)));
}

void AssemblyParametersView::read_parameters()
{
  const QString f_path = QFileDialog::getOpenFileName(this, tr("Read Parameters"), QString::fromStdString(Config::CMSTkModLabBasePath+"/assembly/assembly"), tr("*.cfg"));

  if(f_path.isNull())
  {
    NQLog("AssemblyParametersView", NQLog::Critical) << "read_parameters"
       << ": empty path to input file for assembly parameters, no further action taken";

    return;
  }

  NQLog("AssemblyParametersView", NQLog::Spam) << "read_parameters"
     << ": emitting signal \"read_from_file_request(" << f_path << ")\"";

  emit read_from_file_request(f_path);
}

void AssemblyParametersView::write_parameters()
{
  const QString f_path = QFileDialog::getSaveFileName(this, tr("Write Parameters"), QString::fromStdString(Config::CMSTkModLabBasePath+"/assembly/assembly"), tr("*.cfg"));

  if(f_path.isNull())
  {
    NQLog("AssemblyParametersView", NQLog::Critical) << "write_parameters"
       << ": empty path to output file for assembly parameters, no further action taken";

    return;
  }

  NQLog("AssemblyParametersView", NQLog::Spam) << "write_parameters"
     << ": emitting signal \"write_to_file_request(" << f_path << ")\"";

  emit write_to_file_request(f_path);
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

void AssemblyParametersView::copy_values(const std::map<std::string, double>& map_double)
{
  for(const auto& key : map_double)
  {
    this->setText(key.first, key.second);
  }

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

//Prompt y/n message box. If yes, move to selected ref point absolute position
void AssemblyParametersView::askConfirmMoveToAbsRefPoint(int refPoint)
{
    //Get the coordinates (in GUI) of the selected refPoint
    std::string tmp_tag("");
    switch(refPoint)
    {
        case 1: tmp_tag = "RefPointSensor";
            break;
        case 2: tmp_tag = "PlatformRefPointCalibrationBaseplate";
            break;
        case 3: tmp_tag = "PlatformRefPointCalibrationSpacers";
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
        case 3: tmp_tag = "FromPSSTLToPSPTL";
            break;
        case 4: tmp_tag = "FromSpacerEdgeToPSSRefPoint";
            break;
        case 5: tmp_tag = "FromPSPEdgeToPSPRefPoint";
            break;
        case 6: tmp_tag = "FromPSSPlusSpacersToMaPSAPositionToGluingStage";
            break;
        case 7: tmp_tag = "FromCameraBestFocusToPickupHeight";
            break;
        case 8: tmp_tag = "FromPlatformRefPointCalibrationSpacersToSpacerEdge";
            break;
        case 9: tmp_tag = "FromPlatformRefPointCalibrationBaseplateToPSPEdge";
            break;
        default: return;
    }

    //Get relative movements (not all 4 coordinates are available, depending on variable)
    double dx = 0., dy = 0., dz = 0., da = 0.;
    if(this->has(tmp_tag+"_dX") == true) {dx = this->get(tmp_tag+"_dX")->text().toDouble();}
    if(this->has(tmp_tag+"_dY") == true) {dy = this->get(tmp_tag+"_dY")->text().toDouble();}
    if(this->has(tmp_tag+"_dZ") == true) {dz = this->get(tmp_tag+"_dZ")->text().toDouble();}
    if(this->has(tmp_tag+"_dA") == true) {da = this->get(tmp_tag+"_dA")->text().toDouble();}

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
    "The values are read from a config file (whose path is hardcoded as option <i>[AssemblyParameters_file_path]</i> in the file <i>[assembly/assembly.cfg])</i>, and may be edited interactively and non-permanently by the user.</p>"
    "<p style=color:orange>NB: make sure to read the proper config file, which differs for glass or silicon assembly !</p>"
    "<p>[<b>Dimensions of Assembly Components</b>] -- Reference thickness values of assembly components, used to compute distances along the Z-axis and apply the corresponding movements during assembly.</p>"
    "<p>[<b>Reference Positions</b>] -- Absolute positions of several reference points <i>(related to assembly platform, spacer slots, gluing stage, ec.)</i> expressed in the reference frame of the motion stage (MS). These values must be determined manually as part of a periodic calibration procedure."
    "<ul>"
    "<li> <u>Sensor Marker-1 Ref Point</u>: position of the top-left fiducial marker of a sensor. This position is arbitrary and only used as a starting point for PatRec, which ultimately determines the exact marker position. The purpose of this constant is solely to provide a good starting point for the PatRec to run as fast as possible.</li>"
    "<li> <u>Baseplate Ref Point</u>: position of the inner edge of the black right angle engraved on the bottom-left corner of the platform. This point provides all the necessary information about the positions of the platform in the MS reference frame, since all point-to-point distances on the platform are assumed to be known exactly.</li>"
    "<li> <u>Spacers Ref Point</u>: position of the same platform edge mentioned above, after a -90degrees rotation has been applied to the platform. The XYZ coordinates must be determined independently, while the angle is assumed to be shifted by exactly -90deg w.r.t. its initial value. <i>NB: the axis of rotation of the platform is not assumed to be known exactly, hence this position is measured rather than deduced from other quantities.</i></li>"
    "</ul></p>"
    "<p>[<b>Reference Distances</b>] -- Distances between reference points.</p>"
    ));

    QSpacerItem* horizontalSpacer = new QSpacerItem(3000, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); //Use this to enlarge box width
    QGridLayout* layout = (QGridLayout*) messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    messageBox.exec();

    return;
}

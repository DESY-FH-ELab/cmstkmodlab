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
  tmp_des = "Thickness of Baseplate :";

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

  // dimension: thickness of Spacer
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

  // distance: from Baseplate Ref-Point to PS-s Ref-Point relative to Baseplate
  ++row_index;

  tmp_tag = "FromPSPToPSSPosToGluingStage";
  tmp_des = "From PSS-To-PSP Position to PSS-To-GluingStage Position :";

  map_lineEdit_[tmp_tag+"_dX"] = new QLineEdit(tr(""));
  map_lineEdit_[tmp_tag+"_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dX"))    , row_index, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dX"), row_index, 2, Qt::AlignRight);
  dist_lay->addWidget(new QLabel(tr("dY"))    , row_index, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dY"), row_index, 4, Qt::AlignRight);

  // distance: from best-focus z-position to pickup z-position
  ++row_index;

  tmp_tag = "FromCameraBestFocusToPickupHeight";
  tmp_des = "From Best-Focus Height to Pickup Height :";

  map_lineEdit_[tmp_tag+"_dZ"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tmp_des)     , row_index, 0, Qt::AlignLeft);
  dist_lay->addWidget(new QLabel(tr("dZ"))    , row_index, 5, Qt::AlignRight);
  dist_lay->addWidget(this->get(tmp_tag+"_dZ"), row_index, 6, Qt::AlignRight);

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

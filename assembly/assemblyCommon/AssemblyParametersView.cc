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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QToolBox>
#include <QLabel>

AssemblyParametersView::AssemblyParametersView(QWidget* parent) :
  QWidget(parent),

  dime_wid_(nullptr),
  posi_wid_(nullptr),
  dist_wid_(nullptr),
  imag_wid_(nullptr),

  paramIO_button_read_ (nullptr),
  paramIO_button_write_(nullptr)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  //// DIMENSIONS ----------
  dime_wid_ = new QWidget;

  toolbox->addItem(dime_wid_, tr("Dimensions of Assembly Components [mm]"));

  QVBoxLayout* dime_lay = new QVBoxLayout;
  dime_wid_->setLayout(dime_lay);

  // dimension: baseplate's thickness
  QHBoxLayout* dime_thicknessBasep_lay = new QHBoxLayout;
  dime_lay->addLayout(dime_thicknessBasep_lay);

  map_lineEdit_["thickness_baseplate"] = new QLineEdit(tr(""));

  dime_thicknessBasep_lay->addWidget(new QLabel(tr("Baseplate's thickness :")), 50, Qt::AlignLeft);

  dime_thicknessBasep_lay->addWidget(new QLabel, 27);

  dime_thicknessBasep_lay->addWidget(new QLabel(tr("dZ"))            ,  8, Qt::AlignRight);
  dime_thicknessBasep_lay->addWidget(this->get("thickness_baseplate"), 15, Qt::AlignRight);

  // dimension: PSS sensor's thickness
  QHBoxLayout* dime_thicknessPSS_lay = new QHBoxLayout;
  dime_lay->addLayout(dime_thicknessPSS_lay);

  map_lineEdit_["thickness_PSS"] = new QLineEdit(tr(""));

  dime_thicknessPSS_lay->addWidget(new QLabel(tr("PSS Sensor's thickness :")), 50, Qt::AlignLeft);

  dime_thicknessPSS_lay->addWidget(new QLabel, 27);

  dime_thicknessPSS_lay->addWidget(new QLabel(tr("dZ"))      ,  8, Qt::AlignRight);
  dime_thicknessPSS_lay->addWidget(this->get("thickness_PSS"), 15, Qt::AlignRight);

  // dimension: PSP sensor's thickness
  QHBoxLayout* dime_thicknessPSP_lay = new QHBoxLayout;
  dime_lay->addLayout(dime_thicknessPSP_lay);

  map_lineEdit_["thickness_PSP"] = new QLineEdit(tr(""));

  dime_thicknessPSP_lay->addWidget(new QLabel(tr("PSP Sensor's thickness :")), 50, Qt::AlignLeft);

  dime_thicknessPSP_lay->addWidget(new QLabel, 27);

  dime_thicknessPSP_lay->addWidget(new QLabel(tr("dZ"))      ,  8, Qt::AlignRight);
  dime_thicknessPSP_lay->addWidget(this->get("thickness_PSP"), 15, Qt::AlignRight);

  // dimension: Spacer's thickness
  QHBoxLayout* dime_thicknessSpacer_lay = new QHBoxLayout;
  dime_lay->addLayout(dime_thicknessSpacer_lay);

  map_lineEdit_["thickness_spacer"] = new QLineEdit(tr(""));

  dime_thicknessSpacer_lay->addWidget(new QLabel(tr("Spacer's thickness :")), 50, Qt::AlignLeft);

  dime_thicknessSpacer_lay->addWidget(new QLabel, 27);

  dime_thicknessSpacer_lay->addWidget(new QLabel(tr("dZ"))         ,  8, Qt::AlignRight);
  dime_thicknessSpacer_lay->addWidget(this->get("thickness_spacer"), 15, Qt::AlignRight);

  // dimension: Glue Layer's thickness
  QHBoxLayout* dime_thicknessGlueLayer_lay = new QHBoxLayout;
  dime_lay->addLayout(dime_thicknessGlueLayer_lay);

  map_lineEdit_["thickness_glue_layer"] = new QLineEdit(tr(""));

  dime_thicknessGlueLayer_lay->addWidget(new QLabel(tr("Glue Layer's thickness :")), 50, Qt::AlignLeft);

  dime_thicknessGlueLayer_lay->addWidget(new QLabel, 27);

  dime_thicknessGlueLayer_lay->addWidget(new QLabel(tr("dZ"))             ,  8, Qt::AlignRight);
  dime_thicknessGlueLayer_lay->addWidget(this->get("thickness_glue_layer"), 15, Qt::AlignRight);

  //// ---------------------

  //// POSITIONS -----------
  posi_wid_ = new QWidget;

  toolbox->addItem(posi_wid_, tr("Reference Positions [mm, deg]"));

  QGridLayout* posi_lay = new QGridLayout;
  posi_wid_->setLayout(posi_lay);

  // position: XYZA position to point camera to sensor marker-1 (using reference mark on platform to place sensor)
  map_lineEdit_["refPointSensor_X"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointSensor_Y"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointSensor_Z"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointSensor_A"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tr("Sensor Marker-1 (using platform mark):")), 0, 0, Qt::AlignLeft);

  posi_lay->addWidget(new QLabel(tr("X"))          , 0, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSensor_X"), 0, 2, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Y"))          , 0, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSensor_Y"), 0, 4, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Z"))          , 0, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSensor_Z"), 0, 6, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("A"))          , 0, 7, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSensor_A"), 0, 8, Qt::AlignRight);

  // position: XYZA position to point camera to Spacer Ref-Point (bottom-left edge)
  map_lineEdit_["refPointSpacer_X"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointSpacer_Y"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointSpacer_Z"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointSpacer_A"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tr("Bottom-Left Edge of Spacer (aligned to X-axis):")), 1, 0, Qt::AlignLeft);

  posi_lay->addWidget(new QLabel(tr("X"))          , 1, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSpacer_X"), 1, 2, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Y"))          , 1, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSpacer_Y"), 1, 4, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Z"))          , 1, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSpacer_Z"), 1, 6, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("A"))          , 1, 7, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointSpacer_A"), 1, 8, Qt::AlignRight);

  // position: XYZA position to point camera on Baseplate Ref-Point
  map_lineEdit_["refPointBaseplate_X"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointBaseplate_Y"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointBaseplate_Z"] = new QLineEdit(tr(""));
  map_lineEdit_["refPointBaseplate_A"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tr("Ref-Point of Baseplate (aligned to X-axis):")), 2, 0, Qt::AlignLeft);

  posi_lay->addWidget(new QLabel(tr("X"))             , 2, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointBaseplate_X"), 2, 2, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Y"))             , 2, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointBaseplate_Y"), 2, 4, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Z"))             , 2, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointBaseplate_Z"), 2, 6, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("A"))             , 2, 7, Qt::AlignRight);
  posi_lay->addWidget(this->get("refPointBaseplate_A"), 2, 8, Qt::AlignRight);

  // position: Z-position (height) to put pickup tool in contact with rotation stage
  map_lineEdit_["pickupOnRotStage_Z"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tr("Pickup Position on Rotation Stage :")), 3, 0, Qt::AlignLeft);

  posi_lay->addWidget(new QLabel(tr("Z"))            , 3, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get("pickupOnRotStage_Z"), 3, 6, Qt::AlignRight);

  // position: XY pickup position on glue-dispensing platform + Z-position (height) to put pickup tool in contact with platform
  map_lineEdit_["pickupOnGluingStage_X"] = new QLineEdit(tr(""));
  map_lineEdit_["pickupOnGluingStage_Y"] = new QLineEdit(tr(""));
  map_lineEdit_["pickupOnGluingStage_Z"] = new QLineEdit(tr(""));

  posi_lay->addWidget(new QLabel(tr("Pickup Position on Glue-Dispensing platform :")), 4, 0, Qt::AlignLeft);

  posi_lay->addWidget(new QLabel(tr("X"))               , 4, 1, Qt::AlignRight);
  posi_lay->addWidget(this->get("pickupOnGluingStage_X"), 4, 2, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Y"))               , 4, 3, Qt::AlignRight);
  posi_lay->addWidget(this->get("pickupOnGluingStage_Y"), 4, 4, Qt::AlignRight);

  posi_lay->addWidget(new QLabel(tr("Z"))               , 4, 5, Qt::AlignRight);
  posi_lay->addWidget(this->get("pickupOnGluingStage_Z"), 4, 6, Qt::AlignRight);

  //// ---------------------

  //// DISTANCES -----------
  dist_wid_ = new QWidget;

  toolbox->addItem(dist_wid_, tr("Reference Distances [mm, deg]"));

  QGridLayout* dist_lay = new QGridLayout;
  dist_wid_->setLayout(dist_lay);

  // distance: angle between the camera frame and the XY reference frame of the motion stage
  map_lineEdit_["FromCameraFrameToRefFrame_dA"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tr("Angle between Camera Frame and XY Ref-Frame :")), 0, 0, Qt::AlignLeft);

  dist_lay->addWidget(new QLabel(tr("dA"))                     , 0, 7, Qt::AlignRight);
  dist_lay->addWidget(this->get("FromCameraFrameToRefFrame_dA"), 0, 8, Qt::AlignRight);

  // distance: from baseplate ref-point to position of PSS marker-1 edge relative to baseplate
  map_lineEdit_["basepRefPoint_to_PSSOnBasep_dX"] = new QLineEdit(tr(""));
  map_lineEdit_["basepRefPoint_to_PSSOnBasep_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tr("From Baseplate's Ref-Point to PSS Marker-1 on Baseplate :")), 1, 0, Qt::AlignLeft);

  dist_lay->addWidget(new QLabel(tr("dX"))                       , 1, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get("basepRefPoint_to_PSSOnBasep_dX"), 1, 2, Qt::AlignRight);

  dist_lay->addWidget(new QLabel(tr("dY"))                       , 1, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get("basepRefPoint_to_PSSOnBasep_dY"), 1, 4, Qt::AlignRight);

  // distance: from marker's ref-point to pickup position
  map_lineEdit_["marker_to_pickup_dX"] = new QLineEdit(tr(""));
  map_lineEdit_["marker_to_pickup_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tr("From Marker's Ref-Point to Pickup position :")), 2, 0, Qt::AlignLeft);

  dist_lay->addWidget(new QLabel(tr("dX"))            , 2, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get("marker_to_pickup_dX"), 2, 2, Qt::AlignRight);

  dist_lay->addWidget(new QLabel(tr("dY"))            , 2, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get("marker_to_pickup_dY"), 2, 4, Qt::AlignRight);

  // distance: from best-focus z-position to pickup z-position
  map_lineEdit_["bestfocus_to_pickup_dZ"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tr("From Best-Focus Height to Pickup Height :")), 3, 0, Qt::AlignLeft);

  dist_lay->addWidget(new QLabel(tr("dZ"))               , 3, 5, Qt::AlignRight);
  dist_lay->addWidget(this->get("bestfocus_to_pickup_dZ"), 3, 6, Qt::AlignRight);

  // distance: shift from PSP marker-1 edge to PSS marker-1 edge
  map_lineEdit_["PSP_to_PSS_dX"] = new QLineEdit(tr(""));
  map_lineEdit_["PSP_to_PSS_dY"] = new QLineEdit(tr(""));

  dist_lay->addWidget(new QLabel(tr("Offset from PSP to PSS :")), 4, 0, Qt::AlignLeft);

  dist_lay->addWidget(new QLabel(tr("dX"))      , 4, 1, Qt::AlignRight);
  dist_lay->addWidget(this->get("PSP_to_PSS_dX"), 4, 2, Qt::AlignRight);

  dist_lay->addWidget(new QLabel(tr("dY"))      , 4, 3, Qt::AlignRight);
  dist_lay->addWidget(this->get("PSP_to_PSS_dY"), 4, 4, Qt::AlignRight);

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

    QMessageBox::critical(0
     , tr("[AssemblyParametersView::get]")
     , tr("Failed to find QLineEdit for key: \"%1\"\n.").arg(QString(key.c_str()))
     , QMessageBox::Abort
    );

    throw; // must abort
  }
  else
  {
    ptr = map_lineEdit_.at(key);
  }

  if(ptr == nullptr)
  {
    NQLog("AssemblyParametersView", NQLog::Fatal) << "get"
       << ": null pointer to QLineEdit associated to key \"" << key << "\", closing application";

    QMessageBox::critical(0
     , tr("[AssemblyParametersView::get]")
     , tr("Null pointer to QLineEdit for key: \"%1\"\n.").arg(QString(key.c_str()))
     , QMessageBox::Abort
    );

    throw; // must abort
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

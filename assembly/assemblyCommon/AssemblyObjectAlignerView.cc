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

#include <AssemblyObjectAlignerView.h>
#include <AssemblyUtilities.h>

#include <sstream>
#include <iomanip>
#include <cmath>

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QToolBox>
#include <QLabel>
#include <QScriptEngine>
#include <qnumeric.h>

AssemblyObjectAlignerView::AssemblyObjectAlignerView(QWidget* parent)
 : QWidget(parent)

 , alignm_cfg_wid_(nullptr)
 , alignm_res_wid_(nullptr)

 , alignm_PSS_radbu_   (nullptr)
 , alignm_PSS_dX_linee_(nullptr)
 , alignm_PSS_dY_linee_(nullptr)

 , alignm_PSP_radbu_   (nullptr)
 , alignm_PSP_dX_linee_(nullptr)
 , alignm_PSP_dY_linee_(nullptr)

 , alignm_angtgt_calc_checkbox_(nullptr)
 , alignm_angtgt_linee_(nullptr)

 , alignm_completeAtPosOne_checkbox_(nullptr)

 , alignm_exemeas_radbu_(nullptr)
 , alignm_exemeas_pusbu_(nullptr)

 , alignm_exealig_radbu_(nullptr)
 , alignm_exealig_pusbu_(nullptr)

 , patrecOne_wid_(nullptr)
 , patrecTwo_wid_(nullptr)

 , alignm_mesang_linee_(nullptr)

 , patrecOne_image_ (nullptr)
 , patrecOne_scroll_(nullptr)

 , patrecTwo_image_ (nullptr)
 , patrecTwo_scroll_(nullptr)

 , button_emergencyStop_(nullptr)

 , finder_connected_(false)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  ApplicationConfig* config = ApplicationConfig::instance();

  // Configuration + Execution

  alignm_cfg_wid_ = new QWidget;
  toolbox->addItem(alignm_cfg_wid_, tr("Alignment Configuration"));

  QVBoxLayout* alignm_cfg_lay = new QVBoxLayout;
  alignm_cfg_wid_->setLayout(alignm_cfg_lay);

  // Object Configuration + Execution Modes
  QHBoxLayout* alignm_cfgexe_lay = new QHBoxLayout;
  alignm_cfg_lay->addLayout(alignm_cfgexe_lay);

  // object configuration
  QVBoxLayout* alignm_objcfg_lay = new QVBoxLayout;
  alignm_cfgexe_lay->addLayout(alignm_objcfg_lay);

  QGroupBox* alignm_dXY_box = new QGroupBox;
  alignm_objcfg_lay->addWidget(alignm_dXY_box);

  QVBoxLayout* alignm_dXY_lay = new QVBoxLayout;
  alignm_dXY_box->setLayout(alignm_dXY_lay);

  QHBoxLayout* alignm_PSS_lay = new QHBoxLayout;
  alignm_dXY_lay->addLayout(alignm_PSS_lay);

  alignm_PSS_radbu_ = new QRadioButton(tr("PS-s Sensor"));
  alignm_PSS_radbu_->setStyleSheet("QRadioButton { font-weight : bold; }");

  QLabel* alignm_PSS_dX_label = new QLabel(tr("dX [mm]"));
  alignm_PSS_dX_linee_ = new QLineEdit(tr(""));

  QLabel* alignm_PSS_dY_label = new QLabel(tr("dY [mm]"));
  alignm_PSS_dY_linee_ = new QLineEdit(tr(""));

  alignm_PSS_lay->addWidget(alignm_PSS_radbu_   , 25, Qt::AlignCenter);
  alignm_PSS_lay->addWidget(alignm_PSS_dX_label , 10);
  alignm_PSS_lay->addWidget(alignm_PSS_dX_linee_, 10);
  alignm_PSS_lay->addSpacing(10);
  alignm_PSS_lay->addWidget(alignm_PSS_dY_label , 10);
  alignm_PSS_lay->addWidget(alignm_PSS_dY_linee_, 10);
  alignm_PSS_lay->addSpacing(25);

  connect(alignm_PSS_radbu_, SIGNAL(toggled(bool)), alignm_PSS_dX_label , SLOT(setEnabled(bool)));
  connect(alignm_PSS_radbu_, SIGNAL(toggled(bool)), alignm_PSS_dX_linee_, SLOT(setEnabled(bool)));
  connect(alignm_PSS_radbu_, SIGNAL(toggled(bool)), alignm_PSS_dY_label , SLOT(setEnabled(bool)));
  connect(alignm_PSS_radbu_, SIGNAL(toggled(bool)), alignm_PSS_dY_linee_, SLOT(setEnabled(bool)));

  assembly::QLineEdit_setText(alignm_PSS_dX_linee_, config->getValue<double>("AssemblyObjectAlignerView_PSS_deltaX", 0.));
  assembly::QLineEdit_setText(alignm_PSS_dY_linee_, config->getValue<double>("AssemblyObjectAlignerView_PSS_deltaY", 0.));

  QHBoxLayout* alignm_PSP_lay = new QHBoxLayout;
  alignm_dXY_lay->addLayout(alignm_PSP_lay);

  alignm_PSP_radbu_ = new QRadioButton(tr("PS-p Sensor"));
  alignm_PSP_radbu_->setStyleSheet("QRadioButton { font-weight : bold; }");

  QLabel* alignm_PSP_dX_label = new QLabel(tr("dX [mm]"));
  alignm_PSP_dX_linee_ = new QLineEdit(tr(""));

  QLabel* alignm_PSP_dY_label = new QLabel(tr("dY [mm]"));
  alignm_PSP_dY_linee_ = new QLineEdit(tr(""));

  alignm_PSP_lay->addWidget(alignm_PSP_radbu_   , 25, Qt::AlignCenter);
  alignm_PSP_lay->addWidget(alignm_PSP_dX_label , 10);
  alignm_PSP_lay->addWidget(alignm_PSP_dX_linee_, 10);
  alignm_PSP_lay->addSpacing(10);
  alignm_PSP_lay->addWidget(alignm_PSP_dY_label , 10);
  alignm_PSP_lay->addWidget(alignm_PSP_dY_linee_, 10);
  alignm_PSP_lay->addSpacing(25);

  connect(alignm_PSP_radbu_, SIGNAL(toggled(bool)), alignm_PSP_dX_label , SLOT(setEnabled(bool)));
  connect(alignm_PSP_radbu_, SIGNAL(toggled(bool)), alignm_PSP_dX_linee_, SLOT(setEnabled(bool)));
  connect(alignm_PSP_radbu_, SIGNAL(toggled(bool)), alignm_PSP_dY_label , SLOT(setEnabled(bool)));
  connect(alignm_PSP_radbu_, SIGNAL(toggled(bool)), alignm_PSP_dY_linee_, SLOT(setEnabled(bool)));

  assembly::QLineEdit_setText(alignm_PSP_dX_linee_, config->getValue<double>("AssemblyObjectAlignerView_PSP_deltaX", 0.));
  assembly::QLineEdit_setText(alignm_PSP_dY_linee_, config->getValue<double>("AssemblyObjectAlignerView_PSP_deltaY", 0.));

  alignm_objcfg_lay->addSpacing(10);

  QHBoxLayout* alignm_angtgtOPT_lay = new QHBoxLayout;
  alignm_objcfg_lay->addLayout(alignm_angtgtOPT_lay);

  alignm_angtgt_calc_checkbox_ = new QCheckBox(tr("Determine Target Angle from Sensor Dimensions"));

  connect(alignm_angtgt_calc_checkbox_, SIGNAL(stateChanged(int)), this, SLOT(update_target_angle(int)));

  connect(alignm_PSS_radbu_, SIGNAL(toggled(bool)), this, SLOT(update_target_angle(bool)));
  connect(alignm_PSP_radbu_, SIGNAL(toggled(bool)), this, SLOT(update_target_angle(bool)));

  alignm_angtgtOPT_lay->addSpacing(30);
  alignm_angtgtOPT_lay->addWidget(alignm_angtgt_calc_checkbox_);
  alignm_angtgtOPT_lay->addSpacing(30);

  alignm_objcfg_lay->addSpacing(10);

  QHBoxLayout* alignm_angtgtVAL_lay = new QHBoxLayout;
  alignm_objcfg_lay->addLayout(alignm_angtgtVAL_lay);

  QLabel* alignm_angtgt_label = new QLabel(tr("Target Angle [deg]"));
  alignm_angtgt_linee_ = new QLineEdit(tr(""));

  alignm_angtgtVAL_lay->addSpacing(80);
  alignm_angtgtVAL_lay->addWidget(alignm_angtgt_label , 10);
  alignm_angtgtVAL_lay->addWidget(alignm_angtgt_linee_, 10);
  alignm_angtgtVAL_lay->addSpacing(80);
  // -----

  // execution modes
  QVBoxLayout* alignm_exe_lay = new QVBoxLayout;
  alignm_cfgexe_lay->addLayout(alignm_exe_lay);

  QGroupBox* alignm_exemod_box = new QGroupBox;
  alignm_exe_lay->addWidget(alignm_exemod_box);

  QVBoxLayout* alignm_exemod_lay = new QVBoxLayout;
  alignm_exemod_box->setLayout(alignm_exemod_lay);

  // option: go-back to start before stopping
  alignm_completeAtPosOne_checkbox_ = new QCheckBox("Go back to marker-1 position before completion");
  alignm_exemod_lay->addWidget(alignm_completeAtPosOne_checkbox_);

  alignm_exemod_lay->addSpacing(20);

  // mode: measure angle
  QHBoxLayout* alignm_exemeas_lay = new QHBoxLayout;
  alignm_exemod_lay->addLayout(alignm_exemeas_lay);

  alignm_exemeas_radbu_ = new QRadioButton;
  alignm_exemeas_pusbu_ = new QPushButton(tr("Measure Angle"));

  connect(alignm_exemeas_radbu_, SIGNAL(toggled(bool)), alignm_completeAtPosOne_checkbox_, SLOT(setEnabled(bool)));
  connect(alignm_exemeas_radbu_, SIGNAL(toggled(bool)), alignm_exemeas_pusbu_            , SLOT(setEnabled(bool)));

  alignm_exemeas_lay->addSpacing(26);
  alignm_exemeas_lay->addWidget(alignm_exemeas_radbu_,  4);
  alignm_exemeas_lay->addWidget(alignm_exemeas_pusbu_, 70);

  connect(alignm_exemeas_pusbu_, SIGNAL(clicked()), this, SLOT(transmit_configuration()));
  // ---

  // mode: align object
  QHBoxLayout* alignm_exealig_lay = new QHBoxLayout;
  alignm_exemod_lay->addLayout(alignm_exealig_lay);

  alignm_exealig_radbu_ = new QRadioButton;
  alignm_exealig_pusbu_ = new QPushButton(tr("Align Object"));

  connect(alignm_exealig_radbu_, SIGNAL(toggled(bool)), alignm_exealig_pusbu_       , SLOT(setEnabled(bool)));
  connect(alignm_exealig_radbu_, SIGNAL(toggled(bool)), alignm_angtgt_label         , SLOT(setEnabled(bool)));
  connect(alignm_exealig_radbu_, SIGNAL(toggled(bool)), alignm_angtgt_linee_        , SLOT(setEnabled(bool)));
  connect(alignm_exealig_radbu_, SIGNAL(toggled(bool)), alignm_angtgt_calc_checkbox_, SLOT(setEnabled(bool)));

  alignm_exealig_lay->addSpacing(26);
  alignm_exealig_lay->addWidget(alignm_exealig_radbu_,  4);
  alignm_exealig_lay->addWidget(alignm_exealig_pusbu_, 70);

  connect(alignm_exealig_pusbu_, SIGNAL(clicked()), this, SLOT(transmit_configuration()));
  // ---

  alignm_cfgexe_lay->setStretch(0, 50);
  alignm_cfgexe_lay->setStretch(1, 50);

  // ----------

  alignm_cfg_lay->addSpacing(20);

  // PatRec Configuration
  QHBoxLayout* alignm_PRcfg_lay = new QHBoxLayout;
  alignm_cfg_lay->addLayout(alignm_PRcfg_lay);

  // PatRecWidget #1
  QGroupBox* patrecOne_cfg_box = new QGroupBox(tr("PatRec Marker #1 [Bottom-Left Marker]"));
  patrecOne_cfg_box->setObjectName("patrecOne_cfg_box");
  patrecOne_cfg_box->setStyleSheet("QWidget#patrecOne_cfg_box { font-weight : bold; color : blue; }");

  patrecOne_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecOne_wid_->setToolTip("Pattern Recognition Configuration #1 [Bottom-Left Marker]");

  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectAlignerView_PatRec1_template_fpath", "");
    if(fpath != ""){ patrecOne_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecOne_wid_->threshold_lineEdit()        , config->getValue<int>("AssemblyObjectAlignerView_PatRec1_threshold"        , 100));
    assembly::QLineEdit_setText(patrecOne_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AssemblyObjectAlignerView_PatRec1_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrecOne_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_PatRec1_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_PatRec1_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectAlignerView_PatRec1_angles_finestep", 0.2));
  }

  patrecOne_cfg_box->setLayout(patrecOne_wid_->layout());
  // -----

  // PatRecWidget #2
  QGroupBox* patrecTwo_cfg_box = new QGroupBox(tr("PatRec Marker #2 [Top-Right Marker]"));
  patrecTwo_cfg_box->setObjectName("patrecTwo_cfg_box");
  patrecTwo_cfg_box->setStyleSheet("QWidget#patrecTwo_cfg_box { font-weight : bold; color : blue; }");

  patrecTwo_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecTwo_wid_->setToolTip("Pattern Recognition Configuration #2 [Top-Right Marker]");

  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectAlignerView_PatRec2_template_fpath", "");
    if(fpath != ""){ patrecTwo_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecTwo_wid_->threshold_lineEdit()        , config->getValue<int>("AssemblyObjectAlignerView_PatRec2_threshold"        , 100));
    assembly::QLineEdit_setText(patrecTwo_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AssemblyObjectAlignerView_PatRec2_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrecTwo_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_PatRec2_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_PatRec2_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectAlignerView_PatRec2_angles_finestep", 0.2));
  }

  patrecTwo_cfg_box->setLayout(patrecTwo_wid_->layout());
  // -----

  alignm_PRcfg_lay->addWidget(patrecOne_cfg_box, 50);
  alignm_PRcfg_lay->addWidget(patrecTwo_cfg_box, 50);

  alignm_cfg_lay->addStretch(1);
  // ----------

  // Default Configuration

  alignm_completeAtPosOne_checkbox_->setEnabled(false);
  alignm_completeAtPosOne_checkbox_->setChecked(true);

  alignm_exemeas_radbu_->setChecked(false);
  alignm_exemeas_pusbu_->setEnabled(false);

  alignm_exealig_radbu_->setChecked(true);
  alignm_exealig_pusbu_->setEnabled(true);

  alignm_angtgt_calc_checkbox_->setChecked(true);

  alignm_PSS_radbu_   ->setChecked(true);
  alignm_PSS_dX_label ->setEnabled(true);
  alignm_PSS_dX_linee_->setEnabled(true);
  alignm_PSS_dY_label ->setEnabled(true);
  alignm_PSS_dY_linee_->setEnabled(true);

  alignm_PSP_radbu_   ->setChecked(false);
  alignm_PSP_dX_label ->setEnabled(false);
  alignm_PSP_dX_linee_->setEnabled(false);
  alignm_PSP_dY_label ->setEnabled(false);
  alignm_PSP_dY_linee_->setEnabled(false);

  alignm_angtgt_label ->setEnabled(true);
  alignm_angtgt_linee_->setEnabled(true);

  // ---------------------

  // Results -------------
  alignm_res_wid_ = new QWidget;
  toolbox->addItem(alignm_res_wid_, "Alignment Results");
 
  QVBoxLayout* alignm_res_lay = new QVBoxLayout;
  alignm_res_wid_->setLayout(alignm_res_lay);

  // Numerical Results
  QHBoxLayout* alignm_mesang_lay = new QHBoxLayout;
  alignm_res_lay->addLayout(alignm_mesang_lay);

  QLabel* alignm_mesang_label = new QLabel("Measured Angle [deg]");
  alignm_mesang_linee_ = new QLineEdit("");
  alignm_mesang_linee_->setReadOnly(true);

  alignm_mesang_lay->addWidget(alignm_mesang_label);
  alignm_mesang_lay->addWidget(alignm_mesang_linee_);
  alignm_mesang_lay->addWidget(new QLabel);

  alignm_mesang_lay->setStretch(0,  34);
  alignm_mesang_lay->setStretch(1,  65);
  alignm_mesang_lay->setStretch(2, 101);

  // Graphical Results (PatRec Images)
  QGridLayout* alignm_img_lay = new QGridLayout;
  alignm_res_lay->addLayout(alignm_img_lay);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  patrecOne_image_ = new AssemblyUEyeView(this);
  patrecOne_image_->setMinimumSize(200, 200);
  patrecOne_image_->setPalette(palette);
  patrecOne_image_->setBackgroundRole(QPalette::Background);
  patrecOne_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecOne_image_->setScaledContents(true);
  patrecOne_image_->setAlignment(Qt::AlignCenter);

  QApplication::processEvents();

  patrecOne_scroll_ = new QScrollArea(this);
  patrecOne_scroll_->setMinimumSize(200, 200);
  patrecOne_scroll_->setPalette(palette);
  patrecOne_scroll_->setBackgroundRole(QPalette::Background);
  patrecOne_scroll_->setAlignment(Qt::AlignCenter);
  patrecOne_scroll_->setWidget(patrecOne_image_);
  patrecOne_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  alignm_img_lay->addWidget(patrecOne_scroll_, 0, 0);

  patrecTwo_image_ = new AssemblyUEyeView(this);
  patrecTwo_image_->setMinimumSize(200, 200);
  patrecTwo_image_->setPalette(palette);
  patrecTwo_image_->setBackgroundRole(QPalette::Background);
  patrecTwo_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecTwo_image_->setScaledContents(true);
  patrecTwo_image_->setAlignment(Qt::AlignCenter);

  patrecTwo_scroll_ = new QScrollArea(this);
  patrecTwo_scroll_->setMinimumSize(200, 200);
  patrecTwo_scroll_->setPalette(palette);
  patrecTwo_scroll_->setBackgroundRole(QPalette::Background);
  patrecTwo_scroll_->setAlignment(Qt::AlignCenter);
  patrecTwo_scroll_->setWidget(patrecTwo_image_);
  patrecTwo_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  alignm_img_lay->addWidget(patrecTwo_scroll_, 0, 1);
  // ---------------------

  QHBoxLayout* buttons_lay = new QHBoxLayout;

  button_emergencyStop_ = new QPushButton(tr("Emergency Stop"));

  buttons_lay->addWidget(new QLabel, 33);
  buttons_lay->addWidget(new QLabel, 33);
  buttons_lay->addWidget(button_emergencyStop_, 33);

  layout->addLayout(buttons_lay);
}

void AssemblyObjectAlignerView::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch(event->key())
    {
      case Qt::Key_0:
//        imageView_1->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        imageView_1->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        imageView_1->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        imageView_1->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}

void AssemblyObjectAlignerView::enable()
{
  this->setEnabled(true);
}

void AssemblyObjectAlignerView::show_measured_angle(const double val)
{
  std::stringstream posi_strs;
  posi_strs << val;

  alignm_mesang_linee_->setText(QString::fromStdString(posi_strs.str()));

  return;
}

void AssemblyObjectAlignerView::updateImage(const int stage, const QString& filename)
{
  NQLog("AssemblyObjectAlignerView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";

  const cv::Mat img = assembly::cv_imread(filename, CV_LOAD_IMAGE_UNCHANGED);

  this->updateImage(stage, img);

  return;
}

void AssemblyObjectAlignerView::updateImage(const int stage, const cv::Mat& img)
{
  NQLog("AssemblyObjectAlignerView", NQLog::Spam) << "updateImage(" << stage << ", image)";

  if(stage == 1)
  {
    patrecOne_image_->setImage(img);
//    patrecOne_image_->setZoomFactor(0.3);
  }
  else if(stage == 2)
  {
    patrecTwo_image_->setImage(img);
//    patrecTwo_image_->setZoomFactor(0.5);
  }

  return;
}

void AssemblyObjectAlignerView::update_target_angle(const bool checked)
{
  if(checked){ this->update_target_angle(alignm_angtgt_calc_checkbox_->checkState()); }
}

void AssemblyObjectAlignerView::update_target_angle(const int state)
{
  if(state == 2)
  {
    if(alignm_PSS_radbu_->isChecked())
    {
      bool valid_PSS_DX(false);

      const double PSS_DX_val = alignm_PSS_dX_linee_->text().toDouble(&valid_PSS_DX);

      if(valid_PSS_DX == false)
      {
        NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "update_target_angle(" << state << ")"
           << ": invalid format for PS-s Sensor delta-X distance (" << alignm_PSS_dX_linee_->text() << "), no action taken";

        return;
      }

      bool valid_PSS_DY(false);

      const double PSS_DY_val = alignm_PSS_dY_linee_->text().toDouble(&valid_PSS_DY);

      if(valid_PSS_DY == false)
      {
        NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "update_target_angle(" << state << ")"
           << ": invalid format for PS-s Sensor delta-Y distance (" << alignm_PSS_dY_linee_->text() << "), no action taken";

        return;
      }

      if(PSS_DX_val == 0.)
      {
        NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "update_target_angle(" << state << ")"
           << ": null value for PS-s Sensor delta-X distance, angle cannot be calculated, no action taken";

        return;
      }

      const double angle_PSS = atan(PSS_DY_val / PSS_DX_val) * 180. / M_PI;

      std::stringstream strs_angPSS;
      strs_angPSS << angle_PSS;

      alignm_angtgt_linee_->setText(QString::fromStdString(strs_angPSS.str()));

      alignm_angtgt_linee_->setReadOnly(true);
    }
    else if(alignm_PSP_radbu_->isChecked())
    {
      bool valid_PSP_DX(false);

      const double PSP_DX_val = alignm_PSP_dX_linee_->text().toDouble(&valid_PSP_DX);

      if(valid_PSP_DX == false)
      {
        NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "update_target_angle(" << state << ")"
           << ": invalid format for PS-s Sensor delta-X distance (" << alignm_PSP_dX_linee_->text() << "), no action taken";

        return;
      }

      bool valid_PSP_DY(false);

      const double PSP_DY_val = alignm_PSP_dY_linee_->text().toDouble(&valid_PSP_DY);

      if(valid_PSP_DY == false)
      {
        NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "update_target_angle(" << state << ")"
           << ": invalid format for PS-s Sensor delta-Y distance (" << alignm_PSP_dY_linee_->text() << "), no action taken";

        return;
      }

      if(PSP_DX_val == 0.)
      {
        NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "update_target_angle(" << state << ")"
           << ": null value for PS-s Sensor delta-X distance, angle cannot be calculated, no action taken";

        return;
      }

      const double angle_PSP = atan(PSP_DY_val / PSP_DX_val) * 180. / M_PI;

      std::stringstream strs_angPSP;
      strs_angPSP << angle_PSP;

      alignm_angtgt_linee_->setText(QString::fromStdString(strs_angPSP.str()));

      alignm_angtgt_linee_->setReadOnly(true);
    }
  }
  else if(state == 0)
  {
    alignm_angtgt_linee_->setText(tr(""));

    alignm_angtgt_linee_->setReadOnly(false);
  }

  return;
}

void AssemblyObjectAlignerView::transmit_configuration()
{
  bool valid_conf(false);

  const auto& conf = this->get_configuration(valid_conf);

  if(valid_conf == false)
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object, no action taken";

    return;
  }

  NQLog("AssemblyObjectAlignerView", NQLog::Spam) << "transmit_configuration"
     << ": emitting signal \"configuration(AssemblyObjectAligner::Configuration)\"";

  emit configuration(conf);
}

AssemblyObjectAligner::Configuration AssemblyObjectAlignerView::get_configuration(bool& valid_conf) const
{
  AssemblyObjectAligner::Configuration conf;

  /// Object (dX, dY) Dimensions ---

  if(alignm_PSP_radbu_->isChecked() == alignm_PSS_radbu_->isChecked())
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "get_configuration"
       << ": failed to identify object (X,Y) dimensions, no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }

  QLineEdit const* objDX_linee = alignm_PSP_radbu_->isChecked() ? alignm_PSP_dX_linee_ : alignm_PSS_dX_linee_;
  QLineEdit const* objDY_linee = alignm_PSP_radbu_->isChecked() ? alignm_PSP_dY_linee_ : alignm_PSS_dY_linee_;

  const QString alignm_objDX_qstr = objDX_linee->text();
  const QString alignm_objDY_qstr = objDY_linee->text();

  bool valid_alignm_objDX(false);
  bool valid_alignm_objDY(false);

  const double alignm_objDX_val = alignm_objDX_qstr.toDouble(&valid_alignm_objDX);
  const double alignm_objDY_val = alignm_objDY_qstr.toDouble(&valid_alignm_objDY);

  if(valid_alignm_objDX == false)
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "execute"
       << ": invalid format for object delta-X distance (" << alignm_objDX_qstr << "), no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  else if(valid_alignm_objDY == false)
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "execute"
       << ": invalid format for object delta-Y distance (" << alignm_objDY_qstr << "), no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }

  conf.object_deltaX = alignm_objDX_val;
  conf.object_deltaY = alignm_objDY_val;
  /// ------------------------------

  /// Execution --------------------

  conf.completeAtPosOne = alignm_completeAtPosOne_checkbox_->isChecked();

  if(alignm_exemeas_radbu_->isChecked() == alignm_exealig_radbu_->isChecked())
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "get_configuration"
       << ": failed to identify execution mode, no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  else if(alignm_exemeas_radbu_->isChecked())
  {
    conf.only_measure_angle = true;
  }
  else
  {
    conf.only_measure_angle = false;

    // Target Angle
    const QString angtgt_qstr = alignm_angtgt_linee_->text();

    QScriptEngine qscr_engine;
    const double angtgt_val = qscr_engine.evaluate(angtgt_qstr).toNumber();

    const bool valid_angtgt = qIsFinite(angtgt_val);

    if(valid_angtgt == false)
    {
      NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "get_configuration"
         << ": invalid format for target angle (" << angtgt_qstr << "), no action taken";

      valid_conf = false;

      conf.reset();

      return conf;
    }

    conf.target_angle = angtgt_val;
    // ---------------
  }
  /// ------------------------------

  /// PatRec #1 --------------------
  bool valid_PatRecOne_conf(false);

  conf.PatRecOne_configuration = patrecOne_wid_->get_configuration(valid_PatRecOne_conf);

  if(valid_PatRecOne_conf == false)
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object from PatRec Widget #1, no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  /// ------------------------------

  /// PatRec #2 --------------------
  bool valid_PatRecTwo_conf(false);

  conf.PatRecTwo_configuration = patrecTwo_wid_->get_configuration(valid_PatRecTwo_conf);

  if(valid_PatRecTwo_conf == false)
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object from PatRec Widget #2, no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  /// ------------------------------

  valid_conf = true;

  return conf;
}

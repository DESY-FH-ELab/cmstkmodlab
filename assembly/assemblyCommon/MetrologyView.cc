/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>

#include <MetrologyView.h>
#include <AssemblyUtilities.h>

#include <sstream>
#include <iomanip>
#include <cmath>

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QScriptEngine>
#include <qnumeric.h>


MetrologyView::MetrologyView(QWidget* parent)
 : QWidget(parent)

 , toolbox_(nullptr)

 , metro_cfg_wid_(nullptr)
 , metro_res_wid_(nullptr)

 , metro_completeAtPosOne_checkbox_(nullptr)
 , metro_useAutoFocusing_checkbox_(nullptr)

 , metro_exemetro_pusbu_(nullptr)

 , alignm_angmax_dontIter_linee_(nullptr)
 , alignm_angmax_complete_linee_(nullptr)

 , patrecOne_wid_(nullptr)
 , patrecTwo_wid_(nullptr)
 , patrecThree_wid_(nullptr)
 , patrecFour_wid_(nullptr)

 , metro_mesang_PSp_linee_(nullptr)
 , metro_mesang_PSs_linee_(nullptr)
 , metro_dx_linee_(nullptr)
 , metro_dy_linee_(nullptr)
 , metro_da_linee_(nullptr)

 , patrecOne_image_ (nullptr)
 , patrecOne_scroll_(nullptr)

 , patrecTwo_image_ (nullptr)
 , patrecTwo_scroll_(nullptr)

 , patrecThree_image_ (nullptr)
 , patrecThree_scroll_(nullptr)

 , patrecFour_image_ (nullptr)
 , patrecFour_scroll_(nullptr)

 , button_metrologyEmergencyStop_(nullptr)

 , finder_connected_(false)

 , idx_cfg_wid_(0)
 , idx_results_wid_(0)

 , config_(nullptr)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  toolbox_ = new QToolBox;
  layout->addWidget(toolbox_);

  config_ = ApplicationConfig::instance();

  // Configuration + Execution

  metro_cfg_wid_ = new QWidget;
  toolbox_->addItem(metro_cfg_wid_, tr("Metrology Configuration"));
  idx_cfg_wid_ = toolbox_->indexOf(metro_cfg_wid_);

  QVBoxLayout* metro_cfg_lay = new QVBoxLayout;
  metro_cfg_wid_->setLayout(metro_cfg_lay);

  // Object Configuration + Execution Modes
  QHBoxLayout* metro_cfgexe_lay = new QHBoxLayout;
  metro_cfg_lay->addLayout(metro_cfgexe_lay);

  // execution modes
  QVBoxLayout* metro_exe_lay = new QVBoxLayout;
  metro_cfgexe_lay->addLayout(metro_exe_lay);

  QGroupBox* metro_exemod_box = new QGroupBox;
  metro_exe_lay->addWidget(metro_exemod_box);

  QHBoxLayout* metro_exemod_lay = new QHBoxLayout;
  metro_exemod_box->setLayout(metro_exemod_lay);

  // option: go-back to start before stopping
  QVBoxLayout* metro_exeopt_lay = new QVBoxLayout;
  metro_exemod_lay->addLayout(metro_exeopt_lay);

  metro_completeAtPosOne_checkbox_ = new QCheckBox("Go back to marker-1 position before completion");
  metro_exeopt_lay->addWidget(metro_completeAtPosOne_checkbox_);

  // option: go-back to start before stopping
  metro_useAutoFocusing_checkbox_ = new QCheckBox("Use Auto-Focusing");
  metro_exeopt_lay->addWidget(metro_useAutoFocusing_checkbox_);

  metro_exemod_lay->addSpacing(20);

  // mode: align object
  QHBoxLayout* metro_exemetro_lay = new QHBoxLayout;
  metro_exemod_lay->addLayout(metro_exemetro_lay);

  metro_exemetro_pusbu_ = new QPushButton(tr("Perform Metrology"));

  connect(metro_exemetro_pusbu_, SIGNAL(clicked()), this, SLOT(transmit_configuration()));
  metro_exemetro_lay->addWidget(metro_exemetro_pusbu_);
  // ----------

  metro_cfg_lay->addSpacing(50);

  // PatRec Configuration
  QHBoxLayout* alignm_PR_PSP_cfg_lay = new QHBoxLayout;
  metro_cfg_lay->addLayout(alignm_PR_PSP_cfg_lay);
  QHBoxLayout* alignm_PR_PSS_cfg_lay = new QHBoxLayout;
  metro_cfg_lay->addLayout(alignm_PR_PSS_cfg_lay);

  // PatRecWidget #1
  QGroupBox* patrecOne_cfg_box = new QGroupBox(tr("PatRec Marker PSp #1 [Bottom-Left Marker]"));
  patrecOne_cfg_box->setObjectName("patrecOne_cfg_box");
  patrecOne_cfg_box->setStyleSheet("QWidget#patrecOne_cfg_box { font-weight : bold; color : blue; }");

  patrecOne_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecOne_wid_->setToolTip("Pattern Recognition Configuration PSp #1 [Bottom-Left Marker]");

  if(config_ != nullptr)
  {
    const std::string fpath = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSP1_template_fpath", "");
    if(fpath != ""){ patrecOne_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecOne_wid_->threshold_lineEdit()        , config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_threshold"        , 100));
    assembly::QLineEdit_setText(patrecOne_wid_->adaptiveThreshold_lineEdit(), config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrecOne_wid_->angles_prescan_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_finemax_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_finestep_lineEdit()  , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finestep", 0.2));
  }

  patrecOne_cfg_box->setLayout(patrecOne_wid_->layout());
  // -----

  // PatRecWidget #2
  QGroupBox* patrecTwo_cfg_box = new QGroupBox(tr("PatRec Marker PSp #2 [Top-Right Marker]"));
  patrecTwo_cfg_box->setObjectName("patrecTwo_cfg_box");
  patrecTwo_cfg_box->setStyleSheet("QWidget#patrecTwo_cfg_box { font-weight : bold; color : blue; }");

  patrecTwo_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecTwo_wid_->setToolTip("Pattern Recognition Configuration PSp #2 [Top-Right Marker]");

  if(config_ != nullptr)
  {
    const std::string fpath = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSP2_template_fpath", "");
    if(fpath != ""){ patrecTwo_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecTwo_wid_->threshold_lineEdit()        , config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_threshold"        , 100));
    assembly::QLineEdit_setText(patrecTwo_wid_->adaptiveThreshold_lineEdit(), config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrecTwo_wid_->angles_prescan_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_finemax_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_finestep_lineEdit()  , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finestep", 0.2));
  }

  patrecTwo_cfg_box->setLayout(patrecTwo_wid_->layout());
  // -----
  // PatRecWidget #3
  QGroupBox* patrecThree_cfg_box = new QGroupBox(tr("PatRec Marker PSs #1 [Bottom-Left Marker]"));
  patrecThree_cfg_box->setObjectName("patrecThree_cfg_box");
  patrecThree_cfg_box->setStyleSheet("QWidget#patrecThree_cfg_box { font-weight : bold; color : blue; }");

  patrecThree_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecThree_wid_->setToolTip("Pattern Recognition Configuration PSs #1 [Bottom-Left Marker]");

  if(config_ != nullptr)
  {
    const std::string fpath = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSS1_template_fpath", "");
    if(fpath != ""){ patrecThree_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecThree_wid_->threshold_lineEdit()        , config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_threshold"        , 100));
    assembly::QLineEdit_setText(patrecThree_wid_->adaptiveThreshold_lineEdit(), config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrecThree_wid_->angles_prescan_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecThree_wid_->angles_finemax_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecThree_wid_->angles_finestep_lineEdit()  , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finestep", 0.2));
  }

  patrecThree_cfg_box->setLayout(patrecThree_wid_->layout());
  // -----

  // PatRecWidget #4
  QGroupBox* patrecFour_cfg_box = new QGroupBox(tr("PatRec Marker PSs #2 [Top-Right Marker]"));
  patrecFour_cfg_box->setObjectName("patrecFour_cfg_box");
  patrecFour_cfg_box->setStyleSheet("QWidget#patrecFour_cfg_box { font-weight : bold; color : blue; }");

  patrecFour_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecFour_wid_->setToolTip("Pattern Recognition Configuration PSs #2 [Top-Right Marker]");

  if(config_ != nullptr)
  {
    const std::string fpath = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSS2_template_fpath", "");
    if(fpath != ""){ patrecFour_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecFour_wid_->threshold_lineEdit()        , config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_threshold"        , 100));
    assembly::QLineEdit_setText(patrecFour_wid_->adaptiveThreshold_lineEdit(), config_->getDefaultValue<int>("main", "AssemblyObjectAlignerView_PatRec_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrecFour_wid_->angles_prescan_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecFour_wid_->angles_finemax_lineEdit()   , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecFour_wid_->angles_finestep_lineEdit()  , config_->getDefaultValue<double>("main", "AssemblyObjectAlignerView_PatRec_angles_finestep", 0.2));
  }

  patrecFour_cfg_box->setLayout(patrecFour_wid_->layout());
  // -----

  alignm_PR_PSP_cfg_lay->addWidget(patrecOne_cfg_box, 50);
  alignm_PR_PSP_cfg_lay->addWidget(patrecTwo_cfg_box, 50);
  alignm_PR_PSS_cfg_lay->addWidget(patrecThree_cfg_box, 50);
  alignm_PR_PSS_cfg_lay->addWidget(patrecFour_cfg_box, 50);

  metro_cfg_lay->addStretch(1);
  // ----------

  // ---------------------

  // Results -------------
  metro_res_wid_ = new QWidget;
  toolbox_->addItem(metro_res_wid_, "Metrology Results");
  idx_results_wid_ = toolbox_->indexOf(metro_res_wid_);

  QVBoxLayout* metro_res_lay = new QVBoxLayout;
  metro_res_wid_->setLayout(metro_res_lay);

  QHBoxLayout* metro_mesangs_lay = new QHBoxLayout;
  metro_res_lay->addLayout(metro_mesangs_lay);

  // Numerical Results
  QHBoxLayout* metro_mesang_PSp_lay = new QHBoxLayout;
  metro_mesangs_lay->addLayout(metro_mesang_PSp_lay);

  QLabel* metro_mesang_PSp_label = new QLabel("Measured Angle PSp [deg]");
  metro_mesang_PSp_linee_ = new QLineEdit("");
  metro_mesang_PSp_linee_->setReadOnly(true);

  metro_mesang_PSp_lay->addWidget(metro_mesang_PSp_label);
  metro_mesang_PSp_lay->addWidget(metro_mesang_PSp_linee_);
  metro_mesang_PSp_lay->addWidget(new QLabel);

  metro_mesang_PSp_lay->setStretch(0,  34);
  metro_mesang_PSp_lay->setStretch(1,  65);
  metro_mesang_PSp_lay->setStretch(2, 101);

  QHBoxLayout* metro_mesang_PSs_lay = new QHBoxLayout;
  metro_mesangs_lay->addLayout(metro_mesang_PSs_lay);

  QLabel* metro_mesang_PSs_label = new QLabel("Measured Angle PSs [deg]");
  metro_mesang_PSs_linee_ = new QLineEdit("");
  metro_mesang_PSs_linee_->setReadOnly(true);

  metro_mesang_PSs_lay->addWidget(metro_mesang_PSs_label);
  metro_mesang_PSs_lay->addWidget(metro_mesang_PSs_linee_);
  metro_mesang_PSs_lay->addWidget(new QLabel);

  metro_mesang_PSs_lay->setStretch(0,  34);
  metro_mesang_PSs_lay->setStretch(1,  65);
  metro_mesang_PSs_lay->setStretch(2, 101);

  QVBoxLayout* metro_diff_lay = new QVBoxLayout;
  metro_res_lay->addLayout(metro_diff_lay);

  QHBoxLayout* metro_dx_lay = new QHBoxLayout;
  metro_diff_lay->addLayout(metro_dx_lay);

  QLabel* metro_dx_label = new QLabel("Measured dx [mm]");
  metro_dx_linee_ = new QLineEdit("");
  metro_dx_linee_->setReadOnly(true);

  metro_dx_lay->addWidget(metro_dx_label);
  metro_dx_lay->addWidget(metro_dx_linee_);
  metro_dx_lay->addWidget(new QLabel);

  metro_dx_lay->setStretch(0,  34);
  metro_dx_lay->setStretch(1,  65);
  metro_dx_lay->setStretch(2, 101);

  QHBoxLayout* metro_dy_lay = new QHBoxLayout;
  metro_diff_lay->addLayout(metro_dy_lay);

  QLabel* metro_dy_label = new QLabel("Measured dy [mm]");
  metro_dy_linee_ = new QLineEdit("");
  metro_dy_linee_->setReadOnly(true);

  metro_dy_lay->addWidget(metro_dy_label);
  metro_dy_lay->addWidget(metro_dy_linee_);
  metro_dy_lay->addWidget(new QLabel);

  metro_dy_lay->setStretch(0,  34);
  metro_dy_lay->setStretch(1,  65);
  metro_dy_lay->setStretch(2, 101);

  QHBoxLayout* metro_da_lay = new QHBoxLayout;
  metro_diff_lay->addLayout(metro_da_lay);

  QLabel* metro_da_label = new QLabel("Measured angle difference [deg]");
  metro_da_linee_ = new QLineEdit("");
  metro_da_linee_->setReadOnly(true);

  metro_da_lay->addWidget(metro_da_label);
  metro_da_lay->addWidget(metro_da_linee_);
  metro_da_lay->addWidget(new QLabel);

  metro_da_lay->setStretch(0,  34);
  metro_da_lay->setStretch(1,  65);
  metro_da_lay->setStretch(2, 101);


  // Graphical Results (PatRec Images)
  QGridLayout* metro_img_lay = new QGridLayout;
  metro_res_lay->addLayout(metro_img_lay);

  QPalette palette;
  palette.setColor(QPalette::Window, QColor(220, 220, 220));

  patrecOne_image_ = new AssemblyUEyeView(this);
  patrecOne_image_->setMinimumSize(200, 200);
  patrecOne_image_->setPalette(palette);
  patrecOne_image_->setBackgroundRole(QPalette::Window);
  patrecOne_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecOne_image_->setScaledContents(true);
  patrecOne_image_->setAlignment(Qt::AlignCenter);

  QApplication::processEvents();

  patrecOne_scroll_ = new QScrollArea(this);
  patrecOne_scroll_->setMinimumSize(200, 200);
  patrecOne_scroll_->setPalette(palette);
  patrecOne_scroll_->setBackgroundRole(QPalette::Window);
  patrecOne_scroll_->setAlignment(Qt::AlignCenter);
  patrecOne_scroll_->setWidget(patrecOne_image_);
  patrecOne_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  metro_img_lay->addWidget(patrecOne_scroll_, 0, 0);

  patrecTwo_image_ = new AssemblyUEyeView(this);
  patrecTwo_image_->setMinimumSize(200, 200);
  patrecTwo_image_->setPalette(palette);
  patrecTwo_image_->setBackgroundRole(QPalette::Window);
  patrecTwo_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecTwo_image_->setScaledContents(true);
  patrecTwo_image_->setAlignment(Qt::AlignCenter);

  patrecTwo_scroll_ = new QScrollArea(this);
  patrecTwo_scroll_->setMinimumSize(200, 200);
  patrecTwo_scroll_->setPalette(palette);
  patrecTwo_scroll_->setBackgroundRole(QPalette::Window);
  patrecTwo_scroll_->setAlignment(Qt::AlignCenter);
  patrecTwo_scroll_->setWidget(patrecTwo_image_);
  patrecTwo_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  metro_img_lay->addWidget(patrecTwo_scroll_, 0, 1);

  patrecThree_image_ = new AssemblyUEyeView(this);
  patrecThree_image_->setMinimumSize(200, 200);
  patrecThree_image_->setPalette(palette);
  patrecThree_image_->setBackgroundRole(QPalette::Window);
  patrecThree_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecThree_image_->setScaledContents(true);
  patrecThree_image_->setAlignment(Qt::AlignCenter);

  patrecThree_scroll_ = new QScrollArea(this);
  patrecThree_scroll_->setMinimumSize(200, 200);
  patrecThree_scroll_->setPalette(palette);
  patrecThree_scroll_->setBackgroundRole(QPalette::Window);
  patrecThree_scroll_->setAlignment(Qt::AlignCenter);
  patrecThree_scroll_->setWidget(patrecThree_image_);
  patrecThree_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  metro_img_lay->addWidget(patrecThree_scroll_, 1, 0);

  patrecFour_image_ = new AssemblyUEyeView(this);
  patrecFour_image_->setMinimumSize(200, 200);
  patrecFour_image_->setPalette(palette);
  patrecFour_image_->setBackgroundRole(QPalette::Window);
  patrecFour_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecFour_image_->setScaledContents(true);
  patrecFour_image_->setAlignment(Qt::AlignCenter);

  patrecFour_scroll_ = new QScrollArea(this);
  patrecFour_scroll_->setMinimumSize(200, 200);
  patrecFour_scroll_->setPalette(palette);
  patrecFour_scroll_->setBackgroundRole(QPalette::Window);
  patrecFour_scroll_->setAlignment(Qt::AlignCenter);
  patrecFour_scroll_->setWidget(patrecFour_image_);
  patrecFour_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  metro_img_lay->addWidget(patrecFour_scroll_, 1, 1);
  // ---------------------

  QHBoxLayout* buttons_lay = new QHBoxLayout;

  button_metrologyEmergencyStop_ = new QPushButton(tr("Emergency Stop"));

  buttons_lay->addWidget(new QLabel, 33);
  buttons_lay->addWidget(new QLabel, 33);
  buttons_lay->addWidget(button_metrologyEmergencyStop_, 33);

  layout->addLayout(buttons_lay);
}


void MetrologyView::update_templates(const bool checked)
{
  QRadioButton* ptr_qedit = qobject_cast<QRadioButton*>(sender());
  std::string f_path_1;
  std::string f_path_2;
  std::string f_path_3;
  std::string f_path_4;

  f_path_1 = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSS1_template_fpath", "");
  f_path_2 = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSS2_template_fpath", "");
  f_path_3 = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSP1_template_fpath", "");
  f_path_4 = config_->getDefaultValue<std::string>("main", "AssemblyObjectAlignerView_PatRec_PSP2_template_fpath", "");

  patrecOne_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+f_path_1));
  patrecTwo_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+f_path_2));
  patrecThree_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+f_path_3));
  patrecFour_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+f_path_4));

  return;
}

void MetrologyView::switch_to_results()
{
    NQLog("MetrologyView", NQLog::Critical) << "switch_to_results"
    << ": Switching to results widget";

    toolbox_->setCurrentIndex(idx_results_wid_);
    return;
}

void MetrologyView::transmit_configuration()
{
  QMessageBox* msgBox = new QMessageBox;
  msgBox->setStyleSheet("QLabel{min-width: 300px;}");
  msgBox->setInformativeText("Are you sure you selected the correct template images and defined the relevant B/W thresholds?");
  msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox->setDefaultButton(QMessageBox::Yes);
  int ret = msgBox->exec();
  switch(ret)
  {
    case QMessageBox::Yes:
      break;
    case QMessageBox::No:
      NQLog("MetrologyView", NQLog::Critical) << "transmit_configuration"
      << ": abort start of alignment procedure";
      return;
    default: return;
  }

  bool valid_conf(false);

  const auto& conf = this->get_configuration(valid_conf);

  if(valid_conf == false)
  {
    NQLog("MetrologyView", NQLog::Critical) << "transmit_configuration"
       << ": invalid MetrologyView::Configuration object, no action taken";

    return;
  }

  NQLog("MetrologyView", NQLog::Spam) << "transmit_configuration"
     << ": emitting signal \"configuration(Metrology::Configuration)\"";

  emit configuration(conf);

  switch_to_results();
}


Metrology::Configuration MetrologyView::get_configuration(bool& valid_conf) const
{
  Metrology::Configuration conf;

  // execution option(s)
  conf.complete_at_position1 = metro_completeAtPosOne_checkbox_->isChecked();
  conf.use_autofocusing      = metro_useAutoFocusing_checkbox_ ->isChecked();

  /// PatRec #1 --------------------
  bool valid_PatRecOne_conf(false);

  conf.PatRec_PSP1_configuration = patrecOne_wid_->get_configuration(valid_PatRecOne_conf);

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

  conf.PatRec_PSP2_configuration = patrecTwo_wid_->get_configuration(valid_PatRecTwo_conf);

  if(valid_PatRecTwo_conf == false)
  {
    NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object from PatRec Widget #2, no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  /// ------------------------------


  /// PatRec #3 --------------------
  bool valid_PatRecThree_conf(false);

  conf.PatRec_PSS1_configuration = patrecThree_wid_->get_configuration(valid_PatRecThree_conf);

  if(valid_PatRecThree_conf == false)
  {
      NQLog("AssemblyObjectAlignerView", NQLog::Critical) << "transmit_configuration"
      << ": invalid AssemblyObjectFinderPatRec::Configuration object from PatRec Widget #1, no action taken";

      valid_conf = false;

      conf.reset();

      return conf;
  }
  /// ------------------------------

  /// PatRec #4 --------------------
  bool valid_PatRecFour_conf(false);

  conf.PatRec_PSS2_configuration = patrecFour_wid_->get_configuration(valid_PatRecFour_conf);

  if(valid_PatRecFour_conf == false)
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

void MetrologyView::show_measured_angle(const bool sensor_PSs, const double val)
{
  std::stringstream posi_strs;
  posi_strs << val;

  if(!sensor_PSs){
      metro_mesang_PSp_linee_->setText(QString::fromStdString(posi_strs.str()));
  }else{
      metro_mesang_PSs_linee_->setText(QString::fromStdString(posi_strs.str()));
  }

  return;
}

void MetrologyView::show_results(const double dx, const double dy, const double da)
{
  std::stringstream posi_strs_dx;
  posi_strs_dx << dx;
  metro_dx_linee_->setText(QString::fromStdString(posi_strs_dx.str()));

  std::stringstream posi_strs_dy;
  posi_strs_dy << dy;
  metro_dy_linee_->setText(QString::fromStdString(posi_strs_dy.str()));

  std::stringstream posi_strs_da;
  posi_strs_da << da;
  metro_da_linee_->setText(QString::fromStdString(posi_strs_da.str()));

  return;
}

void MetrologyView::display_infoTab()
{
    QMessageBox::information(this, tr("Information - Metrology"),
            tr("<p>Click 'Perform Metrology' to run the module metrology.</p>"
            "<p>Make sure to move to the first marker of the PSp beforehand and to set template images for PatRec and B/W thresholds.</p>"
    ));

    return;
}

void MetrologyView::updateImage(const int stage, const QString& filename)
{
  NQLog("MetrologyView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";

  const cv::Mat img = assembly::cv_imread(filename, cv::IMREAD_UNCHANGED);

  this->updateImage(stage, img);

  return;
}

void MetrologyView::updateImage(const int stage, const cv::Mat& img)
{
  NQLog("MetrologyView", NQLog::Spam) << "updateImage(" << stage << ", image)";

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
  else if(stage == 3)
  {
    patrecThree_image_->setImage(img);
//    patrecTwo_image_->setZoomFactor(0.5);
  }
  else if(stage == 4)
  {
    patrecFour_image_->setImage(img);
//    patrecTwo_image_->setZoomFactor(0.5);
  }

  return;
}

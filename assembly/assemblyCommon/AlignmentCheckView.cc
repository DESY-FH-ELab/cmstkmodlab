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

#include <AlignmentCheckView.h>
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

AlignmentCheckView::AlignmentCheckView(QWidget* parent)
 : QWidget(parent)

 , alignm_cfg_wid_(nullptr)
 , alignm_res_wid_(nullptr)

   
 , alignm_completeAtPosOne_checkbox_(nullptr)
 , alignm_useAutoFocusing_checkbox_(nullptr)

 , alignm_exealig_radbu_(nullptr)
 , alignm_exealig_pusbu_(nullptr)

 , alignm_init_label_(nullptr)
   
 , alignm_angmax_dontIter_linee_(nullptr)
 , alignm_angmax_complete_linee_(nullptr)

 , patrecOne_wid_(nullptr)
 , patrecTwo_wid_(nullptr)
 , patrecThree_wid_(nullptr)
 , patrecFour_wid_(nullptr)
 , alignm_mesang_linee_(nullptr)


 , alignm_pspblx_linee_(nullptr)
 , alignm_pssblx_linee_(nullptr)
 , alignm_psstlx_linee_(nullptr)
 , alignm_psptlx_linee_(nullptr)
 , alignm_pspbly_linee_(nullptr)
 , alignm_pssbly_linee_(nullptr)
 , alignm_psstly_linee_(nullptr)
 , alignm_psptly_linee_(nullptr)

 , alignm_angle_linee_(nullptr)
 , alignm_xoffs_linee_(nullptr)
 , alignm_yoffs_linee_(nullptr)
   
 , patrecOne_image_ (nullptr)
 , patrecOne_scroll_(nullptr)
   
 , patrecTwo_image_ (nullptr)
 , patrecTwo_scroll_(nullptr)

 , patrecThree_image_ (nullptr)
 , patrecThree_scroll_(nullptr)

 , patrecFour_image_ (nullptr)
 , patrecFour_scroll_(nullptr)

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
  QVBoxLayout* alignm_init_lay = new QVBoxLayout;
  alignm_cfgexe_lay->addLayout(alignm_init_lay);

  alignm_init_label_ = new QLabel(tr("Initialize the stage for Alignment Check!"));
  alignm_init_lay->addWidget(alignm_init_label_, 4);
  //  connect(alignm_init_pusbu_, SIGNAL(clicked()), this, SLOT(transmit_configuration()));

  // execution modes
  QVBoxLayout* alignm_exe_lay = new QVBoxLayout;
  alignm_cfgexe_lay->addLayout(alignm_exe_lay);

  QGroupBox* alignm_exemod_box = new QGroupBox;
  alignm_exe_lay->addWidget(alignm_exemod_box);

  QVBoxLayout* alignm_exemod_lay = new QVBoxLayout;
  alignm_exemod_box->setLayout(alignm_exemod_lay);

  // option: go-back to start before stopping
  QHBoxLayout* alignm_exeopt_lay = new QHBoxLayout;
  alignm_exemod_lay->addLayout(alignm_exeopt_lay);

  alignm_completeAtPosOne_checkbox_ = new QCheckBox("Go back to marker-1 position before completion");
  alignm_exeopt_lay->addWidget(alignm_completeAtPosOne_checkbox_);

  // option: go-back to start before stopping
  alignm_useAutoFocusing_checkbox_ = new QCheckBox("Use Auto-Focusing");
  alignm_exeopt_lay->addWidget(alignm_useAutoFocusing_checkbox_);

  alignm_exemod_lay->addSpacing(5);


  // mode: align object
  QHBoxLayout* alignm_exealig_lay = new QHBoxLayout;
  alignm_exemod_lay->addLayout(alignm_exealig_lay);

  alignm_exealig_radbu_ = new QRadioButton;
  alignm_exealig_pusbu_ = new QPushButton(tr("Measure Alignment"));

  connect(alignm_exealig_radbu_, SIGNAL(toggled(bool)), alignm_exealig_pusbu_  , SLOT(setEnabled(bool)));

  alignm_exealig_lay->addSpacing(9);
  alignm_exealig_lay->addWidget(alignm_exealig_radbu_,  4);
  alignm_exealig_lay->addWidget(alignm_exealig_pusbu_, 70);

  connect(alignm_exealig_pusbu_, SIGNAL(clicked()), this, SLOT(transmit_configuration()));
  // ---

  alignm_cfgexe_lay->setStretch(0, 50);
  alignm_cfgexe_lay->setStretch(1, 50);

  // ----------

  alignm_cfg_lay->addSpacing(20);

  QVBoxLayout* alignm_angmax_lay = new QVBoxLayout;
  alignm_cfg_lay->addLayout(alignm_angmax_lay);

  // parameter: maximum angle for single rotation
  QHBoxLayout* alignm_angmax_dontIter_lay = new QHBoxLayout;
  alignm_angmax_lay->addLayout(alignm_angmax_dontIter_lay);

  // ----------

  alignm_cfg_lay->addSpacing(40);

  // PatRec Configuration
  QGridLayout* alignm_PRcfg_lay = new QGridLayout;
  alignm_cfg_lay->addLayout(alignm_PRcfg_lay);

  // PatRecWidget #1
  QGroupBox* patrecOne_cfg_box = new QGroupBox(tr("PatRec Marker #1 [BL PS-P]"));
  patrecOne_cfg_box->setObjectName("patrecOne_cfg_box");
  patrecOne_cfg_box->setStyleSheet("QWidget#patrecOne_cfg_box { font-weight : bold; color : blue; }");

  patrecOne_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecOne_wid_->setToolTip("Pattern Recognition Configuration #1 [BL PS-P]");

  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectAlignerView_PatRec1_template_fpath", "");
    if(fpath != ""){ patrecOne_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrecOne_wid_->threshold_lineEdit()        , config->getValue<int>("AlignmentCheckView_PatRec1_threshold"        , 10));

    assembly::QLineEdit_setText(patrecOne_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AlignmentCheckView_PatRec1_adaptiveThreshold", 587));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecOne_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finestep", 0.2));
  }

  patrecOne_cfg_box->setLayout(patrecOne_wid_->layout());
  // -----

  // PatRecWidget #2
  QGroupBox* patrecTwo_cfg_box = new QGroupBox(tr("PatRec Marker #2 [BL PS-S]"));
  patrecTwo_cfg_box->setObjectName("patrecTwo_cfg_box");
  patrecTwo_cfg_box->setStyleSheet("QWidget#patrecTwo_cfg_box { font-weight : bold; color : blue; }");

  patrecTwo_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecTwo_wid_->setToolTip("Pattern Recognition Configuration #2 [BL PS-S]");

  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectAlignerView_PatRec1_template_fpath", "");
    if(fpath != ""){ patrecTwo_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }


    assembly::QLineEdit_setText(patrecTwo_wid_->threshold_lineEdit()        , config->getValue<int>("AlignmentCheckView_PatRec2_threshold"        , 17));

    assembly::QLineEdit_setText(patrecTwo_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AlignmentCheckView_PatRec2_adaptiveThreshold", 587));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecTwo_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finestep", 0.2));
  }

  patrecTwo_cfg_box->setLayout(patrecTwo_wid_->layout());
  // -----


QGroupBox* patrecThree_cfg_box = new QGroupBox(tr("PatRec Marker #3  [TL PS-S]"));
  patrecThree_cfg_box->setObjectName("patrecThree_cfg_box");
  patrecThree_cfg_box->setStyleSheet("QWidget#patrecThree_cfg_box { font-weight : bold; color : blue; }");

  patrecThree_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecThree_wid_->setToolTip("Pattern Recognition Configuration #3 [TL PS-S]");

  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectAlignerView_PatRec2_template_fpath", "");
    if(fpath != ""){ patrecThree_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }


    assembly::QLineEdit_setText(patrecThree_wid_->threshold_lineEdit()        , config->getValue<int>("AlignmentCheckView_PatRec2_threshold"        , 17));
    assembly::QLineEdit_setText(patrecThree_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AlignmentCheckView_PatRec2_adaptiveThreshold", 587));
    assembly::QLineEdit_setText(patrecThree_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecThree_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecThree_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finestep", 0.2));

  }

  patrecThree_cfg_box->setLayout(patrecThree_wid_->layout());
  //------------


QGroupBox* patrecFour_cfg_box = new QGroupBox(tr("PatRec Marker #4  [TL PS-P]"));
  patrecFour_cfg_box->setObjectName("patrecFour_cfg_box");
  patrecFour_cfg_box->setStyleSheet("QWidget#patrecFour_cfg_box { font-weight : bold; color : blue; }");

  patrecFour_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrecFour_wid_->setToolTip("Pattern Recognition Configuration #4 [TL PS-P]");

  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectAlignerView_PatRec2_template_fpath", "");
    if(fpath != ""){ patrecFour_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }


    assembly::QLineEdit_setText(patrecFour_wid_->threshold_lineEdit()        , config->getValue<int>("AlignmentCheckView_PatRec2_threshold"        , 10));
    assembly::QLineEdit_setText(patrecFour_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AlignmentCheckView_PatRec2_adaptiveThreshold", 587));
    assembly::QLineEdit_setText(patrecFour_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrecFour_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrecFour_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectAlignerView_Pa\
tRec1_angles_finestep", 0.2));

  }

  patrecFour_cfg_box->setLayout(patrecFour_wid_->layout());
 
  // -----------





  
  alignm_PRcfg_lay->addWidget(patrecOne_cfg_box,0,0);
  alignm_PRcfg_lay->addWidget(patrecTwo_cfg_box,0,1 );

  
  alignm_PRcfg_lay->addWidget(patrecThree_cfg_box,1,0);
  alignm_PRcfg_lay->addWidget(patrecFour_cfg_box,1,1);
  

  // ----------

  // Default Configuration

  alignm_completeAtPosOne_checkbox_->setEnabled(false);
  alignm_completeAtPosOne_checkbox_->setChecked(true);

  alignm_useAutoFocusing_checkbox_->setEnabled(true);
   alignm_useAutoFocusing_checkbox_->setChecked(true);

  alignm_exealig_radbu_->setChecked(true);
  alignm_exealig_pusbu_->setEnabled(true);

  // ---------------------

  // Results -------------
  alignm_res_wid_ = new QWidget;
  toolbox->addItem(alignm_res_wid_, "Photos of PatRec results used for Alignment Calculation");

  QVBoxLayout* alignm_res_lay = new QVBoxLayout;
  alignm_res_wid_->setLayout(alignm_res_lay);
  // alignm_res2_wid_->setLayout(alignm_res_lay);
  
  // Numerical Results
  QHBoxLayout* alignm_mesang_lay = new QHBoxLayout;
  alignm_res_lay->addLayout(alignm_mesang_lay);

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



  patrecThree_image_ = new AssemblyUEyeView(this);
  patrecThree_image_->setMinimumSize(200, 200);
  patrecThree_image_->setPalette(palette);
  patrecThree_image_->setBackgroundRole(QPalette::Background);
  patrecThree_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecThree_image_->setScaledContents(true);
  patrecThree_image_->setAlignment(Qt::AlignCenter);


  patrecThree_scroll_ = new QScrollArea(this);
  patrecThree_scroll_->setMinimumSize(200, 200);
  patrecThree_scroll_->setPalette(palette);
  patrecThree_scroll_->setBackgroundRole(QPalette::Background);
  patrecThree_scroll_->setAlignment(Qt::AlignCenter);
  patrecThree_scroll_->setWidget(patrecThree_image_);
  patrecThree_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  alignm_img_lay->addWidget(patrecThree_scroll_, 1, 0);


  patrecFour_image_ = new AssemblyUEyeView(this);
  patrecFour_image_->setMinimumSize(200, 200);
  patrecFour_image_->setPalette(palette);
  patrecFour_image_->setBackgroundRole(QPalette::Background);
  patrecFour_image_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  patrecFour_image_->setScaledContents(true);
  patrecFour_image_->setAlignment(Qt::AlignCenter);

  patrecFour_scroll_ = new QScrollArea(this);
  patrecFour_scroll_->setMinimumSize(200, 200);
  patrecFour_scroll_->setPalette(palette);
  patrecFour_scroll_->setBackgroundRole(QPalette::Background);
  patrecFour_scroll_->setAlignment(Qt::AlignCenter);
  patrecFour_scroll_->setWidget(patrecFour_image_);
  patrecFour_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  alignm_img_lay->addWidget(patrecFour_scroll_, 1, 1);

  // ----------- Numerical Results Widget
  alignm_res_wid_ = new QWidget;
  toolbox->addItem(alignm_res_wid_, "Numerical Results of Alignment Check");

  QGridLayout* alignm_nres_lay = new QGridLayout;
  alignm_res_wid_->setLayout(alignm_nres_lay);

  QLabel* alignm_pspbl_label = new QLabel("X,Y positon for PSP BL Fiducial [mm]");
  alignm_pspblx_linee_ = new QLineEdit("");
  alignm_pspblx_linee_->setReadOnly(true);
  alignm_pspbly_linee_ = new QLineEdit("");
  alignm_pspbly_linee_->setReadOnly(true);

  alignm_nres_lay->addWidget(alignm_pspbl_label ,0 ,0);
  alignm_nres_lay->addWidget(alignm_pspblx_linee_,0 ,1);
  alignm_nres_lay->addWidget(alignm_pspbly_linee_,0 ,2);  
  alignm_nres_lay->addWidget(new QLabel);

  QLabel* alignm_pssbl_label = new QLabel("X,Y positon for PSS BL Fiducial [mm]");
  alignm_pssblx_linee_ = new QLineEdit("");
  alignm_pssblx_linee_->setReadOnly(true);
  alignm_pssbly_linee_ = new QLineEdit("");
  alignm_pssbly_linee_->setReadOnly(true);

  alignm_nres_lay->addWidget(alignm_pssbl_label ,1 ,0);
  alignm_nres_lay->addWidget(alignm_pssblx_linee_,1 ,1);
  alignm_nres_lay->addWidget(alignm_pssbly_linee_,1 ,2);  
  alignm_nres_lay->addWidget(new QLabel);

  QLabel* alignm_psstl_label = new QLabel("X,Y positon for PSS TL Fiducial [mm]");
  alignm_psstlx_linee_ = new QLineEdit("");
  alignm_psstlx_linee_->setReadOnly(true);
  alignm_psstly_linee_ = new QLineEdit("");
  alignm_psstly_linee_->setReadOnly(true);

  alignm_nres_lay->addWidget(alignm_psstl_label ,2 ,0);
  alignm_nres_lay->addWidget(alignm_psstlx_linee_,2 ,1);
  alignm_nres_lay->addWidget(alignm_psstly_linee_,2 ,2);  
  alignm_nres_lay->addWidget(new QLabel);

  QLabel* alignm_psptl_label = new QLabel("X,Y positon for PSP TL Fiducial [mm]");
  alignm_psptlx_linee_ = new QLineEdit("");
  alignm_psptlx_linee_->setReadOnly(true);
  alignm_psptly_linee_ = new QLineEdit("");
  alignm_psptly_linee_->setReadOnly(true);

  alignm_nres_lay->addWidget(alignm_psptl_label ,3 ,0);
  alignm_nres_lay->addWidget(alignm_psptlx_linee_,3 ,1);
  alignm_nres_lay->addWidget(alignm_psptly_linee_,3 ,2);  
  alignm_nres_lay->addWidget(new QLabel);


  QLabel* alignm_angle_label = new QLabel("Angle from the PSS to the PSP [\xB5 rad]");
  alignm_angle_linee_ = new QLineEdit("");
  alignm_angle_linee_->setReadOnly(true);
  alignm_nres_lay->addWidget(alignm_angle_label ,4 ,0);
  alignm_nres_lay->addWidget(alignm_angle_linee_,4 ,1);

  QLabel* alignm_offs_label = new QLabel("X,Y Offests from PSS to PSP [mm]");
  alignm_xoffs_linee_ = new QLineEdit("");
  alignm_xoffs_linee_->setReadOnly(true);

  alignm_yoffs_linee_ = new QLineEdit("");
  alignm_yoffs_linee_->setReadOnly(true);
  
  alignm_nres_lay->addWidget(alignm_offs_label ,5 ,0);
  alignm_nres_lay->addWidget(alignm_xoffs_linee_,5 ,1);
  alignm_nres_lay->addWidget(alignm_yoffs_linee_,5 ,2);


  
  // ---------------------

  QHBoxLayout* buttons_lay = new QHBoxLayout;

  button_emergencyStop_ = new QPushButton(tr("Emergency Stop"));

  buttons_lay->addWidget(new QLabel, 33);
  buttons_lay->addWidget(new QLabel, 33);
  buttons_lay->addWidget(button_emergencyStop_, 33);

  layout->addLayout(buttons_lay);
}

void AlignmentCheckView::keyReleaseEvent(QKeyEvent* event)
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


void AlignmentCheckView::enable()
{
  this->setEnabled(true);
}

void AlignmentCheckView::show_measured_angle(const double val)
{
  std::stringstream posi_strs;
  posi_strs << val;

  alignm_angle_linee_->setText(QString::fromStdString(posi_strs.str()));

  return;
}
void AlignmentCheckView::show_pspbl_pos(const double val1, const double val2)
{
  std::stringstream posi_strs1;
  std::stringstream posi_strs2;
  
  posi_strs1 << val1;
  posi_strs2 << val2;

  alignm_pspblx_linee_->setText(QString::fromStdString(posi_strs1.str()));
  alignm_pspbly_linee_->setText(QString::fromStdString(posi_strs2.str()));

  return;
}

void AlignmentCheckView::show_pssbl_pos(const double val1, const double val2)
{
  std::stringstream posi_strs1;
  std::stringstream posi_strs2;
  
  posi_strs1 << val1;
  posi_strs2 << val2;

  alignm_pssblx_linee_->setText(QString::fromStdString(posi_strs1.str()));
  alignm_pssbly_linee_->setText(QString::fromStdString(posi_strs2.str()));

  return;
}

void AlignmentCheckView::show_psstl_pos(const double val1, const double val2)
{
  std::stringstream posi_strs1;
  std::stringstream posi_strs2;
  
  posi_strs1 << val1;
  posi_strs2 << val2;

  alignm_psstlx_linee_->setText(QString::fromStdString(posi_strs1.str()));
  alignm_psstly_linee_->setText(QString::fromStdString(posi_strs2.str()));

  return;
}

void AlignmentCheckView::show_psptl_pos(const double val1, const double val2)
{
  std::stringstream posi_strs1;
  std::stringstream posi_strs2;
  
  posi_strs1 << val1;
  posi_strs2 << val2;

  alignm_psptlx_linee_->setText(QString::fromStdString(posi_strs1.str()));
  alignm_psptly_linee_->setText(QString::fromStdString(posi_strs2.str()));

  return;
}
void AlignmentCheckView::show_offs_pos(const double val1, const double val2)
{
  std::stringstream posi_strs1;
  std::stringstream posi_strs2;
  
  posi_strs1 << val1;
  posi_strs2 << val2;

  alignm_xoffs_linee_->setText(QString::fromStdString(posi_strs1.str()));
  alignm_yoffs_linee_->setText(QString::fromStdString(posi_strs2.str()));

  return;
}



void AlignmentCheckView::updateImage(const int stage, const QString& filename)
{
  NQLog("AlignmentCheckView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";

  const cv::Mat img = assembly::cv_imread(filename, CV_LOAD_IMAGE_UNCHANGED);

  this->updateImage(stage, img);

  return;
}

void AlignmentCheckView::updateImage(const int stage, const cv::Mat& img)
{
  NQLog("AlignmentCheckView", NQLog::Spam) << "updateImage(" << stage << ", image)";

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


void AlignmentCheckView::update_autofocusing_checkbox(const int state)
{
  if(state == 2)
  {
    alignm_useAutoFocusing_checkbox_->setEnabled(false);
    alignm_useAutoFocusing_checkbox_->setChecked(true);
  }
  else if(state == 0)
  {
    alignm_useAutoFocusing_checkbox_->setEnabled(true);
  }

  return;
}

void AlignmentCheckView::transmit_configuration()
{
  bool valid_conf(false);

  const auto& conf = this->get_configuration(valid_conf);

  NQLog("AlignmentCheckView", NQLog::Spam) << "transmit_configuration"
						  << ": emitting signal \"configuration(AlignmentCheckView::Configuration)\"";

  emit configuration(conf);
}

AlignmentCheck::Configuration AlignmentCheckView::get_configuration(bool& valid_conf) const
{
  AlignmentCheck::Configuration conf;

  /// Object (dX, dY) Dimensions ---
  /// ------------------------------

  /// Execution --------------------

  // parameter: maximum angle for single rotation
 

  // execution option(s)
  conf.complete_at_position1 = alignm_completeAtPosOne_checkbox_->isChecked();
  conf.use_autofocusing      = alignm_useAutoFocusing_checkbox_ ->isChecked();
  conf.only_measure_angle = false;


  /// PatRec #1 --------------------
  bool valid_PatRecOne_conf(false);

  conf.PatRecOne_configuration = patrecOne_wid_->get_configuration(valid_PatRecOne_conf);

  if(valid_PatRecOne_conf == false)
  {
    NQLog("AlignmentCheckView", NQLog::Critical) << "transmit_configuration"
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
    NQLog("AlignmentCheckView", NQLog::Critical) << "transmit_configuration"
       << ": invalid AssemblyObjectFinderPatRec::Configuration object from PatRec Widget #2, no action taken";

    valid_conf = false;

    conf.reset();

    return conf;
  }
  /// ------------------------------
  bool valid_PatRecThree_conf(true);
  conf.PatRecThree_configuration = patrecThree_wid_->get_configuration(valid_PatRecThree_conf);

  bool valid_PatRecFour_conf(true);
  conf.PatRecFour_configuration = patrecFour_wid_->get_configuration(valid_PatRecFour_conf);
  
  valid_conf = true;

  return conf;
}

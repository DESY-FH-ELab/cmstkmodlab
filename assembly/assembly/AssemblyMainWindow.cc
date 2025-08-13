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

#include <ConradModel.h>
#include <VellemanModel.h>

#include <AssemblyMainWindow.h>
#include <AssemblyLogFileController.h>
#include <AssemblyLogFileView.h>
#include <AssemblyUtilities.h>
#include <DeviceState.h>

#include <string>
#include <filesystem>

#include <QApplication>

#include <opencv2/opencv.hpp>

AssemblyMainWindow::AssemblyMainWindow(const QString& outputdir_path, const QString& logfile_path, QWidget* parent) :
  QMainWindow(parent),

  // Low-Level Controllers (Motion, Camera, Vacuum)
  relayCardModel_(nullptr),
  relayCardManager_(nullptr),

  motion_model_(nullptr),
  motion_manager_(nullptr),
  motion_manager_view_(nullptr),
  motion_thread_(nullptr),
  motionSettings_(nullptr),
  motionSettingsWidget_(nullptr),

  PU_status_(nullptr),
  SP_status_(nullptr),
  BP_status_(nullptr),
  SUB_status_(nullptr),
  vacuum_pickup_(0),
  vacuum_spacer_(0),
  vacuum_basepl_(0),
  vacuum_sub_(0),

  camera_model_(nullptr),
  camera_thread_(nullptr),
//  camera_widget_(nullptr),
  camera_(nullptr),
  camera_ID_(0),

  // High-Level Controllers
  image_ctr_(nullptr),
  zfocus_finder_(nullptr),
  thresholder_(nullptr),
  aligner_(nullptr),
  metrology_(nullptr),
  assembly_(nullptr),
  assemblyV2_(nullptr),
  multipickup_tester_(nullptr),

  finder_(nullptr),
  finder_thread_(nullptr),

  smart_motion_(nullptr),

  // Views
  toolBar_(nullptr),
  main_tab(nullptr),

//  finderView_(nullptr),
//  edgeView_(nullptr),
//  rawView_(nullptr),
  image_view_(nullptr),
  thresholder_view_(nullptr),
  finder_view_(nullptr),
  aligner_view_(nullptr),
  assembly_view_(nullptr),
  metrology_view_(nullptr),
  assemblyV2_view_(nullptr),
  toolbox_view_(nullptr),
  params_view_(nullptr),
  hwctr_view_(nullptr),

  stopwatch_wid_(nullptr),

  button_mainEmergencyStop_(nullptr),
  button_info_(nullptr),
  autofocus_checkbox_(nullptr),

  // flags
  images_enabled_(false),
  aligner_connected_(false),
  metrology_connected_(false),

  // timing
  testTimerCount_(0.),
  liveTimer_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();
    if(config == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "\e[1;31m-------------------------------------------------------------------------------------------------------\e[0m";
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "\e[1;31mInitialization error: ApplicationConfig::instance() not initialized (null pointer), exiting constructor\e[0m";
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "\e[1;31m-------------------------------------------------------------------------------------------------------\e[0m";

      return;
    }

    /// Parameters
    ///   * instance created up here, so controllers can access it
    auto parameters_file_str = config->getValue<std::string>("main", "AssemblyParameters_file_path");
    std::filesystem::path parameters_file_path(parameters_file_str);
    if(parameters_file_path.is_absolute())
    {
      NQLog("AssemblyMainWindow", NQLog::Debug) << "Provided absolute path to parameter file. Opening file: " << parameters_file_path;
      config->append(parameters_file_path, "parameters");
    } else {
      auto abs_path = std::filesystem::path(Config::CMSTkModLabBasePath);
      abs_path /= parameters_file_path;
      NQLog("AssemblyMainWindow", NQLog::Debug) << "Provided relative path to parameter file - compiled absolute path. Opening file: " << abs_path;
      config->append(abs_path, "parameters");
    }
    /// -------------------

    /// Motion
    motion_model_ = new LStepExpressModel(
      config->getValue<std::string>("main", "LStepExpressDevice"),
      config->getValue<std::string>("main", "LStepExpressDevice_ver"),
      config->getValue<std::string>("main", "LStepExpressDevice_iver"),
      1000,
      1000
    );

    connect(motion_model_, SIGNAL(motionInformationChanged()), this, SLOT(update_stage_position()));
    connect(motion_model_, SIGNAL(informationChanged()), this, SLOT(update_stage_position()));

    motion_manager_ = new LStepExpressMotionManager(motion_model_);
    connect(motion_manager_->model(), SIGNAL(emergencyStop_request()), motion_manager_, SLOT(clear_motion_queue()));

    motion_thread_  = new LStepExpressMotionThread(motion_manager_, this);
    motion_thread_->start();

    connect(motion_manager_, SIGNAL(restartMotionStage_request()), this, SLOT(messageBox_restartMotionStage())); //Display a pop-up GUI message whenever the MS gets automatically restarted

    connect(motion_manager_, SIGNAL(warn_user_limit(double, char, double, double)), this, SLOT(warn_on_stage_limits(double, char, double, double)));
    /// -------------------

    /// Camera
    auto camera_config_interval = config->getDefaultValue<int>("main", "camera_config_interval", 10);
    camera_model_ = new AssemblyUEyeModel_t(camera_config_interval);
    camera_model_->updateInformation();

    camera_thread_ = new AssemblyUEyeCameraThread(camera_model_, this);
    camera_thread_->start();

    camera_ID_ = config->getDefaultValue<unsigned int>("main", "camera_ID", 1);
    camera_ = camera_model_->getCameraByID(camera_ID_);
    if(camera_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Critical) << "---------------------------------------------------------------------------------";
      NQLog("AssemblyMainWindow", NQLog::Critical) << "initialization error: null pointer to AssemblyVUEyeCamera object (camera_ID=" << camera_ID_ << ")";
      NQLog("AssemblyMainWindow", NQLog::Critical) << "---------------------------------------------------------------------------------";
    }
    /// -------------------

    std::string assembly_center_str = QString::fromStdString(config->getValue<std::string>("main", "assembly_center")).toUpper().toStdString();
    if(!(assembly_center_str == "FNAL" || assembly_center_str == "BROWN" || assembly_center_str == "DESY")) {
        NQLog("AssemblyAssemblyV2", NQLog::Fatal) << "Invalid assembly center provided: \"" << assembly_center_str << "\". Provide one of the following options: \"FNAL\", \"BROWN\", \"DESY\"";
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setInformativeText(QString("Invalid assembly center provided (\"%1\").\nProvide one of the following options: \"FNAL\", \"BROWN\", \"DESY\"").arg(QString::fromStdString(assembly_center_str)));
        msgBox->setStandardButtons(QMessageBox::Ok);
        int ret = msgBox->exec();
        exit(1);
    }

    /// Vacuum Manager
    std::string relayCardDevice = config->getValue<std::string>("main", "RelayCardDevice");
    if (relayCardDevice=="Velleman")
    {
    	relayCardModel_ = new VellemanModel(config->getValue<std::string>("main", "VellemanDevice"));
    } else {
    	relayCardModel_ = new ConradModel(config->getValue<std::string>("main", "ConradDevice"));
    }

    if(relayCardModel_->getDeviceState()==State::OFF) {
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "Relay card device off - cannot continue.";

      QMessageBox* msgBox = new QMessageBox;
      msgBox->setInformativeText("Relay card device not available!");

      msgBox->setStandardButtons(QMessageBox::Ok);

      int ret = msgBox->exec();
    }

    relayCardManager_ = new RelayCardManager(relayCardModel_);
    /// -------------------

    /// TAB: ASSEMBLY FUNCTIONALITIES --------------------------
    QTabWidget* assembly_tab = new QTabWidget;

    assembly_tab->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    assembly_tab->setTabPosition(QTabWidget::North);

    // IMAGE VIEW ----------------------------------------------
    const QString tabname_Image("Image Viewer");

    image_view_ = new AssemblyImageView(assembly_tab);
    assembly_tab->addTab(image_view_, tabname_Image);

    // Thresholder
    thresholder_ = new AssemblyThresholder();

    // Z-focus finder
    zfocus_finder_ = new AssemblyZFocusFinder(outputdir_path+"/AssemblyZFocusFinder", camera_, motion_manager_);

    connect(image_view_, SIGNAL(threshold_request(int)), thresholder_, SLOT(update_image_binary_threshold(int)));
    connect(image_view_, SIGNAL(adaptiveThreshold_request(int)), thresholder_, SLOT(update_image_binary_adaptiveThreshold(int)));

    connect(thresholder_, SIGNAL(updated_image_binary(cv::Mat)), image_view_, SLOT(update_image_binary(cv::Mat)));

    connect(zfocus_finder_, SIGNAL(show_zscan(QLineSeries&))     , image_view_   , SLOT(update_image_zscan(QLineSeries&)));
    connect(zfocus_finder_, SIGNAL(text_update_request(double))  , image_view_   , SLOT(update_text(double)));

    connect(zfocus_finder_, SIGNAL(focus_config_request())       , image_view_   , SLOT(acquire_autofocus_config()));
    connect(image_view_   , SIGNAL(autofocus_config(double, int)), zfocus_finder_, SLOT(update_focus_config(double, int)));

    image_view_->update_autofocus_config(zfocus_finder_->zrange(), zfocus_finder_->points());

    connect(zfocus_finder_, SIGNAL(sig_update_progBar(int)), image_view_, SLOT(update_progBar(int)));

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Image;
    // ---------------------------------------------------------

    // PATTERN-RECOGNITION VIEW --------------------------------
    const QString tabname_PatRec("Pattern Recognition");

    finder_view_ = new AssemblyObjectFinderPatRecView(assembly_tab);
    assembly_tab->addTab(finder_view_, tabname_PatRec);

    // finder
    finder_ = new AssemblyObjectFinderPatRec(thresholder_, outputdir_path+"/AssemblyObjectFinderPatRec", "rotations");

    finder_->save_subdir_images(true);

    finder_view_->connect_to_finder(finder_);

    finder_thread_ = new AssemblyObjectFinderPatRecThread(finder_, this);
    finder_thread_->start();

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_PatRec;
    // ---------------------------------------------------------

    // ALIGNMENT VIEW ------------------------------------------
    const QString tabname_Alignm("Alignment");

    aligner_view_ = new AssemblyObjectAlignerView(assembly_tab);
    idx_alignment_tab = assembly_tab->addTab(aligner_view_, tabname_Alignm);

    // aligner
    aligner_ = new AssemblyObjectAligner(motion_manager_);

    connect(aligner_view_, SIGNAL(configuration(AssemblyObjectAligner::Configuration)), this, SLOT(start_objectAligner(AssemblyObjectAligner::Configuration)));

    aligner_view_->PatRecOne_Image()->connectImageProducer(aligner_, SIGNAL(image_PatRecOne(cv::Mat)));
    aligner_view_->PatRecTwo_Image()->connectImageProducer(aligner_, SIGNAL(image_PatRecTwo(cv::Mat)));

    connect(aligner_view_->button_alignerEmergencyStop(), SIGNAL(clicked()), this, SLOT(disconnect_objectAligner()));
    connect(aligner_view_->button_alignerEmergencyStop(), SIGNAL(clicked()), motion_manager_, SLOT(emergency_stop()));
    connect(aligner_view_->button_alignerEmergencyStop(), SIGNAL(clicked()), zfocus_finder_ , SLOT(emergencyStop()));

    connect(this, SIGNAL(set_alignmentMode_PSP_request()), aligner_view_, SLOT(set_alignmentMode_PSP()));
    connect(this, SIGNAL(set_alignmentMode_PSS_request()), aligner_view_, SLOT(set_alignmentMode_PSS()));

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Alignm;
    // ---------------------------------------------------------

    // ASSEMBLY VIEW -------------------------------------------
    const QString tabname_Assembly("Assembly");

    smart_motion_ = new AssemblySmartMotionManager(motion_manager_);

    const int assembly_sequence(config->getDefaultValue<int>("main", "assembly_sequence", 1));

    if(assembly_sequence == 1)
    {
      assembly_ = new AssemblyAssembly(motion_manager_, relayCardManager_, smart_motion_);

      assembly_view_ = new AssemblyAssemblyView(assembly_, assembly_tab);
      assembly_tab->addTab(assembly_view_, tabname_Assembly);

      NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Assembly
         << " (assembly_sequence = " << assembly_sequence << ")";
    }
    else if(assembly_sequence == 2)
    {
      assemblyV2_ = new AssemblyAssemblyV2(motion_manager_, relayCardManager_, smart_motion_);

      assemblyV2_view_ = new AssemblyAssemblyV2View(assemblyV2_, assembly_tab);
      assembly_tab->addTab(assemblyV2_view_, tabname_Assembly);

      connect(assemblyV2_, SIGNAL(switchToAlignmentTab_PSP_request()), this, SLOT(update_alignment_tab_psp()));
      connect(assemblyV2_, SIGNAL(switchToAlignmentTab_PSS_request()), this, SLOT(update_alignment_tab_pss()));

      NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Assembly
         << " (assembly_sequence = " << assembly_sequence << ")";
    }
    else
    {
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "invalid value for configuration parameter \"assembly_sequence\" ("
         << assembly_sequence << ") -> GUI Tab " << tabname_Assembly << " will not be created";
    }
    // ---------------------------------------------------------

    // HARDWARE CONTROLLERs VIEW (motion/vacuum) ---------------
    const QString tabname_HWCtrl("HW Controllers (Motion/Vacuum)");

    hwctr_view_ = new AssemblyHardwareControlView(motion_manager_, assembly_tab);
    assembly_tab->addTab(hwctr_view_, tabname_HWCtrl);

    // enable motion stage controllers at startup
    const bool startup_motion_stage = config->getDefaultValue<bool>("main", "startup_motion_stage", false);

    if(startup_motion_stage)
    {
      hwctr_view_->LStepExpress_Widget()->enableMotionControllers();

      // single-shot signal to switch ON motion stage axes automatically
      const int time_to_axes_startup(1.5 * motion_manager_->model()->updateInterval());
      QTimer::singleShot(time_to_axes_startup, hwctr_view_->LStepExpress_Widget(), SLOT(restart()));

      NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_HWCtrl;
    }

    //-- Automatically restart the Motion Stage when the 'positions vector has invalid size' error appears
    connect(motion_manager_, SIGNAL(restartMotionStage_request()), hwctr_view_->LStepExpress_Widget(), SLOT(restart()));
    // ---------------------------------------------------------

    // PARAMETERS VIEW -----------------------------------------
    const QString tabname_Parameters("Parameters");

    params_view_ = new AssemblyParametersView(assembly_tab);
    assembly_tab->addTab(params_view_, tabname_Parameters);

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Parameters;

    connect(params_view_, SIGNAL(request_moveToAbsRefPosition(double,double,double,double)), motion_manager_, SLOT(moveAbsolute(double,double,double,double)));
    connect(params_view_, SIGNAL(request_moveByRelRefDistance(double,double,double,double)), motion_manager_, SLOT(moveRelative(double,double,double,double)));
    // ---------------------------------------------------------

    connect(image_view_, SIGNAL(sigRequestMoveRelative(double,double,double,double)), motion_manager_, SLOT(moveRelative(double,double,double,double)));
    connect(motion_manager_, SIGNAL(motion_finished()), image_view_, SLOT(InfoMotionFinished()));
    // ---------------------------------------------------------

    /// TAB: MANUAL CONTROLLERS AND PARAMETERS -----------------
    QTabWidget* controls_tab = new QTabWidget;

    controls_tab->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    controls_tab->setTabPosition(QTabWidget::North);
    // controls_tab->setUsesScrollButtons(false); //Make all the widgets fit in the tab's width (else : scroll to invisible widgets)

//    finderView_ = new AssemblyUEyeSnapShooter(assembly_tab);
//    assembly_tab->addTab(finderView_, "finder");
//
//    edgeView_ = new AssemblyUEyeSnapShooter(assembly_tab);
//    assembly_tab->addTab(edgeView_, "edges");
//
//    rawView_ = new AssemblyUEyeSnapShooter(assembly_tab);
//    assembly_tab->addTab(rawView_, "raw");
//
//    // U-EYE VIEW ----------------------------------------------
//    const QString tabname_uEye("uEye");
//
//    camera_widget_ = new AssemblyUEyeWidget(camera_model_, this);
//    controls_tab->addTab(camera_widget_, tabname_uEye);
//
//    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_uEye;
//    // ---------------------------------------------------------

    // Metrology VIEW ------------------------------------------
    const QString tabname_Metrology("Metrology");

    metrology_view_ = new MetrologyView(controls_tab);
    controls_tab->addTab(metrology_view_, tabname_Metrology);

    // metrology
    metrology_ = new Metrology(motion_manager_);

    connect(metrology_view_, SIGNAL(configuration(Metrology::Configuration)), this, SLOT(start_metrology(Metrology::Configuration)));

    connect(metrology_view_, SIGNAL(go_to_PSP_marker_signal()), metrology_, SLOT(move_to_PSP_marker()));
    connect(metrology_view_, SIGNAL(go_to_PSS_marker_signal()), metrology_, SLOT(move_to_PSS_marker()));
    connect(metrology_view_, SIGNAL(go_to_PSP_BL_marker_signal()), metrology_, SLOT(move_to_PSP_BL_marker()));
    connect(metrology_view_, SIGNAL(go_to_PSS_BL_marker_signal()), metrology_, SLOT(move_to_PSS_BL_marker()));
    connect(metrology_view_, SIGNAL(go_to_marker_signal()), metrology_, SLOT(move_to_start()));
    connect(metrology_view_, SIGNAL(enable_vacuum_baseplate(int)), relayCardManager_, SLOT(enableVacuum(int)));
    connect(relayCardManager_, SIGNAL(vacuumChannelState(int, SwitchState)), metrology_view_, SLOT(updateVacuumChannelState(int, SwitchState)));

    metrology_view_->PatRecOne_Image()->connectImageProducer(metrology_, SIGNAL(image_PatRecOne(cv::Mat)));
    metrology_view_->PatRecTwo_Image()->connectImageProducer(metrology_, SIGNAL(image_PatRecTwo(cv::Mat)));
    metrology_view_->PatRecThree_Image()->connectImageProducer(metrology_, SIGNAL(image_PatRecThree(cv::Mat)));
    metrology_view_->PatRecFour_Image()->connectImageProducer(metrology_, SIGNAL(image_PatRecFour(cv::Mat)));

    connect(metrology_view_->button_metrologyEmergencyStop(), SIGNAL(clicked()), metrology_view_, SLOT(metrology_abort()));
    connect(metrology_view_->button_metrologyEmergencyStop(), SIGNAL(clicked()), this, SLOT(disconnect_metrology()));
    connect(metrology_view_->button_metrologyEmergencyStop(), SIGNAL(clicked()), motion_manager_, SLOT(emergency_stop()));
    connect(metrology_view_->button_metrologyEmergencyStop(), SIGNAL(clicked()), zfocus_finder_ , SLOT(emergencyStop()));
    connect(metrology_view_->button_metrologyClearResults(), SIGNAL(clicked()), metrology_, SLOT(clear_results()));

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Metrology;
    // ---------------------------------------------------------

    // TOOLBOX VIEW --------------------------------------------
    const QString tabname_Toolbox("Toolbox");

    subassembly_pickup_ = new AssemblySubassemblyPickup(motion_manager_, relayCardManager_, smart_motion_);

    connect(subassembly_pickup_, SIGNAL(switchToAlignmentTab_PSS_request()), this, SLOT(update_alignment_tab_pss()));

    toolbox_view_ = new AssemblyToolboxView(motion_manager_, subassembly_pickup_, controls_tab);
    controls_tab->addTab(toolbox_view_, tabname_Toolbox);

    // multi-pickup tester
    multipickup_tester_ = new AssemblyMultiPickupTester(motion_manager_);

    connect(toolbox_view_->MultiPickupTester_Widget(), SIGNAL(multipickup_request(AssemblyMultiPickupTester::Configuration)), this, SLOT(start_multiPickupTest(AssemblyMultiPickupTester::Configuration)));

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Toolbox;
    // ---------------------------------------------------------

    // TERMINAL VIEW -------------------------------------------
    const QString tabname_Terminal("Terminal View");

    // list of keywords for skimmed outputs in Terminal View
    const QStringList log_skim_keys({
      "[AssemblyImageController]",
      "[AssemblyZFocusFinder]",
      "[AssemblyObjectFinderPatRec]",
      "[AssemblyObjectAligner]",
      "[AssemblyAssembly]",
    });

    AssemblyLogFileView* logview = new AssemblyLogFileView(log_skim_keys);
    controls_tab->addTab(logview, tabname_Terminal);

    AssemblyLogFileController* logctrl = new AssemblyLogFileController(logfile_path);

    connect(logctrl, SIGNAL(new_lines(QStringList)), logview, SLOT(append_text(QStringList)));

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Terminal;
    // ---------------------------------------------------------

    // MOTION-SETTINGS VIEW ------------------------------------
    const QString tabname_MotionSettings("Motion Settings");

    motionSettings_ = new LStepExpressSettings(motion_model_, controls_tab);

    motionSettingsWidget_ = new LStepExpressSettingsWidget(motionSettings_, controls_tab);
    controls_tab->addTab(motionSettingsWidget_, tabname_MotionSettings);

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_MotionSettings;
    // ---------------------------------------------------------

    /// Upper Toolbar ------------------------------------------
    toolBar_ = addToolBar("Tools");
    toolBar_ ->addAction("Camera ON"     , this, SLOT( enable_images()));
    toolBar_ ->addAction("Camera OFF"    , this, SLOT(disable_images()));
    toolBar_ ->addAction("Snapshot"      , this, SLOT(    get_image ()));

    autofocus_checkbox_ = new QCheckBox("Auto-Focusing", this);
    toolBar_->addWidget(autofocus_checkbox_);
    connect(autofocus_checkbox_, SIGNAL(stateChanged(int)), this, SLOT(changeState_autofocus(int)));
    connect(autofocus_checkbox_, SIGNAL(stateChanged(int)), aligner_view_, SLOT(update_autofocusing_checkbox(int)));

    connect(zfocus_finder_, SIGNAL(emergencyStopped()), this, SLOT(restore_autofocus_settings()));

    // toolBar_ ->addAction("Emergency Stop", motion_manager_->model(), SLOT(emergencyStop()));
    button_mainEmergencyStop_ = new QPushButton(tr("Emergency STOP"));
    button_mainEmergencyStop_->setStyleSheet("QPushButton { background-color: rgb(255, 129, 123); font: 18px; border-radius: 8px; padding: 7px; } QPushButton:hover { background-color: red; font: bold 18px; border-radius: 8px; padding: 2px; }");
    toolBar_->addWidget(button_mainEmergencyStop_);

    connect(button_mainEmergencyStop_, SIGNAL(clicked()), motion_manager_, SLOT(emergency_stop()));
    connect(button_mainEmergencyStop_, SIGNAL(clicked()), this, SLOT(disconnect_objectAligner()));
    connect(button_mainEmergencyStop_, SIGNAL(clicked()), zfocus_finder_ , SLOT(emergencyStop()));
    connect(button_mainEmergencyStop_, SIGNAL(clicked()), metrology_view_, SLOT(metrology_abort()));
    connect(button_mainEmergencyStop_, SIGNAL(clicked()), this, SLOT(disconnect_metrology()));
    connect(button_mainEmergencyStop_, SIGNAL(clicked()), this, SLOT(disconnect_multiPickupTest()));

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar_->addWidget(spacer);

    stopwatch_wid_ = new AssemblyStopwatchWidget();
    toolBar_->addWidget(stopwatch_wid_);

    QWidget *spacer2 = new QWidget();
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar_->addWidget(spacer2);

    QWidget *vac_wid = new QWidget();

    QGridLayout* vac_lay = new QGridLayout;

    QLabel* vac_label = new QLabel("Vacuum status");
    vac_lay->addWidget(vac_label, 0, 0, 2, 1);

    QLabel* PU_label = new QLabel("PU");
    QLabel* SP_label = new QLabel("SP");
    QLabel* BP_label = new QLabel("BP");
    QLabel* SUB_label = new QLabel("SUB");

    vac_lay->addWidget(PU_label, 0, 1, Qt::AlignCenter);
    vac_lay->addWidget(SP_label, 0, 2, Qt::AlignCenter);
    vac_lay->addWidget(BP_label, 0, 3, Qt::AlignCenter);
    vac_lay->addWidget(SUB_label, 0, 4, Qt::AlignCenter);

    QString filename(Config::CMSTkModLabBasePath.c_str());

    vacuum_pickup_ = config->getValue<int>("main", "Vacuum_PickupTool");
    vacuum_spacer_ = config->getValue<int>("main", "Vacuum_Spacers");
    vacuum_basepl_ = config->getValue<int>("main", "Vacuum_Baseplate");
    vacuum_sub_ = config->getValue<int>("main", "Vacuum_Subassembly");

    PU_status_ = new QLabel();
    SP_status_ = new QLabel();
    BP_status_ = new QLabel();
    SUB_status_ = new QLabel();

    PU_status_->setMinimumWidth(30);
    SP_status_->setMinimumWidth(30);
    BP_status_->setMinimumWidth(30);
    SUB_status_->setMinimumWidth(30);

    PU_status_->setText("OFF");
    SP_status_->setText("OFF");
    BP_status_->setText("OFF");
    SUB_status_->setText("OFF");

    PU_status_->setStyleSheet("QLabel { color : green ; font: bold }");
    SP_status_->setStyleSheet("QLabel { color : green ; font: bold }");
    BP_status_->setStyleSheet("QLabel { color : green ; font: bold }");
    SUB_status_->setStyleSheet("QLabel { color : green ; font: bold }");

    vac_lay->addWidget(PU_status_, 1, 1, Qt::AlignCenter);
    vac_lay->addWidget(SP_status_, 1, 2, Qt::AlignCenter);
    vac_lay->addWidget(BP_status_, 1, 3, Qt::AlignCenter);
    vac_lay->addWidget(SUB_status_, 1, 4, Qt::AlignCenter);

    vac_wid->setLayout(vac_lay);

    toolBar_->addWidget(vac_wid);

    connect(hwctr_view_->Vacuum_Widget(), SIGNAL(toggleVacuum(int))              , relayCardManager_, SLOT(toggleVacuum(int)));
    connect(hwctr_view_->Vacuum_Widget(), SIGNAL(vacuumChannelState_request(int)), relayCardManager_, SLOT(transmit_vacuumChannelState(int)));

    connect(relayCardManager_, SIGNAL(vacuumChannelState(int, SwitchState)), hwctr_view_->Vacuum_Widget(), SLOT(updateVacuumChannelState(int, SwitchState)));

    connect(relayCardManager_, SIGNAL(vacuumChannelState(int, SwitchState)), this, SLOT(update_vacuum_information(int, SwitchState)));

    connect(relayCardManager_, SIGNAL( enableVacuumButton()), hwctr_view_->Vacuum_Widget(), SLOT( enableVacuumButton()));
    connect(relayCardManager_, SIGNAL(disableVacuumButton()), hwctr_view_->Vacuum_Widget(), SLOT(disableVacuumButton()));

    if(relayCardModel_->getDeviceState()==State::OFF) {
      hwctr_view_->Vacuum_Widget()->disableVacuumButton();
    }
    hwctr_view_->Vacuum_Widget()->updateVacuumChannelsStatus();


    QWidget *stage_wid = new QWidget();

    QGridLayout* stage_lay = new QGridLayout;
    QLabel* x_label = new QLabel("X [mm]");
    QLabel* y_label = new QLabel("Y [mm]");
    QLabel* z_label = new QLabel("Z [mm]");
    QLabel* a_label = new QLabel("A [deg]");

    stage_lay->addWidget(x_label, 0, 0);
    stage_lay->addWidget(y_label, 0, 1);
    stage_lay->addWidget(z_label, 0, 2);
    stage_lay->addWidget(a_label, 0, 3);

    for(unsigned int i=0; i<4; ++i) {
      stage_values_.push_back(new QLabel("-"));
      stage_lay->addWidget(stage_values_.at(i), 1, i);
    }

    stage_wid->setLayout(stage_lay);

    toolBar_->addWidget(stage_wid);

    button_info_ = new QPushButton(tr("Information"));
    button_info_->setStyleSheet("QPushButton { background-color: rgb(215, 214, 213); font: 16px;} QPushButton:hover { background-color: rgb(174, 173, 172); font: 16px;}");
    toolBar_->addWidget(button_info_);
    /// --------------------------------------------------------

    /// Main Tab -----------------------------------------------
    main_tab = new QTabWidget;

    main_tab->setTabPosition(QTabWidget::North);

    idx_module_tab = main_tab->addTab(assembly_tab, tr("Module Assembly"));
    idx_manual_tab = main_tab->addTab(controls_tab, tr("Additional Tools"));

    assembly_tab->setStyleSheet(assembly_tab->styleSheet()+" QTabBar::tab {width: 300px; }");
    controls_tab->setStyleSheet(controls_tab->styleSheet()+" QTabBar::tab {width: 375px; }");
    main_tab    ->setStyleSheet(    main_tab->styleSheet()+" QTabBar::tab {width: 950px; }");

    this->setCentralWidget(main_tab);

    this->updateGeometry();

    connect(button_info_, SIGNAL(clicked()), this, SLOT(displayInfo_activeTab()));
    /// --------------------------------------------------------

    liveTimer_ = new QTimer(this);

    connect(liveTimer_, SIGNAL(timeout()), this, SLOT(liveUpdate()));

    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(quit()));

    NQLog("AssemblyMainWindow", NQLog::Message) << "///////////////////////////////////////////////////////";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                                                   //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//       Automated Pixel-Strip Module Assembly       //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                                                   //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "///////////////////////////////////////////////////////";

    NQLog("AssemblyMainWindow", NQLog::Message) << "application initialized successfully";

    // enable camera at startup
    const bool startup_camera = config->getDefaultValue<bool>("main", "startup_camera", false);

    if(startup_camera)
    {
      this->enable_images();
    }

    if(startup_motion_stage && motion_model_->getDeviceState()==State::OFF) {
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "Motion Stage Controller could not be started!";

      QMessageBox* msgBox = new QMessageBox;
      msgBox->setInformativeText("Motion Stage Controller is not available!");

      msgBox->setStandardButtons(QMessageBox::Ok);

      int ret = msgBox->exec();
    }
    // ------------------------
}

void AssemblyMainWindow::liveUpdate()
{
    NQLog("AssemblyMainWindow", NQLog::Debug) << "liveUpdate: emitting signal \"image_request\"";

    emit image_request();
}

//-- Slot function to display a GUI message whenever the Motion Stage gets automatically restarted
void AssemblyMainWindow::messageBox_restartMotionStage()
{
    NQLog("AssemblyMainWindow", NQLog::Debug) << "messageBox_restartMotionStage: displaying pop-up message box";

    //-- Display informative pop-up before restarting the MS
    QMessageBox::information(this, tr("Motion Stage Error -- Restarting"),
            tr("<p>The motion stage returned a position vector with invalid size, and will be automatically restarted."
            "<p>If it still does not respond, you may try to restart it again by clicking 'Restart Motion Stage' in the tab 'HW Controllers'.</p>"
        ));

    return;
}

void AssemblyMainWindow::enable_images()
{
  if(images_enabled_)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "enable_images"
       << ": images already enabled, no action taken";

    return;
  }

  images_enabled_ = true;

  if(image_ctr_ == nullptr)
  {
    image_ctr_ = new AssemblyImageController(camera_, zfocus_finder_);
  }

  connect(this      , SIGNAL(images_ON())      , image_ctr_, SLOT(enable()));
  connect(this      , SIGNAL(images_OFF())     , image_ctr_, SLOT(disable()));

  connect(image_ctr_, SIGNAL(camera_enabled()) , this      , SLOT(connect_images()));
  connect(image_ctr_, SIGNAL(camera_disabled()), this      , SLOT(disconnect_images()));

  connect(this      , SIGNAL(image_request())  , image_ctr_, SLOT(acquire_image()));
  connect(this      , SIGNAL(autofocus_ON ())  , image_ctr_, SLOT(enable_autofocus()));
  connect(this      , SIGNAL(autofocus_OFF())  , image_ctr_, SLOT(disable_autofocus()));
  connect(image_view_, SIGNAL(request_image()), image_ctr_, SLOT(acquire_image()));

  NQLog("AssemblyMainWindow", NQLog::Message) << "enable_images"
     << ": connecting AssemblyImageController";

  NQLog("AssemblyMainWindow", NQLog::Spam) << "enable_images"
     << ": emitting signal \"images_ON\"";

  emit images_ON();

  if(ApplicationConfig::instance()->hasKey("main", "camera_exposure_time") && camera_ != nullptr)
  {
      connect(this, SIGNAL(changeExposureTime(double)), camera_, SLOT(setExposureTime(double)));
      auto camera_exposure_time = ApplicationConfig::instance()->getValue<double>("main", "camera_exposure_time");
      NQLog("AssemblyMainWindow", NQLog::Message) << QString("Setting camera exposure time to %1 ms").arg(camera_exposure_time);
      emit changeExposureTime(camera_exposure_time);
      disconnect(this, SIGNAL(changeExposureTime(double)), camera_, SLOT(setExposureTime(double)));
  }
}

void AssemblyMainWindow::disable_images()
{
  if(images_enabled_ == false)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "disable_images"
       << ": images already disabled, no action taken";

    return;
  }

  if(image_ctr_ == nullptr)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "disable_images"
       << ": logic error (images enabled, but NULL pointer to AssemblyImageController), no action taken";

    return;
  }

  images_enabled_ = false;

  disconnect(this      , SIGNAL(images_ON())      , image_ctr_, SLOT(enable()));
  disconnect(this      , SIGNAL(images_OFF())     , image_ctr_, SLOT(disable()));

  disconnect(image_ctr_, SIGNAL(camera_enabled()) , this      , SLOT(connect_images()));
  disconnect(image_ctr_, SIGNAL(camera_disabled()), this      , SLOT(disconnect_images()));

  disconnect(this      , SIGNAL(image_request())  , image_ctr_, SLOT(acquire_image()));
  disconnect(this      , SIGNAL(autofocus_ON())   , image_ctr_, SLOT( enable_autofocus()));
  disconnect(this      , SIGNAL(autofocus_OFF())  , image_ctr_, SLOT(disable_autofocus()));
  disconnect(image_view_, SIGNAL(request_image()), image_ctr_, SLOT(acquire_image()));

  NQLog("AssemblyMainWindow", NQLog::Message) << "disable_images"
     << ": disabling AssemblyImageController";

  image_ctr_->disable();
}

void AssemblyMainWindow::changeState_autofocus(const int state)
{
    if(image_ctr_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Warning) << "changeState_autofocus"
         << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

      return;
    }

    if(motion_model_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Warning) << "changeState_autofocus"
         << ": LStepExpressModel not initialized, no action taken (hint: plug-in motion stage cable)";

      return;
    }

    if(state == 2)
    {
      NQLog("AssemblyMainWindow", NQLog::Spam) << "changeState_autofocus"
         << ": emitting signal \"autofocus_ON\"";

      emit autofocus_ON();
    }
    else if(state == 0)
    {
      NQLog("AssemblyMainWindow", NQLog::Spam) << "changeState_autofocus"
         << ": emitting signal \"autofocus_OFF\"";

      emit autofocus_OFF();
    }

    return;
}

void AssemblyMainWindow::restore_autofocus_settings(){
    if(autofocus_checkbox_->checkState()==Qt::Checked){
        NQLog("AssemblyMainWindow", NQLog::Spam) << "restore_autofocus_settings"
           << ": emitting signal \"autofocus_ON\"";

        emit autofocus_ON();
    }else if(autofocus_checkbox_->checkState()==Qt::Unchecked){
        NQLog("AssemblyMainWindow", NQLog::Spam) << "restore_autofocus_settings"
           << ": emitting signal \"autofocus_OFF\"";

        emit autofocus_OFF();
    }
}

void AssemblyMainWindow::get_image()
{
    if(image_ctr_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Warning) << "get_image"
         << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

      return;
    }

    if(image_ctr_->is_enabled() == false)
    {
      NQLog("AssemblyMainWindow", NQLog::Warning) << "get_image"
         << ": ImageController not enabled, no action taken (hint: click \"Camera ON\")";

      return;
    }

    NQLog("AssemblyMainWindow", NQLog::Spam) << "get_image"
       << ": emitting signal \"image_request\"";

    emit image_request();
}

void AssemblyMainWindow::connect_images()
{
//  finderView_->connectImageProducer(finder_, SIGNAL(markerFound  (const cv::Mat&)));
//  edgeView_  ->connectImageProducer(finder_, SIGNAL(edgesDetected(const cv::Mat&)));
//  rawView_   ->connectImageProducer(camera_, SIGNAL(imageAcquired(const cv::Mat&)));

  connect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), image_view_ , SLOT(update_image(cv::Mat)));
  connect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), thresholder_, SLOT(update_image_raw   (cv::Mat)));
  connect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), finder_     , SLOT(update_image_master(cv::Mat)));

  connect(image_view_, SIGNAL(image_loaded(cv::Mat)), image_ctr_, SLOT(retrieve_image(cv::Mat)));

  connect(image_view_->autofocus_button(), SIGNAL(clicked()), image_ctr_, SLOT(acquire_autofocused_image()));

  connect(image_view_->autofocus_emergencyStop_button(), SIGNAL(clicked()), zfocus_finder_, SLOT(emergencyStop()));
  connect(image_view_->autofocus_emergencyStop_button(), SIGNAL(clicked()), image_ctr_    , SLOT(restore_autofocus_settings()));
  connect(image_view_->autofocus_emergencyStop_button(), SIGNAL(clicked()), image_ctr_    , SLOT(disable_autofocus())); //Added explicit call to disable_autofocus() when clicking 'Stop Auto-Focus' (to mitigate rare issues where the ZFocusFinder does not get disconnected, and gets triggered when clicking 'Snapshot') //NB: if needed, can also simply tick/untick the 'Auto-focusing' button to force-disable the autofocus module

  NQLog("AssemblyMainWindow", NQLog::Message) << "connect_images"
     << ": enabled images in application view(s)";

//  liveTimer_->start(2000);
}

void AssemblyMainWindow::disconnect_images()
{
//  finderView_->disconnectImageProducer(finder_, SIGNAL(markerFound  (const cv::Mat&)));
//  edgeView_  ->disconnectImageProducer(finder_, SIGNAL(edgesDetected(const cv::Mat&)));
//  rawView_   ->disconnectImageProducer(camera_, SIGNAL(imagef       (const cv::Mat&)));

  disconnect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), image_view_ , SLOT(update_image(cv::Mat)));
  disconnect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), thresholder_, SLOT(update_image_raw   (cv::Mat)));
  disconnect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), finder_     , SLOT(update_image_master(cv::Mat)));

  disconnect(image_view_, SIGNAL(image_loaded(cv::Mat)), image_ctr_, SLOT(retrieve_image(cv::Mat)));

  disconnect(image_view_->autofocus_button(), SIGNAL(clicked()), image_ctr_, SLOT(acquire_autofocused_image()));

  disconnect(image_view_->autofocus_emergencyStop_button(), SIGNAL(clicked()), zfocus_finder_, SLOT(emergencyStop()));
  disconnect(image_view_->autofocus_emergencyStop_button(), SIGNAL(clicked()), image_ctr_    , SLOT(restore_autofocus_settings()));
  disconnect(image_view_->autofocus_emergencyStop_button(), SIGNAL(clicked()), image_ctr_    , SLOT(disable_autofocus()));

  NQLog("AssemblyMainWindow", NQLog::Message) << "disconnect_images"
     << ": disabled images in application view(s)";

  liveTimer_->stop();
}

void AssemblyMainWindow::start_objectAligner(const AssemblyObjectAligner::Configuration& conf)
{
  if(image_ctr_ == nullptr)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "start_objectAligner"
       << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

    return;
  }

  if(aligner_connected_)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "start_objectAligner"
       << ": AssemblyObjectAligner already connected, no action taken";

    return;
  }

  // acquire image
  connect(aligner_, SIGNAL(image_request()), image_ctr_, SLOT(acquire_image()));
  connect(aligner_, SIGNAL(autofocused_image_request()), image_ctr_, SLOT(acquire_autofocused_image()));

  // master-image updated, go to next step (PatRec)
  connect(finder_, SIGNAL(updated_image_master()), aligner_, SLOT(launch_next_alignment_step()));

  // launch PatRec
  connect(aligner_, SIGNAL(PatRec_request(AssemblyObjectFinderPatRec::Configuration)), finder_, SLOT(launch_PatRec(AssemblyObjectFinderPatRec::Configuration)));

  // show PatRec-edited image in Aligner widget
  connect(finder_, SIGNAL(PatRec_res_image_master_edited(cv::Mat)), aligner_, SLOT(redirect_image(cv::Mat)));

  // use PatRec results for next alignment step
  connect(finder_, SIGNAL(PatRec_results(double, double, double)), aligner_, SLOT(run_alignment(double, double, double)));

  // show measured angle
  connect(aligner_, SIGNAL(measured_angle(double)), aligner_view_, SLOT(show_measured_angle(double)));

  // once completed, disable connections between controllers used for alignment
  connect(aligner_, SIGNAL(execution_completed()), this, SLOT(disconnect_objectAligner()));

  // kick-start alignment
  connect(aligner_, SIGNAL(configuration_updated()), aligner_, SLOT(execute()));

  aligner_view_->Results_Widget()->setEnabled(true);

  aligner_connected_ = true;

  // if successful, emits signal "configuration_updated()"
  aligner_->update_configuration(conf);

  return;
}

void AssemblyMainWindow::disconnect_objectAligner()
{
  if(image_ctr_ == nullptr)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "disconnect_objectAligner"
       << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

    return;
  }

  if(aligner_connected_ == false)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "disconnect_objectAligner"
       << ": AssemblyObjectAligner already disconnected, no action taken";

    return;
  }

  // acquire image
  disconnect(aligner_, SIGNAL(image_request()), image_ctr_, SLOT(acquire_image()));
  disconnect(aligner_, SIGNAL(autofocused_image_request()), image_ctr_, SLOT(acquire_autofocused_image()));

  // master-image updated, go to next step (PatRec)
  disconnect(finder_, SIGNAL(updated_image_master()), aligner_, SLOT(launch_next_alignment_step()));

  // launch PatRec
  disconnect(aligner_, SIGNAL(PatRec_request(AssemblyObjectFinderPatRec::Configuration)), finder_, SLOT(launch_PatRec(AssemblyObjectFinderPatRec::Configuration)));

  // show PatRec-edited image in Aligner widget
  disconnect(finder_, SIGNAL(PatRec_res_image_master_edited(cv::Mat)), aligner_, SLOT(redirect_image(cv::Mat)));

  // use PatRec results for next alignment step
  disconnect(finder_, SIGNAL(PatRec_results(double, double, double)), aligner_, SLOT(run_alignment(double, double, double)));

  // show measured angle
  disconnect(aligner_, SIGNAL(measured_angle(double)), aligner_view_, SLOT(show_measured_angle(double)));

  // once completed, disable connections between controllers used for alignment
  disconnect(aligner_, SIGNAL(execution_completed()), this, SLOT(disconnect_objectAligner()));

  // kick-start alignment
  disconnect(aligner_, SIGNAL(configuration_updated()), aligner_, SLOT(execute()));

  aligner_view_->Configuration_Widget()->setEnabled(true);

  aligner_connected_ = false;

  return;
}


void AssemblyMainWindow::start_metrology(const Metrology::Configuration& conf)
{
  if(image_ctr_ == nullptr)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "start_metrology"
       << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

    return;
  }

  if(metrology_connected_)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "start_metrology"
       << ": Metrology already connected, no action taken";

    return;
  }

  // acquire image
  connect(metrology_, SIGNAL(image_request()), image_ctr_, SLOT(acquire_image()));
  connect(metrology_, SIGNAL(autofocused_image_request()), image_ctr_, SLOT(acquire_autofocused_image()));

  // master-image updated, go to next step (PatRec)
  connect(finder_, SIGNAL(updated_image_master()), metrology_, SLOT(launch_next_metrology_step()));

  // launch PatRec
  connect(metrology_, SIGNAL(PatRec_request(AssemblyObjectFinderPatRec::Configuration)), finder_, SLOT(launch_PatRec(AssemblyObjectFinderPatRec::Configuration)));

  // show PatRec-edited image in Aligner widget
  connect(finder_, SIGNAL(PatRec_res_image_master_edited(cv::Mat)), metrology_, SLOT(redirect_image(cv::Mat)));

  // use PatRec results for next alignment step
  connect(finder_, SIGNAL(PatRec_results(double, double, double)), metrology_, SLOT(run_metrology(double, double, double)));

  // use exitcode of PatRec
  connect(finder_, SIGNAL(PatRec_exitcode(int)), metrology_, SLOT(patrec_complete(int)));

  // show measured angle
  connect(metrology_, SIGNAL(measured_angle(bool, double)), metrology_view_, SLOT(show_measured_angle(bool, double)));

  // show measured Results
  connect(metrology_, SIGNAL(measured_results(double, double, double, double, double, double, double)), metrology_view_, SLOT(show_results(double, double, double, double, double, double, double)));

  // once completed, disable connections between controllers used for alignment
  connect(metrology_, SIGNAL(execution_completed()), this, SLOT(disconnect_metrology()));

  // if failed, also disable connections between controllers used for alignment, but also tell metrology_view_
  connect(metrology_, SIGNAL(execution_failed()), metrology_view_, SLOT(metrology_abort()));
  connect(metrology_, SIGNAL(execution_failed()), this, SLOT(disconnect_metrology()));

  // kick-start alignment
  connect(metrology_, SIGNAL(configuration_updated()), metrology_, SLOT(execute()));

  metrology_view_->Results_Widget()->setEnabled(true);

  metrology_connected_ = true;

  // if successful, emits signal "configuration_updated()"
  metrology_->update_configuration(conf);

  return;
}


void AssemblyMainWindow::disconnect_metrology()
{
  NQLog("AssemblyMainWindow", NQLog::Warning) << "disconnect_metrology";

  if(image_ctr_ == nullptr)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "disconnect_metrology"
       << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

    return;
  }

  if(metrology_connected_ == false)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "disconnect_metrology"
       << ": Metrology already disconnected, no action taken";

    return;
  }

  // acquire image
  disconnect(metrology_, SIGNAL(image_request()), image_ctr_, SLOT(acquire_image()));
  disconnect(metrology_, SIGNAL(autofocused_image_request()), image_ctr_, SLOT(acquire_autofocused_image()));

  // master-image updated, go to next step (PatRec)
  disconnect(finder_, SIGNAL(updated_image_master()), metrology_, SLOT(launch_next_metrology_step()));

  // launch PatRec
  disconnect(metrology_, SIGNAL(PatRec_request(AssemblyObjectFinderPatRec::Configuration)), finder_, SLOT(launch_PatRec(AssemblyObjectFinderPatRec::Configuration)));

  // show PatRec-edited image in Aligner widget
  disconnect(finder_, SIGNAL(PatRec_res_image_master_edited(cv::Mat)), metrology_, SLOT(redirect_image(cv::Mat)));

  // use PatRec results for next alignment step
  disconnect(finder_, SIGNAL(PatRec_results(double, double, double)), metrology_, SLOT(run_metrology(double, double, double)));

  // use exitcode of PatRec
  disconnect(finder_, SIGNAL(PatRec_exitcode(int)), metrology_, SLOT(patrec_complete(int)));

  // show measured angle
  disconnect(metrology_, SIGNAL(measured_angle(bool, double)), metrology_view_, SLOT(show_measured_angle(bool, double)));

  // show measured Results
  disconnect(metrology_, SIGNAL(measured_results(double, double, double, double, double, double, double)), metrology_view_, SLOT(show_results(double, double, double, double, double, double, double)));

  // once completed, disable connections between controllers used for alignment
  disconnect(metrology_, SIGNAL(execution_completed()), this, SLOT(disconnect_metrology()));

  // if failed, also disable connections between controllers used for alignment, but also tell metrology_view_
  disconnect(metrology_, SIGNAL(execution_failed()), this, SLOT(disconnect_metrology()));
  disconnect(metrology_, SIGNAL(execution_failed()), metrology_view_, SLOT(metrology_abort()));

  // kick-start alignment
  disconnect(metrology_, SIGNAL(configuration_updated()), metrology_, SLOT(execute()));

  metrology_view_->Configuration_Widget()->setEnabled(true);

  metrology_connected_ = false;

  return;
}

void AssemblyMainWindow::start_multiPickupTest(const AssemblyMultiPickupTester::Configuration& conf)
{
  if(image_ctr_ == nullptr)
  {
    NQLog("AssemblyMainWindow", NQLog::Warning) << "start_multiPickupTest"
       << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

    return;
  }

  toolbox_view_->MultiPickupTester_Widget()->enable(false);

  connect(multipickup_tester_, SIGNAL(measurement_finished()), multipickup_tester_, SLOT(start_pickup()));
  connect(multipickup_tester_, SIGNAL(pickup_finished())     , multipickup_tester_, SLOT(start_measurement()));
  connect(multipickup_tester_, SIGNAL(test_finished())       , this               , SLOT(disconnect_multiPickupTest()));

  // measurement
  connect(multipickup_tester_, SIGNAL(measurement_request()), image_ctr_, SLOT(acquire_image()));

  connect(finder_, SIGNAL(updated_image_master()), finder_view_->PatRec_exe_button(), SLOT(click()));
  connect(finder_, SIGNAL(PatRec_exitcode(int)), multipickup_tester_, SLOT(finish_measurement(int)));
  // ---

  // pickup (vacuum)
  connect(multipickup_tester_, SIGNAL(vacuum_toggle(int)), relayCardManager_, SLOT(toggleVacuum(int)));

  connect(relayCardManager_, SIGNAL(vacuum_toggled()), multipickup_tester_, SLOT(setup_next_step()));
  // ---

  multipickup_tester_->set_configuration(conf);

  multipickup_tester_->start_measurement();

  return;
}

void AssemblyMainWindow::disconnect_multiPickupTest()
{
  disconnect(multipickup_tester_, SIGNAL(measurement_finished()), multipickup_tester_, SLOT(start_pickup()));
  disconnect(multipickup_tester_, SIGNAL(pickup_finished())     , multipickup_tester_, SLOT(start_measurement()));
  disconnect(multipickup_tester_, SIGNAL(test_finished())       , this               , SLOT(disconnect_multiPickupTest()));

  // measurement
  disconnect(multipickup_tester_, SIGNAL(measurement_request()), image_ctr_, SLOT(acquire_image()));

  disconnect(finder_, SIGNAL(updated_image_master()), finder_view_->PatRec_exe_button(), SLOT(click()));
  disconnect(finder_, SIGNAL(PatRec_exitcode(int)), multipickup_tester_, SLOT(finish_measurement(int)));
  // ---

  // pickup (vacuum)
  disconnect(multipickup_tester_, SIGNAL(vacuum_toggle(int)), relayCardManager_, SLOT(toggleVacuum(int)));

  disconnect(relayCardManager_, SIGNAL(vacuum_toggled()), multipickup_tester_, SLOT(setup_next_step()));
  // ---

  toolbox_view_->MultiPickupTester_Widget()->enable(true);

  NQLog("AssemblyMainWindow", NQLog::Spam) << "disconnect_multiPickupTest"
     << ": emitting signal \"multiPickupTest_disconnected\"";

  emit multiPickupTest_disconnected();

  NQLog("AssemblyMainWindow", NQLog::Message) << "disconnect_multiPickupTest"
     << ": multi-pickup test completed";
}

//Disconnect remaining signal/slots, which did not get disconnected via specific functions //Actually miss several, not needed ?
void AssemblyMainWindow::disconnect_otherSlots()
{
    disconnect(params_view_, SIGNAL(request_moveToAbsRefPosition(double,double,double,double)), motion_manager_, SLOT(moveAbsolute(double,double,double,double)));
    disconnect(params_view_, SIGNAL(request_moveByRelRefDistance(double,double,double,double)), motion_manager_, SLOT(moveRelative(double,double,double,double)));
    disconnect(image_view_, SIGNAL(sigRequestMoveRelative(double,double,double,double)), motion_manager_, SLOT(moveRelative(double,double,double,double)));
    disconnect(motion_manager_, SIGNAL(restartMotionStage_request()), hwctr_view_->LStepExpress_Widget(), SLOT(restart()));
    disconnect(motion_manager_, SIGNAL(restartMotionStage_request()), this, SLOT(messageBox_restartMotionStage()));
    disconnect(assemblyV2_, SIGNAL(switchToAlignmentTab_PSP_request()), this, SLOT(update_alignment_tab_psp()));
    disconnect(assemblyV2_, SIGNAL(switchToAlignmentTab_PSS_request()), this, SLOT(update_alignment_tab_pss()));
    disconnect(subassembly_pickup_, SIGNAL(switchToAlignmentTab_PSS_request()), this, SLOT(update_alignment_tab_pss()));
    disconnect(this, SIGNAL(set_alignmentMode_PSP_request()), aligner_view_, SLOT(set_alignmentMode_PSP()));
    disconnect(this, SIGNAL(set_alignmentMode_PSS_request()), aligner_view_, SLOT(set_alignmentMode_PSS()));

    return;
}

void AssemblyMainWindow::testTimer()
{
    NQLog("AssemblyMainWindow", NQLog::Spam) << "testTimer"
       << ": timeOut=" << testTimerCount_;

    testTimerCount_ += 0.1;

    return;
}

//When the "Information" button is clicked: get the active widget (currently displayed in the GUI), and call its specific message function //NB: consider only 'sub-tabs', not main tabs
void AssemblyMainWindow::displayInfo_activeTab()
{
    //Get the active widget by index //NB: the 2 children of the QTabWidget 'main_tab' are also QTabWidget
    QWidget* widget_tmp = main_tab->widget(main_tab->currentIndex());

    //Cast into a QTabWidget
    QTabWidget* qtabwidget_active = dynamic_cast<QTabWidget*>(widget_tmp);

    //Get the actual active sub-widget
    QWidget* widget_active = qtabwidget_active->currentWidget();

    //Connect signal to info slot of this widget, emit signal, then disconnect
    connect(this, SIGNAL(display_info()), widget_active, SLOT(display_infoTab()));
    emit display_info();
    disconnect(this, SIGNAL(display_info()), widget_active, SLOT(display_infoTab()));

    return;
}

void AssemblyMainWindow::quit_thread(QThread* thread, const QString& msg) const
{
    if(thread != nullptr)
    {
      thread->quit();

      if(thread->wait(2000) == false)
      {
         thread->terminate();
         thread->wait();
      }

      NQLog("AssemblyMainWindow", NQLog::Spam) << "quit_thread: "+msg;
    }
}

void AssemblyMainWindow::quit()
{
    if(camera_ != nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Spam) << "quit"
         << ": emitting signal \"images_OFF\"";

      emit images_OFF();

      camera_ = 0;
    }

    this->quit_thread(motion_thread_, "terminated LStepExpressMotionThread");
    this->quit_thread(camera_thread_, "terminated AssemblyUEyeCameraThread");
    this->quit_thread(finder_thread_, "terminated AssemblyObjectFinderPatRecThread");

    NQLog("AssemblyMainWindow", NQLog::Message) << "quit: application closed";

    return;
}

//-- Automatically switch to 'Alignment' sub-tab and emit signal relevant for PSS/PSP
void AssemblyMainWindow::update_alignment_tab_psp()
{
    this->switchAndUpdate_alignment_tab(true);
}
void AssemblyMainWindow::update_alignment_tab_pss()
{
    this->switchAndUpdate_alignment_tab(false);
}
void AssemblyMainWindow::switchAndUpdate_alignment_tab(bool psp_mode)
{
    // std::cout<<"There are "<<main_tab->count()<<" main tabs"<<std::endl; //Count main tabs
    // QTabWidget* assemblyTab = main_tab->findChild<QTabWidget*>("Module Assembly");
    main_tab->setCurrentIndex(idx_module_tab);

    QList<QTabWidget*> widgets = main_tab->findChildren<QTabWidget*>(); //Get main tabs
    QTabWidget* assemblyTab = widgets[1]; //Get 'Module Assembly' main tab
    // std::cout<<"There are "<<assemblyTab->count()<<" sub-tabs"<<std::endl; //Count sub-tabs

    assemblyTab->setCurrentIndex(idx_alignment_tab); //Switch to 'Alignment' sub-tab

    //Emit signal to set either PSP or PSS alignment mode
    if(psp_mode) {emit set_alignmentMode_PSP_request();}
    else {emit set_alignmentMode_PSS_request();}

    return;
}

void AssemblyMainWindow::update_stage_position()
{

  const auto x_status  = motion_model_->getAxisStatusText(0);
  const auto y_status  = motion_model_->getAxisStatusText(1);
  const auto z_status  = motion_model_->getAxisStatusText(2);
  const auto a_status  = motion_model_->getAxisStatusText(3);

  for(unsigned int i=0; i<4; ++i)
  {
    if(!motion_model_->getAxisEnabled(i)) {
      QString tpl = tr("<font color='%1'>%2</font>");
      stage_values_.at(i)->setText(tpl.arg("darkred","D"));
    } else if(motion_model_->getAxisStatusText(i) == "@")
    {
      QString tpl = tr("<font color='%1'>%2</font>");
      stage_values_.at(i)->setText(tpl.arg("darkgreen",QString::number(motion_model_->getPosition(i), 'f', 2)));
    } else if(motion_model_->getAxisStatusText(i) == "M") {
      QString tpl = tr("<font color='%1'>%2</font>");
      stage_values_.at(i)->setText(tpl.arg("darkblue",QString::number(motion_model_->getPosition(i), 'f', 2)));
    } else {
      QString tpl = tr("<font color='%1'>%2</font>");
      stage_values_.at(i)->setText(tpl.arg("darkred",motion_model_->getAxisStatusText(i)));
    }
  }
}

void AssemblyMainWindow::update_vacuum_information(const int channel, const SwitchState state)
{
  NQLog("AssemblyMainWindow", NQLog::Debug) << "update_vacuum_information("
  << channel << ", " << static_cast<int>(state) << "): updating vacuum information";

  QLabel* to_be_updated;
  if(channel == vacuum_pickup_)
  {
    to_be_updated = PU_status_;
  } else if(channel == vacuum_spacer_)
  {
    to_be_updated = SP_status_;
  } else if(channel == vacuum_basepl_)
  {
    to_be_updated = BP_status_;
  } else if(channel == vacuum_sub_)
  {
    to_be_updated = SUB_status_;
  } else {
    NQLog("AssemblyMainWindow", NQLog::Fatal) << "Vacuum channel " << channel << " not known!";
    return;
  }

  if(to_be_updated == nullptr) {
    NQLog("AssemblyMainWindow", NQLog::Fatal) << "Vacuum channel " << channel << " not initialised!";
  }

  to_be_updated->clear();
  if(state==SwitchState::CHANNEL_ON)
  {
    to_be_updated->setText("ON");
    to_be_updated->setStyleSheet("QLabel { color : red ; font: bold }");
    to_be_updated->setAlignment(Qt::AlignCenter);
  } else if(state==SwitchState::CHANNEL_SWITCHING)
  {
    to_be_updated->setText("SW");
    to_be_updated->setStyleSheet("QLabel { color : orange ; font: bold }");
    to_be_updated->setAlignment(Qt::AlignCenter);
  }
  else if(state==SwitchState::CHANNEL_OFF)
  {
    to_be_updated->setText("OFF");
    to_be_updated->setStyleSheet("QLabel { color : green ; font: bold }");
    to_be_updated->setAlignment(Qt::AlignCenter);
  }
  else if(state==SwitchState::DEVICE_OFF)
  {
    to_be_updated->setText("-");
    to_be_updated->setStyleSheet("QLabel { color : gray ; font: bold }");
    to_be_updated->setAlignment(Qt::AlignCenter);
  }
}

void AssemblyMainWindow::warn_on_stage_limits(const double target_pos, const char axis, const double limit_pos_lower, const double limit_pos_upper)
{
    QMessageBox::warning(0, QString("[AssemblyMainWindow]"),
    QString("Attempting to move %1-axis to %2 . This is beyond the limit of this stage: [ %3 , %4 ]").arg(axis).arg(target_pos).arg(limit_pos_lower).arg(limit_pos_upper),
        QMessageBox::Ok
    );
}

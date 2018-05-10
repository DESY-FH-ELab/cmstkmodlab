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

#include <AssemblyMainWindow.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>
#include <Util.h>

#include <string>

#include <QApplication>
#include <QString>

#include <opencv2/opencv.hpp>

AssemblyMainWindow::AssemblyMainWindow(const unsigned int camera_ID, QWidget* parent) :
  QMainWindow(parent),
  camera_ID_(camera_ID),
  testTimerCount_(0.),
  liveTimer_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();
    if(config == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "-------------------------------------------------------------------------------------------------------";
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "initialization error: ApplicationConfig::instance() not initialized (null pointer), exiting constructor";
      NQLog("AssemblyMainWindow", NQLog::Fatal) << "-------------------------------------------------------------------------------------------------------";

      return;
    }

    // motion
    motion_model_   = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(), 1000, 1000);
    motion_manager_ = new LStepExpressMotionManager(motion_model_);

    motion_thread_  = new LStepExpressMotionThread(motion_manager_, this);
    motion_thread_->start();

    // camera
    camera_model_ = new AssemblyUEyeModel_t(10);
    camera_model_->updateInformation();

    camera_thread_ = new AssemblyUEyeCameraThread(camera_model_, this);
    camera_thread_->start();

    camera_ = camera_model_->getCameraByID(camera_ID_);
    if(camera_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Critical) << "---------------------------------------------------------------------------------";
      NQLog("AssemblyMainWindow", NQLog::Critical) << "initialization error: null pointer to AssemblyVUEyeCamera object (camera_ID=" << camera_ID_ << ")";
      NQLog("AssemblyMainWindow", NQLog::Critical) << "---------------------------------------------------------------------------------";
    }

    // zfocus finder
    zfocus_finder_ = new AssemblyZFocusFinder(camera_, motion_manager_);

    // image controller
    image_ctr_ = nullptr;

    // marker finder
    finder_        = new AssemblyObjectFinderPatRec(Util::QtCacheDirectory()+"/AssemblyObjectFinderPatRec", "rotations");
//    finder_thread_ = new AssemblyObjectFinderPatRecThread(finder_);
//    finder_thread_->start();

    // multi-pickup tester
    multipickup_ = new AssemblyMultiPickupTester(motion_manager_);

    // TAB WIDGET ----------------------------------------------
    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

//    finderView_ = new AssemblyUEyeSnapShooter(tabWidget_);
//    tabWidget_->addTab(finderView_, "finder");

//    edgeView_ = new AssemblyUEyeSnapShooter(tabWidget_);
//    tabWidget_->addTab(edgeView_, "edges");

//    rawView_ = new AssemblyUEyeSnapShooter(tabWidget_);
//    tabWidget_->addTab(rawView_, "raw");

    // IMAGE VIEW ----------------------------------------------
    const QString tabname_Image("Image");

    image_view_ = new AssemblyImageView(tabWidget_);
    tabWidget_->addTab(image_view_, tabname_Image);

    connect(zfocus_finder_ , SIGNAL(show_zscan(QString))           , image_view_    , SLOT(acquire_image_zscan(QString)));
    connect(zfocus_finder_ , SIGNAL(text_update_request(double))   , image_view_    , SLOT(update_text(double)));

    connect(zfocus_finder_ , SIGNAL(focus_config_request())        , image_view_    , SLOT(acquire_autofocus_config()));
    connect(image_view_    , SIGNAL(autofocus_config(double, int)) , zfocus_finder_ , SLOT(update_focus_config(double, int)));

    image_view_->connectImageProducer_autofocus(image_view_, SIGNAL(image_zscan_acquired(const cv::Mat&)));

    image_view_->update_autofocus_config(zfocus_finder_->zrange(), zfocus_finder_->points());

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Image;
    // ---------------------------------------------------------

    // IMAGE-THRESHOLDING VIEW ---------------------------------
    const QString tabname_ImageThresholding("Thresholding");

    thresholder_ = new AssemblyImageThresholder();

    thresholder_view_ = new AssemblyImageThresholderView(tabWidget_);
    tabWidget_->addTab(thresholder_view_, tabname_ImageThresholding);

    connect(thresholder_view_, SIGNAL(threshold_request        (int)), thresholder_, SLOT(update_image_binary_threshold        (int)));
    connect(thresholder_view_, SIGNAL(adaptiveThreshold_request(int)), thresholder_, SLOT(update_image_binary_adaptiveThreshold(int)));

    connect(thresholder_view_, SIGNAL(image_raw_request())   , thresholder_, SLOT(send_image_raw()));
    connect(thresholder_view_, SIGNAL(image_binary_request()), thresholder_, SLOT(send_image_binary()));

    connect(thresholder_, SIGNAL(image_sent(cv::Mat)), thresholder_view_, SLOT(save_image(cv::Mat)));

    // connections: AssemblyObjectFinderPatRec <-> AssemblyImageThresholder
    connect(finder_           , SIGNAL(updated_image_master())        , finder_          , SLOT(send_image_master()));
    connect(finder_           , SIGNAL(sent_image_master(cv::Mat))    , thresholder_ , SLOT(update_image_raw(cv::Mat)));
    connect(thresholder_  , SIGNAL(updated_image_raw())           , thresholder_view_   , SLOT(apply_threshold())); //!! FIXME: should have configurable parameter, w/o relying on thresholder_view_
    connect(thresholder_  , SIGNAL(updated_image_binary(cv::Mat)) , finder_          , SLOT(update_image_master_PatRec(cv::Mat)));
    // ----------

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_ImageThresholding;
    // ---------------------------------------------------------

    // AUTOMATED-ASSEMBLY VIEW ---------------------------------
    const QString tabname_AutoAssembly("Auto Assembly");

    assemblyView_ = new AssemblyAssemblyView(motion_manager_, tabWidget_);
    tabWidget_->addTab(assemblyView_, tabname_AutoAssembly);

    assemblyView_->connect_to_finder(finder_);

    finder_->update_rough_angles      (assemblyView_->PatRec_Widget()->widget_angrough()->get_input_string());
    finder_->update_angscan_parameters(assemblyView_->PatRec_Widget()->widget_angscanp()->get_input_string());

    connect(finder_, SIGNAL(threshold_request        (int)), thresholder_, SLOT(update_image_binary_threshold        (int)));
    connect(finder_, SIGNAL(adaptiveThreshold_request(int)), thresholder_, SLOT(update_image_binary_adaptiveThreshold(int)));

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_AutoAssembly;

    // VACUUM connections
    conradModel_   = new ConradModel(assemblyView_);
    conradManager_ = new ConradManager(conradModel_);

    module_assembler_ = new AssemblyAssembler(motion_manager_);

    connect(assemblyView_->Vacuum_Widget(), SIGNAL(toggleVacuum(int))                  , conradManager_                , SLOT(toggleVacuum(int)));
    connect(conradManager_                , SIGNAL(updateVacuumChannelState(int, bool)), assemblyView_->Vacuum_Widget(), SLOT(updateVacuumChannelState(int, bool)));

    connect(this                          , SIGNAL(updateVacuumChannelsStatus())       , conradManager_                , SLOT(updateVacuumChannelsStatus()));

    NQLog("AssemblyMainWindow", NQLog::Spam) << "emitting signal \"updateVacuumChannelsStatus\"";

    emit updateVacuumChannelsStatus();
    // ---

    // ObjectAligner sub-view
//!!    connect(assemblyView_, SIGNAL(objectAligner_request(const AssemblyObjectAligner::Configuration&)), this, SLOT(connect_objectAligner(const AssemblyObjectAligner::Configuration&)));
    // ---

    // MultiPickupTester sub-view
    connect(assemblyView_, SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)), this, SLOT(connect_multipickupNpatrec(const AssemblyMultiPickupTester::Configuration&)));
    // ---

    // ---------------------------------------------------------

    // PATTERN-RECOGNITION VIEW --------------------------------
    const QString tabname_PatRec("Pat Rec");

    finder_view_ = new AssemblyObjectFinderPatRecView(motion_manager_, tabWidget_);
    tabWidget_->addTab(finder_view_, tabname_PatRec);

    finder_view_->connect_to_finder(finder_);

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_PatRec;
    // ---------------------------------------------------------

//    // U-EYE VIEW ----------------------------------------------
//    const QString tabname_uEye("uEye");
//
//    camera_widget_ = new AssemblyUEyeWidget(camera_model_, this);
//    tabWidget_->addTab(camera_widget_, tabname_uEye);
//
//    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_uEye;
//    // ---------------------------------------------------------

    // POSITIONS VIEW --------------------------------------------
    const QString tabname_Registry("Registry");

    registryView_ = new AssemblyRegistryView(motion_manager_, tabWidget_);
    tabWidget_->addTab(registryView_, tabname_Registry);

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_Registry;
    // -----------------------------------------------------------

    // MOTION-SETTINGS VIEW ------------------------------------
    const QString tabname_MotionSettings("Motion Settings");

    motionSettings_ = new LStepExpressSettings(motion_model_, tabWidget_);

    motionSettingsWidget_ = new LStepExpressSettingsWidget(motionSettings_, tabWidget_);
    tabWidget_->addTab(motionSettingsWidget_, tabname_MotionSettings);

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_MotionSettings;
    // ---------------------------------------------------------

    // MOTION-MANAGER VIEW -------------------------------------
    const QString tabname_MotionManager("Motion Manager");

    motion_manager_view_ = new LStepExpressMotionView(motion_model_, motion_manager_, tabWidget_);
    tabWidget_->addTab(motion_manager_view_, tabname_MotionManager);

    NQLog("AssemblyMainWindow", NQLog::Message) << "added view " << tabname_MotionManager;
    // ---------------------------------------------------------

    // Upper Toolbar -------------------------------------------
    toolBar_ = addToolBar("Tools");
    toolBar_ ->addAction("Camera ON" , this, SLOT( enable_images()));
    toolBar_ ->addAction("Camera OFF", this, SLOT(disable_images()));
    toolBar_ ->addAction("Snapshot"  , this, SLOT(    get_image ()));

    checkbox1 = new QCheckBox("Auto-Focusing", this);
    toolBar_->addWidget(checkbox1);

    checkbox2 = new QCheckBox("Alignment", this);
    toolBar_->addWidget(checkbox2);

//    checkbox3 = new QCheckBox("Assembly", this);
//    toolBar_->addWidget(checkbox3);

    connect(checkbox1, SIGNAL(stateChanged(int)), this, SLOT(changeState_autofocus(int)));
    connect(checkbox2, SIGNAL(stateChanged(int)), this, SLOT(changeState_alignment(int)));
//    connect(checkbox3, SIGNAL(stateChanged(int)), this, SLOT(changeState_SandwichAssembly(int)));

    this->setCentralWidget(tabWidget_);

    this->updateGeometry();
    // ---------------------------------------------------------

    liveTimer_ = new QTimer(this);

    connect(liveTimer_, SIGNAL(timeout()), this, SLOT(liveUpdate()));

    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(quit()));

    NQLog("AssemblyMainWindow", NQLog::Message) << "///////////////////////////////////////////////////////";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                                                   //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                     DESY-CMS                      //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                                                   //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//             Automated Module Assembly             //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                                                   //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//  - AssemblyMainWindow initialized successfully -  //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "//                                                   //";
    NQLog("AssemblyMainWindow", NQLog::Message) << "///////////////////////////////////////////////////////";
}

void AssemblyMainWindow::liveUpdate()
{
    NQLog("AssemblyMainWindow", NQLog::Debug) << "liveUpdate: emitting signal \"image_request\"";

    emit image_request();
}

void AssemblyMainWindow::enable_images()
{
  if(image_ctr_ == nullptr)
  {
    image_ctr_ = new AssemblyImageController(camera_, zfocus_finder_);
  }

  connect(this      , SIGNAL(images_ON())      , image_ctr_, SLOT(enable()));
  connect(this      , SIGNAL(images_OFF())     , image_ctr_, SLOT(disable()));

  connect(image_ctr_, SIGNAL(camera_enabled()) , this      , SLOT(connect_images()));
  connect(image_ctr_, SIGNAL(camera_disabled()), this      , SLOT(disconnect_images()));

  connect(this      , SIGNAL(image_request())  , image_ctr_, SLOT(acquire_image()));
  connect(this      , SIGNAL(autofocus_ON ())  , image_ctr_, SLOT( enable_autofocus()));
  connect(this      , SIGNAL(autofocus_OFF())  , image_ctr_, SLOT(disable_autofocus()));

  NQLog("AssemblyMainWindow", NQLog::Message) << "enable_images"
     << ": ImageController connected";

  NQLog("AssemblyMainWindow", NQLog::Spam) << "enable_images"
     << ": emitting signal \"images_ON\"";

  emit images_ON();
}

void AssemblyMainWindow::disable_images()
{
    disconnect(this      , SIGNAL(images_ON())      , image_ctr_, SLOT(enable()));
    disconnect(this      , SIGNAL(images_OFF())     , image_ctr_, SLOT(disable()));

    disconnect(image_ctr_, SIGNAL(camera_enabled()) , this      , SLOT(connect_images()));
    disconnect(image_ctr_, SIGNAL(camera_disabled()), this      , SLOT(disconnect_images()));

    disconnect(this      , SIGNAL(image_request())  , image_ctr_, SLOT(acquire_image()));
    disconnect(this      , SIGNAL(autofocus_ON())   , image_ctr_, SLOT( enable_autofocus()));
    disconnect(this      , SIGNAL(autofocus_OFF())  , image_ctr_, SLOT(disable_autofocus()));

    NQLog("AssemblyMainWindow", NQLog::Message) << "disable_images"
       << ": ImageController disconnected";

    NQLog("AssemblyMainWindow", NQLog::Spam) << "enable_images"
       << ": emitting image \"images_OFF\"";

    emit images_OFF();
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

void AssemblyMainWindow::changeState_alignment(const int state)
{
    if(image_ctr_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Warning) << "changeState_alignment"
         << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

      return;
    }

    if(state == 2)
    {
      assemblyView_->Alignm_Widget()->enable(true);

      connect   (assemblyView_->Alignm_Widget(), SIGNAL(measure_angle_request(double, double))    , module_assembler_, SLOT(start_alignment(double, double)));
      connect   (assemblyView_->Alignm_Widget(), SIGNAL(alignment_request(double, double, double)), module_assembler_, SLOT(start_alignment(double, double, double)));

      connect   (module_assembler_ , SIGNAL(object_angle(double)), assemblyView_->Alignm_Widget(), SLOT(show_object_angle(double)));
      connect   (module_assembler_ , SIGNAL(alignment_finished()), assemblyView_->Alignm_Widget(), SLOT(enable()));

      connect   (module_assembler_ , SIGNAL(nextAlignmentStep(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));

      connect   (module_assembler_ , SIGNAL(image_request()), image_ctr_  , SLOT(acquire_image()));

//!!      connect   (finder_           , SIGNAL(updated_image_master())        , finder_          , SLOT(send_image_master()));
//!!      connect   (finder_           , SIGNAL(sent_image_master(cv::Mat))    , thresholder_ , SLOT(update_image_raw(cv::Mat)));
//!!      connect   (thresholder_  , SIGNAL(updated_image_raw())           , thresholder_view_   , SLOT(apply_threshold())); //!! FIXME: should have configurable parameter, w/o relying on thresholder_view_
//!!      connect   (thresholder_  , SIGNAL(updated_image_binary(cv::Mat)) , finder_          , SLOT(update_image_master_PatRec(cv::Mat)));
      connect   (finder_           , SIGNAL(updated_image_master_PatRec()) , finder_          , SLOT(run_PatRec_lab_marker()));

      connect   (finder_, SIGNAL(reportObjectLocation(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));

      connect   (module_assembler_ , SIGNAL(motion_finished()), module_assembler_, SLOT(launch_next_alignment_step()));

      NQLog("AssemblyMainWindow", NQLog::Message) << "changeState_alignment: alignment enabled";
    }
    else if(state == 0)
    {
      assemblyView_->Alignm_Widget()->enable(false);

      disconnect(assemblyView_->Alignm_Widget(), SIGNAL(measure_angle_request(double, double))    , module_assembler_, SLOT(start_alignment(double, double)));
      disconnect(assemblyView_->Alignm_Widget(), SIGNAL(alignment_request(double, double, double)), module_assembler_, SLOT(start_alignment(double, double, double)));

      disconnect(module_assembler_ , SIGNAL(object_angle(double)), assemblyView_->Alignm_Widget(), SLOT(show_object_angle(double)));
      disconnect(module_assembler_ , SIGNAL(alignment_finished()), assemblyView_->Alignm_Widget(), SLOT(enable()));

      disconnect(module_assembler_ , SIGNAL(nextAlignmentStep(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));

      disconnect(module_assembler_ , SIGNAL(image_request()), image_ctr_  , SLOT(acquire_image()));

//!!      disconnect(finder_           , SIGNAL(updated_image_master())        , finder_          , SLOT(send_image_master()));
//!!      disconnect(finder_           , SIGNAL(sent_image_master(cv::Mat))    , thresholder_ , SLOT(update_image_raw(cv::Mat)));
//!!      disconnect(thresholder_  , SIGNAL(updated_image_raw())           , thresholder_view_   , SLOT(apply_threshold())); //!! FIXME: should have configurable parameter, w/o relying on thresholder_view_
//!!      disconnect(thresholder_  , SIGNAL(updated_image_binary(cv::Mat)) , finder_          , SLOT(update_image_master_PatRec(cv::Mat)));
      disconnect(finder_           , SIGNAL(updated_image_master_PatRec()) , finder_          , SLOT(run_PatRec_lab_marker()));

      disconnect(finder_, SIGNAL(reportObjectLocation(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));

      disconnect(module_assembler_ , SIGNAL(motion_finished()), module_assembler_, SLOT(launch_next_alignment_step()));

      NQLog("AssemblyMainWindow", NQLog::Message) << "changeState_alignment: alignment disabled";
    }

    return;
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
//  finderView_->connectImageProducer(finder_ , SIGNAL(markerFound  (const cv::Mat&)));
//  edgeView_  ->connectImageProducer(finder_ , SIGNAL(edgesDetected(const cv::Mat&)));
//  rawView_   ->connectImageProducer(camera_ , SIGNAL(imageAcquired(const cv::Mat&)));

  connect(image_ctr_ , SIGNAL(image_acquired(cv::Mat)) , image_view_  , SLOT(update_image(cv::Mat)));
  connect(image_ctr_ , SIGNAL(image_acquired(cv::Mat)) , thresholder_ , SLOT(update_image_raw   (cv::Mat)));
  connect(image_ctr_ , SIGNAL(image_acquired(cv::Mat)) , finder_      , SLOT(update_image_master(cv::Mat)));

  connect(image_view_->autofocus_button(), SIGNAL(clicked()), image_ctr_, SLOT(acquire_autofocused_image()));

  thresholder_view_->connectImageProducer_1(thresholder_, SIGNAL(updated_image_raw   (cv::Mat)));
  thresholder_view_->connectImageProducer_2(thresholder_, SIGNAL(updated_image_binary(cv::Mat)));

  connect(finder_, SIGNAL(image_master_request()), image_ctr_, SLOT(acquire_image()));

  NQLog("AssemblyMainWindow", NQLog::Message) << "connect_images"
     << ": enabled images in application view(s)";

//  liveTimer_->start(2000);
}

void AssemblyMainWindow::disconnect_images()
{
//  finderView_->disconnectImageProducer(finder_ , SIGNAL(markerFound  (const cv::Mat&)));
//  edgeView_  ->disconnectImageProducer(finder_ , SIGNAL(edgesDetected(const cv::Mat&)));
//  rawView_   ->disconnectImageProducer(camera_ , SIGNAL(imagef       (const cv::Mat&)));

  disconnect(image_ctr_ , SIGNAL(image_acquired(cv::Mat)) , image_view_  , SLOT(update_image(cv::Mat)));
  disconnect(image_ctr_ , SIGNAL(image_acquired(cv::Mat)) , thresholder_ , SLOT(update_image_raw   (cv::Mat)));
  disconnect(image_ctr_ , SIGNAL(image_acquired(cv::Mat)) , finder_      , SLOT(update_image_master(cv::Mat)));

  disconnect(image_view_->autofocus_button(), SIGNAL(clicked()), image_ctr_, SLOT(acquire_autofocused_image()));

  thresholder_view_->disconnectImageProducer_1(thresholder_, SIGNAL(updated_image_raw   (cv::Mat)));
  thresholder_view_->disconnectImageProducer_2(thresholder_, SIGNAL(updated_image_binary(cv::Mat)));

  disconnect(finder_, SIGNAL(image_master_request()), image_ctr_, SLOT(acquire_image()));

  NQLog("AssemblyMainWindow", NQLog::Message) << "disconnect_images"
     << ": disabled images in application view(s)";

  liveTimer_->stop();
}

//!!void AssemblyMainWindow::connect_objectAligner(const AssemblyObjectAligner::Configuration& conf)
//!!{
//!!    if(image_ctr_ == nullptr)
//!!    {
//!!      NQLog("AssemblyMainWindow", NQLog::Warning) << "connect_objectAligner"
//!!         << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";
//!!
//!!      return;
//!!    }
//!!
//!!    aligner_->set_configuration(conf);
//!!
//!!    assemblyView_->Alignm_Widget()->enable(true);
//!!
//!!    connect(this, SIGNAL(objectAligner_connected()), aligner_, SLOT(start_measurement()));
//!!
//!!    connect   (assemblyView_->Alignm_Widget(), SIGNAL(measure_angle_request(double, double))    , module_assembler_, SLOT(start_alignment(double, double)));
//!!    connect   (assemblyView_->Alignm_Widget(), SIGNAL(alignment_request(double, double, double)), module_assembler_, SLOT(start_alignment(double, double, double)));
//!!
//!!    connect   (module_assembler_ , SIGNAL(object_angle(double)), assemblyView_->Alignm_Widget(), SLOT(show_object_angle(double)));
//!!    connect   (module_assembler_ , SIGNAL(alignment_finished()), assemblyView_->Alignm_Widget(), SLOT(enable()));
//!!
//!!    connect   (module_assembler_ , SIGNAL(nextAlignmentStep   (int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));
//!!
//!!    connect   (module_assembler_ , SIGNAL(acquireImage())                                   , image_ctr_       , SLOT(acquire_image()));
//!!    connect   (finder_    , SIGNAL(       updated_image_master())                           , finder_   , SLOT(update_image_master_PatRec()));
//!!    connect   (finder_    , SIGNAL(updated_image_master_PatRec())                           , finder_   , SLOT(run_PatRec_lab_marker()));
//!!    connect   (finder_    , SIGNAL(reportObjectLocation(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));
//!!
//!!    connect   (module_assembler_ , SIGNAL(motion_finished())                                , module_assembler_, SLOT(launch_next_alignment_step()));
//!!
//!!    NQLog("AssemblyMainWindow", NQLog::Spam) << "connect_objectAligner"
//!!       << ": emitting signal \"objectAligner_connected\"";
//!!
//!!    emit objectAligner_connected();
//!!}
//!!
//!!void AssemblyMainWindow::disconnect_objectAligner()
//!!{
//!!}

void AssemblyMainWindow::connect_multipickupNpatrec(const AssemblyMultiPickupTester::Configuration& conf)
{
    if(image_ctr_ == nullptr)
    {
      NQLog("AssemblyMainWindow", NQLog::Warning) << "connect_multipickupNpatrec"
         << ": ImageController not initialized, no action taken (hint: click \"Camera ON\")";

      return;
    }

    multipickup_->set_configuration(conf);

    assemblyView_->MultiPickup_Widget()->enable(false);

    connect(this        , SIGNAL(multipickupNpatrec_connected()), multipickup_, SLOT(start_measurement()));
    connect(multipickup_, SIGNAL(measurement_finished())        , multipickup_, SLOT(start_pickup()));
    connect(multipickup_, SIGNAL(pickup_finished())             , multipickup_, SLOT(start_measurement()));
    connect(multipickup_, SIGNAL(test_finished())               , this        , SLOT(disconnect_multipickupNpatrec()));

    // measurement
    connect(multipickup_  , SIGNAL(measurement_request()) , image_ctr_    , SLOT(acquire_image()));
    connect(finder_, SIGNAL(       updated_image_master()), finder_, SLOT(update_image_master_PatRec()));
    connect(finder_, SIGNAL(updated_image_master_PatRec()), finder_, SLOT(run_PatRec_lab_marker()));
    connect(finder_, SIGNAL(PatRec_exitcode(int))  , multipickup_  , SLOT(finish_measurement(int)));
    // ---

    // pickup (vacuum)
    connect(multipickup_  , SIGNAL(vacuum_toggle(int))  , conradManager_, SLOT(toggleVacuum(int)));
    connect(conradManager_, SIGNAL(enableVacuumButton()), multipickup_  , SLOT(setup_next_step()));
    // ---

    NQLog("AssemblyMainWindow", NQLog::Spam) << "connect_multipickupNpatrec"
       << ": emitting signal \"multipickupNpatrec_connected\"";

    emit multipickupNpatrec_connected();
}

void AssemblyMainWindow::disconnect_multipickupNpatrec()
{
    assemblyView_->MultiPickup_Widget()->enable(true);

    disconnect(this        , SIGNAL(multipickupNpatrec_connected()), multipickup_, SLOT(start_measurement()));
    disconnect(multipickup_, SIGNAL(measurement_finished())        , multipickup_, SLOT(start_pickup()));
    disconnect(multipickup_, SIGNAL(pickup_finished())             , multipickup_, SLOT(start_measurement()));
    disconnect(multipickup_, SIGNAL(test_finished())               , this        , SLOT(disconnect_multipickupNpatrec()));

    // measurement
    disconnect(multipickup_  , SIGNAL(measurement_request())        , image_ctr_    , SLOT(acquire_image()));
    disconnect(finder_, SIGNAL(updated_image_master())       , finder_, SLOT(update_image_master_PatRec()));
    disconnect(finder_, SIGNAL(updated_image_master_PatRec()), finder_, SLOT(run_PatRec_lab_marker()));
    disconnect(finder_, SIGNAL(PatRec_exitcode(int))         , multipickup_  , SLOT(finish_measurement(int)));
    // ---

    // pickup (vacuum)
    disconnect(multipickup_  , SIGNAL(vacuum_toggle(int))  , conradManager_, SLOT(toggleVacuum(int)));
    disconnect(conradManager_, SIGNAL(enableVacuumButton()), multipickup_  , SLOT(setup_next_step()));
    // ---

    NQLog("AssemblyMainWindow", NQLog::Spam) << "disconnect_multipickupNpatrec"
       << ": emitting signal \"multipickupNpatrec_disconnected\"";

    emit multipickupNpatrec_disconnected();

    NQLog("AssemblyMainWindow", NQLog::Message) << "disconnect_multipickupNpatrec"
       << ": multi-pickup test completed";
}

void AssemblyMainWindow::testTimer()
{
    NQLog("AssemblyMainWindow", NQLog::Spam) << "testTimer"
       << ": timeOut=" << testTimerCount_;

    testTimerCount_ += 0.1;

    return;
}

void AssemblyMainWindow::quit_thread(QThread* thread, const std::string& msg) const
{
    if(thread)
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
    if(camera_)
    {
      NQLog("AssemblyMainWindow", NQLog::Spam) << "quit"
         << ": emitting signal \"images_OFF\"";

      emit images_OFF();

      camera_ = 0;
    }

    this->quit_thread(motion_thread_       , "terminated LStepExpressMotionThread");
    this->quit_thread(camera_thread_       , "terminated AssemblyUEyeCameraThread");
//    this->quit_thread(finder_thread_, "terminated AssemblyObjectFinderPatRecThread");

    NQLog("AssemblyMainWindow", NQLog::Message) << "quit: application closed";

    return;
}

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
#include <Log.h>

#include <string>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

AssemblyMainWindow::AssemblyMainWindow(const unsigned int camera_ID, QWidget *parent) :
  QMainWindow(parent),

  toolBar_(0),
  tabWidget_(0),

//  finderView_(0),
//  edgeView_(0),
//  rawView_(0),
  autoFocusView_(0),
  thresholdTunerView_(0),
  assembleView_(0),

  checkbox1(0),
  checkbox2(0),
  checkbox3(0),
  checkbox4(0),

  cameraModel_(0),
//!!  cameraWidget_(0),

  camera_ID_(camera_ID),
  camera_(0),
  cameraThread_(0),

  cmdr_zscan(0),
  zfocus_finder_(0),

  finder_(0),
  finderThread_(0),
  finderWidget_(0),

  motionModel_(0),
  motionManager_(0),
  motionThread_(0),
  motionSettings_(0),
  motionSettingsWidget_(0),

  conradModel_(0),
  conradManager_(0),

  module_assembler_(0),

  image_ctr_(0),

  testTimerCount_(0.),
  liveTimer_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    motionModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(), 1000, 1000);

    motionManager_ = new LStepExpressMotionManager(motionModel_);

    motionThread_  = new LStepExpressMotionThread(this);
    motionThread_->start();

    motionModel_  ->moveToThread(motionThread_);
    motionManager_->moveToThread(motionThread_);

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

//    finderView_ = new AssemblyUEyeSnapShooter(tabWidget_);
//    tabWidget_->addTab(finderView_, "finder");

//    edgeView_ = new AssemblyUEyeSnapShooter(tabWidget_);
//    tabWidget_->addTab(edgeView_, "edges");

//    rawView_ = new AssemblyUEyeSnapShooter(tabWidget_);
//    tabWidget_->addTab(rawView_, "raw");

    cameraModel_ = new AssemblyUEyeModel_t(10);
    cameraModel_->updateInformation();

    camera_ = cameraModel_->getCameraByID(camera_ID_);

    if(!camera_)
    {
      const std::string log("null pointer to AssemblyVUEyeCamera object (camera-ID = "+std::to_string(camera_ID_)+")");
      Log::KILL("AssemblyMainWindow::AssemblyMainWindow -- "+log);
    }

    cameraThread_ = new AssemblyUEyeCameraThread(cameraModel_, this);
    cameraThread_->start();

    finder_       = new AssemblySensorMarkerFinder();
    finderThread_ = new AssemblyMarkerFinderThread(finder_, this);
    finderThread_->start();

/////!!!!

cmdr_zscan = new AssemblyScanner(motionModel_, 0);

zfocus_finder_ = new ZFocusFinder(camera_, motionModel_);

/////!!!!

    /* AUTO-FOCUS VIEW ----------------------------------------- */
    const std::string tabname_AutoFocus("Auto-Focus");

    autoFocusView_ = new AssemblyAutoFocus(cmdr_zscan, tabWidget_);
    tabWidget_->addTab(autoFocusView_, QString(tabname_AutoFocus.c_str()));

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "added view \""+tabname_AutoFocus+"\"";
    /* --------------------------------------------------------- */

/////!!!!

connect(autoFocusView_, SIGNAL(run_scan(double, int))  , zfocus_finder_, SLOT(update_focus_inputs(double, int)));

connect(zfocus_finder_, SIGNAL(show_zscan(std::string)), autoFocusView_, SLOT(make_graph(std::string)));
connect(zfocus_finder_, SIGNAL(update_text(double))    , autoFocusView_, SLOT(updateText(double)));

/////!!!!

    /* IMAGE-THRESHOLDING VIEW --------------------------------- */
    const std::string tabname_ImageThresholding("Image Thresholding");

    thresholdTunerView_ = new AssemblyThresholdTuner(tabWidget_);
    tabWidget_->addTab(thresholdTunerView_, QString(tabname_ImageThresholding.c_str()));

    connect(thresholdTunerView_, SIGNAL(updateThresholdLabel())            , finder_            , SLOT(getCurrentGeneralThreshold()));
    connect(this               , SIGNAL(updateThresholdLabel())            , finder_            , SLOT(getCurrentGeneralThreshold()));
    connect(finder_            , SIGNAL(sendCurrentGeneralThreshold(int))  , thresholdTunerView_, SLOT(updateThresholdLabelSlot(int)));
    connect(thresholdTunerView_, SIGNAL(setNewThreshold(int,cv::Mat))      , finder_            , SLOT(setNewGeneralThreshold(int, cv::Mat)));
    connect(finder_            , SIGNAL(sendUpdatedThresholdImage(QString)), thresholdTunerView_, SLOT(updateThresholdImage(QString)));

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "added view \""+tabname_ImageThresholding+"\"";
    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "emitting signal \"updateThresholdLabel\"";

    emit updateThresholdLabel();
    /* --------------------------------------------------------- */

    /* MANUAL-ASSEMBLY VIEW ------------------------------------ */
    const std::string tabname_ManualAssembly("Manual Assembly");

    assembleView_ = new AssemblyModuleAssembler(camera_, finder_, motionModel_, tabWidget_);
    tabWidget_->addTab(assembleView_, QString(tabname_ManualAssembly.c_str()));

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "added view \""+tabname_ManualAssembly+"\"";

    // VACUUM connections
    conradModel_   = new ConradModel(assembleView_);
    conradManager_ = new ConradManager(conradModel_);

    module_assembler_ = new AssemblyAssembler(motionModel_);

    connect(assembleView_->toggle1, SIGNAL(toggleVacuum(int))           , conradManager_, SLOT(toggleVacuum(int)));
//!!    connect(assembleView_         , SIGNAL(runObjectDetection(int, int)), finder_       , SLOT(runObjectDetection(int, int)));

//  connect(assembleView_->toggle1, SIGNAL(toggleVacuum(int))                  , assembleView_->toggle1, SLOT(disableVacuumButton()));
    connect(conradManager_        , SIGNAL(updateVacuumChannelState(int, bool)), assembleView_->toggle1, SLOT(updateVacuumChannelState(int, bool)));
    connect(this                  , SIGNAL(updateVacuumChannelsStatus())       , conradManager_        , SLOT(updateVacuumChannelsStatus()));
//  connect(camera_               , SIGNAL(imageAcquired(cv::Mat))             , finder_               , SLOT(runObjectDetection_labmode(cv::Mat)));

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "emitting signal \"updateVacuumChannelsStatus\"";

    emit updateVacuumChannelsStatus();
    // ---

    /* --------------------------------------------------------- */

//!!    /* U-EYE VIEW ---------------------------------------------- */
//!!    const std::string tabname_uEye("uEye");
//!!
//!!    cameraWidget_ = new AssemblyUEyeWidget(cameraModel_, this);
//!!    tabWidget_->addTab(cameraWidget_, QString(tabname_uEye.c_str()));
//!!
//!!    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "added view \""+tabname_uEye+"\"";
//!!    /* --------------------------------------------------------- */

    /* MOTION-SETTINGS VIEW ------------------------------------ */
    const std::string tabname_MotionSettings("Motion Settings");

    QWidget* widget = new QWidget(tabWidget_);

    motionSettings_ = new LStepExpressSettings(motionModel_, widget);

    motionSettingsWidget_ = new LStepExpressSettingsWidget(motionSettings_, tabWidget_);
    tabWidget_->addTab(motionSettingsWidget_, QString(tabname_MotionSettings.c_str()));

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "added view \""+tabname_MotionSettings+"\"";
    /* --------------------------------------------------------- */

    /* MOTION-MANAGER VIEW ------------------------------------- */
    const std::string tabname_MotionManager("Motion Manager");

    //Probably need a dedicated class for Motionmanager tab widget so
    //no details have to be included here (but this was just for testing)

    tabWidget_->addTab(widget, QString(tabname_MotionManager.c_str()));

    QHBoxLayout* layout  = new QHBoxLayout(widget);
    widget->setLayout(layout);

    QVBoxLayout* layoutv = new QVBoxLayout();

    LStepExpressWidget* motionWidget = new LStepExpressWidget(motionModel_, widget);
    layoutv->addWidget (motionWidget);

//    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "LStepExpressWidget added";

    LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(motionModel_, widget);
    layoutv->addWidget(lStepJoystick);

    layout->addLayout(layoutv);

    QVBoxLayout* layoutv2 = new QVBoxLayout();

    LStepExpressPositionWidget *lStepPosition = new LStepExpressPositionWidget(motionManager_, motionModel_, widget);
    layoutv2->addWidget(lStepPosition);

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "added view \""+tabname_MotionManager+"\"";
    /* --------------------------------------------------------- */

    /* Upper Toolbar ------------------------------------------- */
    toolBar_ = addToolBar("Tools");
    toolBar_ ->addAction("Camera ON" , this, SLOT( enable_images()));
    toolBar_ ->addAction("Camera OFF", this, SLOT(disable_images()));
    toolBar_ ->addAction("Snapshot"  , this, SLOT(    get_image ()));

    checkbox1 = new QCheckBox("Auto-Focusing", this);
    toolBar_->addWidget(checkbox1);

    checkbox2 = new QCheckBox("Precision", this);
    toolBar_->addWidget(checkbox2);

    checkbox3 = new QCheckBox("Assembly", this);
    toolBar_->addWidget(checkbox3);

    checkbox4 = new QCheckBox("Alignment", this);
    toolBar_->addWidget(checkbox4);

    connect(checkbox1, SIGNAL(stateChanged(int)), this, SLOT(changeState_AutoFocus          (int)));
    connect(checkbox2, SIGNAL(stateChanged(int)), this, SLOT(changeState_PrecisionEstimation(int)));
    connect(checkbox3, SIGNAL(stateChanged(int)), this, SLOT(changeState_SandwichAssembly   (int)));
    connect(checkbox4, SIGNAL(stateChanged(int)), this, SLOT(changeState_Alignment          (int)));

    this->setCentralWidget(tabWidget_);

    this->updateGeometry();
    /* --------------------------------------------------------- */

    liveTimer_ = new QTimer(this);

    connect(liveTimer_, SIGNAL(timeout()), this, SLOT(liveUpdate()));

    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(quit()));

    NQLog("AssemblyMainWindow::AssemblyMainWindow") << "constructed";
}

void AssemblyMainWindow::liveUpdate()
{
    NQLog("AssemblyMainWindow::liveUpdate") << "emitting signal \"image\"";

    emit image();
}

void AssemblyMainWindow::enable_images()
{
    NQLog("AssemblyMainWindow::enable_images") << "connecting main-window to image-controller";

    image_ctr_ = new ImageController(camera_, zfocus_finder_);

    connect(this      , SIGNAL(images_ON())      , image_ctr_, SLOT(enable()));
    connect(image_ctr_, SIGNAL(camera_enabled()) , this      , SLOT(connect_images()));

    connect(this      , SIGNAL(images_OFF())     , image_ctr_, SLOT(disable()));
    connect(image_ctr_, SIGNAL(camera_disabled()), this      , SLOT(disconnect_images()));

    connect(this      , SIGNAL(image())          , image_ctr_, SLOT(acquire_image()));
    connect(this      , SIGNAL(AutoFocus_ON ())  , image_ctr_, SLOT( enable_AutoFocus()));
    connect(this      , SIGNAL(AutoFocus_OFF())  , image_ctr_, SLOT(disable_AutoFocus()));

//!!    if(thresholdTunerView_)
//!!    {
//!!      connect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), thresholdTunerView_, SLOT(imageAcquired(cv::Mat)));
//!!      connect(image_ctr_, SIGNAL(image_acquired(cv::Mat)), autoFocusView_     , SLOT(imageAcquired(cv::Mat)));
//!!    }
//!!    else
//!!    {
//!!      NQLog("AssemblyMainWindow::enable_images") << "logic error: AssemblyThresholdTuner not initialized, ImageController not connected to it";
//!!    }

    emit images_ON();
}

void AssemblyMainWindow::disable_images()
{
    if(image_ctr_)
    {
      disconnect(this      , SIGNAL(images_ON())      , image_ctr_, SLOT(enable()));
      disconnect(image_ctr_, SIGNAL(camera_enabled()) , this      , SLOT(connect_images()));

      disconnect(this      , SIGNAL(images_OFF())     , image_ctr_, SLOT(disable()));
      disconnect(image_ctr_, SIGNAL(camera_disabled()), this      , SLOT(disconnect_images()));

      disconnect(this      , SIGNAL(image())          , image_ctr_, SLOT(acquire_image()));
      disconnect(this      , SIGNAL(AutoFocus_ON())   , image_ctr_, SLOT( enable_AutoFocus()));
      disconnect(this      , SIGNAL(AutoFocus_OFF())  , image_ctr_, SLOT(disable_AutoFocus()));

      delete image_ctr_;

      image_ctr_ = 0;

      NQLog("AssemblyMainWindow::disable_images") << "signal-slot connections disabled";
    }

    emit images_OFF();
}

void AssemblyMainWindow::changeState_AutoFocus(int state)
{
    if(!image_ctr_){

      NQLog("AssemblyMainWindow::changeState_AutoFocus") << "ImageController not initialized, no action taken (hint: click \"Camera ON\")";

      return;
    }

    if(!motionModel_){

      NQLog("AssemblyMainWindow::changeState_AutoFocus") << "LStepExpressMotionModel not initialized, no action taken (hint: plug-in motion stage cable)";

      return;
    }

    if(state == 2){

      NQLog("AssemblyMainWindow::changeState_AutoFocus") << "emitting signal \"AutoFocus_ON\"";

      emit AutoFocus_ON();
    }
    else if(state == 0){

      NQLog("AssemblyMainWindow::changeState_AutoFocus") << "emitting signal \"AutoFocus_OFF\"";

      emit AutoFocus_OFF();
    }

    return;
}

void AssemblyMainWindow::changeState_PrecisionEstimation(int /* state */)
{
/*
    if(state == 2){

      NQLog("AssemblyMainWindow::changeState_PrecisionEstimation") << "precision estimation ON";

      connect(assembleView_, SIGNAL(launchPrecisionEstimation(double, double, double, double, double, double, int)), cmdr_zscan,
                             SLOT  (run_precisionestimation  (double, double, double, double, double, double, int)));

      connect   (cmdr_zscan        , SIGNAL(moveAbsolute(double, double, double, double)), motionManager_, SLOT(moveAbsolute(double, double,double, double)));
      connect   (motionModel_, SIGNAL(motionFinished())                            , cmdr_zscan    , SLOT(process_step()));
      connect   (cmdr_zscan        , SIGNAL(toggleVacuum(int))                           , conradManager_, SLOT(toggleVacuum(int)));
      connect   (conradManager_    , SIGNAL(updateVacuumChannelState(int, bool))         , cmdr_zscan    , SIGNAL(nextStep()));

//      connect(cmdr_zscan, SIGNAL(toggleVacuum()), conradManager_, SLOT(changeVacuum()));
//      for testing with random numbers
//        connect(cmdr_zscan, SIGNAL(makeDummies(int, double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));

//      for real lab tests with camera
      connect   (cmdr_zscan, SIGNAL(acquireImage())          , camera_      , SLOT(acquireImage()));
      connect   (cmdr_zscan, SIGNAL(changeVacuumState())     , cmdr_zscan   , SLOT(changeVacuumState()));
      connect   (cmdr_zscan, SIGNAL(showHistos(int, QString)), assembleView_, SLOT(updateImage(int, QString)));

      connect   (camera_   , SIGNAL(imageAcquired(cv::Mat))                           , finder_   , SLOT(runObjectDetection_labmode(cv::Mat)));
      connect   (finder_   , SIGNAL(reportObjectLocation(int, double, double, double)), cmdr_zscan, SLOT(fill_positionvectors(int, double, double, double)));
      connect   (cmdr_zscan, SIGNAL(nextStep())                                       , cmdr_zscan, SLOT(process_step()));

      NQLog("AssemblyMainWindow::changeState_PrecisionEstimation") << "signal-slot connections enabled";
    }
    else if(state == 0 ){

      NQLog("AssemblyMainWindow::changeState_PrecisionEstimation") << "precision estimation OFF";

      disconnect(cmdr_zscan        , SIGNAL(moveAbsolute(double, double,double, double)), motionManager_, SLOT(moveAbsolute(double, double,double, double)));
      disconnect(motionModel_, SIGNAL(motionFinished())                           , camera_       , SLOT(acquireImage()));
      disconnect(cmdr_zscan        , SIGNAL(toggleVacuum(int))                          , conradManager_, SLOT(toggleVacuum(int)));
      disconnect(conradManager_    , SIGNAL(updateVacuumChannelState(int, bool))        , cmdr_zscan    , SIGNAL(nextStep()));
      disconnect(camera_           , SIGNAL(imageAcquired(cv::Mat))                     , finder_       , SLOT(findMarker_templateMatching(int, cv::Mat)));
      disconnect(finder_           , SIGNAL(getImageBlur(cv::Mat, cv::Rect))            , cmdr_zscan    , SLOT(write_image(cv::Mat, cv::Rect)) );
      disconnect(cmdr_zscan        , SIGNAL(make_graph(vector<double>,vector<double>))  , autoFocusView_, SLOT(make_graph(vector<double>,vector<double>)));
      disconnect(cmdr_zscan        , SIGNAL(updateText(double))                         , autoFocusView_, SLOT(updateText(double)));

      NQLog("AssemblyMainWindow::changeState_PrecisionEstimation") << "signal-slot connections disabled";
    }
*/
    return;
}

void AssemblyMainWindow::changeState_SandwichAssembly(int /* state */)
{
/*
    if(state == 2){

      NQLog("AssemblyMainWindow::changeState_SandwichAssembly") << ": state  " << state;

      connect(assembleView_, SIGNAL(launchSandwitchAssembly(double, double, double, double, double, double, double, double, double)), module_assembler_,
                             SLOT  (run_sandwitchassembly  (double, double, double, double, double, double, double, double, double)));

      connect(module_assembler_ , SIGNAL(moveAbsolute(double, double, double, double)), motionManager_   , SLOT(moveAbsolute(double, double,double, double)));
      connect(motionModel_, SIGNAL(motionFinished())                            , module_assembler_, SLOT(process_step()));
      connect(module_assembler_ , SIGNAL(toggleVacuum(int))                           , conradManager_   , SLOT(toggleVacuum(int)));
      connect(conradManager_    , SIGNAL(updateVacuumChannelState(int, bool))         , module_assembler_, SIGNAL(nextStep()));

      //for testing with random numbers
      // connect(cmdr_zscan, SIGNAL(makeDummies(int, double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));

      // for real lab tests with camera
      connect(module_assembler_, SIGNAL(acquireImage())          , camera_      , SLOT(acquireImage()));
      connect(module_assembler_, SIGNAL(showHistos(int, QString)), assembleView_, SLOT(updateImage(int, QString)));
      connect(camera_          , SIGNAL(imageAcquired(cv::Mat))  , finder_      , SLOT(runObjectDetection_labmode(cv::Mat)) );

      connect(finder_          , SIGNAL(reportObjectLocation(int,double,double,double)), module_assembler_, SLOT(centre_marker(int, double,double,double)));
      connect(module_assembler_, SIGNAL(nextStep())                                    , module_assembler_, SLOT(process_step()));
    }
    else if(state == 0){

      NQLog("AssemblyMainWindow::changeState_SandwichAssembly") << ": state  " << state;

      disconnect(assembleView_, SIGNAL(launchSandwitchAssembly(double, double, double, double, double, double, double, double, double)), module_assembler_,
                                SLOT  (run_sandwitchassembly  (double, double, double, double, double, double, double, double, double)));

      disconnect(module_assembler_ , SIGNAL(moveAbsolute(double, double, double, double)), motionManager_   , SLOT(moveAbsolute(double, double,double, double)));
      disconnect(motionModel_, SIGNAL(motionFinished())                            , module_assembler_, SLOT(process_step()));
      disconnect(module_assembler_ , SIGNAL(toggleVacuum(int))                           , conradManager_   , SLOT(toggleVacuum(int)));
      disconnect(conradManager_    , SIGNAL(updateVacuumChannelState(int, bool))         , module_assembler_, SIGNAL(nextStep()));

      // for testing with random numbers
//      disconnect(cmdr_zscan, SIGNAL(makeDummies(int, double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));

      // for real lab tests with camera
      disconnect(module_assembler_, SIGNAL(acquireImage())                                , camera_          , SLOT(acquireImage()));
      disconnect(module_assembler_, SIGNAL(showHistos(int, QString))                      , assembleView_    , SLOT(updateImage(int, QString)));
      disconnect(camera_          , SIGNAL(imageAcquired(cv::Mat))                        , finder_          , SLOT(runObjectDetection_labmode(cv::Mat)) );
      disconnect(finder_          , SIGNAL(reportObjectLocation(int,double,double,double)), module_assembler_, SLOT(fill_positionvectors(int, double,double,double)));
      disconnect(module_assembler_, SIGNAL(nextStep())                                    , module_assembler_, SLOT(process_step()));
    }
*/
    return;
}

void AssemblyMainWindow::changeState_Alignment(int state)
{
    if(state == 2){

      NQLog("AssemblyMainWindow::changeState_Alignment") << "alignment ON";

      connect   (image_ctr_        , SIGNAL(image_acquired(cv::Mat))                          , finder_          , SLOT(runObjectDetection_labmode(cv::Mat)));
      connect   (assembleView_     , SIGNAL(launchAlignment     (int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));
      connect   (module_assembler_ , SIGNAL(acquireImage())                                   , image_ctr_       , SLOT(acquire_image()));
      connect   (motionModel_      , SIGNAL(motionFinished())                                 , module_assembler_, SLOT(launch_next_alignment_step()));
      connect   (module_assembler_ , SIGNAL(moveRelative(double, double, double, double))     , motionManager_   , SLOT(moveRelative(double, double, double, double)));
      connect   (finder_           , SIGNAL(reportObjectLocation(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));
      connect   (module_assembler_ , SIGNAL(nextAlignmentStep   (int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));

      NQLog("AssemblyMainWindow::changeState_Alignment") << "signal-slot connections enabled";
    }
    else if(state == 0){

      NQLog("AssemblyMainWindow::changeState_Alignment") << "alignment OFF";

      disconnect(image_ctr_        , SIGNAL(image_acquired(cv::Mat))                          , finder_          , SLOT(runObjectDetection_labmode(cv::Mat)) );
      disconnect(assembleView_     , SIGNAL(launchAlignment     (int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));
      disconnect(module_assembler_ , SIGNAL(acquireImage())                                   , image_ctr_    , SLOT(acquire_image()));
//      disconnect(motionModel_     , SIGNAL(motionFinished())                                 , module_assembler_, SLOT(launch_next_alignment_step()));
      disconnect(module_assembler_ , SIGNAL(moveRelative(double, double, double, double))     , motionManager_   , SLOT(moveRelative(double, double, double, double)));
      disconnect(finder_           , SIGNAL(reportObjectLocation(int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));
      disconnect(module_assembler_ , SIGNAL(nextAlignmentStep   (int, double, double, double)), module_assembler_, SLOT(run_alignment(int, double, double, double)));

      NQLog("AssemblyMainWindow::changeState_Alignment") << "signal-slot connections disabled";
    }

    return;
}

void AssemblyMainWindow::get_image()
{
    if(image_ctr_ == 0){

      NQLog("AssemblyMainWindow::get_image") << "ImageController not initialized, no action taken (hint: click \"Camera ON\")";

      return;
    }

    if(image_ctr_->is_enabled() == false){

      NQLog("AssemblyMainWindow::get_image") << "ImageController not enabled, no action taken (hint: click \"Camera ON\")";

      return;
    }

    NQLog("AssemblyMainWindow::get_image") << "emitting signal \"image\"";

    emit image();
}

void AssemblyMainWindow::testTimer()
{
    NQLog("AssemblyMainWindow::testTimer") << "timeOut = " << testTimerCount_;
    testTimerCount_ += 0.1;
}

void AssemblyMainWindow::connect_images()
{
//    finderView_        ->connectImageProducer(finder_, SIGNAL(markerFound  (const cv::Mat&)));
//    edgeView_          ->connectImageProducer(finder_, SIGNAL(edgesDetected(const cv::Mat&)));
//    rawView_           ->connectImageProducer(camera_, SIGNAL(imageAcquired(const cv::Mat&)));

    thresholdTunerView_->connectImageProducer(image_ctr_    , SIGNAL(image_acquired(cv::Mat)));
    autoFocusView_     ->connectImageProducer(zfocus_finder_, SIGNAL(image_acquired(cv::Mat)));

//    const bool test = connect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(write_image(cv::Mat)));
//    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(runObjectDetection_labmode(cv::Mat)));
//    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(locatePickup(cv::Mat)));

    NQLog("AssemblyMainWindow::connect_images") << "connecting finder and camera";

    connect(finder_, SIGNAL(getImage())    , image_ctr_, SLOT(acquire_image()));
    connect(finder_, SIGNAL(acquireImage()), image_ctr_, SLOT(acquire_image()));

    connect(finder_, SIGNAL(locatePickupCorner_templateMatching(cv::Mat, cv::Mat)), finder_, SLOT(findMarker_templateMatching(cv::Mat, cv::Mat)));

//!!    connect(camera_, SIGNAL(imageAcquired(const cv::Mat&)), finder_, SLOT(findMarker(const cv::Mat&)));

//    connect image acquired signal of camera model to blur detection SLOT of Z scanner
//!!    connect(camera_, SIGNAL(imageAcquired(const cv::Mat&)), finder_, SLOT(write_image(const cv::Mat&)));

//    liveTimer_->start(2000);
}

void AssemblyMainWindow::disconnect_images()
{
    NQLog("AssemblyMainWindow::disconnect_images") << "disconnecting camera from view(s)";

//    finderView_        ->disconnectImageProducer(finder_, SIGNAL(markerFound  (const cv::Mat&)));
//    edgeView_          ->disconnectImageProducer(finder_, SIGNAL(edgesDetected(const cv::Mat&)));
//    rawView_           ->disconnectImageProducer(camera_, SIGNAL(imagef       (const cv::Mat&)));

//!!    disconnect(camera_, SIGNAL(imageAcquired(const cv::Mat&)), finder_, SLOT(findMarker(const cv::Mat&)));

    thresholdTunerView_->disconnectImageProducer(image_ctr_    , SIGNAL(image_acquired(cv::Mat)));
    autoFocusView_     ->disconnectImageProducer(zfocus_finder_, SIGNAL(image_acquired(cv::Mat)));

    liveTimer_->stop();
}

void AssemblyMainWindow::quit()
{
    if(camera_){

      NQLog("AssemblyMainWindow::quit") << "emitting signal \"images_OFF\"";

      emit images_OFF();

      camera_ = 0;
    }

    if(cameraThread_){

      NQLog("AssemblyMainWindow::quit") << "quitting camera thread";

      if(cameraThread_->wait(2000) == false){
         cameraThread_->terminate();
      }

      cameraThread_->quit();
    }

    return;
}

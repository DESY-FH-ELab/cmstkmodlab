#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyMainWindow.h"

AssemblyMainWindow::AssemblyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    camera_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();
    
    
    lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),
                                               1000, 1000);
    motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    motionThread_ = new LStepExpressMotionThread(this);
    motionThread_->start();
    motionManager_->myMoveToThread(motionThread_);
    

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    finderView_ = new AssemblyUEyeSnapShooter(tabWidget_);
    tabWidget_->addTab(finderView_, "finder");

    edgeView_ = new AssemblyUEyeSnapShooter(tabWidget_);
    tabWidget_->addTab(edgeView_, "edges");

    rawView_ = new AssemblyUEyeSnapShooter(tabWidget_);
    tabWidget_->addTab(rawView_, "raw");
    
    uEyeModel_ = new AssemblyUEyeModel_t(10);
    cameraThread_ = new AssemblyUEyeCameraThread(uEyeModel_, this);
    cameraThread_->start();
   
    finder_ = new AssemblySensorMarkerFinder();
    finderThread_ = new AssemblyMarkerFinderThread(finder_, this);
    finderThread_->start();
 
    thresholdTunerView_ = new AssemblyThresholdTuner(tabWidget_);
    tabWidget_->addTab(thresholdTunerView_, "Threshold");

    connect(thresholdTunerView_, SIGNAL(updateThresholdLabel()), finder_, SLOT(getCurrentGeneralThreshold()));    
    connect(this, SIGNAL(updateThresholdLabel()), finder_, SLOT(getCurrentGeneralThreshold()));
    connect(finder_, SIGNAL(sendCurrentGeneralThreshold(int)), thresholdTunerView_, SLOT(updateThresholdLabelSlot(int)));
    connect(thresholdTunerView_, SIGNAL(setNewThreshold(int,cv::Mat)), finder_, SLOT(setNewGeneralThreshold(int, cv::Mat)));
    //connect(camera_, SIGNAL(imageAcquired(cv::Mat)), thresholdTunerView_, SLOT(imageAcquired(const cv::Mat&)));
    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), thresholdTunerView_, SLOT(imageAcquired(cv::Mat)));
    connect(finder_, SIGNAL(sendUpdatedThresholdImage(QString)), thresholdTunerView_, SLOT(updateThresholdImage(QString)));
    emit updateThresholdLabel();    
    NQLog("AssemblyThresholdTuner") << " : INFO! : initialization signal sent.";

    
    assembleView_ = new AssemblyModuleAssembler(uEyeModel_, finder_, lStepExpressModel_, tabWidget_);
    tabWidget_->addTab(assembleView_, "Manual assembly");

    //autoAssemblyView_ = new AssemblyAutomatedModuleAssembler(uEyeModel_, finder_, lStepExpressModel_, tabWidget_);
    //tabWidget_->addTab(autoAssemblyView_, "Automated assembly");

    conradModel_ = new ConradModel(assembleView_);

    finderWidget_ = new AssemblySensorMarkerFinderWidget(finder_, this);
    tabWidget_->addTab(finderWidget_, "finder config");

    uEyeWidget_ = new AssemblyUEyeWidget(uEyeModel_, this);
    tabWidget_->addTab(uEyeWidget_, "uEye");
    
    cmdr_zscan = new AssemblyScanner(lStepExpressModel_);
    conradManager_ = new ConradManager(conradModel_);
    module_assembler_ = new AssemblyAssembler(lStepExpressModel_);

    connect(assembleView_ -> toggle1, SIGNAL(toggleVacuum(int)), conradManager_, SLOT(toggleVacuum(int)));
    //connect(assembleView_ -> toggle1, SIGNAL(toggleVacuum(int)), assembleView_ -> toggle1, SLOT(disableVacuumButton()));
    connect(conradManager_, SIGNAL(updateVacuumChannelState(int, bool)), assembleView_ -> toggle1, SLOT(updateVacuumChannelState(int, bool)));
    connect(this, SIGNAL(updateVacuumChannelsStatus()), conradManager_, SLOT(updateVacuumChannelsStatus()));
    
    
    emit updateVacuumChannelsStatus();


    NQLog("AssemblyMainWindow") << "assembly scanner constructed";

    
    autoFocusView_ = new AssemblyAutoFocus(cmdr_zscan, tabWidget_);
    tabWidget_->addTab(autoFocusView_, "Auto Focus");
    
    NQLog("AssemblyMainWindow") << "autofocus view  added";

    QWidget * widget;
    widget= new QWidget(tabWidget_);
    tabWidget_->addTab(widget, "Motion Manager");

    NQLog("AssemblyMainWindow") << "motion manager added";

    
    QHBoxLayout * layout = new QHBoxLayout(widget);
    widget->setLayout(layout);
    
    QVBoxLayout * layoutv = new QVBoxLayout(widget);
    
    LStepExpressWidget *lStepExpressWidget = new LStepExpressWidget(lStepExpressModel_, widget);
    layoutv->addWidget(lStepExpressWidget);
    
    NQLog("AssemblyMainWindow") << "LStepExpressWidget added";

    
    LStepExpressJoystickWidget *lStepJoystick = new LStepExpressJoystickWidget(lStepExpressModel_, widget);
    layoutv->addWidget(lStepJoystick);
    
    layout->addLayout(layoutv);
    
    QVBoxLayout * layoutv2 = new QVBoxLayout(widget);

    
    LStepExpressPositionWidget *lStepPosition = new LStepExpressPositionWidget(motionManager_, lStepExpressModel_, widget);
    layoutv2->addWidget(lStepPosition);
    
    
    connect(QApplication::instance(), SIGNAL(aboutToQuit()),
            this, SLOT(quit()));

    toolBar_ = addToolBar("Tools");
    toolBar_->addAction("open", this, SLOT(onOpenCamera()));
    toolBar_->addAction("close", this, SLOT(onCloseCamera()));
    toolBar_->addAction("snapshot", this, SLOT(onSnapShot()));
    
    checkbox1 = new QCheckBox("Enable auto-focusing", this);
    toolBar_->addWidget(checkbox1);
    
    checkbox2 = new QCheckBox("Enable precision estimation", this);
    toolBar_->addWidget(checkbox2);
    
    checkbox3 = new QCheckBox("Enable sandwitch assembly", this);
    toolBar_->addWidget(checkbox3);
  
    connect(checkbox1, SIGNAL(stateChanged(int)), this, SLOT(enableAutoFocus(int)));
        
    connect(checkbox2, SIGNAL(stateChanged(int)), this, SLOT(enablePrecisionEstimation(int)));
  
    connect(checkbox3, SIGNAL(stateChanged(int)), this, SLOT(enableSandwitchAssembly(int)));


    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("AssemblyMainWindow") << "main window constructed";

    liveTimer_ = new QTimer(this);
    connect(liveTimer_, SIGNAL(timeout()), this, SLOT(liveUpdate()));

    QTimer::singleShot(1, uEyeModel_, SLOT(updateInformation()));
}

void AssemblyMainWindow::liveUpdate()
{
    NQLog("AssemblyMainWindow") << ":liveUpdate()";

    emit acquireImage();
}

void AssemblyMainWindow::onOpenCamera()
{
    NQLog("AssemblyMainWindow") << ":onOpenCamera() ";

    camera_ = uEyeModel_->getCameraByID(10);

    motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);

    connect(this, SIGNAL(openCamera()),
            camera_, SLOT(open()));
    connect(this, SIGNAL(closeCamera()),
            camera_, SLOT(close()));
    connect(this, SIGNAL(acquireImage()),
            camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(cameraOpened()),
            this, SLOT(cameraOpened()));
    connect(camera_, SIGNAL(cameraClosed()),
            this, SLOT(cameraClosed()));

    emit openCamera();
}



void AssemblyMainWindow::enablePrecisionEstimation(int state){
    
    
    NQLog("AssemblyMainWindow::enablePrecisionEstimation") << ": state  " << state;
    
    if (state == 2){
        
    connect(assembleView_, SIGNAL(launchPrecisionEstimation(double, double, double, double, double, double, int)),cmdr_zscan, SLOT(run_precisionestimation(double, double, double, double, double, double, int)));

    connect(cmdr_zscan, SIGNAL(moveAbsolute(double, double, double, double)),motionManager_, SLOT(moveAbsolute(double, double,double, double)));
    connect(lStepExpressModel_, SIGNAL(motionFinished()), cmdr_zscan, SLOT(process_step()));
    connect(cmdr_zscan, SIGNAL(toggleVacuum(int)), conradManager_, SLOT(toggleVacuum(int)));
    connect(conradManager_, SIGNAL(updateVacuumChannelState(int, bool)), cmdr_zscan, SIGNAL(nextStep()));
        
        //for testing with random numbers
    // connect(cmdr_zscan, SIGNAL(makeDummies(int, double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));

    //    for real lab tests with camera
    connect(cmdr_zscan, SIGNAL(acquireImage()), camera_, SLOT(acquireImage()));

    connect(cmdr_zscan, SIGNAL(showHistos(int, QString)), assembleView_, SLOT(updateImage(int, QString))); 

    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(runObjectDetection_labmode(cv::Mat)) );
    connect(finder_,SIGNAL(reportObjectLocation(int,double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));
    connect(cmdr_zscan, SIGNAL(nextStep()), cmdr_zscan, SLOT(process_step()));

        
    }else if (state == 0 ){
        
    disconnect(cmdr_zscan, SIGNAL(moveAbsolute(double, double,double, double)),motionManager_, SLOT(moveAbsolute(double, double,double, double)));
    disconnect(lStepExpressModel_, SIGNAL(motionFinished()), camera_, SLOT(acquireImage()));
    disconnect(cmdr_zscan, SIGNAL(toggleVacuum(int)), conradManager_, SLOT(toggleVacuum(int)));
    disconnect(conradManager_, SIGNAL(updateVacuumChannelState(int, bool)), cmdr_zscan, SIGNAL(nextStep()));
    disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(findMarker_templateMatching(int, cv::Mat)) );
    disconnect(finder_, SIGNAL(getImageBlur(cv::Mat, cv::Rect)), cmdr_zscan, SLOT(write_image(cv::Mat, cv::Rect)) );
    disconnect(cmdr_zscan,SIGNAL(make_graph(vector<double>,vector<double>)),autoFocusView_,SLOT(make_graph(vector<double>,vector<double>)));
    disconnect(cmdr_zscan,SIGNAL(updateText(double)),autoFocusView_,SLOT(updateText(double)));
        
    }
}




void AssemblyMainWindow::enableSandwitchAssembly(int state){
    
    
    NQLog("AssemblyMainWindow::enableSandwitchAssembly") << ": state  " << state;
    
    if (state == 2){
        
      connect(assembleView_, SIGNAL(launchSandwitchAssembly(double, double, double, double, double, double, double, double, double)), module_assembler_, SLOT(run_sandwitchassembly(double, double, double, double, double, double, double, double, double)));

    connect(module_assembler_, SIGNAL(moveAbsolute(double, double, double, double)),motionManager_, SLOT(moveAbsolute(double, double,double, double)));
    connect(lStepExpressModel_, SIGNAL(motionFinished()), module_assembler_, SLOT(process_step()));
    connect(module_assembler_, SIGNAL(toggleVacuum(int)), conradManager_, SLOT(toggleVacuum(int)));
    connect(conradManager_, SIGNAL(updateVacuumChannelState(int, bool)), module_assembler_, SIGNAL(nextStep()));
        
        //for testing with random numbers
    // connect(cmdr_zscan, SIGNAL(makeDummies(int, double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));

    //    for real lab tests with camera
    connect(module_assembler_, SIGNAL(acquireImage()), camera_, SLOT(acquireImage()));

    connect(module_assembler_, SIGNAL(showHistos(int, QString)), assembleView_, SLOT(updateImage(int, QString))); 

    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(runObjectDetection_labmode(cv::Mat)) );
    connect(finder_,SIGNAL(reportObjectLocation(int,double,double,double)), module_assembler_, SLOT(fill_positionvectors(int, double,double,double)));
    connect(module_assembler_, SIGNAL(nextStep()), module_assembler_, SLOT(process_step()));

        
    }else if (state == 0 ){
        
    disconnect(assembleView_, SIGNAL(launchSandwitchAssembly(double, double, double, double, double, double, double, double, double)), module_assembler_, SLOT(run_sandwitchassembly(double, double, double, double, double, double, double, double, double)));

    disconnect(module_assembler_, SIGNAL(moveAbsolute(double, double, double, double)),motionManager_, SLOT(moveAbsolute(double, double,double, double)));
    disconnect(lStepExpressModel_, SIGNAL(motionFinished()), module_assembler_, SLOT(process_step()));
    disconnect(module_assembler_, SIGNAL(toggleVacuum(int)), conradManager_, SLOT(toggleVacuum(int)));
    disconnect(conradManager_, SIGNAL(updateVacuumChannelState(int, bool)), module_assembler_, SIGNAL(nextStep()));
        
        //for testing with random numbers
    // disconnect(cmdr_zscan, SIGNAL(makeDummies(int, double,double,double)), cmdr_zscan, SLOT(fill_positionvectors(int, double,double,double)));

    //    for real lab tests with camera
    disconnect(module_assembler_, SIGNAL(acquireImage()), camera_, SLOT(acquireImage()));

    disconnect(module_assembler_, SIGNAL(showHistos(int, QString)), assembleView_, SLOT(updateImage(int, QString))); 

    disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(runObjectDetection_labmode(cv::Mat)) );
    disconnect(finder_,SIGNAL(reportObjectLocation(int,double,double,double)), module_assembler_, SLOT(fill_positionvectors(int, double,double,double)));
    disconnect(module_assembler_, SIGNAL(nextStep()), module_assembler_, SLOT(process_step()));
        
    }
}

void AssemblyMainWindow::enableAutoFocus(int state){

    
NQLog("AssemblyMainWindow::enableAutoFocus") << ": state  " << state;

if (state == 2) {

    
    connect(cmdr_zscan, SIGNAL(moveRelative(double, double,double, double)),motionManager_, SLOT(moveRelative(double, double,double, double)));
    connect(lStepExpressModel_, SIGNAL(motionFinished()), camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(findMarker_templateMatching(cv::Mat)) );
    connect(finder_, SIGNAL(getImageBlur(cv::Mat, cv::Rect)), cmdr_zscan, SLOT(write_image(cv::Mat, cv::Rect)) );
    connect(cmdr_zscan,SIGNAL(make_graph(vector<double>,vector<double>)),autoFocusView_,SLOT(make_graph(vector<double>,vector<double>)));
    connect(cmdr_zscan,SIGNAL(updateText(double)),autoFocusView_,SLOT(updateText(double)));

}else if (state == 0 ){

    disconnect(cmdr_zscan, SIGNAL(moveRelative(double, double,double, double)),motionManager_, SLOT(moveRelative(double, double,double, double)));
    disconnect(lStepExpressModel_, SIGNAL(motionFinished()), camera_, SLOT(acquireImage()));
    disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)), finder_, SLOT(findMarker_templateMatching(int, cv::Mat)) );
    disconnect(finder_, SIGNAL(getImageBlur(cv::Mat, cv::Rect)), cmdr_zscan, SLOT(write_image(cv::Mat, cv::Rect)) );
    disconnect(cmdr_zscan,SIGNAL(make_graph(vector<double>,vector<double>)),autoFocusView_,SLOT(make_graph(vector<double>,vector<double>)));
    disconnect(cmdr_zscan,SIGNAL(updateText(double)),autoFocusView_,SLOT(updateText(double)));
    
}
}


void AssemblyMainWindow::onCloseCamera()
{
    NQLog("AssemblyMainWindow") << ":onCloseCamera()";

    emit closeCamera();
}

void AssemblyMainWindow::onSnapShot()
{
    emit acquireImage();
}



void AssemblyMainWindow::testTimer()
{
    NQLog("AssemblyMainWindow") << "timeOut = " << testTimerCount_;
    testTimerCount_ += 0.1;
}

void AssemblyMainWindow::cameraOpened()
{
    NQLog("AssemblyMainWindow") << ":cameraOpened()";

    finderView_->connectImageProducer(finder_, SIGNAL(markerFound(const cv::Mat&)));

    thresholdTunerView_ -> connectImageProducer(camera_, SIGNAL(imageAcquired(const cv::Mat&)));

    edgeView_->connectImageProducer(finder_, SIGNAL(edgesDetected(const cv::Mat&)));

    rawView_->connectImageProducer(camera_, SIGNAL(imageAcquired(const cv::Mat&)));
    
    //   bool test =   connect(camera_, SIGNAL(imageAcquired(cv::Mat)),  finder_, SLOT(write_image(cv::Mat)) );
    //connect(camera_, SIGNAL(imageAcquired(cv::Mat)),  finder_, SLOT(runObjectDetection_labmode(cv::Mat)) );
    //connect(camera_, SIGNAL(imageAcquired(cv::Mat)),  finder_, SLOT(locatePickup(cv::Mat)) );
    
    
   connect(finder_, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
   connect(finder_, SIGNAL(acquireImage()), camera_, SLOT(acquireImage()));
   connect(finder_, SIGNAL(locatePickupCorner_templateMatching(cv::Mat,cv::Mat)), finder_, SLOT(findMarker_templateMatching(cv::Mat,cv::Mat)));
    
    

    NQLog("AssemblyMainWindow") << " connecting finder and camera()";
    connect(camera_, SIGNAL(imageAcquired(const cv::Mat&)), finder_, SLOT(findMarker(const cv::Mat&)));


  //connect image acquired signal of camera model to blur detection SLOT of Z scanner
   connect(camera_, SIGNAL(imageAcquired(const cv::Mat&)),  finder_, SLOT(write_image(const cv::Mat&)));


    //    liveTimer_->start(2000);
}

void AssemblyMainWindow::cameraClosed()
{
    NQLog("AssemblyMainWindow") << ":cameraClosed()";

    finderView_->disconnectImageProducer(finder_, SIGNAL(markerFound(const cv::Mat&)));

    edgeView_->disconnectImageProducer(finder_, SIGNAL(edgesDetected(const cv::Mat&)));

    rawView_->disconnectImageProducer(camera_, SIGNAL(imagef(const cv::Mat&)));

    //    playView_->disconnectImageProducer(camera_, SIGNAL(imageAcquired(const cv::Mat&)));
    disconnect(camera_, SIGNAL(imageAcquired(const cv::Mat&)),
               finder_, SLOT(findMarker(const cv::Mat&)));

    liveTimer_->stop();
}

void AssemblyMainWindow::quit()
{
    NQLog("AssemblyMainWindow") << "quit";

    if (camera_) {
        emit closeCamera();
        camera_ = 0;
    }

    if (cameraThread_) {
        if (cameraThread_->wait(2000) == false)
            cameraThread_->terminate();
        cameraThread_->quit();
        //delete cameraThread_;
    }
}

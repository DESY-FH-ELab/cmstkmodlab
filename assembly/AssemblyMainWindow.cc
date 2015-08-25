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

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    snapShooter_ = new AssemblyUEyeSnapShooter(tabWidget_);
    tabWidget_->addTab(snapShooter_, "snapshot");

    uEyeModel_ = new AssemblyUEyeModel_t(10);
    cameraThread_ = new AssemblyUEyeCameraThread(uEyeModel_, this);
    cameraThread_->start();

    uEyeWidget_ = new AssemblyUEyeWidget(uEyeModel_, this);

    tabWidget_->addTab(uEyeWidget_, "uEye");

    connect(QApplication::instance(), SIGNAL(aboutToQuit()),
            this, SLOT(quit()));

    toolBar_ = addToolBar("Tools");
    toolBar_->addAction("open", this, SLOT(onOpenCamera()));
    toolBar_->addAction("close", this, SLOT(onCloseCamera()));
    toolBar_->addAction("snapshot", this, SLOT(onSnapShot()));
    toolBar_->addAction("save", snapShooter_, SLOT(snapShot()));

    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("AssemblyMainWindow") << "main window constructed";

    /*
    testTimerCount_ = 0;
    testTimer_ = new QTimer(this);
    connect(testTimer_, SIGNAL(timeout()), this, SLOT(testTimer()));
    testTimer_->start(100);
    */

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
    NQLog("AssemblyMainWindow") << ":onOpenCamera()";

    camera_ = uEyeModel_->getCameraByID(10);

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

    /*
    connect(camera_, SIGNAL(imageAcquired(const QImage&)),
            this, SLOT(imageAcquired(const QImage&)));
    */

    emit openCamera();
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

    snapShooter_->connectCamera(camera_);
    //liveTimer_->start(2000);
}

void AssemblyMainWindow::cameraClosed()
{
    NQLog("AssemblyMainWindow") << ":cameraClosed()";

    snapShooter_->disconnectCamera(camera_);
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

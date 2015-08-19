#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyMainWindow.h"

AssemblyMainWindow::AssemblyMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    uEyeModel_ = new AssemblyUEyeModel_t(10);
    cameraThread_ = new AssemblyUEyeCameraThread(uEyeModel_, this);
    cameraThread_->start();

    uEyeWidget_ = new AssemblyUEyeWidget(uEyeModel_, this);

    tabWidget_->addTab(uEyeWidget_, "uEye");

    connect(QApplication::instance(), SIGNAL(aboutToQuit()),
            this, SLOT(quit()));

    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("AssemblyMainWindow") << "main window constructed";

    testTimerCount_ = 0;
    testTimer_ = new QTimer(this);
    connect(testTimer_, SIGNAL(timeout()), this, SLOT(testTimer()));
    testTimer_->start(100);

    QTimer::singleShot(1, uEyeModel_, SLOT(updateInformation()));
    QTimer::singleShot(2000, this, SLOT(test()));
}

void AssemblyMainWindow::testTimer()
{
    NQLog("AssemblyMainWindow") << "timeOut = " << testTimerCount_;
    testTimerCount_ += 0.1;
}

void AssemblyMainWindow::test()
{
    camera_ = uEyeModel_->getCamera(0);

    connect(this, SIGNAL(initializeCamera()),
            camera_, SLOT(initialize()));
    connect(this, SIGNAL(acquireImage()),
            camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(cameraInitialized()),
            this, SLOT(cameraInitialized()));
    connect(camera_, SIGNAL(imageAcquired(const QImage*)),
            this, SLOT(imageAcquired(const QImage*)));

    emit initializeCamera();
}

void AssemblyMainWindow::cameraInitialized()
{
    NQLog("AssemblyMainWindow") << "cameraInitialized";

    emit acquireImage();
}

void AssemblyMainWindow::imageAcquired(const QImage* image)
{
    NQLog("AssemblyMainWindow") << "imageAcquired";

    image->save("test.jpg");
}

void AssemblyMainWindow::quit()
{
    NQLog("AssemblyMainWindow") << "quit";

    camera_->exit();

    if (cameraThread_) {
        if (cameraThread_->wait(2000) == false)
            cameraThread_->terminate();
        cameraThread_->quit();
        //delete cameraThread_;
    }
}

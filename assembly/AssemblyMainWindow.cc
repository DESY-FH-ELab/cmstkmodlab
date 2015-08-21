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
    QMainWindow(parent)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_ = new AssemblyUEyeView();
    imageView_->setMinimumSize(500, 500);
    imageView_->setPalette(palette);
    imageView_->setBackgroundRole(QPalette::Background);
    //imageView_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_->setScaledContents(true);
    imageView_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea(tabWidget_);
    scrollArea_->setPalette(palette);
    scrollArea_->setBackgroundRole(QPalette::Background);
    scrollArea_->setAlignment(Qt::AlignCenter);
    scrollArea_->setWidget(imageView_);

    tabWidget_->addTab(scrollArea_, "image");

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
void AssemblyMainWindow::onOpenCamera()
{
    NQLog("AssemblyMainWindow") << ":onOpenCamera()";

    camera_ = uEyeModel_->getCamera(0);

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
    connect(camera_, SIGNAL(imageAcquired(const QImage&)),
            this, SLOT(imageAcquired(const QImage&)));

    emit openCamera();
}

void AssemblyMainWindow::onCloseCamera()
{
    NQLog("AssemblyMainWindow") << ":onCloseCamera()";

    emit closeCamera();
}

void AssemblyMainWindow::onSnapShot()
{
    NQLog("AssemblyMainWindow") << ":onSnapShot()";

    emit acquireImage();
}

void AssemblyMainWindow::testTimer()
{
    NQLog("AssemblyMainWindow") << "timeOut = " << testTimerCount_;
    testTimerCount_ += 0.1;
}

void AssemblyMainWindow::test()
{
    camera_ = uEyeModel_->getCamera(0);

    connect(this, SIGNAL(openCamera()),
            camera_, SLOT(open()));
    connect(this, SIGNAL(acquireImage()),
            camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(cameraOpened()),
            this, SLOT(cameraOpened()));
    connect(camera_, SIGNAL(imageAcquired(const QImage*)),
            this, SLOT(imageAcquired(const QImage*)));

    emit openCamera();
}

void AssemblyMainWindow::cameraOpened()
{
    NQLog("AssemblyMainWindow") << "camera opened";

    emit acquireImage();
}

void AssemblyMainWindow::cameraClosed()
{
    NQLog("AssemblyMainWindow") << "camera closed";
}

void AssemblyMainWindow::imageAcquired(const QImage* image)
{
    NQLog("AssemblyMainWindow") << "image acquired";

    image->save("test.jpg");
}

void AssemblyMainWindow::quit()
{
    NQLog("AssemblyMainWindow") << "quit";

    if (cameraThread_) {
        if (cameraThread_->wait(2000) == false)
            cameraThread_->terminate();
        cameraThread_->quit();
        //delete cameraThread_;
    }
}

#include <unistd.h>

#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeFakeCamera.h"

AssemblyUEyeFakeCamera::AssemblyUEyeFakeCamera(QObject *parent)
    : AssemblyVUEyeCamera(parent)
{

}

AssemblyUEyeFakeCamera::~AssemblyUEyeFakeCamera()
{

}

void AssemblyUEyeFakeCamera::updateInformation()
{

}

void AssemblyUEyeFakeCamera::open()
{
    emit cameraOpened();
}

void AssemblyUEyeFakeCamera::close()
{
    usleep(1000);

    emit cameraClosed();
}

void AssemblyUEyeFakeCamera::acquireImage()
{
    usleep(300);

    emit imageAcquired(&image_);
}

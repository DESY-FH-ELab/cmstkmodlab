#include <unistd.h>

#include <iostream>

#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyUEyeFakeCamera.h"

AssemblyUEyeFakeCamera::AssemblyUEyeFakeCamera(QObject *parent)
    : AssemblyVUEyeCamera(parent)
{
    QString filename(Config::CMSTkModLabBasePath.c_str());
    image_ = QImage(filename +  + "/share/assembly/SensorMarker1.png");
}

AssemblyUEyeFakeCamera::~AssemblyUEyeFakeCamera()
{

}

void AssemblyUEyeFakeCamera::updateInformation()
{

}

void AssemblyUEyeFakeCamera::open()
{
    setID("IDS GmbH");
    setVersion("");
    setDate("17.04.2015");

    emit cameraInformationChanged();

    setSensorName("UI348xCP-M");
    setColorMode(1);
    setMaxWidth(2560);
    setMaxHeight(1920);
    setMasterGain(true);
    setRedGain(false);
    setGreenGain(false);
    setBlueGain(false);
    setGlobalShutter(false);
    setPixelSize(220);

    emit cameraInformationChanged();

    usleep(1000);

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

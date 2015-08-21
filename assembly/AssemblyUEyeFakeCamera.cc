#include <unistd.h>

#include <iostream>

#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyUEyeFakeCamera.h"

AssemblyUEyeFakeCamera::AssemblyUEyeFakeCamera(QObject *parent)
    : AssemblyVUEyeCamera(parent)
{
    cameraOpen_ = false;

    QString filename(Config::CMSTkModLabBasePath.c_str());
    image_ = QImage(filename +  + "/share/assembly/SensorMarker1.png");
}

AssemblyUEyeFakeCamera::~AssemblyUEyeFakeCamera()
{
    if (cameraOpen_) close();
}

void AssemblyUEyeFakeCamera::updateInformation()
{

}

void AssemblyUEyeFakeCamera::open()
{
    if (cameraOpen_) return;

    NQLog("AssemblyUEyeFakeCamera") << ":open()";

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

    cameraOpen_ = true;

    emit cameraOpened();
}

void AssemblyUEyeFakeCamera::close()
{
    if (!cameraOpen_) return;

    NQLog("AssemblyUEyeFakeCamera") << ":close()";

    usleep(500000);

    cameraOpen_ = false;

    emit cameraClosed();
}

void AssemblyUEyeFakeCamera::acquireImage()
{
    if (!cameraOpen_) return;

    NQLog("AssemblyUEyeFakeCamera") << ":acquireImage()";

    usleep(300000);

    emit imageAcquired(&image_);
}

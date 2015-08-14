#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeCamera.h"

AssemblyUEyeCamera::AssemblyUEyeCamera(QObject *parent)
    : AssemblyVUEyeCamera(parent)
{

}

AssemblyUEyeCamera::~AssemblyUEyeCamera()
{

}

bool AssemblyUEyeCamera::initialize()
{
    cameraHandle_ = getDeviceID();
    return (bool)is_InitCamera(&cameraHandle_, 0);
}

bool AssemblyUEyeCamera::exit()
{
    return (bool)is_ExitCamera(cameraHandle_);
}

void AssemblyUEyeCamera::updateInformation()
{
    CAMINFO* cameraInfo = new CAMINFO;
    is_GetCameraInfo(cameraHandle_, cameraInfo);

    delete cameraInfo;

    SENSORINFO* sensorInfo = new SENSORINFO;
    is_GetSensorInfo(cameraHandle_, sensorInfo);

    delete sensorInfo;
}

bool AssemblyUEyeCamera::isAvailable() const
{
    return (bool) IS_CAMERA_AVAILABLE(getStatus());
}

void AssemblyUEyeCamera::aquireImage()
{

}

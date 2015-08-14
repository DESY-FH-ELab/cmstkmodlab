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
    if (!is_GetCameraInfo(cameraHandle_, cameraInfo)) {

        setID(cameraInfo->ID);
        setVersion(cameraInfo->Version);
        setDate(cameraInfo->Date);

    } else {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "cannot read camera information";
    }

    delete cameraInfo;

    SENSORINFO* sensorInfo = new SENSORINFO;
    if (!is_GetSensorInfo(cameraHandle_, sensorInfo)) {

        setSensorName(sensorInfo->strSensorName);
        setColorMode(sensorInfo->nColorMode);
        setMaxWidth(sensorInfo->nMaxWidth);
        setMaxHeight(sensorInfo->nMaxHeight);
        setMasterGain(sensorInfo->bMasterGain);
        setRedGain(sensorInfo->bRGain);
        setGreenGain(sensorInfo->bGGain);
        setBlueGain(sensorInfo->bBGain);
        setGlobalShutter(sensorInfo->bGlobShutter);
        setPixelSize(sensorInfo->wPixelSize);

    } else {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "cannot read sensor information";
    }

    delete sensorInfo;
}

bool AssemblyUEyeCamera::isAvailable() const
{
    return (bool) IS_CAMERA_AVAILABLE(getStatus());
}

void AssemblyUEyeCamera::aquireImage()
{

}

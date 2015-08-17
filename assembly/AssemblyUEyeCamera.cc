#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeCamera.h"

AssemblyUEyeCamera::AssemblyUEyeCamera(QObject *parent)
    : AssemblyVUEyeCamera(parent)
{
    bufferProps_.width = 0;
    bufferProps_.height = 0;
    bufferProps_.bitspp = 8;
    bufferProps_.colorformat = IS_CM_MONO8;
    bufferProps_.imgformat = QImage::Format_Invalid;
    bufferProps_.pRgbTable = NULL;
    bufferProps_.tableentries = 0;

    for (int i = 0; i < 256; i++)
        table_[i] = qRgb (i, i, i);

    ZeroMemory(images_, sizeof(images_));
}

AssemblyUEyeCamera::~AssemblyUEyeCamera()
{

}

bool AssemblyUEyeCamera::initialize()
{
    cameraHandle_ = getDeviceID();
    unsigned int ret = is_InitCamera(&cameraHandle_, 0);
    NQLog("AssemblyUEyeCamera::initialize()", NQLog::Message) << "is_InitCamera " << ret;

    ret = is_EnableEvent(cameraHandle_, IS_SET_EVENT_FRAME);
    NQLog("AssemblyUEyeCamera::initialize()", NQLog::Message) << "is_EnableEvent " << ret;

    return (bool)ret;
}

bool AssemblyUEyeCamera::exit()
{
    unsigned int ret = is_ExitCamera(cameraHandle_);
    NQLog("AssemblyUEyeCamera::exit()", NQLog::Message) << "is_ExitCamera "  << ret;

    ret = is_DisableEvent(cameraHandle_, IS_SET_EVENT_FRAME);
    NQLog("AssemblyUEyeCamera::exit()", NQLog::Message) << "is_DisableEvent "  << ret;

    return (bool)ret;
}

bool AssemblyUEyeCamera::startLiveVideo()
{
    return (bool)is_CaptureVideo (cameraHandle_, IS_DONT_WAIT);
}

bool AssemblyUEyeCamera::stopLiveVideo()
{
    return (bool)is_StopLiveVideo(cameraHandle_, IS_WAIT);
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
    return (bool)IS_CAMERA_AVAILABLE(getStatus());
}

void AssemblyUEyeCamera::aquireImage()
{
    unsigned int ret = is_FreezeVideo(cameraHandle_, IS_DONT_WAIT);
    NQLog("AssemblyUEyeCamera::aquireImage()", NQLog::Message) << "is_FreezeVideo " << ret;
}

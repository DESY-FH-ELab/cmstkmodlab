#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeCamera.h"
#include "AssemblyUEyeModel.h"

AssemblyUEyeModel::AssemblyUEyeModel(int updateInterval,
                                     QObject *parent)
    : AssemblyVUEyeModel(updateInterval, parent),
      uEyeCameraList_(0)
{

}

AssemblyUEyeModel::~AssemblyUEyeModel()
{
    NQLog("AssemblyUEyeModel") << "delete";

    if (uEyeCameraList_) delete uEyeCameraList_;
}

void AssemblyUEyeModel::updateInformation()
{
    NQLog("AssemblyUEyeModel") << "updateInformation";

    UEYE_CAMERA_LIST* uEyeCameraList = new UEYE_CAMERA_LIST;

    uEyeCameraList->dwCount = 0;

    if (is_GetCameraList(uEyeCameraList) == IS_SUCCESS) {
        DWORD dw = uEyeCameraList->dwCount;

        if (uEyeCameraList_==0 || dw != uEyeCameraList_->dwCount) {

            NQLog("AssemblyUEyeModel") << dw << " camera(s) connected";

            delete uEyeCameraList_;

            uEyeCameraList_ = (UEYE_CAMERA_LIST*)new char[sizeof(DWORD) + dw * sizeof(UEYE_CAMERA_INFO)];
            uEyeCameraList_->dwCount = dw;

            if (is_GetCameraList(uEyeCameraList_) != IS_SUCCESS) {
                delete uEyeCameraList_;
                uEyeCameraList_ = new UEYE_CAMERA_LIST;
                uEyeCameraList_->dwCount = 0;
                dw = 0;
            }

            clear();

            for (unsigned int idx=0;idx<dw;idx++) {

                const UEYE_CAMERA_INFO* info = &(uEyeCameraList_->uci[idx]);

                QThread * thread = new QThread();
                threads_.push_back(thread);

                AssemblyUEyeCamera* camera = new AssemblyUEyeCamera(0);
                camera->moveToThread(thread);
                cameras_.push_back(camera);
                thread->start();

                // fill cameras

                camera->setCameraID(info->dwCameraID);
                camera->setDeviceID(info->dwDeviceID);
                camera->setSensorID(info->dwSensorID);
                camera->setSerialNumber(QString(info->SerNo).toLong());
                camera->setModelName(info->Model);
                camera->setFullModelName(info->FullModelName);
                camera->setStatus(info->dwStatus);

                NQLog("AssemblyUEyeModel") << "camera " << idx;
                NQLog("AssemblyUEyeModel") << "model name:      " << camera->getModelName();
                NQLog("AssemblyUEyeModel") << "full model name: " << camera->getFullModelName();
                NQLog("AssemblyUEyeModel") << "serial number:   " << camera->getSerialNumber();
                NQLog("AssemblyUEyeModel") << "camera ID:       " << camera->getCameraID();
                NQLog("AssemblyUEyeModel") << "device ID:       " << camera->getDeviceID();
                NQLog("AssemblyUEyeModel") << "sensor ID:       " << camera->getSensorID();
                NQLog("AssemblyUEyeModel") << "status:          " << camera->getStatus();
            }

            emit cameraCountChanged((unsigned int)dw);
        }
    }

    delete uEyeCameraList;

    for (QVector<AssemblyVUEyeCamera*>::iterator it = cameras_.begin();
         it != cameras_.end();
         ++it) {
        (*it)->updateInformation();
    }
}

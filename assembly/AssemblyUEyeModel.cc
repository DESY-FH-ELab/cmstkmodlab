#include <iostream>

#include <QApplication>

#include <nqlogger.h>

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

                AssemblyUEyeCamera_t* camera = new AssemblyUEyeCamera_t(this);
                cameras_.push_back(camera);
                // fill cameras

                camera->setCameraID(info->dwCameraID);
                camera->setDeviceID(info->dwDeviceID);
                camera->setSensorID(info->dwSensorID);
                camera->setSerialNumber(QString(info->SerNo).toLong());
                camera->setModelName(info->Model);
                camera->setFullModelName(info->FullModelName);
                camera->setStatus(info->dwStatus);

                camera->updateInformation();
            }

            emit cameraCountChanged((unsigned int)dw);
        }
    }

    delete uEyeCameraList;
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <AssemblyUEyeCamera.h>
#include <AssemblyUEyeModel.h>
#include <nqlogger.h>

AssemblyUEyeModel::AssemblyUEyeModel(int updateInterval, QObject* parent) :
  AssemblyVUEyeModel(updateInterval, parent),
  uEyeCameraList_(0)
{
    NQLog("AssemblyUEyeModel", NQLog::Debug) << "constructed";
}

AssemblyUEyeModel::~AssemblyUEyeModel()
{
    if(uEyeCameraList_){ delete uEyeCameraList_; }

    NQLog("AssemblyUEyeModel", NQLog::Debug) << "destructed";
}

void AssemblyUEyeModel::updateInformation()
{
    NQLog("AssemblyUEyeModel", NQLog::Debug) << "updateInformation";

    UEYE_CAMERA_LIST* uEyeCameraList = new UEYE_CAMERA_LIST;

    uEyeCameraList->dwCount = 0;

    if(is_GetCameraList(uEyeCameraList) == IS_SUCCESS){

        DWORD dw = uEyeCameraList->dwCount;

        if(uEyeCameraList_==0 || dw != uEyeCameraList_->dwCount){

            NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: " << dw << " camera(s) connected";

            delete uEyeCameraList_;

            uEyeCameraList_ = ((UEYE_CAMERA_LIST*) new char[sizeof(DWORD) + dw * sizeof(UEYE_CAMERA_INFO)]);
            uEyeCameraList_->dwCount = dw;

            if(is_GetCameraList(uEyeCameraList_) != IS_SUCCESS){

                delete uEyeCameraList_;

                uEyeCameraList_ = new UEYE_CAMERA_LIST;
                uEyeCameraList_->dwCount = 0;

                dw = 0;
            }

            clear();

            for(unsigned int idx=0; idx<dw; idx++){

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

                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: Parameters";
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: camera number" << idx;
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: model name      : " << camera->getModelName();
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: full model name : " << camera->getFullModelName();
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: serial number   : " << camera->getSerialNumber();
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: camera ID       : " << camera->getCameraID();
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: device ID       : " << camera->getDeviceID();
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: sensor ID       : " << camera->getSensorID();
                NQLog("AssemblyUEyeModel", NQLog::Message) << "updateInformation: status          : " << camera->getStatus();
            }

            NQLog("AssemblyUEyeModel", NQLog::Debug) << "updateInformation"
               << ": emitting signal \"cameraCountChanged(" << dw << ")\"";

            emit cameraCountChanged(((unsigned int) dw));
        }
    }

    delete uEyeCameraList;

    for(QVector<AssemblyVUEyeCamera*>::iterator it=cameras_.begin(); it!=cameras_.end(); ++it)
    {
        (*it)->updateInformation();
    }

    return;
}

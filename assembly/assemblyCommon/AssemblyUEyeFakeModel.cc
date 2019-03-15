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

#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeFakeCamera.h"
#include "AssemblyUEyeFakeModel.h"

AssemblyUEyeFakeModel::AssemblyUEyeFakeModel(int updateInterval, QObject *parent)
                     : AssemblyVUEyeModel(updateInterval, parent)
{
    NQLog("AssemblyUEyeFakeModel", NQLog::Debug) << "constructed";

    infoUpdated_ = false;
}

AssemblyUEyeFakeModel::~AssemblyUEyeFakeModel()
{
    NQLog("AssemblyUEyeFakeModel", NQLog::Debug) << "destructed";
}

void AssemblyUEyeFakeModel::updateInformation()
{
    if(infoUpdated_){ return; }

    clear();

    const unsigned int camera_N = 2;

    // fill cameras
    for(unsigned int i_cam=0; i_cam<camera_N; ++i_cam)
    {
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: adding new camera";

        QThread * thread = new QThread();
        threads_.push_back(thread);

        AssemblyUEyeFakeCamera* camera = new AssemblyUEyeFakeCamera(0);
        camera->moveToThread(thread);
        cameras_.push_back(camera);

        thread->start();

        camera->setCameraID((1+i_cam) * 10);
        camera->setDeviceID((1+i_cam));
        camera->setSensorID(11);
        camera->setSerialNumber(4102844803);
        camera->setModelName    ("UI348xCP-M");
        camera->setFullModelName("UI348xCP-M");
        camera->setStatus(0);

        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: camera number   : " << i_cam;
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: model name      : " << camera->getModelName();
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: full model name : " << camera->getFullModelName();
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: serial number   : " << camera->getSerialNumber();
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: camera ID       : " << camera->getCameraID();
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: device ID       : " << camera->getDeviceID();
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: sensor ID       : " << camera->getSensorID();
        NQLog("AssemblyUEyeFakeModel", NQLog::Message) << "updateInformation: status          : " << camera->getStatus();

        camera->updateInformation();
    }

    infoUpdated_ = true;

    NQLog("AssemblyUEyeFakeModel", NQLog::Debug) << "updateInformation"
       << ": emitting signal \"cameraCountChanged(" << cameras_.size() << ")\"";

    emit cameraCountChanged(cameras_.size());

    return;
}

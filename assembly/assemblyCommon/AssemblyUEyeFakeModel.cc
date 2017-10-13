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
    NQLog("AssemblyUEyeFakeModel") << "constructed";

    infoUpdated_ = false;
}

AssemblyUEyeFakeModel::~AssemblyUEyeFakeModel()
{
    NQLog("AssemblyUEyeFakeModel") << "delete";
}

void AssemblyUEyeFakeModel::updateInformation()
{
    if(infoUpdated_){ return; }

    clear();

    const unsigned int camera_N = 2;

    // fill cameras
    for(unsigned int i_cam=0; i_cam<camera_N; ++i_cam)
    {
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "adding new camera";

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

        NQLog("AssemblyUEyeFakeModel::updateInformation") << "camera number   : " << i_cam;
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "model name      : " << camera->getModelName();
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "full model name : " << camera->getFullModelName();
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "serial number   : " << camera->getSerialNumber();
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "camera ID       : " << camera->getCameraID();
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "device ID       : " << camera->getDeviceID();
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "sensor ID       : " << camera->getSensorID();
        NQLog("AssemblyUEyeFakeModel::updateInformation") << "status          : " << camera->getStatus();

        camera->updateInformation();
    }

    emit cameraCountChanged((unsigned int) cameras_.size());

    infoUpdated_ = true;

    return;
}

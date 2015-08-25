#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeFakeCamera.h"
#include "AssemblyUEyeFakeModel.h"

AssemblyUEyeFakeModel::AssemblyUEyeFakeModel(int updateInterval, QObject *parent)
    : AssemblyVUEyeModel(updateInterval, parent)
{
    NQLog("AssemblyUEyeFakeModel") << "constructed";

}

AssemblyUEyeFakeModel::~AssemblyUEyeFakeModel()
{
    NQLog("AssemblyUEyeFakeModel") << "delete";
}

void AssemblyUEyeFakeModel::updateInformation()
{
    NQLog("AssemblyUEyeFakeModel") << "updateInformation";

    static bool infoUpdated = false;
    if (infoUpdated) return;

    clear();

    // fill cameras

    {
        QThread * thread = new QThread();
        threads_.push_back(thread);

        AssemblyUEyeFakeCamera *camera = new AssemblyUEyeFakeCamera(0);
        camera->moveToThread(thread);
        cameras_.push_back(camera);
        thread->start();

        camera->setCameraID(10);
        camera->setDeviceID(1);
        camera->setSensorID(11);
        camera->setSerialNumber(4102844803);
        camera->setModelName("UI348xCP-M");
        camera->setFullModelName("UI348xCP-M");
        camera->setStatus(0);

        NQLog("AssemblyUEyeFakeModel") << "camera 0";
        NQLog("AssemblyUEyeFakeModel") << "model name:      " << camera->getModelName();
        NQLog("AssemblyUEyeFakeModel") << "full model name: " << camera->getFullModelName();
        NQLog("AssemblyUEyeFakeModel") << "serial number:   " << camera->getSerialNumber();
        NQLog("AssemblyUEyeFakeModel") << "camera ID:       " << camera->getCameraID();
        NQLog("AssemblyUEyeFakeModel") << "device ID:       " << camera->getDeviceID();
        NQLog("AssemblyUEyeFakeModel") << "sensor ID:       " << camera->getSensorID();
        NQLog("AssemblyUEyeFakeModel") << "status:          " << camera->getStatus();

        camera->updateInformation();
    }

    {
        QThread * thread = new QThread();
        threads_.push_back(thread);

        AssemblyUEyeFakeCamera* camera = new AssemblyUEyeFakeCamera(0);
        camera->moveToThread(thread);
        cameras_.push_back(camera);
        thread->start();

        camera->setCameraID(20);
        camera->setDeviceID(2);
        camera->setSensorID(11);
        camera->setSerialNumber(4102844803);
        camera->setModelName("UI348xCP-M");
        camera->setFullModelName("UI348xCP-M");
        camera->setStatus(0);

        NQLog("AssemblyUEyeFakeModel") << "camera 0";
        NQLog("AssemblyUEyeFakeModel") << "model name:      " << camera->getModelName();
        NQLog("AssemblyUEyeFakeModel") << "full model name: " << camera->getFullModelName();
        NQLog("AssemblyUEyeFakeModel") << "serial number:   " << camera->getSerialNumber();
        NQLog("AssemblyUEyeFakeModel") << "camera ID:       " << camera->getCameraID();
        NQLog("AssemblyUEyeFakeModel") << "device ID:       " << camera->getDeviceID();
        NQLog("AssemblyUEyeFakeModel") << "sensor ID:       " << camera->getSensorID();
        NQLog("AssemblyUEyeFakeModel") << "status:          " << camera->getStatus();

        camera->updateInformation();
    }

    emit cameraCountChanged((unsigned int)cameras_.size());

    infoUpdated = true;
}

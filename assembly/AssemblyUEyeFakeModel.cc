#include <iostream>

#include <QApplication>

#include <nqlogger.h>

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

        AssemblyUEyeCamera_t* camera = new AssemblyUEyeCamera_t(0);
        camera->moveToThread(thread);
        cameras_.push_back(camera);
        thread->start();

        camera->setCameraID(1);
        camera->setDeviceID(1);
        camera->setSensorID(11);
        camera->setSerialNumber(4102844803);
        camera->setModelName("UI348xCP-M");
        camera->setFullModelName("UI348xCP-M");
        camera->setStatus(0);

        NQLog("AssemblyUEyeModel") << "camera 0";
        NQLog("AssemblyUEyeModel") << "model name:      " << camera->getModelName();
        NQLog("AssemblyUEyeModel") << "full model name: " << camera->getFullModelName();
        NQLog("AssemblyUEyeModel") << "serial number:   " << camera->getSerialNumber();
        NQLog("AssemblyUEyeModel") << "camera ID:       " << camera->getCameraID();
        NQLog("AssemblyUEyeModel") << "device ID:       " << camera->getDeviceID();
        NQLog("AssemblyUEyeModel") << "sensor ID:       " << camera->getSensorID();
        NQLog("AssemblyUEyeModel") << "status:          " << camera->getStatus();
    }

    {
        QThread * thread = new QThread();
        threads_.push_back(thread);

        AssemblyUEyeCamera_t* camera = new AssemblyUEyeCamera_t(0);
        camera->moveToThread(thread);
        cameras_.push_back(camera);
        thread->start();

        camera->setCameraID(2);
        camera->setDeviceID(2);
        camera->setSensorID(11);
        camera->setSerialNumber(4102844803);
        camera->setModelName("UI348xCP-M");
        camera->setFullModelName("UI348xCP-M");
        camera->setStatus(0);

        NQLog("AssemblyUEyeModel") << "camera 1";
        NQLog("AssemblyUEyeModel") << "model name:      " << camera->getModelName();
        NQLog("AssemblyUEyeModel") << "full model name: " << camera->getFullModelName();
        NQLog("AssemblyUEyeModel") << "serial number:   " << camera->getSerialNumber();
        NQLog("AssemblyUEyeModel") << "camera ID:       " << camera->getCameraID();
        NQLog("AssemblyUEyeModel") << "device ID:       " << camera->getDeviceID();
        NQLog("AssemblyUEyeModel") << "sensor ID:       " << camera->getSensorID();
        NQLog("AssemblyUEyeModel") << "status:          " << camera->getStatus();
    }

    emit cameraCountChanged((unsigned int)cameras_.size());

    infoUpdated = true;
}

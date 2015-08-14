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

    AssemblyUEyeCamera_t* camera = new AssemblyUEyeCamera_t(0);
    cameras_.push_back(camera);

    // fill cameras

    camera->setCameraID(1);
    camera->setDeviceID(1);
    camera->setSensorID(11);
    camera->setSerialNumber(4102844803);
    camera->setModelName("UI348xCP-M");
    camera->setFullModelName("UI348xCP-M");
    camera->setStatus(0);

    camera->setID("UI348xCP-M");
    camera->setVersion("");
    camera->setDate("13.04.2015");

    camera->setSensorName("UI348xCP-M");
    camera->setColorMode(1);
    camera->setMaxWidth(2560);
    camera->setMaxHeight(1920);
    camera->setMasterGain(true);
    camera->setRedGain(false);
    camera->setGreenGain(false);
    camera->setBlueGain(false);
    camera->setGlobalShutter(false);
    camera->setPixelSize(220);

    NQLog("AssemblyUEyeModel") << "camera 0";
    NQLog("AssemblyUEyeModel") << "model name:      " << camera->getModelName();
    NQLog("AssemblyUEyeModel") << "full model name: " << camera->getFullModelName();
    NQLog("AssemblyUEyeModel") << "serial number:   " << camera->getSerialNumber();
    NQLog("AssemblyUEyeModel") << "camera ID:       " << camera->getCameraID();
    NQLog("AssemblyUEyeModel") << "device ID:       " << camera->getDeviceID();
    NQLog("AssemblyUEyeModel") << "sensor ID:       " << camera->getSensorID();
    NQLog("AssemblyUEyeModel") << "status:          " << camera->getStatus();

    camera->initialize();
    camera->updateInformation();
    camera->exit();

    NQLog("AssemblyUEyeModel") << "id:              " << camera->getID();
    NQLog("AssemblyUEyeModel") << "version:         " << camera->getVersion();
    NQLog("AssemblyUEyeModel") << "date:            " << camera->getDate();

    NQLog("AssemblyUEyeModel") << "sensor name:     " << camera->getSensorName();
    NQLog("AssemblyUEyeModel") << "color mode:      " << camera->getColorMode();
    NQLog("AssemblyUEyeModel") << "max. resolution: " << camera->getMaxWidth() << " x "
                                                      << camera->getMaxHeight();
    NQLog("AssemblyUEyeModel") << "master gain:     " << camera->getMasterGain();
    NQLog("AssemblyUEyeModel") << "RGB gain:        " << camera->getRedGain() << " "
                                                      << camera->getGreenGain() << " "
                                                      << camera->getBlueGain();
    NQLog("AssemblyUEyeModel") << "global shutter:  " << camera->getGlobalShutter();
    NQLog("AssemblyUEyeModel") << "pixel size:      " << camera->getPixelSize();

    infoUpdated = true;
}

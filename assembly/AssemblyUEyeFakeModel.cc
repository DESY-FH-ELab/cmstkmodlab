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
}

#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeFakeCamera.h"

AssemblyUEyeFakeCamera::AssemblyUEyeFakeCamera(QObject *parent)
    : AssemblyVUEyeCamera(parent)
{

}

AssemblyUEyeFakeCamera::~AssemblyUEyeFakeCamera()
{

}

void AssemblyUEyeFakeCamera::updateInformation()
{

}

bool AssemblyUEyeFakeCamera::initialize()
{
    return true;
}

bool AssemblyUEyeFakeCamera::exit()
{
    return true;
}

void AssemblyUEyeFakeCamera::aquireImage()
{

}

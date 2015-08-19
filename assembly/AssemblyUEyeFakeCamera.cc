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

void AssemblyUEyeFakeCamera::open()
{
    return true;
}

void AssemblyUEyeFakeCamera::close()
{
    return true;
}

void AssemblyUEyeFakeCamera::aquireImage()
{

}

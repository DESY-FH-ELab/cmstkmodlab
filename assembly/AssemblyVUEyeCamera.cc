#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVUEyeCamera.h"

AssemblyVUEyeCamera::AssemblyVUEyeCamera(QObject *parent)
    : QObject(parent),
      cameraOpen_(false)
{

}

AssemblyVUEyeCamera::~AssemblyVUEyeCamera()
{

}

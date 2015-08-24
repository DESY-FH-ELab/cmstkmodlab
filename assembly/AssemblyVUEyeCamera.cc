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

size_t AssemblyVUEyeCamera::getCurrentPixelClockIndex() const
{
    size_t idx = 0;
    for (std::vector<unsigned int>::const_iterator it = pixelClocks_.begin();
         it != pixelClocks_.end();
         ++it) {
        if (*it == currentPixelClock_) {
            return idx;
        }
        idx++;
    }
    return idx;
}

#include <iostream>

#include <QApplication>
#include <QString>

#include <QImage>
#include <QLabel>
#include <QPainter>

#include <nqlogger.h>

#include "AssemblyVUEyeCamera.h"
#include <iostream>
#include <cmath>

#include <unistd.h> 



using namespace std;

AssemblyVUEyeCamera::AssemblyVUEyeCamera(QObject *parent)
    : QObject(parent),
      cameraState_(State::OFF)
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








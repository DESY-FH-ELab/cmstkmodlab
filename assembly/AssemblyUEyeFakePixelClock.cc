#include <unistd.h>

#include <iostream>

#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyUEyeFakePixelClock.h"

AssemblyUEyeFakePixelClock::AssemblyUEyeFakePixelClock(QObject *parent)
    : AssemblyVUEyePixelClock(parent)
{
    for (unsigned int f=12;f<54;f+=2) {
        pixelClocks_.push_back(f);
    }
}

AssemblyUEyeFakePixelClock::~AssemblyUEyeFakePixelClock()
{

}

void AssemblyUEyeFakePixelClock::updatePixelClocks(unsigned int /* handle */, bool /* emitOnChange */)
{

}

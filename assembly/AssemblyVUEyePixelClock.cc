#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVUEyePixelClock.h"

AssemblyVUEyePixelClock::AssemblyVUEyePixelClock(QObject *parent)
    : QObject(parent),
      currentPixelClockIndex_(0)
{
    pixelClocks_.push_back(0);
}

AssemblyVUEyePixelClock::~AssemblyVUEyePixelClock()
{

}

unsigned int AssemblyVUEyePixelClock::getCurrentPixelClock() const
{
    if (currentPixelClockIndex_>=getNumberOfPixelClocks())
        return pixelClocks_.back();

    return pixelClocks_[currentPixelClockIndex_];
}

size_t AssemblyVUEyePixelClock::getCurrentPixelClockIndex(unsigned int pixelClock) const
{
    size_t idx = 0;
    for (std::vector<unsigned int>::const_iterator it = pixelClocks_.begin();
         it != pixelClocks_.end();
         ++it) {
        if (*it == pixelClock) {
            return idx;
        }
        idx++;
    }
    return idx;
}

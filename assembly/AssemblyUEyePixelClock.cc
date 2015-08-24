#include <unistd.h>

#include <iostream>

#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblyUEyePixelClock.h"

AssemblyUEyePixelClock::AssemblyUEyeFakePixelClock(QObject *parent)
    : AssemblyVUEyePixelClock(parent)
{

}

AssemblyUEyePixelClock::~AssemblyUEyeFakePixelClock()
{

}

void AssemblyUEyePixelClock::updatePixelClocks(unsigned int handle, bool emitOnChange)
{
    UINT nRange[3];

    ZeroMemory(nRange, sizeof(nRange));

    UINT nMin = 0;
    UINT nMax = 0;
    UINT nInc = 0;

    INT nRet = is_PixelClock(handle, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));

    if (nRet == IS_SUCCESS) {
      nMin = nRange[0];
      nMax = nRange[1];
      nInc = nRange[2];
    }

    if (nMin!=0) {

        bool listChanged = false;
        bool valueChanged = false;

        std::vector<unsigned int> newPixelClocks;
        unsigned int newPixelClock;

        for (UINT f=nMin;f<=nMax;f+=nInc) newPixelClocks.push_back(f);

        if (newPixelClocks!=pixelClocks_) listChanged = true;

        newPixelClock = readPixelClock(handle);

        if (newPixelClock!=getCurrentPixelClock()) valueChanged = true;

        if (listChanged) {
            pixelClocks_ = newPixelClocks;
            currentPixelClockIndex_ = getCurrentPixelClockIndex(newPixelClock);

            emit pixelClockListChanged();
        }
        if (valueChanged) {
            currentPixelClockIndex_ = getCurrentPixelClockIndex(newPixelClock);

            emit pixelClockChanged(handle);
        }
    } else {

        UINT nNumberOfSupportedPixelClocks = 0;

        INT nRet = is_PixelClock(handle, IS_PIXELCLOCK_CMD_GET_NUMBER,
                                 (void*)&nNumberOfSupportedPixelClocks,
                                 sizeof(nNumberOfSupportedPixelClocks));

        if ((nRet == IS_SUCCESS) && (nNumberOfSupportedPixelClocks > 0)) {

            // No camera has more than 150 different pixel clocks.
            // Of course, the list can be allocated dynamically

            UINT nPixelClockList[150];

            ZeroMemory(&nPixelClockList, sizeof(nPixelClockList));

            nRet = is_PixelClock(handle, IS_PIXELCLOCK_CMD_GET_LIST,
                                 (void*)nPixelClockList,
                                 nNumberOfSupportedPixelClocks * sizeof(UINT));

            if ((nRet == IS_SUCCESS)) {

                bool listChanged = false;
                bool valueChanged = false;

                std::vector<unsigned int> newPixelClocks;
                unsigned int newPixelClock;

                for (unsigned int idx=0;idx<nNumberOfSupportedPixelClocks;idx++)
                    newPixelClocks.push_back(nPixelClockList[idx]);

                if (newPixelClocks!=pixelClocks_) listChanged = true;

                newPixelClock = readPixelClock(handle);

                if (newPixelClock!=getCurrentPixelClock()) valueChanged = true;

                if (listChanged) {
                    pixelClocks_ = newPixelClocks;
                    currentPixelClockIndex_ = getCurrentPixelClockIndex(newPixelClock);

                    emit pixelClockListChanged(handle);
                }
                if (valueChanged) {
                    currentPixelClockIndex_ = getCurrentPixelClockIndex(newPixelClock);

                    emit pixelClockChanged(handle);
                }

            } else {
                pixelClocks_.clear();
                pixelClocks_.push_back(0);
                currentPixelClockIndex_ = 0;

                emit pixelClockListChanged(handle);
                emit pixelClockChanged(handle);
            }
        }
    }
}

unsigned int AssemblyUEyePixelClock::readPixelClock(unsigned int handle)
{
    UINT nPixelClock;

    // Get current pixel clock
    nRet = is_PixelClock(handle, IS_PIXELCLOCK_CMD_GET, (void*)&nPixelClock, sizeof(nPixelClock));

    return nPixelClock;
}

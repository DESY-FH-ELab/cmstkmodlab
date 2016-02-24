#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVUEyeCamera.h"
#include <iostream>

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


void AssemblyVUEyeCamera::calibrateSettings()
{
      NQLog("AssemblyVUEyeCamera") << ":calibrateSettings()";
	NQLog("AssemblyVUEyeCamera") << "**********************exposure time scan***************************" ;
      for (int i = 1; i<6;i++){
	NQLog("AssemblyVUEyeCamera") << " :exposure time scan, Step  = "<< i ;
	setExposureTime(i*50.0);
	NQLog("AssemblyVUEyeCamera") << " :exposure time set, sleeping";
	sleep(10);
	NQLog("AssemblyVUEyeCamera") << " awake, acquiring image";
	acquireImage();
	NQLog("AssemblyVUEyeCamera") << " image acquired, back to shleep ";
	sleep(10);
	NQLog("AssemblyVUEyeCamera") << " awake, recycling ";
      }
     

}

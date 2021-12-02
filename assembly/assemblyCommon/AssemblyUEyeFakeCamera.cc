/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <AssemblyUEyeFakeCamera.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>

#include <unistd.h>

AssemblyUEyeFakeCamera::AssemblyUEyeFakeCamera(QObject* parent) :
  AssemblyVUEyeCamera(parent),
  imageIndex_(0)
{
    cameraState_ = State::OFF;

    QString filename(Config::CMSTkModLabBasePath.c_str());

    std::vector<std::string> filenames;

    filenames.clear();
    filenames.push_back(QString(filename + "/share/assembly/oldSpareSensor_master.png").toStdString());
    imageFilenamesForPixelClock_[5] = filenames;

    filenames.clear();
    filenames.push_back(QString(filename + "/share/assembly/oldSpareSensor_master.png").toStdString());
    filenames.push_back(QString(filename + "/share/assembly/markedglass_marker1_master.png").toStdString());
    filenames.push_back(QString(filename + "/share/assembly/markedglass_marker2_master.png").toStdString());
    filenames.push_back(QString(filename + "/share/assembly/SiDummyPSp_master_200218.png").toStdString());
    filenames.push_back(QString(filename + "/share/assembly/SiDummyPSp_master_200120.png").toStdString());
    imageFilenamesForPixelClock_[24] = filenames;

    filenames.clear();
    filenames.push_back(QString(filename + "/share/assembly/oldSpareSensor_master.png").toStdString());
    imageFilenamesForPixelClock_[36] = filenames;

    filenames.clear();
    filenames.push_back(QString(filename + "/share/assembly/oldSpareSensor_master.png").toStdString());
    imageFilenamesForPixelClock_[43] = filenames;

    filenames.clear();
    filenames.push_back(QString(filename + "/share/assembly/oldSpareSensor_master.png").toStdString());
    imageFilenamesForPixelClock_[4] = filenames;

    imageFilenames_ = imageFilenamesForPixelClock_[24];
}

AssemblyUEyeFakeCamera::~AssemblyUEyeFakeCamera()
{
    if (cameraState_ == READY){ close(); }
}

void AssemblyUEyeFakeCamera::open()
{
    if(cameraState_==State::READY || cameraState_==State::INITIALIZING){ return; }

    cameraState_ = State::INITIALIZING;

    NQLog("AssemblyUEyeFakeCamera", NQLog::Debug) << "open";

    setID("IDS GmbH");
    setVersion("");
    setDate("17.04.2015");

    emit cameraInformationChanged();

    setSensorName("UI348xCP-M");
    setColorMode(1);
    setMaxWidth(2560);
    setMaxHeight(1920);
    setMasterGain(true);
    setRedGain(false);
    setGreenGain(false);
    setBlueGain(false);
    setGlobalShutter(false);
    setPixelSize(220);

    emit cameraInformationChanged();

    currentExposureTime_ = 0;

    updatePixelClock();
    updateExposureTime();

    usleep(500000);

    cameraState_ = State::READY;

    emit cameraOpened();
}

void AssemblyUEyeFakeCamera::close()
{
    NQLog("AssemblyUEyeFakeCamera", NQLog::Debug) << "close";

    if(cameraState_ != State::READY){ return; }

    cameraState_ = State::CLOSING;

    usleep(500000);

    cameraState_ = State::OFF;

    NQLog("AssemblyUEyeFakeCamera", NQLog::Debug) << "close"
       << ": emitting signal \"cameraClosed\"";

    emit cameraClosed();
}

void AssemblyUEyeFakeCamera::updateInformation()
{
    if (cameraState_!=State::READY) return;
}

void AssemblyUEyeFakeCamera::updatePixelClock()
{
    if (cameraState_!=State::READY && cameraState_!=State::INITIALIZING) return;

    std::vector<unsigned int> pixelClocks;
    for (unsigned int f=5;f<=43;f+=1) {
        pixelClocks.push_back(f);
    }

    bool listChanged = false;
    bool valueChanged = false;

    if (pixelClocks!=pixelClocks_) {
        pixelClocks_ = pixelClocks;
        currentPixelClock_ = 24;
        listChanged = true;
        valueChanged = true;
    }

    if (listChanged) emit pixelClockListChanged(currentPixelClock_);
    if (valueChanged) {
        emit pixelClockChanged(currentPixelClock_);
        updateExposureTime();
    }
}

void AssemblyUEyeFakeCamera::updateExposureTime()
{
    if (cameraState_!=State::READY && cameraState_!=State::INITIALIZING) return;

    double nMin = 0.1;
    double nMax = 100.0 + currentPixelClock_ * 222.0/19.;
    double nInc = 0.1;

    bool listChanged = false;
    bool valueChanged = false;

    if (nMin != exposureTimeMin_ ||
        nMax != exposureTimeMax_ ||
        nInc != exposureTimeInc_) {

        exposureTimeMin_ = nMin;
        exposureTimeMax_ = nMax;
        exposureTimeInc_ = nInc;

        listChanged = true;
    }

    if (currentExposureTime_==0.) {
        currentExposureTime_ = exposureTimeMax_;
        valueChanged = true;
    }
    if (currentExposureTime_<exposureTimeMin_) {
        currentExposureTime_ = exposureTimeMin_;
        valueChanged = true;
    }
    if (currentExposureTime_>exposureTimeMax_) {
        currentExposureTime_ = exposureTimeMax_;
        valueChanged = true;
    }

    if (listChanged) emit exposureTimeRangeChanged(currentExposureTime_);
    if (valueChanged) emit exposureTimeChanged(currentExposureTime_);
}

void AssemblyUEyeFakeCamera::setPixelClock(unsigned int pc)
{
    if (currentPixelClock_!=pc) {
        currentPixelClock_ = pc;

        if (currentPixelClock_==5) {
            imageFilenames_ = imageFilenamesForPixelClock_[5];
        } else if (currentPixelClock_>36) {
            imageFilenames_ = imageFilenamesForPixelClock_[43];
        } else if (currentPixelClock_>24) {
            imageFilenames_ = imageFilenamesForPixelClock_[36];
        } else if (currentPixelClock_>5) {
            imageFilenames_ = imageFilenamesForPixelClock_[24];
        } else {
            imageFilenames_ = imageFilenamesForPixelClock_[4];
        }

        imageIndex_ = 0;

        emit pixelClockChanged(currentPixelClock_);
        updateExposureTime();
    }
}

void AssemblyUEyeFakeCamera::setExposureTime(double et)
{
    if (currentExposureTime_!=et) {
        currentExposureTime_ = et;
        emit exposureTimeChanged(currentExposureTime_);
    }
}

void AssemblyUEyeFakeCamera::acquireImage()
{
    if(cameraState_ != State::READY){ return; }

    image_ = cv::imread(imageFilenames_[imageIndex_++], cv::IMREAD_GRAYSCALE);

    NQLog("AssemblyUEyeFakeCamera", NQLog::Debug) << "acquireImage"
       << ": emitting signal \"imageAcquired\"";

    emit imageAcquired(image_);

    if(imageIndex_ == imageFilenames_.size()){ imageIndex_ = 0; }
}

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

#include <AssemblyUEyeCamera.h>
#include <nqlogger.h>

AssemblyUEyeCameraEventThread::AssemblyUEyeCameraEventThread() :
  cameraHandle_(0),
  runEventThread_(false)
{
}

AssemblyUEyeCameraEventThread::~AssemblyUEyeCameraEventThread()
{
}

int AssemblyUEyeCameraEventThread::start(HIDS cameraHandle)
{
    int ret = 0;

    cameraHandle_ = cameraHandle;

    ret = is_EnableEvent(cameraHandle_, IS_SET_EVENT_FRAME);
    if(ret == 0)
    {
        NQLog("AssemblyUEyeCameraEventThread", NQLog::Debug) << "start: is_EnableEvent=" << ret;

        runEventThread_ = true;
        QThread::start ();
    }

    return ret;
}

void AssemblyUEyeCameraEventThread::stop ()
{
    runEventThread_ = false;
}

void AssemblyUEyeCameraEventThread::run ()
{
    while (runEventThread_) {
        if (is_WaitEvent (cameraHandle_, IS_SET_EVENT_FRAME, 1000) == IS_SUCCESS) {

            NQLog("AssemblyUEyeCamera", NQLog::Debug) << "run"
               << ": emitting signal \"eventHappened\"";

            emit eventHappened();
        }
    }

    runEventThread_ = false;
    is_DisableEvent (cameraHandle_, IS_SET_EVENT_FRAME);
}

AssemblyUEyeCamera::AssemblyUEyeCamera(QObject *parent) : AssemblyVUEyeCamera(parent)
{
    bufferProps_.width = 0;
    bufferProps_.height = 0;
    bufferProps_.bitspp = 8;
    bufferProps_.colorformat = IS_CM_MONO8;
    //bufferProps_.imgformat = QImage::Format_Invalid;
    //bufferProps_.pRgbTable = nullptr;
    //bufferProps_.tableentries = 0;

    //for (int i = 0; i < 256; i++)
    //    table_[i] = qRgb (i, i, i);

    ZeroMemory(images_, sizeof(images_));

    pixelClocks_.clear();
    currentPixelClock_ = 0;
}

AssemblyUEyeCamera::~AssemblyUEyeCamera()
{
    if (cameraState_==State::READY) close();
}

void AssemblyUEyeCamera::open()
{
    if (cameraState_==State::READY || cameraState_==State::INITIALIZING){ return; }

    cameraState_ = State::INITIALIZING;

    cameraHandle_ = getCameraID();
    unsigned int ret = is_InitCamera(&cameraHandle_, 0);

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "open: is_InitCamera=" << ret;

    CAMINFO* cameraInfo = new CAMINFO;
    if(!is_GetCameraInfo(cameraHandle_, cameraInfo))
    {
        setID(cameraInfo->ID);
        setVersion(cameraInfo->Version);
        setDate(cameraInfo->Date);

        NQLog("AssemblyUEyeCamera", NQLog::Debug) << "open"
           << ": emitting signal \"cameraInformationChanged\"";

        emit cameraInformationChanged();
    }
    else
    {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "open: cannot read camera information";
    }

    delete cameraInfo;

    SENSORINFO* sensorInfo = new SENSORINFO;
    if(!is_GetSensorInfo(cameraHandle_, sensorInfo))
    {
        setSensorName(sensorInfo->strSensorName);
        setColorMode(sensorInfo->nColorMode);
        setMaxWidth(sensorInfo->nMaxWidth);
        setMaxHeight(sensorInfo->nMaxHeight);
        setMasterGain(sensorInfo->bMasterGain); 
        setRedGain(sensorInfo->bRGain);
        setGreenGain(sensorInfo->bGGain);
        setBlueGain(sensorInfo->bBGain);
        setGlobalShutter(sensorInfo->bGlobShutter);
        setPixelSize(sensorInfo->wPixelSize);

        NQLog("AssemblyUEyeCamera", NQLog::Debug) << "open: width  = " << sensorInfo->nMaxWidth;
        NQLog("AssemblyUEyeCamera", NQLog::Debug) << "open: height = " << sensorInfo->nMaxHeight;

        int colormode = 0;
        if (sensorInfo->nColorMode >= IS_COLORMODE_BAYER)
        {
            colormode = IS_CM_BGRA8_PACKED;
        }
        else
        {
            colormode = IS_CM_MONO8;
        }

        if(is_SetColorMode(cameraHandle_, colormode) != IS_SUCCESS)
        {
            NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "open: cannot set color mode";
        }

        NQLog("AssemblyUEyeCamera", NQLog::Debug) << "open"
           << ": emitting signal \"cameraInformationChanged\"";

        emit cameraInformationChanged();
    }
    else
    {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "open"
           << ": cannot read sensor information";
    }

    delete sensorInfo;

    unsigned int uInitialParameterSet = IS_CONFIG_INITIAL_PARAMETERSET_NONE;

    if(is_Configuration(IS_CONFIG_INITIAL_PARAMETERSET_CMD_GET, &uInitialParameterSet, sizeof(unsigned int)))
    {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "open: cannot read sensor initial parameter set";
    }

    if (uInitialParameterSet == IS_CONFIG_INITIAL_PARAMETERSET_NONE) {
        ret = is_ResetToDefault(cameraHandle_);
    }

    ZeroMemory(&cameraProps_, sizeof(cameraProps_));

    // If the camera does not support a continuous AOI -> it uses special image formats
    cameraProps_.bUsesImageFormats = false;
    int nAOISupported = 0;
    if (is_ImageFormat(cameraHandle_, IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED, (void*)&nAOISupported, sizeof(nAOISupported)) == IS_SUCCESS) {
        cameraProps_.bUsesImageFormats = (nAOISupported == 0);
    }

    if (cameraProps_.bUsesImageFormats) {
        // search the default formats
        cameraProps_.nImgFmtNormal  = searchDefaultImageFormats(CAPTMODE_FREERUN | CAPTMODE_SINGLE);
        cameraProps_.nImgFmtDefaultNormal = cameraProps_.nImgFmtNormal;
        cameraProps_.nImgFmtTrigger = searchDefaultImageFormats(CAPTMODE_TRIGGER_SOFT_SINGLE);
        cameraProps_.nImgFmtDefaultTrigger = cameraProps_.nImgFmtTrigger;

        // set the default formats
        if (!is_ImageFormat(cameraHandle_, IMGFRMT_CMD_SET_FORMAT, (void*)&cameraProps_.nImgFmtNormal, sizeof(cameraProps_.nImgFmtNormal))) {
            //m_nImageFormat = nFormat;
            //bRet = TRUE;
        }
    }

    updatePixelClock();
    updateExposureTime();

    setupCapture();

    eventThread_ = new AssemblyUEyeCameraEventThread();
    connect(eventThread_, SIGNAL(eventHappened()),
            this, SLOT(eventHappend()));
    eventThread_->start(cameraHandle_);

    cameraState_ = State::READY;

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "open"
       << ": emitting signal \"cameraOpened\"";

    emit cameraOpened();
}

void AssemblyUEyeCamera::close()
{
    if(cameraState_ != State::READY){ return; }

    cameraState_ = State::CLOSING;

    eventThread_->stop();

    freeImages();

    if (eventThread_->wait(2000) == FALSE){
        eventThread_->terminate();
    }

    unsigned int ret = is_ExitCamera(cameraHandle_);

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "close: is_ExitCamera=" << ret;

    cameraState_ = State::OFF;

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "close"
       << ": emitting signal \"cameraClosed\"";

    emit cameraClosed();
}

void AssemblyUEyeCamera::updateInformation()
{
    if (cameraState_!=State::READY) return;

    CAMINFO* cameraInfo = new CAMINFO;
    if(!is_GetCameraInfo(cameraHandle_, cameraInfo))
    {
        setID(cameraInfo->ID);
        setVersion(cameraInfo->Version);
        setDate(cameraInfo->Date);
    }
    else
    {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "updateInformation: cannot read camera information";
    }

    delete cameraInfo;

    SENSORINFO* sensorInfo = new SENSORINFO;
    if (!is_GetSensorInfo(cameraHandle_, sensorInfo)) {

        setSensorName(sensorInfo->strSensorName);
        setColorMode(sensorInfo->nColorMode);
        setMaxWidth(sensorInfo->nMaxWidth);
        setMaxHeight(sensorInfo->nMaxHeight);
        setMasterGain(sensorInfo->bMasterGain); 
        setRedGain(sensorInfo->bRGain);
        setGreenGain(sensorInfo->bGGain);
        setBlueGain(sensorInfo->bBGain);
        setGlobalShutter(sensorInfo->bGlobShutter);
        setPixelSize(sensorInfo->wPixelSize);

    }
    else
    {
        NQLog("AssemblyUEyeCamera", NQLog::Fatal) << "updateInformation: cannot read sensor information";
    }

    delete sensorInfo;

    updatePixelClock();
    updateExposureTime();
}

void AssemblyUEyeCamera::updatePixelClock()
{
    if (cameraState_!=State::READY && cameraState_!=State::INITIALIZING) return;

    UINT nRange[3];

    ZeroMemory(nRange, sizeof(nRange));

    UINT nMin = 0;
    UINT nMax = 0;
    UINT nInc = 0;

    INT nRet = is_PixelClock(cameraHandle_, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));

    if (nRet == IS_SUCCESS) {
      nMin = nRange[0];
      nMax = nRange[1];
      nInc = nRange[2] ? nRange[2] : 1;
    }

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
       ": " << nMin << " " << nMax << " " << nInc;

    if (nMin!=0) {

        bool listChanged = false;
        bool valueChanged = false;

        std::vector<unsigned int> newPixelClocks;
        unsigned int newPixelClock;

        for (UINT f=nMin;f<=nMax;f+=nInc) newPixelClocks.push_back(f);
        newPixelClock = readPixelClock();

        if (newPixelClocks!=pixelClocks_) listChanged = true;
        if (newPixelClock!=currentPixelClock_) valueChanged = true;

        pixelClocks_ = newPixelClocks;
        currentPixelClock_ = newPixelClock;

        if (listChanged)
        {
          NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
             << ": emitting signal \"pixelClockListChanged(" << newPixelClock << ")\"";

          emit pixelClockListChanged(newPixelClock);
        }

        if(valueChanged)
        {
          NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
             << ": emitting signal \"pixelClockChanged(" << newPixelClock << ")\"";

          emit pixelClockChanged(newPixelClock);
        }

    } else {

        UINT nNumberOfSupportedPixelClocks = 0;

        INT nRet = is_PixelClock(cameraHandle_, IS_PIXELCLOCK_CMD_GET_NUMBER,
                                 (void*)&nNumberOfSupportedPixelClocks,
                                 sizeof(nNumberOfSupportedPixelClocks));

        if ((nRet == IS_SUCCESS) && (nNumberOfSupportedPixelClocks > 0)) {

            // No camera has more than 150 different pixel clocks.
            // Of course, the list can be allocated dynamically

            UINT nPixelClockList[150];

            ZeroMemory(&nPixelClockList, sizeof(nPixelClockList));

            nRet = is_PixelClock(cameraHandle_, IS_PIXELCLOCK_CMD_GET_LIST,
                                 (void*)nPixelClockList,
                                 nNumberOfSupportedPixelClocks * sizeof(UINT));

            if ((nRet == IS_SUCCESS)) {

                bool listChanged = false;
                bool valueChanged = false;

                std::vector<unsigned int> newPixelClocks;
                unsigned int newPixelClock;

                for (unsigned int idx=0;idx<nNumberOfSupportedPixelClocks;idx++)
                    newPixelClocks.push_back(nPixelClockList[idx]);
                newPixelClock = readPixelClock();

                if (newPixelClocks!=pixelClocks_) listChanged = true;
                if (newPixelClock!=currentPixelClock_) valueChanged = true;

                pixelClocks_ = newPixelClocks;
                currentPixelClock_ = newPixelClock;

                if(listChanged)
                {
                  NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
                     << ": emitting signal \"pixelClockListChanged(" << newPixelClock << ")\"";

                  emit pixelClockListChanged(newPixelClock);
                }

                if(valueChanged)
                {
                  NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
                     << ": emitting signal \"pixelClockChanged(" << newPixelClock << ")\"";

                  emit pixelClockChanged(newPixelClock);
                }
            }
            else
            {
                pixelClocks_.clear();
                pixelClocks_.push_back(0);
                currentPixelClock_ = 0;

                NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
                   << ": emitting signal \"pixelClockListChanged(" << 0 << ")\"";

                emit pixelClockListChanged(0);

                NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updatePixelClock"
                   << ": emitting signal \"pixelClockChanged(" << 0 << ")\"";

                emit pixelClockChanged(0);
            }
        }
    }
}

void AssemblyUEyeCamera::updateExposureTime()
{
    if (cameraState_!=State::READY && cameraState_!=State::INITIALIZING) return;

    double nRange[3];

    ZeroMemory(nRange, sizeof(nRange));

    double nMin = 0;
    double nMax = 0;
    double nInc = 0;

    INT nRet = is_Exposure(cameraHandle_, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void*)nRange, sizeof(nRange));

    if(nRet == IS_SUCCESS)
    {
        nMin = nRange[0];
        nMax = nRange[1];
        nInc = nRange[2];

        NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updateExposureTime"
           ": " << nMin << " " << nMax << " " << nInc;

        bool  listChanged = false;
        bool valueChanged = false;

        double newExposureTime;

        newExposureTime = readExposureTime();

        if (nMin!=exposureTimeMin_ || nMax!=exposureTimeMax_ || nInc!=exposureTimeInc_) listChanged = true;
        if (newExposureTime!=currentExposureTime_) valueChanged = true;

        exposureTimeMin_ = nMin;
        exposureTimeMax_ = nMax;
        exposureTimeInc_ = nInc;
        currentExposureTime_ = newExposureTime;

        if(listChanged)
        {
          NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updateExposureTime"
             << ": emitting signal \"exposureTimeRangeChanged(" << newExposureTime << ")\"";

          emit exposureTimeRangeChanged(newExposureTime);
        }

        if(valueChanged)
        {
          NQLog("AssemblyUEyeCamera", NQLog::Debug) << "updateExposureTime"
             << ": emitting signal \"exposureTimeChanged(" << newExposureTime << ")\"";

          emit exposureTimeChanged(newExposureTime);
        }
    }
}

bool AssemblyUEyeCamera::isAvailable() const
{
    return ((bool) IS_CAMERA_AVAILABLE(getStatus()));
}

void AssemblyUEyeCamera::eventHappend()
{
    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "eventHappend: new frame available";

    INT dummy = 0;
    char *pLast = nullptr, *pMem = nullptr;

    is_GetActSeqBuf(cameraHandle_, &dummy, &pMem, &pLast);
    lastBuffer_ = pLast;

    int nNum;
    INT ret = IS_SUCCESS;

    if (!cameraHandle_) return;

    if (bufferProps_.width < 1 || bufferProps_.height < 1)
        return;

    nNum = getImageNumber(lastBuffer_);
    ret = is_LockSeqBuf(cameraHandle_, nNum, lastBuffer_);

    memcpy(image_.ptr(), (uchar*)lastBuffer_, bufferProps_.width * bufferProps_.height * 1);

    is_UnlockSeqBuf(cameraHandle_, nNum, lastBuffer_);

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "eventHappend"
       << ": emitting signal \"imageAcquired\"";

    emit imageAcquired(image_);
}

void AssemblyUEyeCamera::acquireImage()
{
  if(cameraState_ != READY)
  {
      NQLog("AssemblyUEyeCamera", NQLog::Critical) << "acquireImage: camera is not ready, no action taken";

      return;
  }

  NQLog("AssemblyUEyeCamera", NQLog::Spam) << "acquireImage: camera is ready";

  unsigned int ret = is_FreezeVideo(cameraHandle_, IS_DONT_WAIT);

  if(ret == IS_SUCCESS)
  {
      NQLog("AssemblyUEyeCamera", NQLog::Spam) << "acquireImage: image acquired successfully";
  }

  return;
}

int AssemblyUEyeCamera::searchDefaultImageFormats(int supportMask)
{
    int ret = IS_SUCCESS;
    int nNumber;
    int format = 0;
    IMAGE_FORMAT_LIST *pFormatList;
    IS_RECT rectAOI;

    if ((ret=is_ImageFormat(cameraHandle_, IMGFRMT_CMD_GET_NUM_ENTRIES, (void*)&nNumber, sizeof(nNumber))) == IS_SUCCESS &&
        (ret=is_AOI(cameraHandle_, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI))) == IS_SUCCESS) {

        int i = 0;
        int nSize = sizeof(IMAGE_FORMAT_LIST) + (nNumber - 1) * sizeof(IMAGE_FORMAT_LIST);
        pFormatList = (IMAGE_FORMAT_LIST*)(new char[nSize]);
        pFormatList->nNumListElements = nNumber;
        pFormatList->nSizeOfListEntry = sizeof(IMAGE_FORMAT_INFO);

        if ((ret=is_ImageFormat(cameraHandle_, IMGFRMT_CMD_GET_LIST, (void*)pFormatList, nSize)) == IS_SUCCESS) {
            for (i=0; i<nNumber; i++) {
                if ((pFormatList->FormatInfo[i].nSupportedCaptureModes & supportMask) &&
                    pFormatList->FormatInfo[i].nHeight == (UINT)rectAOI.s32Height &&
                    pFormatList->FormatInfo[i].nWidth  == (UINT)rectAOI.s32Width)
                {
                    format = pFormatList->FormatInfo[i].nFormatID;
                    break;
                }
            }
        }
    }

    return format;
}

void AssemblyUEyeCamera::setupCapture()
{
    int width, height;

    // init the memorybuffer properties
    ZeroMemory(&bufferProps_, sizeof(bufferProps_));

    IS_RECT rectAOI;
    INT nRet = is_AOI(cameraHandle_, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    if (nRet == IS_SUCCESS) {
        width  = rectAOI.s32Width;
        height = rectAOI.s32Height;

        // get current colormode
        int colormode = is_SetColorMode(cameraHandle_, IS_GET_COLOR_MODE);

        if(colormode == IS_CM_BGR5_PACKED) {
            is_SetColorMode(cameraHandle_, IS_CM_BGR565_PACKED);
            colormode = IS_CM_BGR565_PACKED;
        }

        NQLog("AssemblyUEyeCamera", NQLog::Debug) << "setupCapture: color mode = " << colormode;

        ZeroMemory(&bufferProps_, sizeof(bufferProps_));
        bufferProps_.width  = width;
        bufferProps_.height = height;
        bufferProps_.colorformat = colormode;
        bufferProps_.bitspp = getBitsPerPixel(colormode);
        bufferProps_.cvimgformat = CV_8UC1;

        switch (colormode)
        {
        default:
        case IS_CM_MONO8:
        case IS_CM_SENSOR_RAW8:
            //bufferProps_.pRgbTable = table_;
            //bufferProps_.tableentries = 256;
            bufferProps_.cvimgformat = CV_8UC1;
            break;
        case IS_CM_BGR565_PACKED:
            bufferProps_.cvimgformat = CV_8UC1;
            break;
        case IS_CM_RGB8_PACKED:
        case IS_CM_BGR8_PACKED:
            bufferProps_.cvimgformat = CV_8UC1;
            break;
        case IS_CM_RGBA8_PACKED:
        case IS_CM_BGRA8_PACKED:
            bufferProps_.cvimgformat = CV_8UC1;
            break;
        }

        image_.create(bufferProps_.height, bufferProps_.width, bufferProps_.cvimgformat);

        allocImages();
    }
}

int AssemblyUEyeCamera::getBitsPerPixel(int colormode)
{
    switch (colormode)
    {
    default:
    case IS_CM_MONO8:
    case IS_CM_SENSOR_RAW8:
        return 8;   // occupies 8 Bit
    case IS_CM_MONO12:
    case IS_CM_MONO16:
    case IS_CM_SENSOR_RAW12:
    case IS_CM_SENSOR_RAW16:
    case IS_CM_BGR5_PACKED:
    case IS_CM_BGR565_PACKED:
    case IS_CM_UYVY_PACKED:
    case IS_CM_CBYCRY_PACKED:
        return 16;  // occupies 16 Bit
    case IS_CM_RGB8_PACKED:
    case IS_CM_BGR8_PACKED:
        return 24;
    case IS_CM_RGBA8_PACKED:
    case IS_CM_BGRA8_PACKED:
    case IS_CM_RGBY8_PACKED:
    case IS_CM_BGRY8_PACKED:
    case IS_CM_RGB10_PACKED:
    case IS_CM_BGR10_PACKED:
        return 32;
    }
}

unsigned int AssemblyUEyeCamera::readPixelClock()
{
    UINT nPixelClock;

    // Get current pixel clock
    INT nRet = is_PixelClock(cameraHandle_, IS_PIXELCLOCK_CMD_GET, (void*)&nPixelClock, sizeof(nPixelClock));

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "readPixelClock: nPixelClock=" << nPixelClock;

    return nPixelClock;
}

void AssemblyUEyeCamera::setPixelClock(unsigned int pixelClock)
{
    if (cameraState_!=State::READY) return;

    if (currentPixelClock_==pixelClock) return;

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "setPixelClock: pixelClock" << pixelClock;

    UINT nPixelClock = pixelClock;

    // Get current pixel clock
    INT nRet = is_PixelClock(cameraHandle_, IS_PIXELCLOCK_CMD_SET, (void*)&nPixelClock, sizeof(nPixelClock));

    readPixelClock();
}

double AssemblyUEyeCamera::readExposureTime()
{
    double nExposureTime;

    // Get current pixel clock
    INT nRet = is_Exposure(cameraHandle_, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&nExposureTime, sizeof(nExposureTime));

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "readExposureTime: nExposureTime=" << nExposureTime;

    return nExposureTime;
//    return 1.e-3*std::round(1.e3*nExposureTime);
}

void AssemblyUEyeCamera::setExposureTime(double exposureTime)
{
    if(cameraState_ != State::READY){ return; }

    if(currentExposureTime_ == exposureTime){ return; }

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "setExposureTime: exposureTime=" << exposureTime;

    double nExposureTime = exposureTime;

    // Get current pixel clock
    INT nRet = is_Exposure(cameraHandle_, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&nExposureTime, sizeof(nExposureTime));

    NQLog("AssemblyUEyeCamera", NQLog::Debug) << "setExposureTime: delta=" << 1e6*(nExposureTime - readExposureTime());
}

bool AssemblyUEyeCamera::allocImages()
{
    lastBuffer_ = nullptr;

    int nWidth = 0;
    int nHeight = 0;

    UINT nAbsPosX;
    UINT nAbsPosY;

    is_AOI(cameraHandle_, IS_AOI_IMAGE_GET_POS_X_ABS, (void*)&nAbsPosX , sizeof(nAbsPosX));
    is_AOI(cameraHandle_, IS_AOI_IMAGE_GET_POS_Y_ABS, (void*)&nAbsPosY , sizeof(nAbsPosY));

    is_ClearSequence(cameraHandle_);

    for (unsigned int i = 0; i < sizeof(images_) / sizeof(images_[0]); i++) {

        if (images_[i].pBuf) {
            is_FreeImageMem (cameraHandle_, images_[i].pBuf, images_[i].nImageID);
            images_[i].pBuf = nullptr;
            images_[i].nImageID = 0;
        }

        nWidth = bufferProps_.width;
        nHeight = bufferProps_.height;

        if (nAbsPosX) {
            bufferProps_.width = nWidth = getMaxWidth();
        }
        if (nAbsPosY)
        {
            bufferProps_.height = nHeight = getMaxHeight();
        }

        if (is_AllocImageMem (cameraHandle_, nWidth, nHeight, bufferProps_.bitspp, &images_[i].pBuf,
                              &images_[i].nImageID) != IS_SUCCESS)
            return FALSE;
        if (is_AddToSequence (cameraHandle_, images_[i].pBuf, images_[i].nImageID) != IS_SUCCESS)
            return FALSE;

        images_[i].nImageSeqNum = i + 1;
        images_[i].nBufferSize = nWidth * nHeight * bufferProps_.bitspp / 8;
    }

    return TRUE;
}

bool AssemblyUEyeCamera::freeImages()
{
    lastBuffer_ = nullptr;
    for (unsigned int i = 0; i < sizeof(images_) / sizeof(images_[0]); i++)
    {
        if (images_[i].pBuf)
        {
            is_FreeImageMem (cameraHandle_, images_[i].pBuf, images_[i].nImageID);
            images_[i].pBuf = nullptr;
            images_[i].nImageID = 0;
        }
    }

    return true;
}

int AssemblyUEyeCamera::getImageNumber(char * pBuffer)
{
    for (unsigned int i = 0; i < sizeof(images_) / sizeof(images_[0]); i++)
        if (images_[i].pBuf == pBuffer)
            return images_[i].nImageSeqNum;

    return 0;
}

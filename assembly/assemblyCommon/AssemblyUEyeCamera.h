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

#ifndef ASSEMBLYUEYECAMERA_H
#define ASSEMBLYUEYECAMERA_H

#include <AssemblyVUEyeCamera.h>
#include <ueye.h>

#include <QThread>

#include <opencv2/opencv.hpp>

typedef struct _UEYE_IMAGE
{
    char *pBuf;
    INT nImageID;
    INT nImageSeqNum;
    INT nBufferSize;
} UEYE_IMAGE, *PUEYE_IMAGE;

class AssemblyUEyeCameraEventThread : public QThread
{
 Q_OBJECT

  public:

    AssemblyUEyeCameraEventThread();
    virtual ~AssemblyUEyeCameraEventThread();

    int start(HIDS cameraHandle);
    void stop();

  signals:

    void eventHappened();

  protected:

    void run();

  private:

    HIDS cameraHandle_;
    volatile bool runEventThread_;
};

class AssemblyUEyeCamera : public AssemblyVUEyeCamera
{
 Q_OBJECT

  public:

    explicit AssemblyUEyeCamera(QObject *parent);
    ~AssemblyUEyeCamera();

    void updateInformation();
    void updatePixelClock();
    void updateExposureTime();

    bool isAvailable() const;

  public slots:

    void open();
    void close();

    void acquireImage();

    void setPixelClock(unsigned int);
    void setExposureTime(double);

  protected slots:

    void eventHappend();

  protected:

    int searchDefaultImageFormats(int supportMask);
    void setupCapture();
    int getBitsPerPixel(int colormode);
    bool allocImages();
    bool freeImages();
    int getImageNumber(char * pBuffer);
    unsigned int readPixelClock();
    double readExposureTime();

  private:

    HIDS cameraHandle_;
    AssemblyUEyeCameraEventThread* eventThread_;

    struct sCameraProps
    {
        bool bUsesImageFormats;
        int nImgFmtNormal;
        int nImgFmtDefaultNormal;
        int nImgFmtTrigger;
        int nImgFmtDefaultTrigger;
    } cameraProps_;

    struct sBufferProps {
        int width;
        int height;
        int colorformat;
        int bitspp;
        int cvimgformat;
        //QRgb *pRgbTable;
        //int tableentries;
        int imageformat;
    } bufferProps_;
    //QRgb table_[256];

    char *lastBuffer_;
    UEYE_IMAGE images_[5];

    cv::Mat image_;
};

#endif // ASSEMBLYUEYECAMERA_H

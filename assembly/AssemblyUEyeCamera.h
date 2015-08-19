#ifndef ASSEMBLYUEYECAMERA_H
#define ASSEMBLYUEYECAMERA_H

#include <uEye.h>

#include <QThread>
#include <QColor>

#include "AssemblyVUEyeCamera.h"

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
    bool isAvailable() const;

    bool startLiveVideo();
    bool stopLiveVideo();

public slots:

    void open();
    void close();

    void acquireImage();

protected slots:

    void eventHappend();

protected:

    int searchDefaultImageFormats(int supportMask);
    void setupCapture();
    int getBitsPerPixel(int colormode);
    bool allocImages();
    bool freeImages();
    int getImageNumber(char * pBuffer);

signals:

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
        QImage::Format imgformat;
        QRgb *pRgbTable;
        int tableentries;
        int imageformat;
    } bufferProps_;
    QRgb table_[256];

    char *lastBuffer_;
    UEYE_IMAGE images_[5];

    QImage image_;
};

#endif // ASSEMBLYUEYECAMERA_H

#ifndef ASSEMBLYUEYECAMERA_H
#define ASSEMBLYUEYECAMERA_H

#include <uEye.h>

#include <QImage>
#include <QColor>

#include "AssemblyVUEyeCamera.h"

typedef struct _UEYE_IMAGE
{
    char *pBuf;
    INT nImageID;
    INT nImageSeqNum;
    INT nBufferSize;
} UEYE_IMAGE, *PUEYE_IMAGE;

class AssemblyUEyeCamera : public AssemblyVUEyeCamera
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCamera(QObject *parent);
    ~AssemblyUEyeCamera();

    void updateInformation();
    bool isAvailable() const;

    bool initialize();
    bool exit();

    bool startLiveVideo();
    bool stopLiveVideo();

public slots:

    void aquireImage();

protected slots:

protected:

signals:

private:

    HIDS cameraHandle_;

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

    UEYE_IMAGE images_[5];
};

#endif // ASSEMBLYUEYECAMERA_H

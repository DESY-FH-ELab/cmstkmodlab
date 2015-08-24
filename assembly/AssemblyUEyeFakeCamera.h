#ifndef ASSEMBLYUEYEFAKECAMERA_H
#define ASSEMBLYUEYEFAKECAMERA_H

#include "AssemblyVUEyeCamera.h"

class AssemblyUEyeFakeCamera : public AssemblyVUEyeCamera
{
    Q_OBJECT
public:
    explicit AssemblyUEyeFakeCamera(QObject *parent);
    ~AssemblyUEyeFakeCamera();

    void updateInformation();
    void updatePixelClock();
    void updateExposureTime();

    bool isAvailable() const { return true; }

public slots:

    void open();
    void close();

    void acquireImage();

    void setPixelClock(unsigned int pc) { currentPixelClock_ = pc; }
    void setExposureTime(double et) { currentExposureTime_ = et; }

protected slots:

protected:

    QImage image_[3];
    unsigned int imageIndex_;

signals:

private:
};

#endif // ASSEMBLYUEYEFAKECAMERA_H

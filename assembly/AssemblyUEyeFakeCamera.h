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

    bool isAvailable() const { return true; }

public slots:

    void open();
    void close();

    void acquireImage();

protected slots:

protected:

    QImage image_[3];
    unsigned int imageIndex_;

signals:

private:
};

#endif // ASSEMBLYUEYEFAKECAMERA_H

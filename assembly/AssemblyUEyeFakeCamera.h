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
    bool isAvailable() const { return true; }

public slots:

    void open();
    void close();

    void acquireImage();

protected slots:

protected:

signals:

private:

};

#endif // ASSEMBLYUEYEFAKECAMERA_H

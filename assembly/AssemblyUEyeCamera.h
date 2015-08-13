#ifndef ASSEMBLYUEYECAMERA_H
#define ASSEMBLYUEYECAMERA_H

#include "AssemblyVUEyeCamera.h"

class AssemblyUEyeCamera : public AssemblyVUEyeCamera
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCamera(QObject *parent);
    ~AssemblyUEyeCamera();

public slots:

    void aquireImage();

protected slots:

protected:

signals:

private:

};

#endif // ASSEMBLYUEYECAMERA_H

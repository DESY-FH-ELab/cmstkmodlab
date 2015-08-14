#ifndef ASSEMBLYUEYECAMERA_H
#define ASSEMBLYUEYECAMERA_H

#include <uEye.h>

#include "AssemblyVUEyeCamera.h"

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

public slots:

    void aquireImage();

protected slots:

protected:

signals:

private:

    HIDS cameraHandle_;
};

#endif // ASSEMBLYUEYECAMERA_H

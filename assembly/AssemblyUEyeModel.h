#ifndef ASSEMBLYUEYEMODEL_H
#define ASSEMBLYUEYEMODEL_H

#ifndef USE_FAKEIO
#include <uEye.h>
#endif

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "AssemblyVUEyeModel.h"

class AssemblyUEyeModel : public AssemblyVUEyeModel
{
    Q_OBJECT
public:
    explicit AssemblyUEyeModel(int updateInterval = 60,
                               QObject *parent = 0);
    ~AssemblyUEyeModel();

    unsigned int getCameraCount() const;
    AssemblyUEyeCamera_t * getCameraInfo(unsigned int idx) const;

    unsigned int getCameraStatus(unsigned int idx) const;
    bool isCameraAvailable(unsigned int idx) const;

public slots:

    void updateInformation();

protected slots:

protected:

signals:

    void cameraCountChanged(unsigned int);

private:

#ifndef USE_FAKEIO
    UEYE_CAMERA_LIST* uEyeCameraList_;
#endif
};

#endif // ASSEMBLYUEYEMODEL_H

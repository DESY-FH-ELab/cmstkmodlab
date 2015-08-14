#ifndef ASSEMBLYUEYEMODEL_H
#define ASSEMBLYUEYEMODEL_H

#include <uEye.h>

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

public slots:

    void updateInformation();

protected slots:

protected:

private:

    UEYE_CAMERA_LIST* uEyeCameraList_;
};

#endif // ASSEMBLYUEYEMODEL_H

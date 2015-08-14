#ifndef ASSEMBLYVUEYEMODEL_H
#define ASSEMBLYVUEYEMODEL_H

#include <vector>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QVector>

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeCamera.h"
typedef AssemblyUEyeFakeCamera AssemblyUEyeCamera_t;
#else
#include "AssemblyUEyeCamera.h"
typedef AssemblyUEyeCamera AssemblyUEyeCamera_t;
#endif

class AssemblyVUEyeModel : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyVUEyeModel(int updateInterval, QObject *parent);
    ~AssemblyVUEyeModel();

    size_t getCameraCount() const;
    AssemblyUEyeCamera_t * getCamera(size_t idx);

public slots:

    virtual void updateInformation() = 0;

protected slots:

protected:

    int updateInterval_;
    QTimer* timer_;

    QMutex mutex_;

    QVector<AssemblyUEyeCamera_t*> cameras_;

signals:

    void cameraCountChanged(unsigned int);
};

#endif // ASSEMBLYVUEYEMODEL_H

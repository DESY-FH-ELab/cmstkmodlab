#ifndef ASSEMBLYVUEYEMODEL_H
#define ASSEMBLYVUEYEMODEL_H

#include <vector>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QVector>

#include "AssemblyVUEyeCamera.h"

class AssemblyVUEyeModel : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyVUEyeModel(int updateInterval, QObject *parent);
    ~AssemblyVUEyeModel();

    size_t getCameraCount() const;
    AssemblyVUEyeCamera * getCamera(size_t idx);
    AssemblyVUEyeCamera * getCameraByID(unsigned int id);

public slots:

    virtual void updateInformation() = 0;

protected slots:

protected:

    int updateInterval_;
    QTimer* timer_;

    QMutex mutex_;

    QVector<QThread*> threads_;
    QVector<AssemblyVUEyeCamera*> cameras_;
    void clear();

signals:

    void cameraCountChanged(unsigned int);
};

#endif // ASSEMBLYVUEYEMODEL_H

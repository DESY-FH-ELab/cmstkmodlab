#ifndef ASSEMBLYUEYEMODEL_H
#define ASSEMBLYUEYEMODEL_H

#include <uEye.h>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

class AssemblyUEyeModel : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyUEyeModel(int updateInterval = 60,
                               QObject *parent = 0);

    ~AssemblyUEyeModel();

    unsigned int getCameraCount() const;
    UEYE_CAMERA_INFO* getCameraInfo(unsigned int idx) const;

    unsigned int getCameraStatus(unsigned int idx) const;
    bool isCameraAvailable(unsigned int idx) const;

public slots:

  void updateInformation();

protected slots:

protected:

  int updateInterval_;
  QTimer* timer_;

  QMutex mutex_;

signals:

    void cameraCountChanged(unsigned int);

private:

  UEYE_CAMERA_LIST* uEyeCameraList_;
};

#endif // ASSEMBLYUEYEMODEL_H

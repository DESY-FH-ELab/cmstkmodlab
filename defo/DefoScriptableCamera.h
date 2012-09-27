#ifndef DEFOSCRIPTABLECAMERA_H
#define DEFOSCRIPTABLECAMERA_H

#include <QObject>
#include <QMutex>

#include "DefoCameraModel.h"

class DefoScriptableCamera :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableCamera(
      DefoCameraModel* cameraModel
    , QObject *parent = 0
  );
public slots:
  void takePicture();

signals:
  void acquirePicture(bool);

protected:
  DefoCameraModel* cameraModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLECAMERA_H

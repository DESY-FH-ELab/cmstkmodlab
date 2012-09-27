#include <iostream>

#include <QMutexLocker>

#include "DefoScriptableCamera.h"

DefoScriptableCamera::DefoScriptableCamera(
    DefoCameraModel* cameraModel
  , QObject *parent
) :
    QObject(parent)
  , cameraModel_(cameraModel)
{
  connect(this, SIGNAL(acquirePicture(bool)),
          cameraModel_, SLOT(acquirePicture(bool)));
}

void DefoScriptableCamera::takePicture() {
  QMutexLocker locker(&mutex_);
  emit(acquirePicture(true));
  sleep(2);
}

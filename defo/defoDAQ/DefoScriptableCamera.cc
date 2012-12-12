#include <iostream>

#include <QMutexLocker>

#include "DefoScriptableCamera.h"

DefoScriptableCamera::DefoScriptableCamera(
    DefoScriptModel* scriptModel
  , DefoCameraModel* cameraModel
  , QObject *parent
) :
    QObject(parent)
  , scriptModel_(scriptModel)
  , cameraModel_(cameraModel)
{
  connect(this, SIGNAL(acquirePicture(bool)),
          cameraModel_, SLOT(acquirePicture(bool)));
}

void DefoScriptableCamera::takePicture() {
  QMutexLocker locker(&mutex_);

  scriptModel_->message("take picture");

  emit(acquirePicture(true));
  sleep(2);
}

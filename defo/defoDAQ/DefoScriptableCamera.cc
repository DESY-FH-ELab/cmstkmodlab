#include <unistd.h>

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
  connect(this, SIGNAL(setComment(QString)),
          cameraModel_, SLOT(setComment(QString)));
  connect(this, SIGNAL(changeNumberOfPictures(int)),
          cameraModel_, SLOT(setNumberOfImages(int)));
}

void DefoScriptableCamera::takePicture() {

  QMutexLocker locker(&mutex_);

  scriptModel_->message("take picture");

  emit acquirePicture(true);
  sleep(2);
}

void DefoScriptableCamera::comment(const QString & text) {

  QMutexLocker locker(&mutex_);

  emit setComment(text);
}

void DefoScriptableCamera::setNumberOfPictures(int count) {

  QMutexLocker locker(&mutex_);

  emit changeNumberOfPictures(count);
}

QScriptValue DefoScriptableCamera::numberOfPictures() {

  QMutexLocker locker(&mutex_);

  return QScriptValue(cameraModel_->getNumberOfImages());
}

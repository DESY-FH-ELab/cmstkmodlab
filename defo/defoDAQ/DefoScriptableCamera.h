#ifndef DEFOSCRIPTABLECAMERA_H
#define DEFOSCRIPTABLECAMERA_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <DefoScriptModel.h>

#include "DefoCameraModel.h"

class DefoScriptableCamera :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableCamera(
      DefoScriptModel* scriptModel
    , DefoCameraModel* cameraModel
    , QObject *parent = 0
  );
public slots:
  void takePicture();
  void comment(const QString& text);

  void setNumberOfPictures(int count);
  QScriptValue numberOfPictures();

signals:
  void acquirePicture(bool);
  void setComment(const QString&);
  void changeNumberOfPictures(int);

protected:
  DefoScriptModel* scriptModel_;
  DefoCameraModel* cameraModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLECAMERA_H

#ifndef DEFOSCRIPTABLECAMERA_H
#define DEFOSCRIPTABLECAMERA_H

#include <QObject>

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
  void test1();
  void test2(float value);
  float getTemp1();
protected:
  DefoCameraModel* cameraModel_;
};

#endif // DEFOSCRIPTABLECAMERA_H

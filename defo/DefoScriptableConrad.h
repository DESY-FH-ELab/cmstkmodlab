#ifndef DEFOSCRIPTABLECONRAD_H
#define DEFOSCRIPTABLECONRAD_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "DefoConradModel.h"

class DefoScriptableConrad :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableConrad(
      DefoConradModel* conradModel
    , QObject *parent = 0
  );

public slots:
  void enablePanel(unsigned int panel);
  void disablePanel(unsigned int panel);
  QScriptValue panel(unsigned int panel);
  void enableCalibrationLEDs();
  void disableCalibrationLEDs();
  QScriptValue calibrationLEDs();

signals:
  void enableSwitch( DefoConradModel::DeviceSwitch device );
  void disableSwitch( DefoConradModel::DeviceSwitch device );

protected:
  DefoConradModel* conradModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLECONRAD_H

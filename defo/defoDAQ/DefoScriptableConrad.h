#ifndef DEFOSCRIPTABLECONRAD_H
#define DEFOSCRIPTABLECONRAD_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <DefoScriptModel.h>

#include "DefoConradModel.h"

class DefoScriptableConrad :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableConrad(
      DefoScriptModel* scriptModel
    , DefoConradModel* conradModel
    , QObject *parent = 0
  );

public slots:
  void enablePanel(unsigned int panel);
  void disablePanel(unsigned int panel);
  QScriptValue panelState(unsigned int panel);
  void enableCalibrationLEDs();
  void disableCalibrationLEDs();
  QScriptValue calibrationLEDsState();

signals:
  void enableSwitch( DefoConradModel::DeviceSwitch device );
  void disableSwitch( DefoConradModel::DeviceSwitch device );

protected:
  DefoScriptModel* scriptModel_;
  DefoConradModel* conradModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLECONRAD_H

#ifndef DEFOSCRIPTABLECONRAD_H
#define DEFOSCRIPTABLECONRAD_H

#include <QObject>
#include <QMutex>

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
  void enablePanel(int panel);
  void disablePanel(int panel);
  void enableCalibrationLEDs();
  void disableCalibrationLEDs();

signals:
  void enableSwitch( DefoConradModel::DeviceSwitch device );
  void disableSwitch( DefoConradModel::DeviceSwitch device );

protected:
  DefoConradModel* conradModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLECONRAD_H

#ifndef DEFOSCRIPTABLEKEITHLEY_H
#define DEFOSCRIPTABLEKEITHLEY_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "DefoKeithleyModel.h"

class DefoScriptableKeithley :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableKeithley(
      DefoKeithleyModel* keithleyModel
    , QObject *parent = 0
  );

public slots:
  /*
  void enablePanel(int panel);
  void disablePanel(int panel);
  QScriptValue panel(int panel);
  void enableCalibrationLEDs();
  void disableCalibrationLEDs();
  QScriptValue calibrationLEDs();
  */
signals:
  /*
  void enableSwitch( DefoConradModel::DeviceSwitch device );
  void disableSwitch( DefoConradModel::DeviceSwitch device );
  */
protected:
  DefoKeithleyModel* keithleyModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEKEITHLEY_H

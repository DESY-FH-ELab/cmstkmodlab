#ifndef DEFOSCRIPTABLEJULABO_H
#define DEFOSCRIPTABLEJULABO_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "DefoJulaboModel.h"

class DefoScriptableJulabo :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableJulabo(
      DefoJulaboModel* julaboModel
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
  DefoJulaboModel* julaboModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEJULABO_H

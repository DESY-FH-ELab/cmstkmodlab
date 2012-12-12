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

  QScriptValue state(unsigned int channel);
  QScriptValue temperature(unsigned int channel);

protected:
  DefoKeithleyModel* keithleyModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEKEITHLEY_H

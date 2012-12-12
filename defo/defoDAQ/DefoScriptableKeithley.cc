#include <iostream>

#include <QMutexLocker>

#include "DefoScriptableKeithley.h"

DefoScriptableKeithley::DefoScriptableKeithley(
    DefoKeithleyModel* keithleyModel
  , QObject *parent
) :
    QObject(parent)
  , keithleyModel_(keithleyModel)
{

}

QScriptValue DefoScriptableKeithley::state(unsigned int channel) {

  QMutexLocker locker(&mutex_);

  if (channel>9) return QScriptValue(0);

  int state = static_cast<int>(keithleyModel_->getSensorState(channel));
  return QScriptValue(state);
}

QScriptValue DefoScriptableKeithley::temperature(unsigned int channel) {

  QMutexLocker locker(&mutex_);

  if (channel>9) return QScriptValue(-99);

  return QScriptValue(keithleyModel_->getTemperature(channel));
}

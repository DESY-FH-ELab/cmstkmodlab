#include <iostream>

#include <QMutexLocker>

#include "ScriptableKeithley.h"

ScriptableKeithley::ScriptableKeithley(KeithleyModel* keithleyModel, QObject *parent) :
    QObject(parent),
    keithleyModel_(keithleyModel)
{

}

QScriptValue ScriptableKeithley::state(unsigned int channel) {

  QMutexLocker locker(&mutex_);

  if (channel>9) return QScriptValue(0);

  int state = static_cast<int>(keithleyModel_->getSensorState(channel));
  return QScriptValue(state);
}

QScriptValue ScriptableKeithley::temperature(unsigned int channel) {

  QMutexLocker locker(&mutex_);

  if (channel>9) return QScriptValue(-99);

  return QScriptValue(keithleyModel_->getTemperature(channel));
}

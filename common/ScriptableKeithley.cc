#include <unistd.h>

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

QScriptValue ScriptableKeithley::temperatureAsString(unsigned int channel)
{
    QMutexLocker locker(&mutex_);

    if (channel>9) return QScriptValue("-99");

    return QScriptValue(QString::number(keithleyModel_->getTemperature(channel), 'f', 2));
}

void ScriptableKeithley::waitForTemperatureAbove(unsigned int channel,
                                                 float temperature,
                                                 int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = keithleyModel_->getTemperature(channel);
    locker.unlock();
    if (temp>temperature) break;
    sleep(60);
  }
}

void ScriptableKeithley::waitForTemperatureBelow(unsigned int channel,
                                                 float temperature,
                                                 int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = keithleyModel_->getTemperature(channel);
    locker.unlock();
    if (temp<temperature) break;
    sleep(60);
  }
}

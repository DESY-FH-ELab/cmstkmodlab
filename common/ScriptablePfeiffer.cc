#include <iostream>

#include <QMutexLocker>

#include "ScriptablePfeiffer.h"

ScriptablePfeiffer::ScriptablePfeiffer(PfeifferModel* pfeifferModel, QObject *parent) :
    QObject(parent),
    pfeifferModel_(pfeifferModel)
{

}

//QScriptValue ScriptablePfeiffer::state(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(0);

//  int state = static_cast<int>(pfeifferModel_->getSensorState(channel));
//  return QScriptValue(state);
//}

//QScriptValue ScriptablePfeiffer::temperature(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(-99);

//  return QScriptValue(pfeifferModel_->getTemperature(channel));
//}

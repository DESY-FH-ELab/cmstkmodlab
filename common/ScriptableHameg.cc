#include <iostream>

#include <QMutexLocker>

#include "ScriptableHameg.h"

ScriptableHameg::ScriptableHameg(HamegModel* hamegModel, QObject *parent) :
    QObject(parent),
    hamegModel_(hamegModel)
{

}

//QScriptValue ScriptableHameg::state(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(0);

//  int state = static_cast<int>(hamegModel_->getSensorState(channel));
//  return QScriptValue(state);
//}

//QScriptValue ScriptableHameg::temperature(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(-99);

//  return QScriptValue(hamegModel_->getTemperature(channel));
//}

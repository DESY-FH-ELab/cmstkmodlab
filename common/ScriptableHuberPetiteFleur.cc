#include <iostream>

#include <QMutexLocker>

#include "ScriptableHuberPetiteFleur.h"

ScriptableHuberPetiteFleur::ScriptableHuberPetiteFleur(HuberPetiteFleurModel* huberPetiteFleurModel,
                                                       QObject *parent) :
    QObject(parent),
    huberPetiteFleurModel_(huberPetiteFleurModel)
{

}

//QScriptValue ScriptableHuberPetiteFleur::state(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(0);

//  int state = static_cast<int>(huberPetiteFleurModel_->getSensorState(channel));
//  return QScriptValue(state);
//}

//QScriptValue ScriptableHuberPetiteFleur::temperature(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(-99);

//  return QScriptValue(huberPetiteFleurModel_->getTemperature(channel));
//}

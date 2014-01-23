#include <unistd.h>

#include <iostream>

#include <QMutexLocker>

#include "ScriptableHuberPetiteFleur.h"

ScriptableHuberPetiteFleur::ScriptableHuberPetiteFleur(HuberPetiteFleurModel* huberPetiteFleurModel,
                                                       QObject *parent) :
    QObject(parent),
    huberPetiteFleurModel_(huberPetiteFleurModel)
{

}

void ScriptableHuberPetiteFleur::switchCirculatorOn() {

  QMutexLocker locker(&mutex_);
  huberPetiteFleurModel_->setCirculatorEnabled(true);
}

void ScriptableHuberPetiteFleur::switchCirculatorOff() {

  QMutexLocker locker(&mutex_);
  huberPetiteFleurModel_->setCirculatorEnabled(false);
}

void ScriptableHuberPetiteFleur::setWorkingTemperature(double temperature) {

 QMutexLocker locker(&mutex_);
 huberPetiteFleurModel_->setWorkingTemperatureValue(temperature);
}

QScriptValue ScriptableHuberPetiteFleur::isCirculatorOn() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(huberPetiteFleurModel_->isCirculatorEnabled());
}

QScriptValue ScriptableHuberPetiteFleur::getWorkingTemperature() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(huberPetiteFleurModel_->getWorkingTemperatureParameter().getValue());
}

QScriptValue ScriptableHuberPetiteFleur::getBathTemperature() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(huberPetiteFleurModel_->getBathTemperature());
}

void ScriptableHuberPetiteFleur::waitForTemperatureAbove(float temperature,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = huberPetiteFleurModel_->getBathTemperature();
    locker.unlock();
    if (temp>temperature) break;
    sleep(60);
  }
}

void ScriptableHuberPetiteFleur::waitForTemperatureBelow(float temperature,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = huberPetiteFleurModel_->getBathTemperature();
    locker.unlock();
    if (temp<temperature) break;
    sleep(60);
  }
}

void ScriptableHuberPetiteFleur::waitForStableTemperature(float deltaT,
                                                          int delay,
                                                          int timeout) {
  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = huberPetiteFleurModel_->getBathTemperature();
    locker.unlock();
    if (fabs(oldTemp-temp)<=deltaT) count++;
    oldTemp = temp;
    if (count>=delay) break;
    sleep(60);
  }
}


#include <unistd.h>

#include <iostream>

#include <QMutexLocker>

#include "ScriptableArduinoPres.h"

ScriptableArduinoPres::ScriptableArduinoPres(ArduinoPresModel* ArduinoPresModel,
                                                       QObject *parent) :
    QObject(parent),
    ArduinoPresModel_(ArduinoPresModel)
{

}

QScriptValue ScriptableArduinoPres::getPressureA() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(ArduinoPresModel_->getPressureA());
}

QScriptValue ScriptableArduinoPres::getPressureB() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(ArduinoPresModel_->getPressureB());
}

void ScriptableArduinoPres::waitForPressureAAbove(float pressure,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureA();
    locker.unlock();
    if (temp>pressure) break;
    sleep(60);
  }
}

void ScriptableArduinoPres::waitForPressureBAbove(float pressure,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureB();
    locker.unlock();
    if (temp>pressure) break;
    sleep(60);
  }
}


void ScriptableArduinoPres::waitForPressureABelow(float pressure,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureA();
    locker.unlock();
    if (temp<pressure) break;
    sleep(60);
  }
}

void ScriptableArduinoPres::waitForPressureBBelow(float pressure,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureB();
    locker.unlock();
    if (temp<pressure) break;
    sleep(60);
  }
}



void ScriptableArduinoPres::waitForStablePressureA(float deltaP,
                                                          int delay,
                                                          int timeout) {
  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureA();
    locker.unlock();
    if (fabs(oldTemp-temp)<=deltaP) count++;
    oldTemp = temp;
    if (count>=delay) break;
    sleep(60);
  }
}

void ScriptableArduinoPres::waitForStablePressureB(float deltaP,
                                                          int delay,
                                                          int timeout) {
  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureB();
    locker.unlock();
    if (fabs(oldTemp-temp)<=deltaP) count++;
    oldTemp = temp;
    if (count>=delay) break;
    sleep(60);
  }
}

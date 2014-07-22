#include <unistd.h>

#include <iostream>

#include <QMutexLocker>

#include "ScriptableIota.h"

ScriptableIota::ScriptableIota(IotaModel* IotaModel,
                                                       QObject *parent) :
    QObject(parent),
    IotaModel_(IotaModel)
{

}

void ScriptableIota::switchPumpOn() {

  QMutexLocker locker(&mutex_);
  IotaModel_->setPumpEnabled(true);
}

void ScriptableIota::switchPumpOff() {

  QMutexLocker locker(&mutex_);
  IotaModel_->setPumpEnabled(false);
}

void ScriptableIota::setPressure(float pressure) {

 QMutexLocker locker(&mutex_);
 IotaModel_->setPressureValue(pressure);
}

void ScriptableIota::setFlow(float flow) {

 QMutexLocker locker(&mutex_);
 IotaModel_->setFlowValue(flow);
}

QScriptValue ScriptableIota::isPumpOn() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(IotaModel_->isPumpEnabled());
}

QScriptValue ScriptableIota::getActPressure() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(IotaModel_->getActPressure());
}

QScriptValue ScriptableIota::getSetPressure() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(IotaModel_->getSetPressureParameter().getValue());
}

QScriptValue ScriptableIota::getActFlow() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(IotaModel_->getActFlow());
}

QScriptValue ScriptableIota::getSetFlow() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(IotaModel_->getSetFlowParameter().getValue());
}

void ScriptableIota::waitForPressureAbove(float pressure,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActPressure();
    locker.unlock();
    if (temp>pressure) break;
    sleep(60);
  }
}

void ScriptableIota::waitForFlowAbove(float flow,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActFlow();
    locker.unlock();
    if (temp>flow) break;
    sleep(60);
  }
}

void ScriptableIota::waitForPressureBelow(float pressure,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActPressure();
    locker.unlock();
    if (temp<pressure) break;
    sleep(60);
  }
}

void ScriptableIota::waitForFlowBelow(float flow,
                                                         int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActFlow();
    locker.unlock();
    if (temp<flow) break;
    sleep(60);
  }
}

void ScriptableIota::waitForStablePressure(float deltaP,
                                                          int delay,
                                                          int timeout) {
  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActPressure();
    locker.unlock();
    if (fabs(oldTemp-temp)<=deltaP) count++;
    oldTemp = temp;
    if (count>=delay) break;
    sleep(60);
  }
}

void ScriptableIota::waitForStableFlow(float deltaF,
                                                          int delay,
                                                          int timeout) {
  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActFlow();
    locker.unlock();
    if (fabs(oldTemp-temp)<=deltaF) count++;
    oldTemp = temp;
    if (count>=delay) break;
    sleep(60);
  }
}

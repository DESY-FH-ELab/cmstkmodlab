/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <chrono>
#include <thread>

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
  return QScriptValue((int)IotaModel_->isPumpEnabled());
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

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActPressure();
    locker.unlock();

    if (temp>pressure) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableIota::waitForFlowAbove(float flow,
                                      int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActFlow();
    locker.unlock();

    if (temp>flow) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableIota::waitForPressureBelow(float pressure,
                                          int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActPressure();
    locker.unlock();

    if (temp<pressure) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableIota::waitForFlowBelow(float flow,
                                      int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActFlow();
    locker.unlock();

    if (temp<flow) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableIota::waitForStablePressure(float deltaP,
                                           int delay,
                                           int timeout) {

  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActPressure();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaP) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableIota::waitForStableFlow(float deltaF,
                                       int delay,
                                       int timeout) {

  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = IotaModel_->getActFlow();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaF) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}

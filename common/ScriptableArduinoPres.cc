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

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureA();
    locker.unlock();

    if (temp>pressure) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableArduinoPres::waitForPressureBAbove(float pressure,
                                                  int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureB();
    locker.unlock();

    if (temp>pressure) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableArduinoPres::waitForPressureABelow(float pressure,
                                                  int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureA();
    locker.unlock();

    if (temp<pressure) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableArduinoPres::waitForPressureBBelow(float pressure,
                                                  int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureB();
    locker.unlock();

    if (temp<pressure) break;

    std::this_thread::sleep_for(60s);
 }
}

void ScriptableArduinoPres::waitForStablePressureA(float deltaP,
                                                   int delay,
                                                   int timeout) {

  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureA();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaP) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableArduinoPres::waitForStablePressureB(float deltaP,
                                                   int delay,
                                                   int timeout) {

  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = ArduinoPresModel_->getPressureB();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaP) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}

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

#include "ScriptableHuberUnistat525w.h"

ScriptableHuberUnistat525w::ScriptableHuberUnistat525w(HuberUnistat525wModel* model,
                                                       QObject *parent) :
    QObject(parent),
    model_(model)
{

}

void ScriptableHuberUnistat525w::setTemperatureSetPoint(double temperature)
{
  QMutexLocker locker(&mutex_);
  model_->setTemperatureSetPoint(temperature);
}

QScriptValue ScriptableHuberUnistat525w::getTemperatureSetPoint()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getTemperatureSetPoint());
}

void ScriptableHuberUnistat525w::switchTemperatureControlOn()
{
  QMutexLocker locker(&mutex_);
  model_->setTemperatureControlEnabled(true);
}

void ScriptableHuberUnistat525w::switchTemperatureControlOff()
{
  QMutexLocker locker(&mutex_);
  model_->setTemperatureControlEnabled(false);
}

QScriptValue ScriptableHuberUnistat525w::isTemperatureControlOn()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getTemperatureControlEnabled());
}

void ScriptableHuberUnistat525w::switchCirculatorOn()
{
  QMutexLocker locker(&mutex_);
  model_->setCirculatorEnabled(true);
}

void ScriptableHuberUnistat525w::switchCirculatorOff()
{
  QMutexLocker locker(&mutex_);
  model_->setCirculatorEnabled(false);
}

QScriptValue ScriptableHuberUnistat525w::isCirculatorOn()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getCirculatorEnabled());
}

QScriptValue ScriptableHuberUnistat525w::getBathTemperature()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getBathTemperature());
}

QScriptValue ScriptableHuberUnistat525w::getReturnTemperature()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getReturnTemperature());
}

QScriptValue ScriptableHuberUnistat525w::getPumpPressure()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getPumpPressure());
}

QScriptValue ScriptableHuberUnistat525w::getPower()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getPower());
}

QScriptValue ScriptableHuberUnistat525w::getCoolingWaterInletTemperature()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getCoolingWaterInletTemperature());
}

QScriptValue ScriptableHuberUnistat525w::getCoolingWaterOutletTemperature()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getCoolingWaterOutletTemperature());
}

void ScriptableHuberUnistat525w::waitForTemperatureAbove(float temperature,
                                                         int timeout)
{
  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getBathTemperature();
    locker.unlock();

    if (temp>temperature) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableHuberUnistat525w::waitForTemperatureBelow(float temperature,
                                                         int timeout)
{
  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getBathTemperature();
    locker.unlock();

    if (temp<temperature) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableHuberUnistat525w::waitForStableTemperature(float deltaT,
                                                          int delay,
                                                          int timeout)
{
  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getBathTemperature();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaT) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}


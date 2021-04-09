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
#include <cmath>
#include <QMutexLocker>

#include "ScriptableMarta.h"

ScriptableMarta::ScriptableMarta(MartaModel* model,
                                 QObject *parent)
 : QObject(parent),
   model_(model)
{

}

void ScriptableMarta::setTemperatureSetPoint(double temperature)
{
  QMutexLocker locker(&mutex_);
  model_->setTemperatureSetpoint(temperature);
}

QScriptValue ScriptableMarta::getTemperatureSetPoint()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getTemperatureSetpoint());
}

void ScriptableMarta::setSpeedSetPoint(double speed)
{
  QMutexLocker locker(&mutex_);
  model_->setSpeedSetpoint(speed);
}

QScriptValue ScriptableMarta::getSpeedSetPoint()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getSpeedSetpoint());
}

void ScriptableMarta::setFlowSetPoint(double flow)
{
  QMutexLocker locker(&mutex_);
  model_->setFlowSetpoint(flow);
}

QScriptValue ScriptableMarta::getFlowSetPoint()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getFlowSetpoint());
}

void ScriptableMarta::waitForTemperatureAbove(float temperature,
                                              int timeout)
{
  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getST04CO2();
    locker.unlock();

    if (temp>temperature) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableMarta::waitForTemperatureBelow(float temperature,
                                              int timeout)
{
  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getST04CO2();
    locker.unlock();

    if (temp<temperature) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableMarta::waitForStableTemperature(float deltaT,
                                               int delay,
                                               int timeout)
{
  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getST04CO2();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaT) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}


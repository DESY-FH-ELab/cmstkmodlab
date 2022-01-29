/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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

#include <nqlogger.h>

#include "ScriptableMarta.h"

ScriptableMarta::ScriptableMarta(MartaModel* model,
                                 QObject *parent)
 : VScriptableDevice(parent),
   model_(model),
   abortRequested_(false)
{

}

void ScriptableMarta::abort()
{
  abortRequested_ = true;
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

  model_->statusMessage(QString("wait for T > %1 deg C ...").arg(temperature));
  NQLogMessage("ScriptableHuberUnistat525w") << QString("wait for T > %1 deg C ...").arg(temperature);

  int t = 0;

  while (1) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getST04CO2();
    locker.unlock();

    if (temp>temperature) return;

    for (int s=0;s<60;++s) {
      if (abortRequested_) {
        abortRequested_ = false;

        model_->statusMessage("execution aborted");
        NQLogMessage("ScriptableMarta") << "execution aborted";

        return;
      }

      std::this_thread::sleep_for(1s);

      t++;
      if (t>timeout) {
        model_->statusMessage("timeout");
        NQLogMessage("ScriptableMarta") << "timeout";

        return;
      }
    }
  }

  model_->statusMessage("done");
  NQLogMessage("ScriptableMarta") << "done";
}

void ScriptableMarta::waitForTemperatureBelow(float temperature,
                                              int timeout)
{
  using namespace std::chrono_literals;

  model_->statusMessage(QString("wait for T < %1 deg C ...").arg(temperature));
  NQLogMessage("ScriptableHuberUnistat525w") << QString("wait for T < %1 deg C ...").arg(temperature);

  int t = 0;

  while (1) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getST04CO2();
    locker.unlock();

    if (temp<temperature) break;

    for (int s=0;s<60;++s) {
      if (abortRequested_) {
        abortRequested_ = false;

        model_->statusMessage("execution aborted");
        NQLogMessage("ScriptableMarta") << "execution aborted";

        return;
      }

      std::this_thread::sleep_for(1s);

      t++;
      if (t>timeout) {
        model_->statusMessage("timeout");
        NQLogMessage("ScriptableMarta") << "timeout";

        return;
      }
    }
  }

  model_->statusMessage("done");
  NQLogMessage("ScriptableMarta") << "done";
}

void ScriptableMarta::waitForStableTemperature(float deltaT,
                                               int delay,
                                               int timeout)
{
  using namespace std::chrono_literals;

  model_->statusMessage(QString("wait for stable T (%1)").arg(deltaT));
  NQLog("ScriptableHuberUnistat525w") << QString("wait for stable T (%1)").arg(deltaT);

  QMutexLocker locker(&mutex_);
  double oldTemp = model_->getST04CO2();
  locker.unlock();

  int t = 0;
  int count = 0;

  while (1) {

    for (int s=0;s<60;++s) {
      if (abortRequested_) {
        abortRequested_ = false;

        model_->statusMessage("execution aborted");
        NQLogMessage("ScriptableMarta") << "execution aborted";

        return;
      }

      std::this_thread::sleep_for(1s);

      t++;
      if (t>timeout) {
        model_->statusMessage("timeout");
        NQLogMessage("ScriptableMarta") << "timeout";

        return;
      }
    }

    QMutexLocker locker(&mutex_);
    double temp = model_->getST04CO2();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaT) {
      count += 60;
    } else {
      count = 0;
    }
    oldTemp = temp;
    if (count>=delay) break;
  }

  model_->statusMessage("done");
  NQLogMessage("ScriptableMarta") << "done";
}


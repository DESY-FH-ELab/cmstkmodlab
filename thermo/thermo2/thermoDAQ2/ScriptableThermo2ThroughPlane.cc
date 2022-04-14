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

#include <QMutexLocker>

#include <nqlogger.h>

#include "ScriptableThermo2ThroughPlane.h"

ScriptableThermo2ThroughPlane::ScriptableThermo2ThroughPlane(Thermo2ThroughPlaneModel* model,
                                                             QObject *parent)
  : QObject(parent),
    model_(model)
{
  connect(this, SIGNAL(changeSinkTemperature(double)),
          model_, SLOT(setTemperatureSetPoint(double)));
  connect(this, SIGNAL(changeSourcePower(double)),
          model_, SLOT(setSourcePower(double)));
}

void ScriptableThermo2ThroughPlane::abort()
{
  abortRequested_ = true;
}

QScriptValue ScriptableThermo2ThroughPlane::getSinkTemperature()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getTemperatureSetPoint();
  return QScriptValue(value);
}

void ScriptableThermo2ThroughPlane::setSinkTemperature(double temperature)
{
  NQLogDebug("ScriptableThermo2ThroughPlane") << "setSinkTemperature(double temperature) "
      << temperature;

  QMutexLocker locker(&mutex_);
  emit changeSinkTemperature(temperature);
}


QScriptValue ScriptableThermo2ThroughPlane::getSourcePower()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getSourcePower();
  return QScriptValue(value);
}

void ScriptableThermo2ThroughPlane::setSourcePower(double power)
{
  NQLogDebug("ScriptableThermo2ThroughPlane") << "setSourcePower(double power) "
      << power;

  QMutexLocker locker(&mutex_);
  emit changeSourcePower(power);
}

QScriptValue ScriptableThermo2ThroughPlane::getTopSensor(unsigned int position)
{
  QMutexLocker locker(&mutex_);
  int value = model_->getKeithleyTopSensor(position);
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getBottomSensor(unsigned int position)
{
  QMutexLocker locker(&mutex_);
  int value = model_->getKeithleyBottomSensor(position);
  return QScriptValue(value);
}


QScriptValue ScriptableThermo2ThroughPlane::getTopTemperature(unsigned int position)
{
  QMutexLocker locker(&mutex_);
  double value = model_->getKeithleyTopTemperature(position);
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getBottomTemperature(unsigned int position)
{
  QMutexLocker locker(&mutex_);
  double value = model_->getKeithleyBottomTemperature(position);
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getGradientTop()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getGradientTop();
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getPowerTop()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getPowerTop();
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getSampleTemperatureTop()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getSampleTemperatureTop();
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getSampleTemperatureMiddle()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getSampleTemperatureMiddle();
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getSampleTemperatureBottom()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getSampleTemperatureBottom();
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getGradientBottom()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getGradientBottom();
  return QScriptValue(value);
}

QScriptValue ScriptableThermo2ThroughPlane::getPowerBottom()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getPowerBottom();
  return QScriptValue(value);
}

void ScriptableThermo2ThroughPlane::waitForStableSampleTemperature(float FOMlimit,
    int delay, int timeout)
{
  using namespace std::chrono_literals;

  model_->statusMessage(QString("wait for stable sample temperature (%1)").arg(FOMlimit));
  NQLog("ScriptableThermo2ThroughPlane") << QString("wait for stable sample temperature (%1)").arg(FOMlimit);

  int t = 0;
  int count = 0;

  while (1) {

    for (int s=0;s<60;++s) {
      if (abortRequested_) {
        abortRequested_ = false;

        model_->statusMessage("execution aborted");
        NQLogMessage("ScriptableHuberUnistat525w") << "execution aborted";

        return;
      }

      std::this_thread::sleep_for(1s);

      t++;
      if (t>timeout) {
        model_->statusMessage("timeout");
        NQLogMessage("ScriptableHuberUnistat525w") << "timeout";

        return;
      }
    }

    QMutexLocker locker(&mutex_);

    /*
    const HistoryFifo<double>& sampleTMiddleHistory = model_->getSampleTemperatureMiddleHistory();
    size_t pos = sampleTMiddleHistory.indexInPast(900);
    double deltaTime = sampleTMiddleHistory.deltaTime(pos); // [s]
    double delta, gradient;
    std::pair<double,double> stats;
    double mean, variance;
    if (deltaTime>0) {
      delta = sampleTMiddleHistory.delta(pos); // [K]
      gradient = sampleTMiddleHistory.gradient(pos); // [K/s]
      stats = sampleTMiddleHistory.stats(pos);
      mean = stats.first; // [K]
      variance = stats.second; // [K^2]
    }
    */

    locker.unlock();

    /*
    if (deltaTime>0) {
      double FOM = 999;

      if (FOM<FOMlimit) break;
    }
    */
  }

  model_->statusMessage("done");
  NQLog("ScriptableThermo2ThroughPlane") << "done";
}

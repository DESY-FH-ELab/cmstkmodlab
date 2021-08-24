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

#include <QMutexLocker>

#include <nqlogger.h>

#include "ScriptableThermo2ThroughPlane.h"

ScriptableThermo2ThroughPlane::ScriptableThermo2ThroughPlane(Thermo2ThroughPlaneModel* model,
                                                             QObject *parent)
  : QObject(parent),
    model_(model)
{
  connect(this, SIGNAL(changeSinkTemperature(double)),
          model_, SLOT(setSinkTemperature(double)));
  connect(this, SIGNAL(changeSourcePower(double)),
          model_, SLOT(setSourcePower(double)));
}

QScriptValue ScriptableThermo2ThroughPlane::getSinkTemperature()
{
  QMutexLocker locker(&mutex_);
  double value = model_->getSinkTemperature();
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

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
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

#include "ScriptableHameg.h"

ScriptableHameg::ScriptableHameg(HamegModel* hamegModel, QObject *parent) :
    QObject(parent),
    hamegModel_(hamegModel)
{
  connect(this, SIGNAL(changeRemoteMode(bool)),
          hamegModel_, SLOT(setRemoteMode(bool)));
  connect(this, SIGNAL(changeOutputState(bool)),
          hamegModel_, SLOT(switchOutputOn(bool)));
  connect(this, SIGNAL(changeSetVoltage(int, float)),
          hamegModel_, SLOT(setVoltage(int, float)));
  connect(this, SIGNAL(changeSetCurrent(int, float)),
          hamegModel_, SLOT(setCurrent(int, float)));
}

QScriptValue ScriptableHameg::isRemoteMode()
{
  QMutexLocker locker(&mutex_);
  bool value = hamegModel_->isRemoteMode();
  return QScriptValue(value);
}

QScriptValue ScriptableHameg::isOutputEnabled()
{
  QMutexLocker locker(&mutex_);
  bool value = hamegModel_->isOutputEnabled();
  return QScriptValue(value);
}

QScriptValue ScriptableHameg::isConstantVoltageMode(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>2) return QScriptValue(-1);
  bool value = hamegModel_->isConstantVoltageMode(channel);
  return QScriptValue(value);
}

void ScriptableHameg::remoteOn()
{
  QMutexLocker locker(&mutex_);
  emit changeRemoteMode(true);
}

void ScriptableHameg::remoteOff()
{
  QMutexLocker locker(&mutex_);
  emit changeRemoteMode(false);
}

void ScriptableHameg::switchOutputOn()
{
  QMutexLocker locker(&mutex_);
  emit changeOutputState(true);
}

void ScriptableHameg::switchOutputOff()
{
  QMutexLocker locker(&mutex_);
  emit changeOutputState(false);
}

void ScriptableHameg::setVoltage(int channel, float voltage)
{
  if (channel<1 || channel>2 || voltage<0) return;
  QMutexLocker locker(&mutex_);
  emit changeSetVoltage(channel, voltage);
}

QScriptValue ScriptableHameg::getVoltage(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>2) return QScriptValue(-1);
  float value = hamegModel_->getVoltage(channel);
  return QScriptValue(value);
}

void ScriptableHameg::setCurrent(int channel, float current)
{
  if (channel<1 || channel>2 || current<0) return;
  QMutexLocker locker(&mutex_);
  emit changeSetCurrent(channel, current);
}

QScriptValue ScriptableHameg::getCurrent(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>2) return QScriptValue(-1);
  float value = hamegModel_->getCurrent(channel);
  return QScriptValue(value);
}

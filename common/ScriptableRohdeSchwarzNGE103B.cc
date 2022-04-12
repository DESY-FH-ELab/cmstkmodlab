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

#include <QMutexLocker>

#include <nqlogger.h>

#include "ScriptableRohdeSchwarzNGE103B.h"

ScriptableRohdeSchwarzNGE103B::ScriptableRohdeSchwarzNGE103B(RohdeSchwarzNGE103BModel* model,
                                                             QObject *parent)
  : QObject(parent),
    model_(model)
{
  connect(this, SIGNAL(changeOutputState(int, bool)),
          model_, SLOT(setOutputState(int, bool)));
  connect(this, SIGNAL(changeSetVoltage(int, float)),
          model_, SLOT(setVoltage(int, float)));
  connect(this, SIGNAL(changeSetCurrent(int, float)),
          model_, SLOT(setCurrent(int, float)));
  connect(this, SIGNAL(changeEasyRampDuration(int, float)),
          model_, SLOT(setEasyRampDuration(int, float)));
  connect(this, SIGNAL(changeEasyRampState(int, bool)),
          model_, SLOT(setEasyRampState(int, bool)));
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getOutputState(int channel)
{
  QMutexLocker locker(&mutex_);
  bool value = model_->getOutputState(channel);
  return QScriptValue(value);
}

void ScriptableRohdeSchwarzNGE103B::setOutputState(int channel, bool state)
{
  NQLogDebug("ScriptableRohdeSchwarzNGE103B") << "setOutputState(int channel, bool state) "
      << channel << " " << state;

  if (channel<1 || channel>3) return;
  QMutexLocker locker(&mutex_);
  emit changeOutputState(channel, state);
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getVoltage(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>3) return QScriptValue(-1);
  float value = model_->getVoltage(channel);
  return QScriptValue(value);
}

void ScriptableRohdeSchwarzNGE103B::setVoltage(int channel, float voltage)
{
  NQLogDebug("ScriptableRohdeSchwarzNGE103B") << "setVoltage(int channel, float voltage) "
      << channel << " " << voltage;

  if (channel<1 || channel>3 || voltage<0) return;
  QMutexLocker locker(&mutex_);
  emit changeSetVoltage(channel, voltage);
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getMeasuredVoltage(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>3) return QScriptValue(-1);
  float value = model_->getMeasuredVoltage(channel);
  return QScriptValue(value);
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getCurrent(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>3) return QScriptValue(-1);
  float value = model_->getCurrent(channel);
  return QScriptValue(value);
}

void ScriptableRohdeSchwarzNGE103B::setCurrent(int channel, float current)
{
  NQLogDebug("ScriptableRohdeSchwarzNGE103B") << "setCurrent(int channel, float current) "
      << channel << " " << current;

  if (channel<1 || channel>3 || current<0) return;
  QMutexLocker locker(&mutex_);
  emit changeSetCurrent(channel, current);
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getMeasuredCurrent(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>3) return QScriptValue(-1);
  float value = model_->getMeasuredCurrent(channel);
  return QScriptValue(value);
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getEasyRampDuration(int channel)
{
  QMutexLocker locker(&mutex_);
  if (channel<1 || channel>3) return QScriptValue(-1);
  bool value = model_->getEasyRampDuration(channel);
  return QScriptValue(value);
}

void ScriptableRohdeSchwarzNGE103B::setEasyRampDuration(int channel, float duration)
{
  NQLogDebug("ScriptableRohdeSchwarzNGE103B") << "setEasyRampDuration(int channel, float duration) "
      << channel << " " << duration;

  if (channel<1 || channel>3 || duration<0) return;
  QMutexLocker locker(&mutex_);
  emit changeEasyRampDuration(channel, duration);
}

QScriptValue ScriptableRohdeSchwarzNGE103B::getEasyRampState(int channel)
{
  QMutexLocker locker(&mutex_);
  bool value = model_->getEasyRampState(channel);
  return QScriptValue(value);
}

void ScriptableRohdeSchwarzNGE103B::setEasyRampState(int channel, bool state)
{
  NQLogDebug("ScriptableRohdeSchwarzNGE103B") << "setEasyRampState(int channel, bool state) "
      << channel << " " << state;

  if (channel<1 || channel>3) return;
  QMutexLocker locker(&mutex_);
  emit changeEasyRampState(channel, state);
}

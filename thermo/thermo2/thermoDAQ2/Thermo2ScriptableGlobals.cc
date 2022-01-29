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

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include <QMutexLocker>
#include <QDateTime>
#include <QDir>

#include <nqlogger.h>
#include <MattermostBot.h>

#include "Thermo2ScriptableGlobals.h"

Thermo2ScriptableGlobals::Thermo2ScriptableGlobals(Thermo2ScriptModel* scriptModel,
                                                   QObject *parent)
  : VScriptableDevice(parent),
    scriptModel_(scriptModel),
    abortRequested_(false)
{

}

void Thermo2ScriptableGlobals::abort()
{
  NQLogDebug("Thermo2ScriptableGlobals") << "abort()";

  abortRequested_ = true;
}

void Thermo2ScriptableGlobals::startMeasurement()
{
  NQLogDebug("Thermo2ScriptableGlobals") << "startMeasurement()";
  scriptModel_->startMeasurement();
}

void Thermo2ScriptableGlobals::stopMeasurement()
{
  NQLogDebug("Thermo2ScriptableGlobals") << "stopMeasurement()";
  scriptModel_->stopMeasurement();
}

void Thermo2ScriptableGlobals::wait(int seconds)
{
  using namespace std::chrono_literals;

  static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

  QString message;
  QDateTime dt = QDateTime::currentDateTime().addSecs(seconds);

  message = QString("wait for %1 second(s)").arg(seconds);
  this->message(message);
  NQLogMessage("Thermo2ScriptableGlobals") << message;

  message = QString("estimated time of completion: %1 ...").arg(dt.toString(TIME_FORMAT));
  this->message(message);
  NQLogMessage("Thermo2ScriptableGlobals") << message;

  for (int ts=0;ts<seconds;ts++) {
    
    if (abortRequested_) {
      abortRequested_ = false;

      message = QString("execution aborted");
      this->message(message);
      NQLogMessage("Thermo2ScriptableGlobals") << message;

      return;
    }
    
    std::this_thread::sleep_for(1s);

    if (QDateTime::currentDateTime()>=dt) break;
  }

  this->message("done");
  NQLogMessage("Thermo2ScriptableGlobals") << "done";
}

void Thermo2ScriptableGlobals::message(int value)
{
  NQLogDebug("Thermo2ScriptableGlobals") << "message(int value) " << value;
  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void Thermo2ScriptableGlobals::message(uint value)
{
  NQLogDebug("Thermo2ScriptableGlobals") << "message(uint value) " << value;
  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void Thermo2ScriptableGlobals::message(double value)
{
  NQLogDebug("Thermo2ScriptableGlobals") << "message(double value) " << value;
  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void Thermo2ScriptableGlobals::message(const QString & text)
{
  NQLogDebug("Thermo2ScriptableGlobals") << "message(const QString & text) " << text;
  QMutexLocker locker(&mutex_);
  scriptModel_->message(text);
}

void Thermo2ScriptableGlobals::log(const QString & text)
{
  QMutexLocker locker(&mutex_);
  scriptModel_->log(text);
}

QScriptValue Thermo2ScriptableGlobals::uTime() const
{
  uint utime = QDateTime::currentDateTime().toUTC().toTime_t();
  return QScriptValue(utime);
}

QScriptValue Thermo2ScriptableGlobals::eTime()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(scriptModel_->executionTime());
}

QScriptValue Thermo2ScriptableGlobals::mkUTime(int year, int month, int day,
                                               int hour, int minute, int second) const
{
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  return QScriptValue(utime);
}

void Thermo2ScriptableGlobals::mattermost(const QString& message)
{
  MattermostBot bot;
  bot.postMessage(message);
}

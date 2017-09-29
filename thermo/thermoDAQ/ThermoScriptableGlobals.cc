/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
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

#include <QMutexLocker>
#include <QDateTime>
#include <QDir>

#include <nqlogger.h>
#include <SlackBot.h>

#include "ThermoScriptableGlobals.h"

ThermoScriptableGlobals::ThermoScriptableGlobals(ThermoScriptModel* scriptModel, QObject *parent) :
    QObject(parent),
    scriptModel_(scriptModel)
{

}

void ThermoScriptableGlobals::startMeasurement() {

  scriptModel_->startMeasurement();
}

void ThermoScriptableGlobals::stopMeasurement() {

  scriptModel_->stopMeasurement();
}

void ThermoScriptableGlobals::wait(int seconds) {

  static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

  QString message;
  QDateTime dt = QDateTime::currentDateTime().addSecs(seconds);

  message = QString("wait for %1 second(s)").arg(seconds);
  this->message(message);
  NQLog("thermoDAQ") << message;

  message = QString("estimated time of completion: %1 ...").arg(dt.toString(TIME_FORMAT));
  this->message(message);
  NQLog("thermoDAQ") << message;

  sleep(seconds);

  this->message("done");
  NQLog("thermoDAQ") << "done";
}

void ThermoScriptableGlobals::message(int value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void ThermoScriptableGlobals::message(uint value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void ThermoScriptableGlobals::message(double value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void ThermoScriptableGlobals::message(const QString & text) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(text);
}

void ThermoScriptableGlobals::log(const QString & text) {

  QMutexLocker locker(&mutex_);
  scriptModel_->log(text);
}

QScriptValue ThermoScriptableGlobals::uTime() const
{
  uint utime = QDateTime::currentDateTime().toUTC().toTime_t();
  return QScriptValue(utime);
}

QScriptValue ThermoScriptableGlobals::eTime() {
  QMutexLocker locker(&mutex_);
  return QScriptValue(scriptModel_->executionTime());
}

QScriptValue ThermoScriptableGlobals::mkUTime(int year, int month, int day,
				    int hour, int minute, int second) const {
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  return QScriptValue(utime);
}

void ThermoScriptableGlobals::tweet(const QString& /* user */, const QString& /* pw */,
                                    const QString& message)
{
  slack(message);
}

void ThermoScriptableGlobals::slack(const QString& message)
{
  SlackBot bot("thermoDAQ");
  bot.postMessage(message);
}

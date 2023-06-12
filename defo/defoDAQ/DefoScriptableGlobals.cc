#include <unistd.h>

#include <iostream>

#include <QMutexLocker>
#include <QDateTime>

#include <nqlogger.h>
#include <SlackBot.h>

#include "DefoScriptableGlobals.h"

DefoScriptableGlobals::DefoScriptableGlobals(
    DefoScriptModel* scriptModel
  , QObject *parent
) :
    QObject(parent)
  , scriptModel_(scriptModel)
{ }

void DefoScriptableGlobals::wait(int seconds) {

  static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

  QString message;
  QDateTime dt = QDateTime::currentDateTime().addSecs(seconds);

  message = QString("wait for %1 second(s)").arg(seconds);
  this->message(message);
  NQLog("defoDAQ") << message;

  message = QString("estimated time of completion: %1 ...").arg(dt.toString(TIME_FORMAT));
  this->message(message);
  NQLog("defoDAQ") << message;

  sleep(seconds);

  this->message("done");
  NQLog("defoDAQ") << "done";
}

void DefoScriptableGlobals::message(int value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void DefoScriptableGlobals::message(uint value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void DefoScriptableGlobals::message(double value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void DefoScriptableGlobals::message(const QString & text) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(text);
}

void DefoScriptableGlobals::newMeasurement() {

  QMutexLocker locker(&mutex_);
  scriptModel_->doPrepareNewMeasurement();
}

QScriptValue DefoScriptableGlobals::uTime() const
{
  uint utime = QDateTime::currentDateTime().toUTC().toTime_t();
  return QScriptValue(utime);
}

QScriptValue DefoScriptableGlobals::eTime() {
  QMutexLocker locker(&mutex_);
  return QScriptValue(scriptModel_->executionTime());
}

QScriptValue DefoScriptableGlobals::mkUTime(int year, int month, int day,
				    int hour, int minute, int second) const {
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  return QScriptValue(utime);
}

void DefoScriptableGlobals::slack(const QString& message)
{
  SlackBot bot("defoDAQ", "main");
  bot.postMessage(message);
}

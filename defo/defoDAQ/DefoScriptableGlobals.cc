#include <unistd.h>

#include <iostream>

#include <QMutexLocker>
#include <QDateTime>

#include "DefoScriptableGlobals.h"

DefoScriptableGlobals::DefoScriptableGlobals(
    DefoScriptModel* scriptModel
  , QObject *parent
) :
    QObject(parent)
  , scriptModel_(scriptModel)
{ }

void DefoScriptableGlobals::newMeasurement() {

  QMutexLocker locker(&mutex_);
  scriptModel_->doPrepareNewMeasurement();
}

void DefoScriptableGlobals::wait(int seconds) {

  QString message = QString("wait for %1 second(s) ...").arg(seconds);
  this->message(message);
  sleep(seconds);
  this->message("done");
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

QScriptValue DefoScriptableGlobals::uTime() const
{
  uint utime = QDateTime::currentDateTimeUtc().toTime_t();
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

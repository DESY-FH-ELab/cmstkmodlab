#include <iostream>

#include <QMutexLocker>
#include <QDateTime>

#include "ThermoScriptableGlobals.h"

ThermoScriptableGlobals::ThermoScriptableGlobals(ThermoScriptModel* scriptModel, QObject *parent) :
    QObject(parent),
    scriptModel_(scriptModel)
{

}

//void ThermoScriptableGlobals::newMeasurement() {

//  QMutexLocker locker(&mutex_);
//  scriptModel_->doPrepareNewMeasurement();
//}

void ThermoScriptableGlobals::wait(int seconds) {

  QString message = QString("wait for %1 second(s) ...").arg(seconds);
  this->message(message);
  sleep(seconds);
  this->message("done");
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

QScriptValue ThermoScriptableGlobals::uTime() const
{
  uint utime = QDateTime::currentDateTimeUtc().toTime_t();
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

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

  std::cout << "void DefoScriptableGlobals::wait(int seconds) " << seconds << std::endl;
  sleep(seconds);
  std::cout << "void DefoScriptableGlobals::wait(int seconds) done" << std::endl;
}

void DefoScriptableGlobals::message(int value) {

  QMutexLocker locker(&mutex_);
  QString message = QString("%1")
    .arg(value);
  scriptModel_->doAppendMessageText(message);
}

void DefoScriptableGlobals::message(uint value) {

  QMutexLocker locker(&mutex_);
  QString message = QString("%1")
    .arg(value);
  scriptModel_->doAppendMessageText(message);
}

void DefoScriptableGlobals::message(double value) {

  QMutexLocker locker(&mutex_);
  QString message = QString("%1")
    .arg(value);
  scriptModel_->doAppendMessageText(message);
}

void DefoScriptableGlobals::message(const QString & text) {

  QMutexLocker locker(&mutex_);
  QString message = QString("%1")
    .arg(text);
  scriptModel_->doAppendMessageText(message);
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

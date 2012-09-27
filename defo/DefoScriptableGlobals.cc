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

void DefoScriptableGlobals::wait(int seconds) {
  sleep(seconds);
}

uint DefoScriptableGlobals::eTime() {
  QMutexLocker locker(&mutex_);
  return scriptModel_->executionTime();
}

uint DefoScriptableGlobals::uTime() {
  uint utime = QDateTime::currentDateTimeUtc().toTime_t();
  return utime;
}
  
uint DefoScriptableGlobals::mkUTime(int year, int month, int day, 
				    int hour, int minute, int second) {
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  return utime;
}

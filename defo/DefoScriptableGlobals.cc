#include <iostream>

#include <QDateTime>

#include "DefoScriptableGlobals.h"

DefoScriptableGlobals::DefoScriptableGlobals(
    DefoScriptModel* scriptModel
  , QObject *parent
) :
    QObject(parent)
  , scriptModel_(scriptModel)
{

}

void DefoScriptableGlobals::wait(int seconds) {
  std::cout << "sc: wait(" << seconds << ")" << std::endl;
  sleep(seconds);
}

uint DefoScriptableGlobals::eTime() {
  std::cout << "sc: eTime() " <<  scriptModel_->executionTime() << std::endl;
  return scriptModel_->executionTime();
}

uint DefoScriptableGlobals::uTime() {
  uint utime = QDateTime::currentDateTimeUtc().toTime_t();
  std::cout << "sc: uTime() " <<  utime << std::endl;
  return utime;
}
  
uint DefoScriptableGlobals::mkUTime(int year, int month, int day, 
				    int hour, int minute, int second) {
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  std::cout << "sc: mkUTime() " <<  utime << std::endl;
  return utime;
}

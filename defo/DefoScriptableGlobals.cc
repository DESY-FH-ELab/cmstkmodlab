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

  std::cout << "void DefoScriptableGlobals::wait(int seconds) " << seconds << std::endl;
  sleep(seconds);
  std::cout << "void DefoScriptableGlobals::wait(int seconds) done" << std::endl;
}

void DefoScriptableGlobals::print(int value) {

  std::cout << "int:    " << value << std::endl;
}

void DefoScriptableGlobals::print(uint value) {

  std::cout << "uint:   " << value << std::endl;
}

void DefoScriptableGlobals::print(double value) {

  std::cout << "double: " << value << std::endl;
}

void DefoScriptableGlobals::print(QString text) {

  std::cout << "text:   " << text.toAscii().constData() << std::endl;
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
				    int hour, int minute, int second) {
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  return QScriptValue(utime);
}

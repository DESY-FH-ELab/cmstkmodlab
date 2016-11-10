#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>

#include <QMutexLocker>
#include <QDateTime>
#include <QDir>

#include <nqlogger.h>
#include <SlackBot.h>

#include "MicroScriptableGlobals.h"

MicroScriptableGlobals::MicroScriptableGlobals(MicroScriptModel* scriptModel, QObject *parent) :
    QObject(parent),
    scriptModel_(scriptModel)
{

}

void MicroScriptableGlobals::startMeasurement() {

  scriptModel_->startMeasurement();
}

void MicroScriptableGlobals::stopMeasurement() {

  scriptModel_->stopMeasurement();
}

void MicroScriptableGlobals::wait(int seconds) {

  static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

  QString message;
  QDateTime dt = QDateTime::currentDateTime().addSecs(seconds);

  message = QString("wait for %1 second(s)").arg(seconds);
  this->message(message);
  NQLog("MicroDAQ") << message;

  message = QString("estimated time of completion: %1 ...").arg(dt.toString(TIME_FORMAT));
  this->message(message);
  NQLog("MicroDAQ") << message;

  sleep(seconds);

  this->message("done");
  NQLog("MicroDAQ") << "done";
}

void MicroScriptableGlobals::message(int value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void MicroScriptableGlobals::message(uint value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void MicroScriptableGlobals::message(double value) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(value);
}

void MicroScriptableGlobals::message(const QString & text) {

  QMutexLocker locker(&mutex_);
  scriptModel_->message(text);
}

void MicroScriptableGlobals::log(const QString & text) {

  QMutexLocker locker(&mutex_);
  scriptModel_->log(text);
}

QScriptValue MicroScriptableGlobals::uTime() const
{
  uint utime = QDateTime::currentDateTime().toUTC().toTime_t();
  return QScriptValue(utime);
}

QScriptValue MicroScriptableGlobals::eTime() {
  QMutexLocker locker(&mutex_);
  return QScriptValue(scriptModel_->executionTime());
}

QScriptValue MicroScriptableGlobals::mkUTime(int year, int month, int day,
				    int hour, int minute, int second) const {
  QDateTime dt(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
  uint utime = dt.toTime_t();
  return QScriptValue(utime);
}

void MicroScriptableGlobals::tweet(const QString& /* user */, const QString& /* pw */,
                                    const QString& message)
{
  slack(message);
}

void MicroScriptableGlobals::slack(const QString& message)
{
  SlackBot bot("microDAQ");
  bot.postMessage(message);
}

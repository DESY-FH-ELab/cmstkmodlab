#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>

#ifndef NO_TWITTER
#include <twitcurl.h>
#endif

#include <QMutexLocker>
#include <QDateTime>
#include <QDir>

#include "ThermoScriptableGlobals.h"

ThermoScriptableGlobals::ThermoScriptableGlobals(ThermoScriptModel* scriptModel, QObject *parent) :
    QObject(parent),
    scriptModel_(scriptModel)
{

}

void ThermoScriptableGlobals::startDAQ() {

  QMutexLocker locker(&mutex_);
  scriptModel_->startDAQ();
}

void ThermoScriptableGlobals::stopDAQ() {

  QMutexLocker locker(&mutex_);
  scriptModel_->stopDAQ();
}

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

void ThermoScriptableGlobals::tweet(const QString& user, const QString& pw,
                                    const QString& message)
{
#ifndef NO_TWITTER

    twitCurl twitterObj;

    std::string username = user.toStdString();
    std::string password = pw.toStdString();

    twitterObj.setTwitterUsername( username );
    twitterObj.setTwitterPassword( password );

    QDir homeDir = QDir::home();
    std::ifstream ifile(homeDir.filePath(".twitterTkModLab").toStdString().c_str());
    std::string line;
    ifile >> line; twitterObj.getOAuth().setConsumerKey(line);
    ifile >> line; twitterObj.getOAuth().setConsumerSecret(line);
    ifile >> line; twitterObj.getOAuth().setOAuthTokenKey(line);
    ifile >> line; twitterObj.getOAuth().setOAuthTokenSecret(line);
    ifile.close();

    std::string replyMsg;
    if (twitterObj.accountVerifyCredGet()) {
        twitterObj.getLastWebResponse(replyMsg);
        printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str());

        std::string m = QDateTime::currentDateTime().toString("yyMMddhhmm").toStdString();
        m += ": ";
        m += message.toStdString();
        m += " #TkModLab";
        if (twitterObj.statusUpdate(m)) {
            twitterObj.getLastWebResponse(replyMsg);
            printf("\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
        }
    }

#endif
}

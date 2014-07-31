#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include <QMutexLocker>

#include "nqlogger.h"

#include "Ringbuffer.h"
#include "ScriptableKeithley.h"

ScriptableKeithley::ScriptableKeithley(KeithleyModel* keithleyModel, QObject *parent) :
    QObject(parent),
    keithleyModel_(keithleyModel)
{

}

QScriptValue ScriptableKeithley::state(unsigned int channel) {

  QMutexLocker locker(&mutex_);

  if (channel>9) return QScriptValue(0);

  int state = static_cast<int>(keithleyModel_->getSensorState(channel));
  return QScriptValue(state);
}

QScriptValue ScriptableKeithley::temperature(unsigned int channel) {

  QMutexLocker locker(&mutex_);

  if (channel>9) return QScriptValue(-99);

  return QScriptValue(keithleyModel_->getTemperature(channel));
}

QScriptValue ScriptableKeithley::temperatureAsString(unsigned int channel)
{
  QMutexLocker locker(&mutex_);
  
  if (channel>9) return QScriptValue("-99");
  
  return QScriptValue(QString::number(keithleyModel_->getTemperature(channel), 'f', 2));
}

void ScriptableKeithley::waitForStableTemperature(const QString & channels,
						  int timeout) {

  std::vector<unsigned int> activeChannels;
  std::istringstream iss(channels.toStdString().c_str());
  int c;
  QString channelString;
  while (iss >> c) {
    if (c>=0 && c<10) {
      activeChannels.push_back(c);
      channelString += QString("%1 ").arg(c);
    }
  }

  if (activeChannels.size()>10 || activeChannels.size()==0) {
    return;
  }

  keithleyModel_->statusMessage(QString("wait for stable temperatures on channels %1 ...").arg(channelString));
  NQLog("keithley") << QString("wait for stable temperatures on channels %1 ...").arg(channelString);

  Ringbuffer<float,10> buffer[10];
  for (int c=0;c<10;++c) {
    for (int b=0;b<10;++b) {
      buffer[c].push_back(-999);
    }
  }

  float current[10];

  int t = 0;
  while (t<=timeout) {

    bool stable = true;
    for (std::vector<unsigned int>::iterator it = activeChannels.begin();
	 it!=activeChannels.end();
	 ++it) {
      current[*it] = keithleyModel_->getTemperature(*it);
      buffer[*it].push_back(current[*it]);

      float delta = current[*it]-buffer[*it].get();
      NQLog("keithley") << QString("dT(%1) = %2").arg(*it).arg(delta);

      if (std::fabs(delta)>=0.01) stable = false;
    }
    if (stable) break;

    sleep(60);
    t += 60;
  }

  keithleyModel_->statusMessage("done");
  NQLog("keithley") << "done";
}

void ScriptableKeithley::waitForTemperatureAbove(unsigned int channel,
                                                 float temperature,
                                                 int timeout) {

  keithleyModel_->statusMessage(QString("wait for T(%1) > %2 deg C ...").arg(channel).arg(temperature));
  NQLog("keithley") << QString("wait for T(%1) > %2 deg C ...").arg(channel).arg(temperature);

  for (int m=0;m<=timeout;m+=60) {
    QMutexLocker locker(&mutex_);
    double temp = keithleyModel_->getTemperature(channel);
    locker.unlock();
    if (temp>temperature) break;
    sleep(60);
  }

  keithleyModel_->statusMessage("done");
  NQLog("keithley") << "done";
}

void ScriptableKeithley::waitForTemperatureBelow(unsigned int channel,
                                                 float temperature,
                                                 int timeout) {

  keithleyModel_->statusMessage(QString("wait for T(%1) < %2 deg C ...").arg(channel).arg(temperature));
  NQLog("keithley") << QString("wait for T(%1) < %2 deg C ...").arg(channel).arg(temperature);

  for (int m=0;m<=timeout;m+=60) {
    QMutexLocker locker(&mutex_);
    double temp = keithleyModel_->getTemperature(channel);
    locker.unlock();
    if (temp<temperature) break;
    sleep(60);
  }

  keithleyModel_->statusMessage("done");
  NQLog("keithley") << "done";
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <cmath>

#include <QMutexLocker>

#include "nqlogger.h"

#include "Ringbuffer.h"
#include "ScriptableKeithleyDAQ6510.h"

ScriptableKeithleyDAQ6510::ScriptableKeithleyDAQ6510(KeithleyDAQ6510Model* keithleyModel,
                                                     QObject *parent)
 : QObject(parent),
   keithleyModel_(keithleyModel)
{

}

QScriptValue ScriptableKeithleyDAQ6510::state(unsigned int sensor)
{
  QMutexLocker locker(&mutex_);

  unsigned int card = sensor / 100;
  unsigned int channel = sensor % 100;

  if (card<1 || card>2) return QScriptValue(0);
  if (channel<1 || channel>10) return QScriptValue(0);

  int state = static_cast<int>(keithleyModel_->getSensorState(sensor));
  return QScriptValue(state);
}

QScriptValue ScriptableKeithleyDAQ6510::temperature(unsigned int sensor)
{
  QMutexLocker locker(&mutex_);

  unsigned int card = sensor / 100;
  unsigned int channel = sensor % 100;

  if (card<1 || card>2) return QScriptValue(0);
  if (channel<1 || channel>10) return QScriptValue(0);

  return QScriptValue(keithleyModel_->getTemperature(sensor));
}

QScriptValue ScriptableKeithleyDAQ6510::temperatureAsString(unsigned int sensor)
{
  QMutexLocker locker(&mutex_);
  
  unsigned int card = sensor / 100;
  unsigned int channel = sensor % 100;

  if (card<1 || card>2) return QScriptValue("-99");
  if (channel<1 || channel>10) return QScriptValue("-99");

  return QScriptValue(QString::number(keithleyModel_->getTemperature(sensor), 'f', 2));
}

void ScriptableKeithleyDAQ6510::setUpdateInterval(int updateInterval)
{
  QMutexLocker locker(&mutex_);

  keithleyModel_->setUpdateInterval(updateInterval);
}

void ScriptableKeithleyDAQ6510::waitForStableTemperature(const QString & sensors,
                                                         int timeout)
{
  using namespace std::chrono_literals;

  std::vector<unsigned int> activeSensors;
  std::istringstream iss(sensors.toStdString().c_str());
  int c;
  QString sensorString;

  while (iss >> c) {
    unsigned int card = c / 100;
    unsigned int channel = c % 100;

    if (card>=1 && card<=2 && channel>=1 && channel<=10) {
      activeSensors.push_back(c);
      sensorString += QString("%1 ").arg(c);
    }
  }

  if (activeSensors.size()>20 || activeSensors.size()==0) {
    return;
  }

  keithleyModel_->statusMessage(QString("wait for stable temperatures on sensors %1 ...").arg(sensorString));
  NQLog("keithley") << QString("wait for stable temperatures on sensors %1 ...").arg(sensorString);

  std::map<unsigned int,Ringbuffer<float,10>> buffer;
  for (unsigned int card=1;card<=2;++card) {
    for (unsigned int channel=1;channel<=10;++channel) {
      buffer[card*100+channel].push_back(-999);
    }
  }
  std::map<unsigned int,float> current;

  int t = 0;
  while (t<=timeout) {

    bool stable = true;
    float temperature;
    for (unsigned int sensor : activeSensors) {
      temperature = keithleyModel_->getTemperature(sensor);

      current[sensor] = temperature;
      buffer[sensor].push_back(temperature);

      float delta = temperature-buffer[sensor].get();
      NQLog("keithley") << QString("dT(%1) = %2").arg(sensor).arg(delta);

      if (std::fabs(delta)>=0.01) stable = false;
    }
    if (stable) break;

    std::this_thread::sleep_for(60s);

    t++;
  }

  keithleyModel_->statusMessage("done");
  NQLog("keithley") << "done";
}

void ScriptableKeithleyDAQ6510::waitForTemperatureAbove(unsigned int sensor,
                                                        float temperature,
                                                        int timeout)
{
  using namespace std::chrono_literals;

  keithleyModel_->statusMessage(QString("wait for T(%1) > %2 deg C ...").arg(sensor).arg(temperature));
  NQLog("keithley") << QString("wait for T(%1) > %2 deg C ...").arg(sensor).arg(temperature);

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = keithleyModel_->getTemperature(sensor);
    locker.unlock();

    if (temp>temperature) break;

    std::this_thread::sleep_for(60s);
  }

  keithleyModel_->statusMessage("done");
  NQLog("keithley") << "done";
}

void ScriptableKeithleyDAQ6510::waitForTemperatureBelow(unsigned int sensor,
                                                        float temperature,
                                                        int timeout)
{
  using namespace std::chrono_literals;

  keithleyModel_->statusMessage(QString("wait for T(%1) < %2 deg C ...").arg(sensor).arg(temperature));
  NQLog("keithley") << QString("wait for T(%1) < %2 deg C ...").arg(sensor).arg(temperature);

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = keithleyModel_->getTemperature(sensor);
    locker.unlock();

    if (temp<temperature) break;

    std::this_thread::sleep_for(60s);
  }

  keithleyModel_->statusMessage("done");
  NQLog("keithley") << "done";
}

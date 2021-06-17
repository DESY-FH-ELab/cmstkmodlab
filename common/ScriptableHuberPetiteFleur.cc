/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
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

#include <QMutexLocker>

#include "ScriptableHuberPetiteFleur.h"

ScriptableHuberPetiteFleur::ScriptableHuberPetiteFleur(HuberPetiteFleurModel* huberPetiteFleurModel,
                                                       QObject *parent) :
    QObject(parent),
    huberPetiteFleurModel_(huberPetiteFleurModel)
{

}

void ScriptableHuberPetiteFleur::switchCirculatorOn() {

  QMutexLocker locker(&mutex_);
  huberPetiteFleurModel_->setCirculatorEnabled(true);
}

void ScriptableHuberPetiteFleur::switchCirculatorOff() {

  QMutexLocker locker(&mutex_);
  huberPetiteFleurModel_->setCirculatorEnabled(false);
}

void ScriptableHuberPetiteFleur::setWorkingTemperature(double temperature) {

 QMutexLocker locker(&mutex_);
 huberPetiteFleurModel_->setWorkingTemperatureValue(temperature);
}

QScriptValue ScriptableHuberPetiteFleur::isCirculatorOn() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(huberPetiteFleurModel_->isCirculatorEnabled());
}

QScriptValue ScriptableHuberPetiteFleur::getWorkingTemperature() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(huberPetiteFleurModel_->getWorkingTemperatureParameter().getValue());
}

QScriptValue ScriptableHuberPetiteFleur::getBathTemperature() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(huberPetiteFleurModel_->getBathTemperature());
}

void ScriptableHuberPetiteFleur::waitForTemperatureAbove(float temperature,
                                                         int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = huberPetiteFleurModel_->getBathTemperature();
    locker.unlock();

    if (temp>temperature) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableHuberPetiteFleur::waitForTemperatureBelow(float temperature,
                                                         int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = huberPetiteFleurModel_->getBathTemperature();
    locker.unlock();

    if (temp<temperature) break;

    std::this_thread::sleep_for(60s);
  }
}

void ScriptableHuberPetiteFleur::waitForStableTemperature(float deltaT,
                                                          int delay,
                                                          int timeout) {
  using namespace std::chrono_literals;

  int count = 0;
  double oldTemp = -999;
  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = huberPetiteFleurModel_->getBathTemperature();
    locker.unlock();

    if (fabs(oldTemp-temp)<=deltaT) count++;
    oldTemp = temp;
    if (count>=delay) break;

    std::this_thread::sleep_for(60s);
  }
}


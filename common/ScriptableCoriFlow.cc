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

#include "ScriptableCoriFlow.h"

ScriptableCoriFlow::ScriptableCoriFlow(CoriFlowModel* CoriFlowModel,
                                             QObject *parent) :
    QObject(parent),
    CoriFlowModel_(CoriFlowModel)
{

}

QScriptValue ScriptableCoriFlow::getTemp() {

  QMutexLocker locker(&mutex_);
  return QScriptValue(CoriFlowModel_->getTemp());
}


void ScriptableCoriFlow::waitForTempBelow(float temperature,
                                          int timeout) {

  using namespace std::chrono_literals;

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = CoriFlowModel_->getTemp();
    locker.unlock();

    if (temp<temperature) break;

    std::this_thread::sleep_for(60s);
  }
}


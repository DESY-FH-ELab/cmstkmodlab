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

#include <unistd.h>

#include <iostream>

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


void ScriptableCoriFlow::waitForTempBelow(float /* temperature */,
                                                  int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = CoriFlowModel_->getTemp();
    locker.unlock();
    if (temp<temp) break;
    sleep(60);
  }
}


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


void ScriptableCoriFlow::waitForTempBelow(float temperature,
                                                  int timeout) {

  for (int m=0;m<=timeout;++m) {
    QMutexLocker locker(&mutex_);
    double temp = CoriFlowModel_->getTemp();
    locker.unlock();
    if (temp<temp) break;
    sleep(60);
  }
}


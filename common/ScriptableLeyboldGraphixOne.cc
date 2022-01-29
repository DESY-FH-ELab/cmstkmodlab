/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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
#include <cmath>
#include <QMutexLocker>

#include <nqlogger.h>

#include "ScriptableLeyboldGraphixOne.h"

ScriptableLeyboldGraphixOne::ScriptableLeyboldGraphixOne(LeyboldGraphixOneModel* model,
							 QObject *parent)
 : VScriptableDevice(parent),
   model_(model),
   abortRequested_(false)
{

}

void ScriptableLeyboldGraphixOne::abort()
{
  abortRequested_ = true;
}

QScriptValue ScriptableLeyboldGraphixOne::getPressure()
{
  QMutexLocker locker(&mutex_);
  return QScriptValue(model_->getPressure());
}

void ScriptableLeyboldGraphixOne::waitForPressureBelow(float pressure,
						                               int timeout)
{
  using namespace std::chrono_literals;

  model_->statusMessage(QString("wait for pressure < %1 ...").arg(pressure));
  NQLogMessage("ScriptableLeyboldGraphixOne") << QString("wait for pressure < %1 ...").arg(pressure);

  int t = 0;

  while (1) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getPressure();
    locker.unlock();

    if (temp<pressure) break;

    for (int s=0;s<60;++s) {
      if (abortRequested_) {
        abortRequested_ = false;

        model_->statusMessage("execution aborted");
        NQLogMessage("ScriptableLeyboldGraphixOne") << "execution aborted";

        return;
      }

      std::this_thread::sleep_for(1s);

      t++;
      if (t>timeout) {
        model_->statusMessage("timeout");
        NQLogMessage("ScriptableLeyboldGraphixOne") << "timeout";

        return;
      }
    }
  }

  model_->statusMessage("done");
  NQLogMessage("ScriptableLeyboldGraphixOne") << "done";
}

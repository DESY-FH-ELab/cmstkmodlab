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
  NQLogDebug("ScriptableLeyboldGraphixOne") << "abort()";

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

  for (int m=0;m<=timeout;++m) {

    QMutexLocker locker(&mutex_);
    double temp = model_->getPressure();
    locker.unlock();

    if (temp<pressure) break;

    if (abortRequested_) {
      abortRequested_ = false;
      
      NQLogMessage("ScriptableLeyboldGraphixOne") << "execution aborted";

      break;
    }

    std::this_thread::sleep_for(60s);
  }
}

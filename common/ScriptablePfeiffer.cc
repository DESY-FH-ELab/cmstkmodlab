/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QMutexLocker>

#include "ScriptablePfeiffer.h"

ScriptablePfeiffer::ScriptablePfeiffer(PfeifferModel* pfeifferModel, QObject *parent) :
    QObject(parent),
    pfeifferModel_(pfeifferModel)
{

}

//QScriptValue ScriptablePfeiffer::state(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(0);

//  int state = static_cast<int>(pfeifferModel_->getSensorState(channel));
//  return QScriptValue(state);
//}

//QScriptValue ScriptablePfeiffer::temperature(unsigned int channel) {

//  QMutexLocker locker(&mutex_);

//  if (channel>9) return QScriptValue(-99);

//  return QScriptValue(pfeifferModel_->getTemperature(channel));
//}

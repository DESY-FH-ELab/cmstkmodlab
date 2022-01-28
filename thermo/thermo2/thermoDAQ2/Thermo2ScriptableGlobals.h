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

#ifndef THERMO2SCRIPTABLEGLOBALS_H
#define THERMO2SCRIPTABLEGLOBALS_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <VScriptableDevice.h>

#include "Thermo2ScriptModel.h"

class Thermo2ScriptableGlobals : public VScriptableDevice
{
  Q_OBJECT

public:
  explicit Thermo2ScriptableGlobals(Thermo2ScriptModel* scriptModel, QObject *parent = 0);

  void abort();
  
public slots:

  void startMeasurement();
  void startDAQ() { startMeasurement(); }
  void stopMeasurement();
  void stopDAQ() { stopMeasurement(); }

  void wait(int seconds);
  void message(int value);
  void message(uint value);
  void message(double value);
  void message(const QString & text);
  void log(const QString & text);
  QScriptValue uTime() const;
  QScriptValue eTime();
  QScriptValue mkUTime(int year, int month, int day,
                       int hour, int minute, int second) const;

  void mattermost(const QString& message);

protected:

  Thermo2ScriptModel* scriptModel_;

  bool abortRequested_;
  
  // For thread safety
  QMutex mutex_;
};

#endif // THERMO2SCRIPTABLEGLOBALS_H

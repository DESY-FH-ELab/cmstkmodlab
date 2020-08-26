/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMOSCRIPTABLEGLOBALS_H
#define THERMOSCRIPTABLEGLOBALS_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "ThermoScriptModel.h"

class ThermoScriptableGlobals :
      public QObject
{
  Q_OBJECT

public:
  explicit ThermoScriptableGlobals(ThermoScriptModel* scriptModel, QObject *parent = 0);

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

  void tweet(const QString& user, const QString& pw,
             const QString& message);

  void slack(const QString& message);

protected:

  ThermoScriptModel* scriptModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // THERMOSCRIPTABLEGLOBALS_H

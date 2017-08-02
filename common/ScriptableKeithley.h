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

#ifndef SCRIPTABLEKEITHLEY_H
#define SCRIPTABLEKEITHLEY_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <KeithleyModel.h>

class ScriptableKeithley : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableKeithley(KeithleyModel* keithleyModel, QObject *parent = 0);

public slots:

  QScriptValue state(unsigned int channel);
  QScriptValue temperature(unsigned int channel);
  QScriptValue temperatureAsString(unsigned int channel);

  void waitForStableTemperature(const QString & channels,
                                int timeout);

  void waitForTemperatureAbove(unsigned int channel,
                               float temperature,
                               int timeout);
  void waitForTemperatureBelow(unsigned int channel,
                               float temperature,
                               int timeout);

protected:
  KeithleyModel* keithleyModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEKEITHLEY_H

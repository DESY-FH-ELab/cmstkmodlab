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

#ifndef _SCRIPTABLEHUBERUNISTAT525W_H_
#define _SCRIPTABLEHUBERUNISTAT525W_H_

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <HuberUnistat525wModel.h>

class ScriptableHuberUnistat525w : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableHuberUnistat525w(HuberUnistat525wModel* model,
                                      QObject *parent = 0);

public slots:

  void setTemperatureSetPoint(double temperature);
  QScriptValue getTemperatureSetPoint();

  void switchTemperatureControlOn();
  void switchTemperatureControlOff();
  QScriptValue isTemperatureControlOn();

  void switchCirculatorOn();
  void switchCirculatorOff();
  QScriptValue isCirculatorOn();

  QScriptValue getBathTemperature();
  QScriptValue getReturnTemperature();

  QScriptValue getPumpPressure();
  QScriptValue getPower();

  QScriptValue getCoolingWaterInletTemperature();
  QScriptValue getCoolingWaterOutletTemperature();

  void waitForTemperatureAbove(float temperature,
                               int timeout);
  void waitForTemperatureBelow(float temperature,
                               int timeout);
  void waitForStableTemperature(float deltaT,
                                int delay,
                                int timeout);

protected:

  HuberUnistat525wModel* model_;

  // For thread safety
  QMutex mutex_;
};

#endif // _SCRIPTABLEHUBERUNISTAT525W_H_

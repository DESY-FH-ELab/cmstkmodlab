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

#ifndef _SCRIPTABLEMARTA_H_
#define _SCRIPTABLEMARTA_H_

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <VScriptableDevice.h>

#include <MartaModel.h>

class ScriptableMarta : public VScriptableDevice
{
  Q_OBJECT
public:
  explicit ScriptableMarta(MartaModel* model,
      QObject *parent = 0);

  void abort();

public slots:

  void setTemperatureSetPoint(double temperature);
  QScriptValue getTemperatureSetPoint();

  void setSpeedSetPoint(double speed);
  QScriptValue getSpeedSetPoint();

  void setFlowSetPoint(double flow);
  QScriptValue getFlowSetPoint();

  void waitForTemperatureAbove(float temperature,
                               int timeout);
  void waitForTemperatureBelow(float temperature,
                               int timeout);
  void waitForStableTemperature(float deltaT,
                                int delay,
                                int timeout);

protected:

  MartaModel* model_;

  bool abortRequested_;

  // For thread safety
  QMutex mutex_;
};

#endif // _SCRIPTABLEMARTA_H_

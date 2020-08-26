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

#ifndef SCRIPTABLEIOTA_H
#define SCRIPTABLEIOTA_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <IotaModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableIota : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableIota(IotaModel* petiteFleurModel,
                          QObject *parent = 0);

  public slots:

  void switchPumpOn();
  void switchPumpOff();
  void setPressure(float pressure);
  void setFlow(float flow);

  QScriptValue isPumpOn();
  QScriptValue getActPressure();
  QScriptValue getActFlow();
  QScriptValue getSetPressure();
  QScriptValue getSetFlow();

  void waitForPressureAbove(float pressure,
                            int timeout);
  void waitForPressureBelow(float pressure,
                            int timeout);
  void waitForStablePressure(float deltaP,
                             int delay,
                             int timeout);
  void waitForFlowAbove(float flow,
                        int timeout);
  void waitForFlowBelow(float flow,
                        int timeout);
  void waitForStableFlow(float deltaF,
                         int delay,
                         int timeout);

  protected:
  IotaModel* IotaModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEIOTA_H

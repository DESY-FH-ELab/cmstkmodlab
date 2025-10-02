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

#ifndef SCRIPTABLEHUBERPETITEFLEUR_H
#define SCRIPTABLEHUBERPETITEFLEUR_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <HuberPetiteFleurModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableHuberPetiteFleur : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableHuberPetiteFleur(HuberPetiteFleurModel* petiteFleurModel,
                                      QObject *parent = 0);

public slots:

  void switchCirculatorOn();
  void switchCirculatorOff();
  void setWorkingTemperature(double temperature);

  QScriptValue isCirculatorOn();
  QScriptValue getWorkingTemperature();
  QScriptValue getBathTemperature();

  void waitForTemperatureAbove(float temperature,
                               int timeout);
  void waitForTemperatureBelow(float temperature,
                               int timeout);
  void waitForStableTemperature(float deltaT,
                                int delay,
                                int timeout);

protected:
  HuberPetiteFleurModel* huberPetiteFleurModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEHUBERPETITEFLEUR_H

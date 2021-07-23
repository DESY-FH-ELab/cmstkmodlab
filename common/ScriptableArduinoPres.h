/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef SCRIPTABLEARDUINOPRES_H
#define SCRIPTABLEARDUINOPRES_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <ArduinoPresModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableArduinoPres : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableArduinoPres(ArduinoPresModel* arduinoPresModel,
                                 QObject *parent = 0);

  public slots:

  QScriptValue getPressureA();
  QScriptValue getPressureB();
  

  void waitForPressureAAbove(float pressure,int timeout);
  void waitForPressureBAbove(float pressure,int timeout);
  void waitForPressureABelow(float pressure,int timeout);
  void waitForPressureBBelow(float pressure,int timeout);
  void waitForStablePressureA(float deltaP,int delay,int timeout);
  void waitForStablePressureB(float deltaP,int delay,int timeout);


  protected:
  ArduinoPresModel* ArduinoPresModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEARDUINOPRES_H

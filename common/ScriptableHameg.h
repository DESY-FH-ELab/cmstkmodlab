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

#ifndef SCRIPTABLEHAMEG_H
#define SCRIPTABLEHAMEG_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <HamegModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableHameg : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableHameg(HamegModel* hamegModel, QObject *parent = 0);

public slots:

  QScriptValue isRemoteMode();
  QScriptValue isOutputEnabled();
  QScriptValue isConstantVoltageMode(int channel);

  void remoteOn();
  void remoteOff();

  void switchOutputOn();
  void switchOutputOff();

  void setVoltage(int channel, float voltage);
  QScriptValue getVoltage(int channel);
  void setCurrent(int channel, float current);
  QScriptValue getCurrent(int channel);

signals:

  void changeRemoteMode(bool);
  void changeOutputState(bool);
  void changeSetVoltage(int, float);
  void changeSetCurrent(int, float);

protected:
  HamegModel* hamegModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEHAMEG_H

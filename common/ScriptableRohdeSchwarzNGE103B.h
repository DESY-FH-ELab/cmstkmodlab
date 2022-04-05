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

#ifndef SCRIPTABLEROHDESCHWARZNGE103B_H
#define SCRIPTABLEROHDESCHWARZNGE103B_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <RohdeSchwarzNGE103BModel.h>

class ScriptableRohdeSchwarzNGE103B : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableRohdeSchwarzNGE103B(RohdeSchwarzNGE103BModel* model, QObject *parent = 0);

public slots:

  QScriptValue getOutputState(int channel);
  void setOutputState(int channel, bool state);

  QScriptValue getVoltage(int channel);
  void setVoltage(int channel, float voltage);
  QScriptValue getMeasuredVoltage(int channel);

  QScriptValue getCurrent(int channel);
  void setCurrent(int channel, float current);
  QScriptValue getMeasuredCurrent(int channel);

  QScriptValue getEasyRampDuration(int channel);
  void setEasyRampDuration(int channel, float voltage);

  QScriptValue getEasyRampState(int channel);
  void setEasyRampState(int channel, bool state);

signals:

  void changeOutputState(int, bool);
  void changeSetVoltage(int, float);
  void changeSetCurrent(int, float);
  void changeEasyRampDuration(int, float);
  void changeEasyRampState(int, bool);

protected:

  RohdeSchwarzNGE103BModel* model_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEROHDESCHWARZNGE103B_H

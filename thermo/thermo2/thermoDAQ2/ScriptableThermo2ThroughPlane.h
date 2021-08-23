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

#ifndef SCRIPTABLETHERMO2THROUGHPLANE_H
#define SCRIPTABLETHERMO2THROUGHPLANE_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <Thermo2ThroughPlaneModel.h>

class ScriptableThermo2ThroughPlane : public QObject
{
  Q_OBJECT
public:

  explicit ScriptableThermo2ThroughPlane(Thermo2ThroughPlaneModel* model, QObject *parent = 0);

public slots:

  /*
  QScriptValue getOutputState(int channel);
  void setOutputState(int channel, bool state);

  QScriptValue getVoltage(int channel);
  void setVoltage(int channel, float voltage);
  QScriptValue getMeasuredVoltage(int channel);
  QScriptValue getMeasuredVoltageHistory(int channel, int secondsAgo);

  QScriptValue getCurrent(int channel);
  void setCurrent(int channel, float current);
  QScriptValue getMeasuredCurrent(int channel);
  QScriptValue getMeasuredCurrentHistory(int channel, int secondsAgo);

  QScriptValue getEasyRampDuration(int channel);
  void setEasyRampDuration(int channel, float voltage);

  QScriptValue getEasyRampState(int channel);
  void setEasyRampState(int channel, bool state);
  */

signals:

  /*
  void changeOutputState(int, bool);
  void changeSetVoltage(int, float);
  void changeSetCurrent(int, float);
  void changeEasyRampDuration(int, float);
  void changeEasyRampState(int, bool);
  */

protected:

  Thermo2ThroughPlaneModel* model_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLETHERMO2THROUGHPLANE_H

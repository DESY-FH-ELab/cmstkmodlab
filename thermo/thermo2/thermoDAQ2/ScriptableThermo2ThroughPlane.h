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

  explicit ScriptableThermo2ThroughPlane(Thermo2ThroughPlaneModel* model,
      QObject *parent = 0);

  void abort();

public slots:

  QScriptValue getSinkTemperature();
  void setSinkTemperature(double temperature);

  QScriptValue getSourcePower();
  void setSourcePower(double power);

  QScriptValue getTopSensor(unsigned int position);
  QScriptValue getBottomSensor(unsigned int position);

  QScriptValue getTopTemperature(unsigned int position);
  QScriptValue getBottomTemperature(unsigned int position);

  QScriptValue getGradientTop();
  QScriptValue getPowerTop();
  QScriptValue getSampleTemperatureTop();
  QScriptValue getSampleTemperatureMiddle();
  QScriptValue getSampleTemperatureBottom();
  QScriptValue getGradientBottom();
  QScriptValue getPowerBottom();

  void waitForStableSampleTemperature(float FOMlimit, int delay, int timeout);

signals:

  void changeSinkTemperature(double);
  void changeSourcePower(double);

protected:

  Thermo2ThroughPlaneModel* model_;

  bool abortRequested_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLETHERMO2THROUGHPLANE_H

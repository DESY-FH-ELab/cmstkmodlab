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

#ifndef SCRIPTABLEKEITHLEYDAQ6510_H
#define SCRIPTABLEKEITHLEYDAQ6510_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <VScriptableDevice.h>

#include <KeithleyDAQ6510Model.h>

class ScriptableKeithleyDAQ6510 : public VScriptableDevice
{
  Q_OBJECT
public:
  explicit ScriptableKeithleyDAQ6510(KeithleyDAQ6510Model* keithleyModel,
                                     QObject *parent = 0);

  void abort();

public slots:

  QScriptValue state(unsigned int sensor);
  QScriptValue temperature(unsigned int sensor);
  QScriptValue temperatureAsString(unsigned int sensor);

  void setUpdateInterval(int updateInterval);

  void waitForStableTemperature(const QString & sensor,
                                int timeout);

  void waitForTemperatureAbove(unsigned int sensor,
                               float temperature,
                               int timeout);
  void waitForTemperatureBelow(unsigned int sensor,
                               float temperature,
                               int timeout);

protected:
  KeithleyDAQ6510Model* keithleyModel_;

  bool abortRequested_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEKEITHLEYDAQ6510_H

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMOSCRIPTMODEL_H
#define THERMOSCRIPTMODEL_H

#include <QObject>
#include <QTimer>
#include <QTextDocument>

#include "ThermoDAQModel.h"
#include "HuberPetiteFleurModel.h"
#include "KeithleyModel.h"
#include "HamegModel.h"
#include "PfeifferModel.h"
#include "IotaModel.h"
#include "ArduinoPresModel.h"

#include "ThermoScriptThread.h"

class ThermoScriptModel : public QObject
{
    Q_OBJECT
public:

  explicit ThermoScriptModel(ThermoDAQModel* daqModel,
                             HuberPetiteFleurModel* huberModel,
                             KeithleyModel* keithleyModel,
                             HamegModel* hamegModel,
                             PfeifferModel* pfeifferModel,
                             IotaModel* iotaModel,
                             ArduinoPresModel* arduinoPresModel,
                             QObject *parent = 0);

  QTextDocument* scriptDocument() { return script_; }
  const QString& currentScriptFilename() const { return currentScriptFilename_; }

  void openScript(const QString filename);
  void saveScript(const QString filename);
  void executeScript();
  void abortScript();
  int executionTime() const { return executionTime_; }

  void startMeasurement();
  void stopMeasurement();
  void message(int value);
  void message(uint value);
  void message(double value);
  void message(const QString & text);
  void log(const QString & text);

public slots:
  void executionStarted();
  void executionFinished();
  void doClearMessageText();
  void doAppendMessageText(const QString & text);

protected slots:
  void executionHeartBeat();

protected:

  QTextDocument* script_;
  QString currentScriptFilename_;

  QTimer executionTimer_;
  int executionTime_;
  ThermoDAQModel* daqModel_;
  HuberPetiteFleurModel* huberModel_;
  KeithleyModel* keithleyModel_;
  HamegModel* hamegModel_;
  PfeifferModel* pfeifferModel_;
  IotaModel* iotaModel_;
  ArduinoPresModel* arduinoPresModel_;
  ThermoScriptThread* scriptThread_;

signals:
  void prepareNewMeasurement();
  void scriptLoaded();
  void setControlsEnabled(bool enabled);
  void clearMessageText();
  void appendMessageText(const QString & text);
};

#endif // THERMOSCRIPTMODEL_H

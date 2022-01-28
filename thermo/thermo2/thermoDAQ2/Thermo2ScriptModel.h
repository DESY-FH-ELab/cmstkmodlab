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

#ifndef THERMO2SCRIPTMODEL_H
#define THERMO2SCRIPTMODEL_H

#include <QObject>
#include <QTimer>
#include <QTextDocument>

#include "Thermo2DAQModel.h"

#include "LeyboldGraphixOneModel.h"
#include "HuberUnistat525wModel.h"
#include "MartaModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"
#include "Thermo2ThroughPlaneModel.h"

#include "Thermo2ScriptThread.h"

class Thermo2ScriptModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2ScriptModel(Thermo2DAQModel* daqModel,
			      LeyboldGraphixOneModel* leyboldModel,
			      HuberUnistat525wModel* huberModel,
			      MartaModel* martaModel,
			      RohdeSchwarzNGE103BModel* nge103BModel,
			      KeithleyDAQ6510Model* keithleyModel,
			      Thermo2ThroughPlaneModel* t2tpModel,
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

public slots:
  void executionStarted();
  void executionFinished();
  void doClearMessageText();
  void doAppendMessageText(const QString & text);
  void log(const QString & text);

protected slots:
  void executionHeartBeat();

protected:

  QTextDocument* script_;
  QString currentScriptFilename_;

  QTimer executionTimer_;
  int executionTime_;

  Thermo2DAQModel* daqModel_;

  LeyboldGraphixOneModel* leyboldModel_;
  HuberUnistat525wModel* huberModel_;
  MartaModel* martaModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;
  Thermo2ThroughPlaneModel* t2tpModel_;

  Thermo2ScriptThread* scriptThread_;

signals:
  void prepareNewMeasurement();
  void scriptLoaded();
  void setControlsEnabled(bool enabled);
  void clearMessageText();
  void appendMessageText(const QString & text);
};

#endif // THERMO2SCRIPTMODEL_H

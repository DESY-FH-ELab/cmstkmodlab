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

#ifndef THERMO2MAINWINDOW_H
#define THERMO2MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>

#include "HuberUnistat525wModel.h"
#include "MartaModel.h"
#include "AgilentTwisTorr304Model.h"
#include "LeyboldGraphixOneModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"

#include "HuberUnistat525wWidget.h"
#include "MartaWidget.h"
#include "AgilentTwisTorr304Widget.h"
#include "LeyboldGraphixOneWidget.h"
#include "RohdeSchwarzNGE103BWidget.h"
#include "KeithleyDAQ6510Widget.h"

#include "Thermo2DAQModel.h"
#include "Thermo2DAQThread.h"
#include "Thermo2DAQStreamer.h"
#include "Thermo2CommunicationServer.h"
#include "Thermo2DAQServer.h"
#include "Thermo2ScriptModel.h"
#include "Thermo2ThroughPlaneModel.h"

#include "ApplicationConfigViewer.h"

class Thermo2MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit Thermo2MainWindow(QWidget *parent = 0);

public slots:

  void quit();
  void controlStateChanged(bool);  

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  bool chillerAndVacuumActive_;
  bool martaActive_;
  bool throughPlaneActive_;

  HuberUnistat525wModel* huberModel_;
  MartaModel* martaModel_;
  AgilentTwisTorr304Model* agilentModel_;
  LeyboldGraphixOneModel* leyboldModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;

  HuberUnistat525wWidget* huberWidget_;
  MartaWidget* martaWidget_;
  AgilentTwisTorr304Widget* agilentWidget_;
  LeyboldGraphixOneWidget* leyboldWidget_;
  RohdeSchwarzNGE103BWidget* nge103BWidget_;
  KeithleyDAQ6510Widget* keithleyWidget_;
  
  Thermo2DAQModel* daqModel_;
  Thermo2DAQThread* daqThread_;
  Thermo2DAQStreamer* daqStreamer_;
  Thermo2CommunicationServer* commServer_;
  Thermo2DAQServer* daqServer_;
  Thermo2ScriptModel* scriptModel_;

  Thermo2ThroughPlaneModel* throughPlaneModel_;

  ApplicationConfigViewer* configViewer_;
};

#endif // THERMO2MAINWINDOW_H

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
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
//#include <QPushButton>
//#include <QFileDialog>
//#include <QComboBox>
//#include <QImage>
//#include <QTimer>

#include "RohdeSchwarzNGE103BModel.h"

//#include "HuberPetiteFleurModel.h"
//#include "HuberPetiteFleurWidget.h"

//#include "KeithleyModel.h"
//#include "KeithleyWidget.h"

#include "Thermo2DAQModel.h"
#include "Thermo2DAQThread.h"
#include "Thermo2DAQStreamer.h"
//#include "ThermoDAQWebInfo.h"
#include "Thermo2DAQServer.h"
#include "Thermo2ScriptModel.h"

class Thermo2MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit Thermo2MainWindow(QWidget *parent = 0);

public slots:

  void quit();

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  RohdeSchwarzNGE103BModel* nge103BModel_;
  //HuberPetiteFleurModel* huberModel_;
  //KeithleyModel* keithleyModel_;
  //PfeifferModel* pfeifferModel_;
  //IotaModel* iotaModel_;
  //ArduinoPresModel* arduinoPresModel_;

  Thermo2DAQModel* daqModel_;
  Thermo2DAQThread* daqThread_;
  Thermo2DAQStreamer* daqStreamer_;
  //ThermoDAQWebInfo* daqWebInfo_;
  Thermo2DAQServer* daqServer_;
  Thermo2ScriptModel* scriptModel_;
};

#endif // THERMO2MAINWINDOW_H

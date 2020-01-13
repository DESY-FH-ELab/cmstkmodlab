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

#ifndef THERMODISPLAY2MAINWINDOW_H
#define THERMODISPLAY2MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QTabWidget>

#include "ThermoDAQ2Client.h"
#include "ThermoDAQ2NetworkReader.h"
#include "ThermoDAQ2DisplayWidget.h"
#include "ThermoDAQ2ValueVector.h"

class ThermoDisplay2MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoDisplay2MainWindow(QWidget *parent = 0);

public slots:

  void requestData();
  void updateInfo();
  void exportPlot();
  void clearData();

protected:

  QTimer* timer_;

  QTabWidget* tabWidget_;

  ThermoDAQ2Client* client_;
  ThermoDAQ2NetworkReader* reader_;

  ThermoDAQ2ValueVector<double> U1_;
  ThermoDAQ2DisplayPlotItem* U1Plot_;
  ThermoDAQ2ValueVector<double> U2_;
  ThermoDAQ2DisplayPlotItem* U2Plot_;
  ThermoDAQ2ValueVector<double> U3_;
  ThermoDAQ2DisplayPlotItem* U3Plot_;

  ThermoDAQ2ValueVector<double> I1_;
  ThermoDAQ2DisplayPlotItem* I1Plot_;
  ThermoDAQ2ValueVector<double> I2_;
  ThermoDAQ2DisplayPlotItem* I2Plot_;
  ThermoDAQ2ValueVector<double> I3_;
  ThermoDAQ2DisplayPlotItem* I3Plot_;

  ThermoDAQ2ValueVector<double> temperature_[2][10];
  ThermoDAQ2DisplayPlotItem* temperaturePlot_[2][10];

  ThermoDAQ2VoltageDisplayWidget* voltageDisplay_;
  ThermoDAQ2CurrentDisplayWidget* currentDisplay_;

  ThermoDAQ2TemperatureDisplayWidget* tempDisplay_;
};

#endif // THERMODISPLAY2MAINWINDOW_H

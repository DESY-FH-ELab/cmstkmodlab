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

#ifndef THERMODISPLAY2MAINWINDOW_H
#define THERMODISPLAY2MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QTabWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include "Thermo2ThroughPlaneFitter.h"

#include "ThermoDAQ2Client.h"
#include "ThermoDAQ2NetworkReader.h"

#include "ThermoDisplay2ChartView.h"
#include "ThermoDisplay2Chart.h"
#include "ThermoDisplay2LineSeries.h"

#include "ApplicationConfigViewer.h"

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoDisplay2MainWindow(QWidget *parent = 0);

public slots:

  void requestData();
  void updateInfo();

  void clearData();
  void savePlots();

protected slots:

  void configurationChanged();

protected:

  QTimer* timer_;

  QTabWidget* tabWidget_;

  ThermoDAQ2Client* client_;
  ThermoDAQ2NetworkReader* reader_;

  bool chillerAndVacuumActive_;
  bool martaActive_;
  bool throughPlaneActive_;

  double resistance_;
  double cableResistance_;
  double kBlock_;
  double ABlock_;
  unsigned int nge103BChannel_;
  std::array<unsigned int,6> keithleyTopSensors_;
  std::array<unsigned int,6> keithleyTopCards_;
  std::array<unsigned int,6> keithleyTopChannels_;
  std::array<double,6> keithleyTopPositions_;
  std::array<double,6> keithleyTopCor0_;
  std::array<double,6> keithleyTopCor1_;
  std::array<double,6> keithleyTopCor2_;
  std::array<double,6> keithleyTopCor3_;
  std::array<unsigned int,6> keithleyBottomSensors_;
  std::array<unsigned int,6> keithleyBottomCards_;
  std::array<unsigned int,6> keithleyBottomChannels_;
  std::array<double,6> keithleyBottomPositions_;
  std::array<double,6> keithleyBottomCor0_;
  std::array<double,6> keithleyBottomCor1_;
  std::array<double,6> keithleyBottomCor2_;
  std::array<double,6> keithleyBottomCor3_;
  unsigned int keithleyAmbientSensor_;
  unsigned int keithleyAmbientCard_;
  unsigned int keithleyAmbientChannel_;

  Thermo2ThroughPlaneFitter fitter_;

  ThermoDisplay2ChartView *ChillerTSChartView_;
  ThermoDisplay2TemperatureStateChart *ChillerTSChart_;
  ThermoDisplay2LineSeries *ChillerTInternalSeries_;
  ThermoDisplay2LineSeries *ChillerTProcessSeries_;
  ThermoDisplay2LineSeries *ChillerTReturnSeries_;
  ThermoDisplay2LineSeries *ChillerTCWISeries_;
  ThermoDisplay2LineSeries *ChillerTCWOSeries_;
  ThermoDisplay2LineSeries *ChillerSTCSeries_;
  ThermoDisplay2LineSeries *ChillerSCSeries_;

  ThermoDisplay2ChartView *ChillerPPChartView_;
  ThermoDisplay2PowerPressureChart *ChillerPPChart_;
  ThermoDisplay2LineSeries *ChillerPowerSeries_;
  ThermoDisplay2LineSeries *ChillerPressureSeries_;

  ThermoDisplay2ChartView *VacuumChartView_;
  ThermoDisplay2LogPressureChart *VacuumPressureChart_;
  ThermoDisplay2LineSeries *VacuumPressureSeries_;

  ThermoDisplay2ChartView *UChartView_;
  ThermoDisplay2VoltageChart *UChart_;
  ThermoDisplay2LineSeries *U1Series_;
  ThermoDisplay2LineSeries *U2Series_;
  ThermoDisplay2LineSeries *U3Series_;

  ThermoDisplay2ChartView *IChartView_;
  ThermoDisplay2CurrentChart *IChart_;
  ThermoDisplay2LineSeries *I1Series_;
  ThermoDisplay2LineSeries *I2Series_;
  ThermoDisplay2LineSeries *I3Series_;

  ThermoDisplay2ChartView *TChartView_[2];
  ThermoDisplay2TemperatureChart *TChart_[2];
  ThermoDisplay2LineSeries *TSeries_[2][10];

  ThermoDisplay2PressureDeltaPressureChartView *MartaPressureChartView_;
  ThermoDisplay2LinPressureDeltaPressureChart *MartaPressureChart_;
  ThermoDisplay2LineSeries *MartaPT03Series_;
  ThermoDisplay2LineSeries *MartaPT05Series_;
  ThermoDisplay2LineSeries *MartaPT01CO2Series_;
  ThermoDisplay2LineSeries *MartaPT02CO2Series_;
  ThermoDisplay2LineSeries *MartaPT03CO2Series_;
  ThermoDisplay2LineSeries *MartaPT04CO2Series_;
  //ThermoDisplay2LineSeries *MartaPT05CO2Series_;
  ThermoDisplay2LineSeries *MartaPT06CO2Series_;
  ThermoDisplay2LineSeries *MartaDP01CO2Series_;
  ThermoDisplay2LineSeries *MartaDP02CO2Series_;
  ThermoDisplay2LineSeries *MartaDP03CO2Series_;
  ThermoDisplay2LineSeries *MartaDP04CO2Series_;

  ThermoDisplay2TemperatureDeltaTemperatureChartView *MartaTemperatureChartView_;
  ThermoDisplay2TemperatureDeltaTemperatureChart *MartaTemperatureChart_;
  ThermoDisplay2LineSeries *MartaTSetPointSeries_;
  ThermoDisplay2LineSeries *MartaTT02Series_;
  ThermoDisplay2LineSeries *MartaTT01CO2Series_;
  ThermoDisplay2LineSeries *MartaTT02CO2Series_;
  ThermoDisplay2LineSeries *MartaTT03CO2Series_;
  ThermoDisplay2LineSeries *MartaTT04CO2Series_;
  //ThermoDisplay2LineSeries *MartaTT05CO2Series_;
  ThermoDisplay2LineSeries *MartaTT06CO2Series_;
  ThermoDisplay2LineSeries *MartaTT07CO2Series_;
  ThermoDisplay2LineSeries *MartaST01CO2Series_;
  ThermoDisplay2LineSeries *MartaST02CO2Series_;
  ThermoDisplay2LineSeries *MartaST03CO2Series_;
  ThermoDisplay2LineSeries *MartaST04CO2Series_;
  ThermoDisplay2LineSeries *MartaDT02CO2Series_;
  ThermoDisplay2LineSeries *MartaDT03CO2Series_;

  ThermoDisplay2ChartView *ThroughPlaneTChartView_;
  ThermoDisplay2TemperatureChart *ThroughPlaneTChart_;
  ThermoDisplay2LineSeries *ThroughPlaneTAmbient_;
  ThermoDisplay2LineSeries *ThroughPlaneTSink_;
  ThermoDisplay2LineSeries *ThroughPlaneTopTSeries_[6];
  ThermoDisplay2LineSeries *ThroughPlaneTSampleTop_;
  ThermoDisplay2LineSeries *ThroughPlaneBottomTSeries_[6];
  ThermoDisplay2LineSeries *ThroughPlaneTSampleBottom_;
  ThermoDisplay2LineSeries *ThroughPlaneTSampleMiddle_;
  ThermoDisplay2ChartView *ThroughPlanePChartView_;
  ThermoDisplay2PowerChart *ThroughPlanePChart_;
  ThermoDisplay2LineSeries *ThroughPlanePSource_;
  ThermoDisplay2LineSeries *ThroughPlanePTop_;
  ThermoDisplay2LineSeries *ThroughPlanePBottom_;

  ApplicationConfigViewer* configViewer_;
};

#endif // THERMODISPLAY2MAINWINDOW_H

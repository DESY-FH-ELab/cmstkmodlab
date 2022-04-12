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

#ifndef THERMODISPLAY2CHART_H
#define THERMODISPLAY2CHART_H

#include <QObject>
#include <QDateTime>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QCategoryAxis>

#include "ThermoDisplay2DateTimeAxis.h"
#include "ThermoDisplay2ValueAxis.h"

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2Chart : public QChart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2Chart();

  virtual void addSeries(QAbstractSeries *series) = 0;

  virtual void refreshAxes() = 0;

  void setTheme(QChart::ChartTheme theme);

public slots:

  void connectMarkers();
  void updateLegend();
  void handleMarkerClicked();
  void clearData();
  virtual void leftYAxisDoubleClicked() { }
  virtual void rightYAxisDoubleClicked() { }
  void xAxisDoubleClicked();
  void refreshXAxis();
  void areaChanged(const QRectF &);

protected:

  ThermoDisplay2DateTimeAxis *axisX_;
};

class ThermoDisplay2TemperatureChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2TemperatureChart();

  void addSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

    void leftYAxisDoubleClicked();
    void refreshYAxis();

protected:

  ThermoDisplay2ValueAxis *axisY_;
};

class ThermoDisplay2TemperatureStateChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2TemperatureStateChart();

  void addSeries(QAbstractSeries *series);
  void addTemperatureSeries(QAbstractSeries *series);
  void addStateSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void refreshTemperatureAxis();

protected:

  ThermoDisplay2ValueAxis *axisTemperatureY_;
  QCategoryAxis *axisStateY_;
};

class ThermoDisplay2VoltageChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2VoltageChart();

  void addSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void refreshYAxis();

protected:

  ThermoDisplay2ValueAxis *axisY_;
};

class ThermoDisplay2CurrentChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2CurrentChart();

  void addSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void refreshYAxis();

protected:

  ThermoDisplay2ValueAxis *axisY_;
};

class ThermoDisplay2PowerChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2PowerChart();

  void addSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void refreshYAxis();

protected:

  ThermoDisplay2ValueAxis *axisY_;
};

class ThermoDisplay2PowerPressureChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2PowerPressureChart();

  void addSeries(QAbstractSeries *series);
  void addPowerSeries(QAbstractSeries *series);
  void addPressureSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void rightYAxisDoubleClicked();
  void refreshPowerAxis();
  void refreshPressureAxis();

protected:

  ThermoDisplay2ValueAxis *axisPowerY_;
  ThermoDisplay2ValueAxis *axisPressureY_;
};

class ThermoDisplay2LinPressureChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2LinPressureChart();

  void addSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void refreshYAxis();

protected:

  ThermoDisplay2ValueAxis *axisY_;
};

class ThermoDisplay2LinPressureDeltaPressureChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2LinPressureDeltaPressureChart();

  void addSeries(QAbstractSeries *series);
  void addPressureSeries(QAbstractSeries *series);
  void addDeltaPressureSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void rightYAxisDoubleClicked();
  void refreshPressureAxis();
  void refreshDeltaPressureAxis();

protected:

  ThermoDisplay2ValueAxis *axisPressureAxis_;
  ThermoDisplay2ValueAxis *axisDeltaPressureAxis_;
};

class ThermoDisplay2TemperatureDeltaTemperatureChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2TemperatureDeltaTemperatureChart();

  void addSeries(QAbstractSeries *series);
  void addTemperatureSeries(QAbstractSeries *series);
  void addDeltaTemperatureSeries(QAbstractSeries *series);

  void refreshAxes();

public slots:

  void leftYAxisDoubleClicked();
  void rightYAxisDoubleClicked();
  void refreshTemperatureAxis();
  void refreshDeltaTemperatureAxis();

protected:

  ThermoDisplay2ValueAxis *axisTemperatureAxis_;
  ThermoDisplay2ValueAxis *axisDeltaTemperatureAxis_;
};

class ThermoDisplay2LogPressureChart : public ThermoDisplay2Chart
{
  Q_OBJECT
public:

  explicit ThermoDisplay2LogPressureChart();

  void addSeries(QAbstractSeries *series);

  void refreshAxes();

protected:

  void refreshPressureAxis();

  QLogValueAxis *axisPressureY_;
};

#endif // THERMODISPLAY2CHART_H

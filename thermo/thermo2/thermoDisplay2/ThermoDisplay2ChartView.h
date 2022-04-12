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

#ifndef THERMODISPLAY2CHARTVIEW_H
#define THERMODISPLAY2CHARTVIEW_H

#include <QObject>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include "ThermoDisplay2Chart.h"
#include "ThermoDisplay2Callout.h"

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2ChartView : public QChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2ChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

  void refreshAxes();

public slots:

  virtual void tooltip(QPointF point, bool state) = 0;

protected:

  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
 
  ThermoDisplay2Chart * chart_;
  ThermoDisplay2Callout * callout_;

 signals:

  void leftYAxisDoubleClicked();
  void rightYAxisDoubleClicked();
  void xAxisDoubleClicked();
};

class ThermoDisplay2TemperatureChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2TemperatureChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2TemperatureStateChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2TemperatureStateChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2VoltageChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2VoltageChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2CurrentChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2CurrentChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2PowerChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2PowerChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2PowerPressureChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2PowerPressureChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2PressureDeltaPressureChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2PressureDeltaPressureChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2TemperatureDeltaTemperatureChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2TemperatureDeltaTemperatureChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

class ThermoDisplay2PressureChartView : public ThermoDisplay2ChartView
{
  Q_OBJECT
public:

  explicit ThermoDisplay2PressureChartView(ThermoDisplay2Chart *chart, QWidget *parent = nullptr);

public slots:

  void tooltip(QPointF point, bool state);
};

#endif // THERMODISPLAY2CHART_H

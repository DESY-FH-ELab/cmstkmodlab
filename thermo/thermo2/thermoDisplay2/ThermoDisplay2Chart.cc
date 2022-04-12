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

#include <iostream>
#include <cmath>

#include <QFont>
#include <QFontMetrics>
#include <QtCharts/QLegendMarker>

#include <nqlogger.h>

#include "ThermoDisplay2Chart.h"
#include "ThermoDisplay2LineSeries.h"

ThermoDisplay2Chart::ThermoDisplay2Chart()
  : QChart()
{
  axisX_ = new ThermoDisplay2DateTimeAxis();
  axisX_->setFormat("dd.MM. hh:mm");
  axisX_->setTitleText("Time");
  addAxis(axisX_, Qt::AlignBottom);

  legend()->detachFromChart();
  legend()->setBackgroundVisible(true);
  legend()->setBrush(QBrush(QColor(240, 240, 240, 220)));
  QFont font = legend()->font();
  font.setPointSizeF(8);
  legend()->setFont(font);

  connect(axisX_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshXAxis()));

  connect(this, SIGNAL(plotAreaChanged(const QRectF)),
          this, SLOT(areaChanged(const QRectF)));
}

void ThermoDisplay2Chart::connectMarkers()
{
  const auto markers = legend()->markers();
  for (QLegendMarker *marker : markers) {
    QObject::connect(marker, SIGNAL(clicked()),
                     this, SLOT(handleMarkerClicked()));
  }
}

void ThermoDisplay2Chart::updateLegend()
{
  const auto markers = legend()->markers();
  for (QLegendMarker *marker : markers) {

    ThermoDisplay2LineSeries * series = qobject_cast<ThermoDisplay2LineSeries*>(marker->series());

    series->setVisible(series->isEnabled());
    marker->setVisible(true);

    qreal alpha = 1.0;

    if (!marker->series()->isVisible())
      alpha = 0.5;

    QColor color;
    QBrush brush = marker->labelBrush();
    color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setLabelBrush(brush);

    brush = marker->brush();
    color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setBrush(brush);

    QPen pen = marker->pen();
    color = pen.color();
    color.setAlphaF(alpha);
    pen.setColor(color);
    marker->setPen(pen);
  }
}

void ThermoDisplay2Chart::handleMarkerClicked()
{
  QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());

  if (marker->type()==QLegendMarker::LegendMarkerTypeXY) {

    ThermoDisplay2LineSeries * series = qobject_cast<ThermoDisplay2LineSeries*>(marker->series());

    if (series->isEnabled()) {
      series->setVisible(!series->isVisible());
      marker->setVisible(true);

      qreal alpha = 1.0;

      if (!marker->series()->isVisible())
        alpha = 0.5;

      QColor color;
      QBrush brush = marker->labelBrush();
      color = brush.color();
      color.setAlphaF(alpha);
      brush.setColor(color);
      marker->setLabelBrush(brush);

      brush = marker->brush();
      color = brush.color();
      color.setAlphaF(alpha);
      brush.setColor(color);
      marker->setBrush(brush);

      QPen pen = marker->pen();
      color = pen.color();
      color.setAlphaF(alpha);
      pen.setColor(color);
      marker->setPen(pen);
    }
  }
}

void ThermoDisplay2Chart::clearData()
{
  auto ss = series();
  for (QAbstractSeries *s : ss) {
    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(s);
    ls->clear();
    ls->resetInitialized();
  }
}

void ThermoDisplay2Chart::setTheme(QChart::ChartTheme theme)
{
  QChart::setTheme(theme);
  updateLegend();
}

void ThermoDisplay2Chart::refreshXAxis()
{
  axisX_->refresh(series());
}

void ThermoDisplay2Chart::xAxisDoubleClicked()
{
  axisX_->configure();
}

void ThermoDisplay2Chart::areaChanged(const QRectF & plotRect)
{
  if (plotRect.width()<40) return;
  if (plotRect.height()<40) return;

  qreal maxWidth = 0;
  qreal maxHeight = 0;

  QFontMetrics fm(legend()->font());

  auto ss = series();
  for (QAbstractSeries *s : ss) {
    ThermoDisplay2LineSeries* ls = dynamic_cast<ThermoDisplay2LineSeries*>(s);
    if (ls) {
      QRectF boundingRect = fm.boundingRect(ls->name());
      maxWidth = std::max(maxWidth, boundingRect.width());
      maxHeight = std::max(maxHeight, boundingRect.height());
    }
  }

  qreal availableHeight = plotRect.height()-10;
  qreal nSeries = series().size();
  qreal nRows = nSeries;
  qreal nColumns = 1;

  while (nRows*(maxHeight+8)+14 > availableHeight) {
    //std::cout << nRows << std::endl;
    nColumns++;
    nRows = std::ceil(nSeries/nColumns);
  }

  legend()->setGeometry(QRectF(plotRect.x()+10, plotRect.y()+5,
                               nColumns*(maxWidth+24)+16,
                               nRows*(maxHeight+8)+14));
  legend()->update();
}

ThermoDisplay2TemperatureChart::ThermoDisplay2TemperatureChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new ThermoDisplay2ValueAxis();
  axisY_->setLabelFormat("%.2f");
  axisY_->setTitleText("T [°C]");
  addAxis(axisY_, Qt::AlignLeft);

  connect(axisY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshYAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2TemperatureChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisY_);
}

void ThermoDisplay2TemperatureChart::refreshAxes()
{
  refreshXAxis();
  refreshYAxis();
}

void ThermoDisplay2TemperatureChart::refreshYAxis()
{
  axisY_->refresh(series());
}

void ThermoDisplay2TemperatureChart::leftYAxisDoubleClicked()
{
  axisY_->configure();
}

ThermoDisplay2TemperatureStateChart::ThermoDisplay2TemperatureStateChart()
  : ThermoDisplay2Chart()
{
  axisTemperatureY_ = new ThermoDisplay2ValueAxis();
  axisTemperatureY_->setLabelFormat("%.2f");
  axisTemperatureY_->setTitleText("T [°C]");
  addAxis(axisTemperatureY_, Qt::AlignLeft);

  connect(axisTemperatureY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshTemperatureAxis()));

  axisStateY_ = new QCategoryAxis();
  axisStateY_->setTitleText("Status");
  axisStateY_->setMin(-0.5);
  axisStateY_->setMax(1.5);
  axisStateY_->setStartValue(-0.5);
  axisStateY_->append("Off", 0.5);
  axisStateY_->append("On", 1.5);
  addAxis(axisStateY_, Qt::AlignRight);
}

void ThermoDisplay2TemperatureStateChart::addSeries(QAbstractSeries *series)
{
  addTemperatureSeries(series);
}

void ThermoDisplay2TemperatureStateChart::addTemperatureSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisTemperatureY_);
}

void ThermoDisplay2TemperatureStateChart::addStateSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisStateY_);
}

void ThermoDisplay2TemperatureStateChart::refreshAxes()
{
  refreshXAxis();
  refreshTemperatureAxis();
}

void ThermoDisplay2TemperatureStateChart::refreshTemperatureAxis()
{
  axisTemperatureY_->refresh(series());
}

void ThermoDisplay2TemperatureStateChart::leftYAxisDoubleClicked()
{
  axisTemperatureY_->configure();
}

ThermoDisplay2VoltageChart::ThermoDisplay2VoltageChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new ThermoDisplay2ValueAxis();
  axisY_->setLabelFormat("%.2f");
  axisY_->setTitleText("U [V]");
  addAxis(axisY_, Qt::AlignLeft);

  connect(axisY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshYAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2VoltageChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisY_);
}

void ThermoDisplay2VoltageChart::refreshAxes()
{
  refreshXAxis();
  refreshYAxis();
}

void ThermoDisplay2VoltageChart::refreshYAxis()
{
  axisY_->refresh(series());
}

void ThermoDisplay2VoltageChart::leftYAxisDoubleClicked()
{
  axisY_->configure();
}
ThermoDisplay2CurrentChart::ThermoDisplay2CurrentChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new ThermoDisplay2ValueAxis();
  axisY_->setLabelFormat("%.2f");
  axisY_->setTitleText("I [A]");
  addAxis(axisY_, Qt::AlignLeft);

  connect(axisY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshYAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2CurrentChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisY_);
}

void ThermoDisplay2CurrentChart::refreshAxes()
{
  refreshXAxis();
  refreshYAxis();
}

void ThermoDisplay2CurrentChart::refreshYAxis()
{
  axisY_->refresh(series());
}

void ThermoDisplay2CurrentChart::leftYAxisDoubleClicked()
{
  axisY_->configure();
}

ThermoDisplay2PowerChart::ThermoDisplay2PowerChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new ThermoDisplay2ValueAxis();
  axisY_->setLabelFormat("%.2f");
  axisY_->setTitleText("P [W]");
  addAxis(axisY_, Qt::AlignLeft);

  connect(axisY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshYAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2PowerChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisY_);
}

void ThermoDisplay2PowerChart::refreshAxes()
{
  refreshXAxis();
  refreshYAxis();
}

void ThermoDisplay2PowerChart::refreshYAxis()
{
  axisY_->refresh(series());
}

void ThermoDisplay2PowerChart::leftYAxisDoubleClicked()
{
  axisY_->configure();
}

ThermoDisplay2PowerPressureChart::ThermoDisplay2PowerPressureChart()
  : ThermoDisplay2Chart()
{
  axisPowerY_ = new ThermoDisplay2ValueAxis();
  axisPowerY_->setLabelFormat("%.2f");
  axisPowerY_->setTitleText("Power [kW]");
  addAxis(axisPowerY_, Qt::AlignLeft);

  connect(axisPowerY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshPowerAxis()));

  axisPressureY_ = new ThermoDisplay2ValueAxis();
  axisPressureY_->setLabelFormat("%.2f");
  axisPressureY_->setTitleText("Pressure [bar]");
  addAxis(axisPressureY_, Qt::AlignRight);

  connect(axisPressureY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshPressureAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2PowerPressureChart::addSeries(QAbstractSeries *series)
{
  addPowerSeries(series);
}

void ThermoDisplay2PowerPressureChart::addPowerSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisPowerY_);
}

void ThermoDisplay2PowerPressureChart::addPressureSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisPressureY_);
}

void ThermoDisplay2PowerPressureChart::refreshAxes()
{
  refreshXAxis();
  refreshPowerAxis();
  refreshPressureAxis();
}

void ThermoDisplay2PowerPressureChart::refreshPowerAxis()
{
  axisPowerY_->refresh(series());
}

void ThermoDisplay2PowerPressureChart::leftYAxisDoubleClicked()
{
  axisPowerY_->configure();
}

void ThermoDisplay2PowerPressureChart::refreshPressureAxis()
{
  axisPressureY_->refresh(series());
}

void ThermoDisplay2PowerPressureChart::rightYAxisDoubleClicked()
{
  axisPressureY_->configure();
}

ThermoDisplay2LinPressureChart::ThermoDisplay2LinPressureChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new ThermoDisplay2ValueAxis();
  axisY_->setLabelFormat("%.2f");
  axisY_->setTitleText("Pressure [bar]");
  addAxis(axisY_, Qt::AlignLeft);

  connect(axisY_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshYAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2LinPressureChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisY_);
}

void ThermoDisplay2LinPressureChart::refreshAxes()
{
  refreshXAxis();
  refreshYAxis();
}

void ThermoDisplay2LinPressureChart::refreshYAxis()
{
  axisY_->refresh(series());
}

void ThermoDisplay2LinPressureChart::leftYAxisDoubleClicked()
{
  axisY_->configure();
}

ThermoDisplay2LinPressureDeltaPressureChart::ThermoDisplay2LinPressureDeltaPressureChart()
  : ThermoDisplay2Chart()
{
  axisPressureAxis_ = new ThermoDisplay2ValueAxis(0);
  axisPressureAxis_->setLabelFormat("%.0f");
  axisPressureAxis_->setTitleText("Pressure [bar]");
  addAxis(axisPressureAxis_, Qt::AlignLeft);

  connect(axisPressureAxis_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshPressureAxis()));

  axisDeltaPressureAxis_ = new ThermoDisplay2ValueAxis(2);
  axisDeltaPressureAxis_->setLabelFormat("%.2f");
  axisDeltaPressureAxis_->setTitleText("Pressure Diff. [bar]");
  addAxis(axisDeltaPressureAxis_, Qt::AlignRight);

  connect(axisDeltaPressureAxis_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshDeltaPressureAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2LinPressureDeltaPressureChart::addSeries(QAbstractSeries *series)
{
  addPressureSeries(series);
}

void ThermoDisplay2LinPressureDeltaPressureChart::addPressureSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisPressureAxis_);
}

void ThermoDisplay2LinPressureDeltaPressureChart::addDeltaPressureSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisDeltaPressureAxis_);
}

void ThermoDisplay2LinPressureDeltaPressureChart::refreshAxes()
{
  refreshXAxis();
  refreshPressureAxis();
  refreshDeltaPressureAxis();
}

void ThermoDisplay2LinPressureDeltaPressureChart::refreshPressureAxis()
{
  axisPressureAxis_->refresh(series());
}

void ThermoDisplay2LinPressureDeltaPressureChart::leftYAxisDoubleClicked()
{
  axisPressureAxis_->configure();
}

void ThermoDisplay2LinPressureDeltaPressureChart::refreshDeltaPressureAxis()
{
  axisDeltaPressureAxis_->refresh(series());
}

void ThermoDisplay2LinPressureDeltaPressureChart::rightYAxisDoubleClicked()
{
  axisDeltaPressureAxis_->configure();
}

ThermoDisplay2TemperatureDeltaTemperatureChart::ThermoDisplay2TemperatureDeltaTemperatureChart()
  : ThermoDisplay2Chart()
{
  axisTemperatureAxis_ = new ThermoDisplay2ValueAxis(0);
  axisTemperatureAxis_->setLabelFormat("%.0f");
  axisTemperatureAxis_->setTitleText("Temperature [°C]");
  addAxis(axisTemperatureAxis_, Qt::AlignLeft);

  connect(axisTemperatureAxis_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshTemperatureAxis()));

  axisDeltaTemperatureAxis_ = new ThermoDisplay2ValueAxis(2);
  axisDeltaTemperatureAxis_->setLabelFormat("%.2f");
  axisDeltaTemperatureAxis_->setTitleText("Temperature Diff. [°C]");
  addAxis(axisDeltaTemperatureAxis_, Qt::AlignRight);

  connect(axisDeltaTemperatureAxis_, SIGNAL(axisModeChanged()),
          this, SLOT(refreshDeltaTemperatureAxis()));

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::addSeries(QAbstractSeries *series)
{
  addTemperatureSeries(series);
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::addTemperatureSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisTemperatureAxis_);
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::addDeltaTemperatureSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisDeltaTemperatureAxis_);
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::refreshAxes()
{
  refreshXAxis();
  refreshTemperatureAxis();
  refreshDeltaTemperatureAxis();
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::refreshTemperatureAxis()
{
  axisTemperatureAxis_->refresh(series());
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::leftYAxisDoubleClicked()
{
  axisTemperatureAxis_->configure();
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::refreshDeltaTemperatureAxis()
{
  axisDeltaTemperatureAxis_->refresh(series());
}

void ThermoDisplay2TemperatureDeltaTemperatureChart::rightYAxisDoubleClicked()
{
  axisDeltaTemperatureAxis_->configure();
}

ThermoDisplay2LogPressureChart::ThermoDisplay2LogPressureChart()
  : ThermoDisplay2Chart()
{
  axisPressureY_ = new QLogValueAxis();
  axisPressureY_->setLabelFormat("%.2f");
  axisPressureY_->setTitleText("Pressure [mbar]");
  addAxis(axisPressureY_, Qt::AlignLeft);

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2LogPressureChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisPressureY_);
}

void ThermoDisplay2LogPressureChart::refreshAxes()
{
  refreshXAxis();
  refreshPressureAxis();
}

void ThermoDisplay2LogPressureChart::refreshPressureAxis()
{
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  auto ss = series();
  for (QAbstractSeries *s : ss) {
    ThermoDisplay2LineSeries* ls = dynamic_cast<ThermoDisplay2LineSeries*>(s);
    if (ls && ls->isInitialized()) {

      auto axes = ls->attachedAxes();
      for (QAbstractAxis * axis : axes) {

        if (axis->alignment()==Qt::AlignLeft) {
          minY = std::min(minY, ls->minY());
          maxY = std::max(maxY, ls->maxY());
          hasValues = true;
        }
      }
    }
  }
  if (!hasValues) return;

  minY = (int)std::log10(minY);
  maxY = (int)std::log10(maxY);
  axisPressureY_->setRange(std::pow(10,minY-1.05), std::pow(10,maxY+1.05));
}

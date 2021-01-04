/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
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

#include <QtCharts/QLegendMarker>

#include <nqlogger.h>

#include "ThermoDisplay2Chart.h"
#include "ThermoDisplay2LineSeries.h"

ThermoDisplay2Chart::ThermoDisplay2Chart()
  : QChart()
{
  axisX_ = new QDateTimeAxis();
  axisX_->setFormat("dd.MM. hh:mm:ss");
  axisX_->setTitleText("Time");
  addAxis(axisX_, Qt::AlignBottom);
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
  qreal minX = std::numeric_limits<qreal>::max();
  qreal maxX = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minX = std::min(minX, s->minX());
      maxX = std::max(maxX, s->maxX());
      hasValues = true;
    }
  }
  if (!hasValues) return;

  QDateTime dtMin = QDateTime::fromMSecsSinceEpoch(minX);
  int temp = dtMin.time().msec();
  dtMin = dtMin.addMSecs(-temp);
  temp = dtMin.time().second();
  dtMin = dtMin.addSecs(-temp);

  QDateTime dtMax = QDateTime::fromMSecsSinceEpoch(maxX);
  temp = dtMin.time().msec();
  dtMax = dtMax.addMSecs(-temp);

  qint64 deltaX = dtMin.secsTo(dtMax) / 60;
  deltaX = 0.1*deltaX + 1;

  temp = dtMax.time().second();
  dtMax = dtMax.addSecs(-temp+deltaX*60);

  axisX_->setRange(dtMin, dtMax);
}

void ThermoDisplay2Chart::xAxisDoubleClicked()
{
  NQLogDebug("ThermoDisplay2Chart") << "xAxisDoubleClicked()";
}

ThermoDisplay2TemperatureChart::ThermoDisplay2TemperatureChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new QValueAxis();
  axisY_->setLabelFormat("%.1f");
  axisY_->setTitleText("T [°C]");
  addAxis(axisY_, Qt::AlignLeft);

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

  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minY = std::min(minY, s->minY());
      maxY = std::max(maxY, s->maxY());
      hasValues = true;
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  axisY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
}

ThermoDisplay2TemperatureStateChart::ThermoDisplay2TemperatureStateChart()
  : ThermoDisplay2Chart()
{
  axisTemperatureY_ = new QValueAxis();
  axisTemperatureY_->setLabelFormat("%.2f");
  axisTemperatureY_->setTitleText("T [°C]");
  addAxis(axisTemperatureY_, Qt::AlignLeft);

  axisStateY_ = new QCategoryAxis();
  axisStateY_->setTitleText("Status");
  axisStateY_->setMin(-0.5);
  axisStateY_->setMax(1.5);
  axisStateY_->setStartValue(-0.5);
  axisStateY_->append("Off", 0.5);
  axisStateY_->append("On", 1.5);
  addAxis(axisStateY_, Qt::AlignRight);

  legend()->setAlignment(Qt::AlignTop);
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
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {

      QList<QAbstractAxis*> axes = s->attachedAxes();
      for (QList<QAbstractAxis*>::Iterator ita = axes.begin();
           ita!=axes.end();
           ++ita) {

        if ((*ita)->alignment()==Qt::AlignLeft) {
          minY = std::min(minY, s->minY());
          maxY = std::max(maxY, s->maxY());
          hasValues = true;
        }
      }
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  deltaY = std::floor(deltaY);
  axisTemperatureY_->setRange(std::floor(minY)-0.1*deltaY, std::ceil(maxY)+0.1*deltaY);
}

ThermoDisplay2VoltageChart::ThermoDisplay2VoltageChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new QValueAxis();
  axisY_->setLabelFormat("%.1f");
  axisY_->setTitleText("U [V]");
  addAxis(axisY_, Qt::AlignLeft);

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

  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minY = std::min(minY, s->minY());
      maxY = std::max(maxY, s->maxY());
      hasValues = true;
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  axisY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
}

ThermoDisplay2CurrentChart::ThermoDisplay2CurrentChart()
  : ThermoDisplay2Chart()
{
  axisY_ = new QValueAxis();
  axisY_->setLabelFormat("%.1f");
  axisY_->setTitleText("I [A]");
  addAxis(axisY_, Qt::AlignLeft);

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

  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minY = std::min(minY, s->minY());
      maxY = std::max(maxY, s->maxY());
      hasValues = true;
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  axisY_->setRange(minY-0.01*deltaY, maxY+0.01*deltaY);
}

ThermoDisplay2PowerPressureChart::ThermoDisplay2PowerPressureChart()
  : ThermoDisplay2Chart()
{
  axisPowerY_ = new QValueAxis();
  axisPowerY_->setLabelFormat("%.2f");
  axisPowerY_->setTitleText("Power [kW]");
  addAxis(axisPowerY_, Qt::AlignLeft);

  axisPressureY_ = new QValueAxis();
  axisPressureY_->setLabelFormat("%.2f");
  axisPressureY_->setTitleText("Pressure [bar]");
  addAxis(axisPressureY_, Qt::AlignRight);

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
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {

      QList<QAbstractAxis*> axes = s->attachedAxes();
      for (QList<QAbstractAxis*>::Iterator ita = axes.begin();
           ita!=axes.end();
           ++ita) {

        if ((*ita)->alignment()==Qt::AlignLeft) {
          minY = std::min(minY, s->minY());
          maxY = std::max(maxY, s->maxY());
          hasValues = true;
        }
      }
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<1.0) deltaY = 1.0;
  axisPowerY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
}

void ThermoDisplay2PowerPressureChart::refreshPressureAxis()
{
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
      it!=series().end();
      ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {

      QList<QAbstractAxis*> axes = s->attachedAxes();
      for (QList<QAbstractAxis*>::Iterator ita = axes.begin();
          ita!=axes.end();
          ++ita) {

        if ((*ita)->alignment()==Qt::AlignRight) {
          minY = std::min(minY, s->minY());
          maxY = std::max(maxY, s->maxY());
          hasValues = true;
        }
      }
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<1.0) deltaY = 1.0;
  axisPressureY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
}

ThermoDisplay2LinPressureChart::ThermoDisplay2LinPressureChart()
  : ThermoDisplay2Chart()
{
  axisPressureY_ = new QValueAxis();
  axisPressureY_->setLabelFormat("%.2f");
  axisPressureY_->setTitleText("Pressure [bar]");
  addAxis(axisPressureY_, Qt::AlignLeft);

  legend()->setAlignment(Qt::AlignTop);
}

void ThermoDisplay2LinPressureChart::addSeries(QAbstractSeries *series)
{
  QChart::addSeries(series);

  series->attachAxis(axisX_);
  series->attachAxis(axisPressureY_);
}

void ThermoDisplay2LinPressureChart::refreshAxes()
{
  refreshXAxis();
  refreshPressureAxis();
}

void ThermoDisplay2LinPressureChart::refreshPressureAxis()
{
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
      it!=series().end();
      ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {

      QList<QAbstractAxis*> axes = s->attachedAxes();
      for (QList<QAbstractAxis*>::Iterator ita = axes.begin();
          ita!=axes.end();
          ++ita) {

        if ((*ita)->alignment()==Qt::AlignRight) {
          minY = std::min(minY, s->minY());
          maxY = std::max(maxY, s->maxY());
          hasValues = true;
        }
      }
    }
  }
  if (!hasValues) return;

  qreal deltaY = maxY-minY;
  if (deltaY<1.0) deltaY = 1.0;
  axisPressureY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
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
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
      it!=series().end();
      ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {

      QList<QAbstractAxis*> axes = s->attachedAxes();
      for (QList<QAbstractAxis*>::Iterator ita = axes.begin();
          ita!=axes.end();
          ++ita) {

        if ((*ita)->alignment()==Qt::AlignLeft) {
          minY = std::min(minY, s->minY());
          maxY = std::max(maxY, s->maxY());
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

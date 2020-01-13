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

#include <QtCharts/QLegendMarker>

#include <nqlogger.h>

#include "ThermoDisplay2Chart.h"
#include "ThermoDisplay2LineSeries.h"

ThermoDisplay2Chart::ThermoDisplay2Chart()
  : QChart()
{

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

ThermoDisplay2VoltageChart::ThermoDisplay2VoltageChart()
  : ThermoDisplay2Chart()
{
  axisX_ = new QDateTimeAxis();
  axisX_->setFormat("dd.MM. hh:mm:ss");
  axisX_->setTitleText("Time");
  addAxis(axisX_, Qt::AlignBottom);

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
  qreal minX = std::numeric_limits<qreal>::max();
  qreal maxX = -std::numeric_limits<qreal>::max();
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minX = std::min(minX, s->minX());
      maxX = std::max(maxX, s->maxX());
      minY = std::min(minY, s->minY());
      maxY = std::max(maxY, s->maxY());
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

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  axisY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
}

ThermoDisplay2CurrentChart::ThermoDisplay2CurrentChart()
  : ThermoDisplay2Chart()
{
  axisX_ = new QDateTimeAxis();
  axisX_->setFormat("dd.MM. hh:mm:ss");
  axisX_->setTitleText("Time");
  addAxis(axisX_, Qt::AlignBottom);

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
  qreal minX = std::numeric_limits<qreal>::max();
  qreal maxX = -std::numeric_limits<qreal>::max();
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minX = std::min(minX, s->minX());
      maxX = std::max(maxX, s->maxX());
      minY = std::min(minY, s->minY());
      maxY = std::max(maxY, s->maxY());
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

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  axisY_->setRange(minY-0.01*deltaY, maxY+0.01*deltaY);
}

ThermoDisplay2TemperatureChart::ThermoDisplay2TemperatureChart()
  : ThermoDisplay2Chart()
{
  axisX_ = new QDateTimeAxis();
  axisX_->setFormat("dd.MM. hh:mm:ss");
  axisX_->setTitleText("Time");
  addAxis(axisX_, Qt::AlignBottom);

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
  qreal minX = std::numeric_limits<qreal>::max();
  qreal maxX = -std::numeric_limits<qreal>::max();
  qreal minY = std::numeric_limits<qreal>::max();
  qreal maxY = -std::numeric_limits<qreal>::max();

  bool hasValues = false;
  for (QList<QAbstractSeries*>::Iterator it = series().begin();
       it!=series().end();
       ++it) {
    ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
    if (s && s->isInitialized()) {
      minX = std::min(minX, s->minX());
      maxX = std::max(maxX, s->maxX());
      minY = std::min(minY, s->minY());
      maxY = std::max(maxY, s->maxY());
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

  qreal deltaY = maxY-minY;
  if (deltaY<5.0) deltaY = 5.0;
  axisY_->setRange(minY-0.1*deltaY, maxY+0.1*deltaY);
}

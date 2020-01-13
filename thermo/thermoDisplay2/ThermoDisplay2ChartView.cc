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

#include <QDateTime>
#include <QtCharts/QLegendMarker>

#include <nqlogger.h>

#include "ThermoDisplay2LineSeries.h"
#include "ThermoDisplay2ChartView.h"

ThermoDisplay2ChartView::ThermoDisplay2ChartView(ThermoDisplay2Chart *chart, QWidget *parent)
  : QChartView(chart, parent),
    chart_(chart),
    callout_(nullptr)
{
  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  chart->setAcceptHoverEvents(true);

  auto ss = chart->series();
  for (QAbstractSeries *s : ss) {
    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(s);

    connect(ls, SIGNAL(hovered(const QPointF, bool)),
            this, SLOT(tooltip(const QPointF, bool)));
  }

  setMouseTracking(true);
}

ThermoDisplay2VoltageChartView::ThermoDisplay2VoltageChartView(ThermoDisplay2Chart *chart, QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2VoltageChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {
    callout_->setText(QString("%1\n%2\n%3 V").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(point.y(), 0, 'f', 3));
    callout_->setAnchor(point);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2CurrentChartView::ThermoDisplay2CurrentChartView(ThermoDisplay2Chart *chart, QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2CurrentChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {
    callout_->setText(QString("%1\n%2\n%3 A").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(point.y(), 0, 'f', 3));
    callout_->setAnchor(point);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2TemperatureChartView::ThermoDisplay2TemperatureChartView(ThermoDisplay2Chart *chart, QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2TemperatureChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {
    callout_->setText(QString("%1\n%2\n%3 °C").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(point.y(), 0, 'f', 3));
    callout_->setAnchor(point);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

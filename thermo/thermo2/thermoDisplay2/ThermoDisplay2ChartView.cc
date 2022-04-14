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

  connect(this, SIGNAL(leftYAxisDoubleClicked()),
          chart_, SLOT(leftYAxisDoubleClicked()));

  connect(this, SIGNAL(rightYAxisDoubleClicked()),
          chart_, SLOT(rightYAxisDoubleClicked()));

  connect(this, SIGNAL(xAxisDoubleClicked()),
          chart_, SLOT(xAxisDoubleClicked()));
}

void ThermoDisplay2ChartView::refreshAxes()
{
  if (callout_) callout_->hide();
  chart_->refreshAxes();
}

void ThermoDisplay2ChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
  NQLogDebug("ThermoDisplay2ChartView") << "mouseDoubleClickEvent(QMouseEvent *event)";

  QPointF localPos = event->localPos();
  qreal x = localPos.x();
  qreal y = localPos.y();

  QRectF plotArea = chart_->plotArea();
  qreal left = plotArea.x();
  qreal right = plotArea.x() + plotArea.width();
  qreal top = plotArea.y();
  qreal bottom = plotArea.y() + plotArea.height();

  if (x < left && y > top && y < bottom) {
    NQLogDebug("ThermoDisplay2ChartView") << "left y-axis";
    emit leftYAxisDoubleClicked();
  }
  if (x > right && y > top && y < bottom) {
    NQLogDebug("ThermoDisplay2ChartView") << "right y-axis";
    emit rightYAxisDoubleClicked();
  }
  if (x > left && x < right && y > bottom) {
    NQLogDebug("ThermoDisplay2ChartView") << "x-axis";
    emit xAxisDoubleClicked();
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

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());
    
    callout_->setText(QString("%1\n%2\n%3: %4 °C").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
    callout_->setAnchor(point);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2TemperatureStateChartView::ThermoDisplay2TemperatureStateChartView(ThermoDisplay2Chart *chart,
                                                                                 QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2TemperatureStateChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());

    QAbstractAxis* axis = ls->attachedAxes().at(1);
    if (axis->alignment()==Qt::AlignLeft) {
      callout_->setText(QString("%1\n%2\n%3: %4 °C").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
    } else {
      callout_->setText(QString("%1\n%2").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")));
    }
    callout_->setAnchor(point, ls);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}
ThermoDisplay2VoltageChartView::ThermoDisplay2VoltageChartView(ThermoDisplay2Chart *chart, QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2VoltageChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_==nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());
    
    callout_->setText(QString("%1\n%2\n%3: %4 V").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
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

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());
    
    callout_->setText(QString("%1\n%2\n%3\n%4 A").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
    callout_->setAnchor(point);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2PowerChartView::ThermoDisplay2PowerChartView(ThermoDisplay2Chart *chart, QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2PowerChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());
    
    callout_->setText(QString("%1\n%2\n%3: %4 W").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
    callout_->setAnchor(point);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2PowerPressureChartView::ThermoDisplay2PowerPressureChartView(ThermoDisplay2Chart *chart,
                                                                           QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2PowerPressureChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());

    QAbstractAxis* axis = ls->attachedAxes().at(1);
    if (axis->alignment()==Qt::AlignLeft) {
      callout_->setText(QString("%1\n%2\n%3\n%4 kW").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
    } else if (axis->alignment()==Qt::AlignRight) {
      callout_->setText(QString("%1\n%2\n%3\n%4 bar").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 3));
    } else {
      callout_->setText(QString("%1\n%2").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")));
    }
    callout_->setAnchor(point, ls);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2PressureDeltaPressureChartView::ThermoDisplay2PressureDeltaPressureChartView(ThermoDisplay2Chart *chart,
                                                                                           QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2PressureDeltaPressureChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());

    QAbstractAxis* axis = ls->attachedAxes().at(1);
    if (axis->alignment()==Qt::AlignLeft) {
      callout_->setText(QString("%1\n%2\n%3\n%4 bar").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 2));
    } else if (axis->alignment()==Qt::AlignRight) {
      callout_->setText(QString("%1\n%2\n%3\n%4 bar").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 2));
    } else {
      callout_->setText(QString("%1\n%2").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")));
    }
    callout_->setAnchor(point, ls);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2TemperatureDeltaTemperatureChartView::ThermoDisplay2TemperatureDeltaTemperatureChartView(ThermoDisplay2Chart *chart,
												       QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2TemperatureDeltaTemperatureChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());

    QAbstractAxis* axis = ls->attachedAxes().at(1);
    if (axis->alignment()==Qt::AlignLeft) {
      callout_->setText(QString("%1\n%2\n%3\n%4 °C").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 2));
    } else if (axis->alignment()==Qt::AlignRight) {
      callout_->setText(QString("%1\n%2\n%3\n%4 °C").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(point.y(), 0, 'f', 2));
    } else {
      callout_->setText(QString("%1\n%2").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")));
    }
    callout_->setAnchor(point, ls);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

ThermoDisplay2PressureChartView::ThermoDisplay2PressureChartView(ThermoDisplay2Chart *chart,
                                                                 QWidget *parent)
  : ThermoDisplay2ChartView(chart, parent)
{

}

void ThermoDisplay2PressureChartView::tooltip(QPointF point, bool state)
{
  QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.x());

  if (callout_ == nullptr)
    callout_ = new ThermoDisplay2Callout(chart_);

  if (state) {

    ThermoDisplay2LineSeries *ls = qobject_cast<ThermoDisplay2LineSeries*>(sender());

    double pressure = point.y();
    char dummy[40];
    if (pressure<1) {
      sprintf(dummy, "%.02E", pressure);
    } else if (pressure<10) {
      sprintf(dummy, "%.01f", pressure);
    } else {
      sprintf(dummy, "%d", (int)pressure);
    }

    callout_->setText(QString("%1\n%2\n%3\n%4 mbar").arg(dt.toString("dd/MM/yyyy")).arg(dt.toString("hh:mm:ss")).arg(ls->name()).arg(dummy));
    callout_->setAnchor(point, ls);
    callout_->setZValue(11);
    callout_->updateGeometry();
    callout_->show();
  } else {
    callout_->hide();
  }
}

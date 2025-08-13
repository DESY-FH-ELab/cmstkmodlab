/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODISPLAY2CALLOUT_H
#define THERMODISPLAY2CALLOUT_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QAbstractSeries>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>

#include "ThermoDisplay2Chart.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2Callout : public QGraphicsItem
{
public:

  explicit ThermoDisplay2Callout(ThermoDisplay2Chart *chart);

  void setText(const QString &text);
  void setAnchor(QPointF point, QAbstractSeries *series = nullptr);
  void updateGeometry();

  QRectF boundingRect() const;
  void paint(QPainter *painter,
             const QStyleOptionGraphicsItem *option,
             QWidget *widget);

protected:

  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

protected:

  QString text_;
  QRectF textRect_;
  QRectF rect_;
  QPointF anchor_;
  QAbstractSeries* series_;
  QFont font_;
  ThermoDisplay2Chart * chart_;
};

#endif // THERMODISPLAY2CALLOUT_H

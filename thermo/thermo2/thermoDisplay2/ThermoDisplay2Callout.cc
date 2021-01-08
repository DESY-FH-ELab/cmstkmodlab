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

#include <QWidget>
#include <QDateTime>
#include <QtCharts/QLegendMarker>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>

#include <nqlogger.h>

#include "ThermoDisplay2Callout.h"

ThermoDisplay2Callout::ThermoDisplay2Callout(ThermoDisplay2Chart *chart)
  : QGraphicsItem(chart),
    chart_(chart)
{

}

QRectF ThermoDisplay2Callout::boundingRect() const
{
  QPointF anchor = mapFromParent(chart_->mapToPosition(anchor_, series_));
  QRectF rect;
  rect.setLeft(qMin(rect_.left(), anchor.x()));
  rect.setRight(qMax(rect_.right(), anchor.x()));
  rect.setTop(qMin(rect_.top(), anchor.y()));
  rect.setBottom(qMax(rect_.bottom(), anchor.y()));
  return rect;
}

void ThermoDisplay2Callout::paint(QPainter *painter,
                                  const QStyleOptionGraphicsItem * /* option */,
                                  QWidget *widget)
{
  QPainterPath path;
  path.addRoundedRect(rect_, 5, 5);

  QPointF anchor = mapFromParent(chart_->mapToPosition(anchor_, series_));
  if (!rect_.contains(anchor_)) {
    QPointF point1, point2;

    // establish the position of the anchor point in relation to rect_
    bool above = anchor.y() <= rect_.top();
    bool aboveCenter = anchor.y() > rect_.top() && anchor.y() <= rect_.center().y();
    bool belowCenter = anchor.y() > rect_.center().y() && anchor.y() <= rect_.bottom();
    bool below = anchor.y() > rect_.bottom();

    bool onLeft = anchor.x() <= rect_.left();
    bool leftOfCenter = anchor.x() > rect_.left() && anchor.x() <= rect_.center().x();
    bool rightOfCenter = anchor.x() > rect_.center().x() && anchor.x() <= rect_.right();
    bool onRight = anchor.x() > rect_.right();

    // get the nearest rect_ corner.
    qreal x = (onRight + rightOfCenter) * rect_.width();
    qreal y = (below + belowCenter) * rect_.height();
    bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
    bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

    qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
    qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);;
    point1.setX(x1);
    point1.setY(y1);

    qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);;
    qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);;
    point2.setX(x2);
    point2.setY(y2);

    path.moveTo(point1);
    path.lineTo(anchor);
    path.lineTo(point2);
    path = path.simplified();
  }

  QPalette pal = widget->palette();
  QColor bgColor(pal.color(QPalette::Window));
  QColor fgColor(pal.color(QPalette::WindowText));

  painter->setBrush(bgColor);
  painter->drawPath(path);
  painter->setPen(QPen(fgColor));
  painter->drawText(textRect_, text_);
}

void ThermoDisplay2Callout::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  event->setAccepted(true);
}

void ThermoDisplay2Callout::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton){
    setPos(mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)));
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
  }
}

void ThermoDisplay2Callout::setText(const QString &text)
{
  text_ = text;
  QFontMetrics metrics(font_);
  textRect_ = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, text_);
  textRect_.translate(5, 5);
  prepareGeometryChange();
  rect_ = textRect_.adjusted(-5, -5, 5, 5);
}

void ThermoDisplay2Callout::setAnchor(QPointF point, QAbstractSeries *series)
{
  anchor_ = point;
  series_ = series;
}

void ThermoDisplay2Callout::updateGeometry()
{
  prepareGeometryChange();
  setPos(chart_->mapToPosition(anchor_, series_) + QPoint(10, -50));
}

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

#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoAlignmentModel.h"

DefoAlignmentModel::DefoAlignmentModel(QObject *parent) :
    QObject(parent)
{
  origin_.setX(0.50);
  origin_.setY(0.25);
  tip_.setX(0.50);
  tip_.setY(0.75);

  selectedPoint_ = None;
}

float DefoAlignmentModel::getClosestPoint(AlignmentPoint & point,
                                          float width, float height,
                                          float x, float y) {

  point = None;
  float distance = 1.e9;

  float xp, yp, temp;

  xp = origin_.x()*width;
  yp = origin_.y()*height;
  temp = std::sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp));
  if (temp<distance) {
    distance = temp;
    point = Origin;
  }

  xp = tip_.x()*width;
  yp = tip_.y()*height;
  temp = std::sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp));
  if (temp<distance) {
    distance = temp;
    point = Tip;
  }

  return distance;
}

void DefoAlignmentModel::setPoint(AlignmentPoint point, float newX, float newY)
{
  if (newX<0.025 || newX>0.975 || newY<0.025 || newY>0.975) return;
  if (point==None) return;

  QPointF * p = 0;
  QPointF * other = 0;
  switch (point) {
  case Origin:
    p = &origin_;
    other = &tip_;
    break;
  case Tip:
    p = &tip_;
    other = &origin_;
    break;
  default:
    break;
  }

  float length = std::sqrt((newX-other->x())*(newX-other->x()) + (newY-other->y())*(newY-other->y()));
  if (length<0.05) return;

  bool needUpdate = (newX!=p->x()) || (newY!=p->y());

  p->setX(newX);
  p->setY(newY);

  if (needUpdate) emit alignmentChanged(getAngle());
}

void DefoAlignmentModel::reset() {
  if (tip_.x() == origin_.x()) return;
  float mx = (tip_.x() + origin_.x())/2.0;
  origin_.setX(mx);
  tip_.setX(mx);
  emit alignmentChanged(getAngle());
}

double DefoAlignmentModel::getAngle() const {

  double dx = origin_.x()-tip_.x();
  double dy = origin_.y()-tip_.y();

  if (dy<0) {
    return std::atan2(-dx, -dy);
  } else {
    return std::atan2(dx, dy);
  }
}

void DefoAlignmentModel::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoAlignment");

  stream.writeStartElement("Origin");
  stream.writeAttribute("x", QString().setNum(origin_.x(), 'e', 6));
  stream.writeAttribute("y", QString().setNum(origin_.y(), 'e', 6));
  stream.writeEndElement();

  stream.writeStartElement("Tip");
  stream.writeAttribute("x", QString().setNum(tip_.x(), 'e', 6));
  stream.writeAttribute("y", QString().setNum(tip_.y(), 'e', 6));
  stream.writeEndElement();

  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoAlignmentModel::read(const QString& filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoAlignment") {
      // do something
    }

    if (stream.isStartElement() && stream.name()=="Origin") {
      double x = stream.attributes().value("x").toString().toDouble();
      double y = stream.attributes().value("y").toString().toDouble();
      origin_.setX(x);
      origin_.setY(y);
    }

    if (stream.isStartElement() && stream.name()=="Tip") {
      double x = stream.attributes().value("x").toString().toDouble();
      double y = stream.attributes().value("y").toString().toDouble();
      tip_.setX(x);
      tip_.setY(y);
    }
  }

  emit alignmentChanged(getAngle());
}

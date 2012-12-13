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
  switch (point) {
  case Origin:
    p = &origin_;
    break;
  case Tip:
    p = &tip_;
    break;
  default:
    break;
  }

  bool needUpdate = (newX!=p->x()) || (newY!=p->y());

  p->setX(newX);
  p->setY(newY);

  if (needUpdate) emit alignmentChanged(getAngle());
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

void DefoAlignmentModel::write(const QDir& path)
{
  QString fileLocation = path.absoluteFilePath("alignment.xml");

  QFile file(fileLocation);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoAlignment");

  stream.writeStartElement("Origin");
  stream.writeAttribute("x", QString().setNum(origin_.x()));
  stream.writeAttribute("y", QString().setNum(origin_.y()));
  stream.writeEndElement();

  stream.writeStartElement("Tip");
  stream.writeAttribute("x", QString().setNum(tip_.x()));
  stream.writeAttribute("y", QString().setNum(tip_.y()));
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
      float x = stream.attributes().value("x").toString().toFloat();
      float y = stream.attributes().value("y").toString().toFloat();
      origin_.setX(x);
      origin_.setY(y);
    }

    if (stream.isStartElement() && stream.name()=="Tip") {
      float x = stream.attributes().value("x").toString().toFloat();
      float y = stream.attributes().value("y").toString().toFloat();
      tip_.setX(x);
      tip_.setY(y);
    }
  }

  emit alignmentChanged(getAngle());
}

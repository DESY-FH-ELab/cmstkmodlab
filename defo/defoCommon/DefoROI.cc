#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoROI.h"

DefoROI::DefoROI()
{
  push_back(QPointF(0.05, 0.05));
  push_back(QPointF(0.95, 0.05));
  push_back(QPointF(0.95, 0.95));
  push_back(QPointF(0.05, 0.95));

  selectedPoint_ = -1;
}

float DefoROI::getClosestPoint(int & index,
                               float width, float height,
                               float x, float y)
{
  index = -1;
  float distance = 1.e9;
  float temp;
  for (int j=0;j<size();++j) {
    const QPointF & p =at(j);
    float xp = p.x()*width;
    float yp = p.y()*height;

    temp = std::sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp));
    if (temp<distance) {
      distance = temp;
      index = j;
    }
  }
  return distance;
}

bool DefoROI::setPoint(int index, float newX, float newY)
{
  if (newX<0.025 || newX>0.975 || newY<0.025 || newY>0.975) return false;

  QPointF & p = this->operator[](index);

  bool needUpdate = (newX!=p.x()) || (newY!=p.y());

  p.setX(newX);
  p.setY(newY);

  if (needUpdate) return true;

  return false;
}

bool DefoROI::containsPoint(float width, float height,
                            float x, float y)
{
  QPointF p(x/width, y/height);
  return QPolygonF::containsPoint(p, Qt::OddEvenFill);
}

bool DefoROI::insertPointBefore()
{
  if (selectedPoint_==-1) return false;

  if (selectedPoint_==0) {
    const QPointF & lastP = last();
    const QPointF & firstP = first();
    QPointF np((lastP.x()+firstP.x())/2,
               (lastP.y()+firstP.y())/2);
    insert(0, np);
    return true;
  } else {
    const QPointF & lastP = at(selectedPoint_-1);
    const QPointF & firstP = at(selectedPoint_);
    QPointF np((lastP.x()+firstP.x())/2,
               (lastP.y()+firstP.y())/2);
    insert(selectedPoint_, np);
    return true;
  }
  return false;
}

bool DefoROI::insertPointAfter()
{
  if (selectedPoint_==-1) return false;

  if (selectedPoint_==size()-1) {
    const QPointF & lastP = last();
    const QPointF & firstP = first();
    QPointF np((lastP.x()+firstP.x())/2,
               (lastP.y()+firstP.y())/2);
    push_back(np);
    return true;
  } else {
    const QPointF & lastP = at(selectedPoint_);
    const QPointF & firstP = at(selectedPoint_+1);
    QPointF np((lastP.x()+firstP.x())/2,
               (lastP.y()+firstP.y())/2);
    insert(selectedPoint_+1, np);
    return true;
  }
  return false;
}

bool DefoROI::removePoint()
{
  if (selectedPoint_==-1) return false;
  if (size()>3) {
    remove(selectedPoint_);
    return true;
  }
  return false;
}

void DefoROI::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoROI");

  stream.writeStartElement("Points");
  stream.writeAttribute("count", QString().setNum(size()));
  for (int i=0;i<size();++i) {
    stream.writeStartElement("Point");
    stream.writeAttribute("index", QString().setNum(i));
    const QPointF &p = at(i);
    stream.writeAttribute("x", QString().setNum(p.x(), 'e', 6));
    stream.writeAttribute("y", QString().setNum(p.y(), 'e', 6));
    stream.writeEndElement();
  }
  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoROI::read(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoROI") {
      // do something
    }

    if (stream.isStartElement() && stream.name()=="Points") {
      int count = stream.attributes().value("count").toString().toInt();
      resize(count);
    }

    if (stream.isStartElement() && stream.name()=="Point") {
      int index = stream.attributes().value("index").toString().toInt();
      QPointF & p = this->operator[](index);
      double x = stream.attributes().value("x").toString().toDouble();
      double y = stream.attributes().value("y").toString().toDouble();
      p.setX(x);
      p.setY(y);
    }
  }
}

bool DefoROI::assignFrom(DefoROI* other)
{
  if (other==this) return false;

  clear();

  for (int i=0;i<other->size();++i) {
    append(other->at(i));
  }

  return true;
}
}

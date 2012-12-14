#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoROIModel.h"

DefoROIModel::DefoROIModel(QObject *parent) :
    QObject(parent)
{
  push_back(QPointF(0.05, 0.05));
  push_back(QPointF(0.95, 0.05));
  push_back(QPointF(0.95, 0.95));
  push_back(QPointF(0.05, 0.95));
  selectedPoint_ = -1;
}

float DefoROIModel::getClosestPoint(int & index,
                                        float width, float height,
                                        float x, float y) {

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

void DefoROIModel::setPoint(int index, float newX, float newY)
{
  if (newX<0.025 || newX>0.975 || newY<0.025 || newY>0.975) return;

  QPointF & p = this->operator[](index);

  bool needUpdate = (newX!=p.x()) || (newY!=p.y());

  p.setX(newX);
  p.setY(newY);

  if (needUpdate) emit roiChanged();
}

bool DefoROIModel::containsPoint(float width, float height,
                                 float x, float y) {

  QPointF p(x/width, y/height);
  return QPolygonF::containsPoint(p, Qt::OddEvenFill);
}

void DefoROIModel::appendPoint() {
  const QPointF & lastP = last();
  const QPointF & firstP = first();
  QPointF np((lastP.x()+firstP.x())/2,
             (lastP.y()+firstP.y())/2);
  push_back(np);
  emit roiChanged();
}

void DefoROIModel::removePoint() {
  if (size()>3) {
    pop_back();
    emit roiChanged();
  }
}

void DefoROIModel::write(const QDir& path)
{
  QString fileLocation = path.absoluteFilePath("roi.xml");

  QFile file(fileLocation);
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

void DefoROIModel::read(const QString& filename) {

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

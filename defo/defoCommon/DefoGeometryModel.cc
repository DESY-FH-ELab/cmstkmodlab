#include <string>
#include <iostream>
#include <fstream>

#include <QFile>
#include <QXmlStreamWriter>

#include "ApplicationConfig.h"

#include "DefoGeometryModel.h"

DefoGeometryModel::DefoGeometryModel(
    QObject *parent
  ) :
    QObject(parent)
{
  angle1_ = ApplicationConfig::instance()->getValue<double>( "ANGLE1" );
  angle2_ = ApplicationConfig::instance()->getValue<double>( "ANGLE2" );
  angle3_ = ApplicationConfig::instance()->getValue<double>( "ANGLE3" );
  distance_ = ApplicationConfig::instance()->getValue<double>( "DISTANCE" );
  height1_ = ApplicationConfig::instance()->getValue<double>( "HEIGHT1" );
  height2_ = ApplicationConfig::instance()->getValue<double>( "HEIGHT2" );
}

void DefoGeometryModel::setAngle1(double v) {
  bool valueChanged = !(v==angle1_);
  angle1_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("ANGLE1", angle1_);
    emit geometryChanged();
  }
}

void DefoGeometryModel::setAngle2(double v) {
  bool valueChanged = !(v==angle2_);
  angle2_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("ANGLE2", angle2_);
    emit geometryChanged();
  }
}

void DefoGeometryModel::setAngle3(double v) {
  bool valueChanged = !(v==angle3_);
  angle3_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("ANGLE3", angle3_);
    emit geometryChanged();
  }
}

void DefoGeometryModel::setDistance(double v) {
  bool valueChanged = !(v==distance_);
  distance_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("DISTANCE", distance_);
    emit geometryChanged();
  }
}

void DefoGeometryModel::setHeight1(double v) {
  bool valueChanged = !(v==height1_);
  height1_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("HEIGHT1", height1_);
    emit geometryChanged();
  }
}

void DefoGeometryModel::setHeight2(double v) {
  bool valueChanged = !(v==height2_);
  height2_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("HEIGHT2", height2_);
    emit geometryChanged();
  }
}

void DefoGeometryModel::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoGeometry");

  stream.writeAttribute("angle1", QString().setNum(angle1_, 'e', 6));
  stream.writeAttribute("angle2", QString().setNum(angle2_, 'e', 6));
  stream.writeAttribute("angle3", QString().setNum(angle3_, 'e', 6));
  stream.writeAttribute("distance", QString().setNum(distance_, 'e', 6));
  stream.writeAttribute("height1", QString().setNum(height1_, 'e', 6));
  stream.writeAttribute("height2", QString().setNum(height2_, 'e', 6));

  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoGeometryModel::read(const QString& filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoGeometry") {
      angle1_ = stream.attributes().value("angle1").toString().toDouble();
      angle2_ = stream.attributes().value("angle2").toString().toDouble();
      if (stream.attributes().hasAttribute("angle2")) {
	angle3_ = stream.attributes().value("angle3").toString().toDouble();
      } else {
	angle3_ = 0;
      }
      distance_ = stream.attributes().value("distance").toString().toDouble();
      height1_ = stream.attributes().value("height1").toString().toDouble();
      height2_ = stream.attributes().value("height2").toString().toDouble();
    }
  }

  emit geometryChanged();
}

#include <string>
#include <iostream>
#include <fstream>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoConfig.h"
#include "DefoGeometryModel.h"
#include "DefoConfigReader.h"

DefoGeometryModel::DefoGeometryModel(
    QObject *parent
  ) :
    QObject(parent)
{
  DefoConfigReader cfgReader( std::string(Defo::CMSTkModLabBasePath) + "/defo/defo.cfg" );

  angle1_ = cfgReader.getValue<double>( "ANGLE1" );
  angle2_ = cfgReader.getValue<double>( "ANGLE2" );
  distance_ = cfgReader.getValue<double>( "DISTANCE" );
  height_ = cfgReader.getValue<double>( "HEIGHT" );
}

void DefoGeometryModel::setAngle1(double v) {
  bool valueChanged = (v==angle1_);
  angle1_ = v;
  if (valueChanged) emit geometryChanged();
}

void DefoGeometryModel::setAngle2(double v) {
  bool valueChanged = (v==angle2_);
  angle2_ = v;
  if (valueChanged) emit geometryChanged();
}

void DefoGeometryModel::setDistance(double v) {
  bool valueChanged = (v==distance_);
  distance_ = v;
  if (valueChanged) emit geometryChanged();
}

void DefoGeometryModel::setHeight(double v) {
  bool valueChanged = (v==height_);
  height_ = v;
  if (valueChanged) emit geometryChanged();
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
  stream.writeAttribute("distance", QString().setNum(distance_, 'e', 6));
  stream.writeAttribute("height", QString().setNum(height_, 'e', 6));

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
      distance_ = stream.attributes().value("distance").toString().toDouble();
      height_ = stream.attributes().value("height").toString().toDouble();
    }
  }

  emit geometryChanged();
}

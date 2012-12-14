#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoColorSelectionModel.h"

DefoColorSelectionModel::DefoColorSelectionModel(QObject *parent) :
    QObject(parent)
{
  color_.setHsvF(0.0, 0.0, 1.0, 1.0);
}

float DefoColorSelectionModel::getDistance(float hue, float saturation) {

  float dhue = hue - color_.hueF();
  float dsat = saturation - color_.saturationF();
  return std::sqrt(dhue*dhue + dsat*dsat);
}

void DefoColorSelectionModel::setColor(float hue, float saturation)
{
  if (hue<0.0 || hue>1.0 || saturation<0.0 || saturation>1.0) return;

  bool needUpdate = (hue!=color_.hueF()) || (saturation!=color_.saturationF());

  color_.setHsvF(hue, saturation, 1.0, 1.0);

  if (needUpdate) emit colorChanged(hue, saturation);
}

void DefoColorSelectionModel::write(const QDir& path)
{
  QString fileLocation = path.absoluteFilePath("color.xml");

  QFile file(fileLocation);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoColor");
  stream.writeAttribute("H", QString().setNum(color_.hueF(), 'e', 6));
  stream.writeAttribute("S", QString().setNum(color_.saturationF(), 'e', 6));
  stream.writeEndElement();

  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoColorSelectionModel::read(const QString& filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoColor") {
      double H = stream.attributes().value("H").toString().toDouble();
      double S = stream.attributes().value("S").toString().toDouble();
      color_.setHsvF(H, S, 1.0, 1.0);
     }
  }

  emit colorChanged(color_.hueF(), color_.saturationF());
}

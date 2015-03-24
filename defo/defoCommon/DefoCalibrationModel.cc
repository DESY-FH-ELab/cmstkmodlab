#include <string>
#include <iostream>
#include <fstream>

#include <QFile>
#include <QXmlStreamWriter>

#include "ApplicationConfig.h"

#include "DefoCalibrationModel.h"

DefoCalibrationModel::DefoCalibrationModel(QObject *parent)
: QObject(parent)
{
  calibX_ = ApplicationConfig::instance()->getValue<double>( "CALIBX" );
  calibY_ = ApplicationConfig::instance()->getValue<double>( "CALIBY" );
}

void DefoCalibrationModel::setCalibX(double v) {
  bool valueChanged = !(v==calibX_);
  calibX_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("CALIBX", calibX_);
    emit calibrationChanged();
  }
}

void DefoCalibrationModel::setCalibY(double v) {
  bool valueChanged = !(v==calibY_);
  calibY_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("CALIBY", calibY_);
    emit calibrationChanged();
  }
}

void DefoCalibrationModel::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoCalibration");

  stream.writeAttribute("x", QString().setNum(calibX_, 'e', 6));
  stream.writeAttribute("y", QString().setNum(calibY_, 'e', 6));

  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoCalibrationModel::read(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoCalibration") {
      calibX_ = stream.attributes().value("x").toString().toDouble();
      calibY_ = stream.attributes().value("y").toString().toDouble();
    }
  }

  emit calibrationChanged();
}

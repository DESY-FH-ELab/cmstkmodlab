#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "DefoRecoMeasurement.h"
#include "DefoRecoMeasurementListModel.h"

DefoRecoMeasurementListModel::DefoRecoMeasurementListModel(QObject *parent) :
    DefoMeasurementListModel(parent)
{}

void DefoRecoMeasurementListModel::write(const QDir& path)
{
  DefoMeasurement* measurement;
  for (int i = 0; i < this->getMeasurementCount(); ++i) {
    measurement = this->getMeasurement(i);
    if (measurement->isPreview()) continue;
    measurement->write(path);
  }
}

void DefoRecoMeasurementListModel::read(const QString& filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);
  DefoRecoMeasurement* measurement;
  QDir basepath(filename);
  basepath.cdUp();

  while (!stream.atEnd()) {
    stream.readNextStartElement();
    if (stream.isStartElement() && stream.name()=="DefoMeasurement") {
      QString timestamp = stream.attributes().value("timestamp").toString();
      QDateTime dt = QDateTime::fromString(timestamp, "yyyyMMddhhmmss");

      measurement = new DefoRecoMeasurement(dt);
      measurement->read(basepath);

      measurement->readImages();

      addMeasurement(measurement);
    }
  }
}

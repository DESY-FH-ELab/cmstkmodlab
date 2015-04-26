#include <iostream>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoRecoMeasurement.h"

DefoRecoMeasurement::DefoRecoMeasurement(const QString& imageLocation, bool preview) :
    DefoMeasurement(imageLocation, preview)
{
  recoPointRecognitionThresholds_.resize(DefoPointRecognitionModel::MAXTHRESHOLDS);
}

void DefoRecoMeasurement::setRecoThresholdValue(DefoPointRecognitionModel::Threshold threshold,
                                       int value)
{
  recoPointRecognitionThresholds_[threshold] = value;
}

int DefoRecoMeasurement::getRecoThresholdValue(DefoPointRecognitionModel::Threshold threshold)
{
  return recoPointRecognitionThresholds_[threshold];
}

void DefoRecoMeasurement::write(const QDir& path)
{
  QString fileLocation = path.absoluteFilePath("%1reco.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));

  QFile file(fileLocation);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoRecoMeasurement");
  stream.writeAttribute("timestamp", timestamp_.toString("yyyyMMddhhmmss"));

  stream.writeStartElement("Thresholds");
  for (uint i=0;i<recoPointRecognitionThresholds_.size();++i) {
    stream.writeStartElement("Threshold");
    stream.writeAttribute("index", QString().setNum(i+1));
    stream.writeAttribute("value", QString().setNum(recoPointRecognitionThresholds_[i]));
    stream.writeEndElement();
  }
  stream.writeEndElement();

  stream.writeStartElement("HalfSquareWidth");
  stream.writeAttribute("value", QString().setNum(recoPointRecognitionHalfSquareWidth_));
  stream.writeEndElement();

  stream.writeEndElement();

  stream.writeEndDocument();

  fileLocation = path.absoluteFilePath("%1roi.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));
  roi_.write(fileLocation);
}

void DefoRecoMeasurement::read(const QDir& path)
{
  DefoMeasurement::read(path);

  recoPointRecognitionThresholds_.resize(pointRecognitionThresholds_.size());
  for (unsigned int i=0;i<recoPointRecognitionThresholds_.size();++i) {
    recoPointRecognitionThresholds_[i] = pointRecognitionThresholds_[i];
  }
  recoPointRecognitionHalfSquareWidth_ = pointRecognitionHalfSquareWidth_;

  QString fileLocation = path.absoluteFilePath("%1reco.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));

  QFile file(fileLocation);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoRecoMeasurement") {
      // consistency check
      QString timestamp = stream.attributes().value("timestamp").toString();
      QDateTime dt = QDateTime::fromString(timestamp, "yyyyMMddhhmmss");
      if (dt!=getTimeStamp()) {
        // do something
      }
    }

    if (stream.isStartElement() && stream.name()=="Thresholds") {
      recoPointRecognitionThresholds_.resize(3);
    }

    if (stream.isStartElement() && stream.name()=="Threshold") {
      unsigned int index = stream.attributes().value("index").toString().toUInt() - 1;
      int threshold = stream.attributes().value("value").toString().toInt();
      recoPointRecognitionThresholds_[index] = threshold;
    }

    if (stream.isStartElement() && stream.name()=="HalfSquareWidth") {
      int hsw = stream.attributes().value("value").toString().toInt();
      recoPointRecognitionHalfSquareWidth_ = hsw;
    }
  }

  fileLocation = path.absoluteFilePath("%1roi.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));
  roi_.read(fileLocation);
}

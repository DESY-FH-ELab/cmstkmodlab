#include <nqlogger.h>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoExifReader.h"
#include "DefoImageAverager.h"

#include "DefoMeasurement.h"

DefoMeasurement::DefoMeasurement(const QDateTime& timestamp)
  : timestamp_(timestamp),
    previewImage_(false)
{

}

DefoMeasurement::DefoMeasurement(const QString& imageLocation, bool preview)
  : timestamp_(QDateTime::currentDateTime().toUTC()),
    previewImage_(preview)
{
  imageLocations_.append(imageLocation);
  readImages();
}

DefoMeasurement::DefoMeasurement(const QStringList& imageLocations)
  : timestamp_(QDateTime::currentDateTime().toUTC()),
    imageLocations_(imageLocations),
    previewImage_(false)
{
  readImages();
}

void DefoMeasurement::readImages()
{
  NQLogMessage("DefoMeasurement") << "readImages " << imageLocations_.size();

  if (imageLocations_.size()==0) {

    return;

  } else if (imageLocations_.size()==1) {

    if (image_.isNull()) {
      QImage temp(imageLocations_.front());
      QMatrix matrix;
      matrix.rotate(90);
      image_ = temp.transformed(matrix);
    }

  } else {

    if (image_.isNull()) {
      DefoImageAverager averager(imageLocations_);
      QImage temp(averager.getAveragedImage());
      QMatrix matrix;
      matrix.rotate(90);
      image_ = temp.transformed(matrix);
    }

  }
}

void DefoMeasurement::setTimeStamp(const QDateTime& dt)
{
  timestamp_ = dt;
}

const QDateTime & DefoMeasurement::getTimeStamp() const
{
  return timestamp_;
}

QImage DefoMeasurement::getImage() const
{
  return image_;
}

int DefoMeasurement::getWidth() const
{
  return image_.width();
}

int DefoMeasurement::getHeight() const
{
  return image_.height();
}

void DefoMeasurement::setImageLocation(const QString& imageLocation)
{
  imageLocations_.clear();
  imageLocations_.append(imageLocation);
}

void DefoMeasurement::setImageLocations(const QStringList& imageLocations)
{
  imageLocations_ = imageLocations;
}

void DefoMeasurement::readExifData()
{
  DefoExifReader reader(imageLocations_.front());
  reader.read();

  exifFocalLength_ = reader.getFloatValue("Exif.Photo.FocalLength");
  exifExposureTime_ = reader.getFloatValue("Exif.Photo.ExposureTime");
  exifExposureTimeString_ = reader.getStringValue("Exif.Photo.ExposureTime");
  exifAperture_ = reader.getLongValue("Exif.Photo.FNumber");
  exifISO_ = reader.getLongValue("Exif.Photo.ISOSpeedRatings");
}

void DefoMeasurement::acquireData(const DefoCameraModel* model)
{
  comment_ = model->commentDocument()->toPlainText();
  calibAmplitude_ = model->getCalibAmplitude();
}

void DefoMeasurement::acquireData(const DefoPointRecognitionModel* model)
{
  pointRecognitionThresholds_.clear();

  pointRecognitionThresholds_.push_back(model->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_1));
  pointRecognitionThresholds_.push_back(model->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_2));
  pointRecognitionThresholds_.push_back(model->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_3));
  
  pointRecognitionHalfSquareWidth_ = model->getHalfSquareWidth();
}

void DefoMeasurement::acquireData(const DefoConradModel* model)
{
  conradState_ = model->getDeviceState();
  if (conradState_ != READY) return;

  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_1));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_2));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_3));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_4));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_5));
  ledState_ = model->getSwitchState(DefoConradModel::CALIBRATION_LEDS);
}

void DefoMeasurement::acquireData(const DefoJulaboModel* model)
{
  julaboState_ = model->getDeviceState();
  if (julaboState_ != READY) return;

  circulatorState_ = model->isCirculatorEnabled();
  bathTemperature_ = model->getBathTemperature();
}

void DefoMeasurement::acquireData(const KeithleyModel* model)
{
  keithleyState_ = model->getDeviceState();
  if (keithleyState_ != READY) return;
  
  temperatureSensorStates_.clear();
  temperatures_.clear();
  
  for (unsigned int channel = 0;channel<10;++channel) {
    temperatureSensorStates_.push_back(model->getSensorState(channel));
    if (temperatureSensorStates_.back()==READY) {
      temperatures_.push_back(model->getTemperature(channel));
    } else {
      temperatures_.push_back(0);
    }
  }
}

void DefoMeasurement::write(const QDir& path)
{
  QString fileLocation = path.absoluteFilePath("%1.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));

  QFile file(fileLocation);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoMeasurement");
  stream.writeAttribute("timestamp", timestamp_.toString("yyyyMMddhhmmss"));

  stream.writeStartElement("Comment");
  stream.writeCharacters(comment_);
  stream.writeEndElement();

  stream.writeStartElement("Images");
  stream.writeAttribute("count", QString().setNum(imageLocations_.size()));
  stream.writeEndElement();

  stream.writeStartElement("Calibration");
  stream.writeAttribute("amplitude", QString().setNum(calibAmplitude_));
  stream.writeEndElement();

  stream.writeStartElement("Thresholds");
  for (uint i=0;i<pointRecognitionThresholds_.size();++i) {
    stream.writeStartElement("Threshold");
    stream.writeAttribute("index", QString().setNum(i+1));
    stream.writeAttribute("value", QString().setNum(pointRecognitionThresholds_[i]));
    stream.writeEndElement();
  }
  stream.writeEndElement();
  
  stream.writeStartElement("HalfSquareWidth");
  stream.writeAttribute("value", QString().setNum(pointRecognitionHalfSquareWidth_));
  stream.writeEndElement();

  stream.writeStartElement("Exif");
  stream.writeStartElement("FocalLength");
  stream.writeAttribute("value", QString().setNum(exifFocalLength_));
  stream.writeEndElement();
  stream.writeStartElement("ExposureTime");
  stream.writeAttribute("value", QString().setNum(exifExposureTime_));
  stream.writeAttribute("string", exifExposureTimeString_);
  stream.writeEndElement();
  stream.writeStartElement("Aperture");
  stream.writeAttribute("value", QString().setNum(exifAperture_));
  stream.writeEndElement();
  stream.writeStartElement("ISO");
  stream.writeAttribute("value", QString().setNum(exifISO_));
  stream.writeEndElement();
  stream.writeEndElement();
  
  stream.writeStartElement("Conrad");
  stream.writeAttribute("available", QString().setNum(conradState_==READY));
  if (conradState_==READY) {
    for (unsigned int idx = 0;idx<5;++idx) {
      QString name = "Panel";
      name += QString().setNum(idx+1);
      stream.writeStartElement(name);
      stream.writeAttribute("on", QString().setNum(panelStates_[idx]==READY));
      stream.writeEndElement();
    }
    stream.writeStartElement("LEDs");
    stream.writeAttribute("on", QString().setNum(ledState_==READY));
    stream.writeEndElement();
    stream.writeEndElement();
  }

  stream.writeStartElement("Julabo");
  stream.writeAttribute("available", QString().setNum(julaboState_==READY));
  if (julaboState_==READY) {
    stream.writeStartElement("Circulator");
    stream.writeAttribute("on", QString().setNum((int)circulatorState_));
    stream.writeEndElement();
    stream.writeStartElement("BathTemperature");
    stream.writeAttribute("value", QString().setNum(bathTemperature_));
    stream.writeEndElement();
  }
  stream.writeEndElement();
  
  stream.writeStartElement("Keithley");
  stream.writeAttribute("available", QString().setNum(keithleyState_==READY));
  if (keithleyState_==READY) {
    for (unsigned int idx = 0;
         idx < temperatureSensorStates_.size() && idx < temperatures_.size();
         ++idx) {
      if (temperatureSensorStates_[idx]==READY) {
        stream.writeStartElement("TemperatureSensor");
        stream.writeAttribute("index", QString().setNum(idx));
        stream.writeAttribute("temperature", QString().setNum(temperatures_[idx]));
        stream.writeEndElement();
      }
    }
  }
  stream.writeEndElement();
  
  stream.writeEndElement();
  
  stream.writeEndDocument();
}

void DefoMeasurement::read(const QDir&path)
{
  pointRecognitionHalfSquareWidth_ = 15;

  QString fileLocation = path.absoluteFilePath("%1.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));

  comment_ = "";

  QFile file(fileLocation);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);
  int nImages = 0;
  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoMeasurement") {
      // consistency check
      QString timestamp = stream.attributes().value("timestamp").toString();
      QDateTime dt = QDateTime::fromString(timestamp, "yyyyMMddhhmmss");
      if (dt!=getTimeStamp()) {
        // do something
      }
      nImages = 0;
    }

    if (stream.isStartElement() && stream.name()=="Comment") {
      comment_ = stream.readElementText();
    }

    if (stream.isStartElement() && stream.name()=="Images") {
      nImages = stream.attributes().value("count").toString().toInt();

      QStringList imageLocations;
      for (int i=1;i<=nImages;i++) {
        QString imageLocation = path.absoluteFilePath("%1_%2.jpg");
        imageLocation = imageLocation.arg(getTimeStamp().toString("yyyyMMddhhmmss")).arg(QString::number(i));
        imageLocations.append(imageLocation);
      }

      setImageLocations(imageLocations);
    }

    if (stream.isStartElement() && stream.name()=="Calibration") {
      calibAmplitude_ = stream.attributes().value("amplitude").toString().toInt();
    }

    if (stream.isStartElement() && stream.name()=="Thresholds") {
      pointRecognitionThresholds_.resize(DefoPointRecognitionModel::MAXTHRESHOLDS);
    }

    if (stream.isStartElement() && stream.name()=="Threshold") {
      unsigned int index = stream.attributes().value("index").toString().toUInt() - 1;
      int threshold = stream.attributes().value("value").toString().toInt();
      pointRecognitionThresholds_[index] = threshold;
    }

    if (stream.isStartElement() && stream.name()=="HalfSquareWidth") {
      int hsw = stream.attributes().value("value").toString().toInt();
      pointRecognitionHalfSquareWidth_ = hsw;
    }

    if (stream.isStartElement() && stream.name()=="FocalLength") {
      exifFocalLength_ = stream.attributes().value("value").toString().toFloat();
    }
    if (stream.isStartElement() && stream.name()=="ExposureTime") {
      exifExposureTime_ = stream.attributes().value("value").toString().toFloat();
      exifExposureTimeString_ = stream.attributes().value("string").toString();
    }
    if (stream.isStartElement() && stream.name()=="Aperture") {
      exifAperture_ = stream.attributes().value("value").toString().toFloat();
    }
    if (stream.isStartElement() && stream.name()=="ISO") {
      exifISO_ = stream.attributes().value("value").toString().toInt();
    }

    if (stream.isStartElement() && stream.name()=="Conrad") {
      bool state = stream.attributes().value("available").toString().toInt();
      panelStates_.resize(5);
      for (unsigned int idx = 0;idx<5;++idx) {
        panelStates_[idx] = OFF;
      }
      ledState_ = OFF;
      if (state) {
        conradState_ = READY;
      } else {
        conradState_ = OFF;
      }
    }
    if (stream.isStartElement() && stream.name().toString().startsWith("Panel")) {
      unsigned int index = stream.name().toString().replace("Panel", "").toUInt() - 1;
      bool state = stream.attributes().value("on").toString().toInt();
      if (state) {
        panelStates_[index] = READY;
      } else {
        panelStates_[index] = OFF;
      }      
    }
    if (stream.isStartElement() && stream.name()=="LEDs") {
      bool state = stream.attributes().value("on").toString().toInt();
      if (state) {
        ledState_ = READY;
      } else {
        ledState_ = OFF;
      }      
    }

    if (stream.isStartElement() && stream.name()=="Julabo") {
      bool state = stream.attributes().value("available").toString().toInt();
      circulatorState_ = false;
      bathTemperature_ = -99.9;
      if (state) {
        julaboState_ = READY;
      } else {
        julaboState_ = OFF;
      }
    }
    if (stream.isStartElement() && stream.name()=="Circulator") {
      circulatorState_ = stream.attributes().value("value").toString().toInt();
    }
    if (stream.isStartElement() && stream.name()=="BathTemperature") {
      bathTemperature_ = stream.attributes().value("value").toString().toFloat();
    }
 
    if (stream.isStartElement() && stream.name()=="Keithley") {
      bool state = stream.attributes().value("available").toString().toInt();
      temperatureSensorStates_.resize(10);
      temperatures_.resize(10);
      for (unsigned int idx = 0;idx < 10;++idx) {
        temperatureSensorStates_[idx] = OFF;
        temperatures_[idx] = -99.9;
      }
      if (state) {
        keithleyState_ = READY;
      } else {
        keithleyState_ = OFF;
      }
    }
    if (stream.isStartElement() && stream.name()=="TemperatureSensor") {
      unsigned int idx = stream.attributes().value("index").toString().toUInt();
      temperatureSensorStates_[idx] = READY;
      temperatures_[idx] = stream.attributes().value("threshold").toString().toFloat();
    }
  }

  if (nImages==0) {
    QStringList imageLocations;
    QString imageLocation = path.absoluteFilePath("%1.jpg");
    imageLocation = imageLocation.arg(getTimeStamp().toString("yyyyMMddhhmmss"));
    imageLocations.append(imageLocation);
    setImageLocations(imageLocations);
  }
}

DefoMeasurementPair::DefoMeasurementPair(DefoMeasurement* reference,
                                         DefoMeasurement* deformed)
  : std::pair<DefoMeasurement*,DefoMeasurement*>(reference, deformed)
{

}

void DefoMeasurementPair::setSurface(const DefoSurface& surface)
{
  surface_ = surface;
}

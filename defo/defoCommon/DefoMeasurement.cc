#include <iostream>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoExifReader.h"

#include "DefoMeasurement.h"

DefoMeasurement::DefoMeasurement(const QString& imageLocation, bool preview) :
    timestamp_(QDateTime::currentDateTimeUtc())
  , imageLocation_(imageLocation)
  , previewImage_(preview)
{
  if (image_.isNull()) {
    QImage temp(imageLocation_);
    QMatrix matrix;
    matrix.rotate(90);
    image_ = temp.transformed(matrix);
  }
}

void DefoMeasurement::setTimeStamp(const QDateTime& dt) {
  timestamp_ = dt;
}

const QDateTime & DefoMeasurement::getTimeStamp() const {
  return timestamp_;
}

QImage DefoMeasurement::getImage() const {

  return image_;
}

/**
  Image recognition algorithm that searches for the reflection of grid dots
  in the measurement picture. An QRect area that defines a subregion of the
  image may be provided. In the case this points to NULL, the whole image is
  searched for suitable points.
  */
const DefoPointCollection* DefoMeasurement::findPoints(
    const QRect* searchArea
  , const QPolygonF* roi
  , int step1Threshold
  , int step2Threshold
  , int step3Threshold
  , int halfSquareWidth
) const {

  DefoPointCollection* points = new DefoPointCollection();

  DefoSquareCollection forbiddenAreas;
  const int step2TotalThreshold = 4 * step2Threshold;

//  const int width = image_.width();
//  const int height = image_.height();

  // FIXME central logging
//  if( debugLevel_ >= 2 )
//    std::cout << " [DefoMeasurement::findPoints] =2= Image has: "
//              << width << " x " << height << " pixels" << std::endl;

  QImage image = getImage();

  // Determine intersection of area and image
  QRect imageArea = image.rect();
  QRect area;
  if (searchArea == NULL)
    area = imageArea;
  else
    area = imageArea & *searchArea;

  // FIXME central logging
//  if( debugLevel_ >= 2 )
//    std::cout <<  " [DefoMeasurement::findPoints] =2= Scanning area ("
//              <<  area.x() << ',' <<  area.y() << ';'
//              <<  area.width() << ',' <<  area.height() << ')' << std::endl;

  // Scan x-line per x-line
  int y = area.y();
  while (  y < (area.y() + area.height()) ) {

    int x = area.x();
    while ( x < (area.x() + area.width())) {

      // Check if the point is in the allowed area AND bright enough
      // FIXME leap over forbidden areas instead of having to skip every pixel

      if (!forbiddenAreas.isInside( DefoPoint( x, y ) )
           && qGray( image.pixel(x,y) ) > step1Threshold
           && imageArea.contains( x+3, y+3 )
      ) {

        // We now have an initial seed. Check average amplitude
        // of some more pixels ahead
        double theProbe = 0.;
        theProbe += qGray( image.pixel( x + 2, y + 2 ) );
        theProbe += qGray( image.pixel( x + 2, y + 3 ) );
        theProbe += qGray( image.pixel( x + 3, y + 2 ) );
        theProbe += qGray( image.pixel( x + 3 ,y + 3 ) );

        // Check if the grayscale value is high enough for the (premultiplied)
        // second threshold.
        if ( theProbe > step2TotalThreshold ) {

          // have a "cornfirmed" seed here..
          // now reconstruct its position;
          // do this recursively 3 times for improving the coordinates

          DefoPoint intermediate( x, y );
          QRect searchRect(
                intermediate.getPixX() - halfSquareWidth
              , intermediate.getPixY() - halfSquareWidth
              , 2 * halfSquareWidth
              , 2 * halfSquareWidth
          );

          int i = 0;

          // check if this point is still in the area, otherwise drop it
          // FIXME DefoSquare intersections
          while ( i < 4 && imageArea.contains( searchRect ) ) {
            intermediate = getCenterOfGravity(
                  image
                , searchRect
                , step3Threshold
            );
            searchRect.setCoords(
                  intermediate.getPixX() - halfSquareWidth
                , intermediate.getPixY() - halfSquareWidth
                , intermediate.getPixX() + halfSquareWidth
                , intermediate.getPixY() + halfSquareWidth
            );

//            std::cout << "i=" << i << ": " << intermediate << std::endl;

            ++i;
          }

          if ( i == 4 ) { // Iterated without drifting

            // check again since the point can be reconstructed at a distance
            // from the seed
            if (
                area.contains(intermediate.getPixX(), intermediate.getPixY())
                && !forbiddenAreas.isInside(intermediate)
            ) {
              // FIXME implement point/brightness isolation

              // Create DefoSquare around last found COG
              DefoSquare searchSquare( intermediate, halfSquareWidth );

              // FIXME central logging
//              if( debugLevel_ >= 3 )
//                std::cout << " [DefoMeasurement::findPoints] =3= Reconstructed "
//                          << "point at: x: " << intermediate.getX()
//                          << " y: " << intermediate.getY() << std::endl;

              // save square around this point as already tagged
              forbiddenAreas.push_back( searchSquare );

              if (roi->size()==0) {
                intermediate.setValid(true);
                points->push_back(intermediate);
              } else {
                if (roi->containsPoint(QPointF(intermediate.getPixX(), intermediate.getPixY()),
                                       Qt::OddEvenFill)) {
                  intermediate.setValid(true);
                  points->push_back(intermediate);
                }
              }
            }
          }
          // FIXME central logging
//          else if ( debugLevel_ >= 3 ) { // Log if point drifted away

//            std::cout << " [DefoMeasurement::findPoints] =3= (Pos1) Point: x: "
//                      << x << " y: " << y << " jumped to: x: "
//                      << intermediate.getX() << " y: " << intermediate.getY()
//                      << " . Dropping it. " << std::endl;

//          }

        }

        ++x;

      }
      else { // inside forbidden areas
        // TODO find area, skip it
//        x += 2*halfSquareWidth;
        ++x;
      }

    }

    ++y;

  }

//  if( debugLevel_ >= 2 )
//    std::cout << " [DefoMeasurement::findPoints] =2= "
//              << points.size() << " points found." << std::endl;

  // Now that all points have been found, determine their color
  determinePointColors(image, points, halfSquareWidth, step3Threshold);

  return points;

}

/**
  * Loops over all the points retrieved by calling findPoints and calls setBlue
  * of the DefoPoint. Please remark that this function does not check if there
  * is only one blue point, but tags all points exceeding the threshold!
  */
void DefoMeasurement::determinePointColors(
    const QImage& image
  , DefoPointCollection* points
  , int halfSquareWidth
  , int threshold
) const {

  QRect area;
  QColor color;
//  double yellow;
//  double blue;

  for ( DefoPointCollection::iterator it = points->begin();
        it < points->end();
        ++it
  ) {

    area.setCoords(
        it->getPixX() - halfSquareWidth
      , it->getPixY() - halfSquareWidth
      , it->getPixX() + halfSquareWidth
      , it->getPixY() + halfSquareWidth
    );

    /*
     * FIXME "Blueishness" is ill-defined.
     * Pixels that are perfectly white (r=g=b=1) will validate as blue.
     *    isBlue = ( 2*b / (r+g) ) > threshold (< 1)
     * A HSV-scheme with restrictions on the value and saturation might be
     * better
     */
    // Determine blue - yellow ratio
//    yellow = ( color.red() + color.green() ) / 2.0 ;
//    blue = color.blue();

    color = getAverageColor(image, area, threshold); //.toHsv();
    it->setColor( color );
  }

}

/**
  * Returns the grayscale weighted center of gravity of a certain image region
  * defined by area. If area is (partially) outside image, only pixels inside
  * image are taken into account. Otherwise a point at (0,0) is returned.
  */
// FIXME DefoSquare <> QRect
DefoPoint DefoMeasurement::getCenterOfGravity(
    const QImage& image
  , const QRect &area
  , int threshold
) const {

  // Ensure complete overlap of the area, everything outside is undefined
//  QRect area = area & image_.rect();
  DefoPoint weightedSum(0, 0);

  if ( !area.isNull() ) {
    const int right = area.x() + area.width();
    const int bottom = area.y() + area.height();

    int gray;
    double totalGray = 0.;
    double weightedX = 0.;
    double weightedY = 0.;

    for (int x = area.x(); x < right; ++x) {
      for (int y = area.y(); y < bottom; ++y) {

        gray = qGray( image.pixel(x, y) );

        // Threshold 3 reduces false positives
        if ( gray > threshold ) {
          totalGray += gray;
          weightedX += x*gray;
          weightedY += y*gray;
        }

      }
    }

//    std::cout << weightedX << ','
//              << weightedY << ','
//              << totalGray << ','
//              << (totalGray > 0) << std::endl;
    if ( totalGray > 0 )
      weightedSum.setPosition( weightedX/totalGray, weightedY/totalGray );

//    std::cout << weightedSum << std::endl;
    // otherwise position is (0,0) anyway
  }

  return weightedSum;

}


/**
  * Returns the average color of an area the image. If area is (partially)
  * outside image, only the pixels inside image are taken into account.
  * If the overlap of area and image is empty, rgb(0,0,0) is returned.
  */
const QColor DefoMeasurement::getAverageColor(
    const QImage& image
  , const QRect &area
  , int threshold
) const {

  // Ensure complete overlap of the area, everything outside is undefined
  QRect realArea = area & image.rect();
  QColor average(0, 0, 0);

  if ( !realArea.isNull() ) {
    QRgb pixel;
    int brightness = 0;
    double red = 0;
    double green = 0;
    double blue = 0;

    int totalBrightness = 0;

    const int right = realArea.x() + realArea.width();
    const int bottom = realArea.y() + realArea.height();
//    const int pixelCount = realArea.width() * realArea.height();

    for (int x = realArea.x(); x < right; ++x) {
      for (int y = realArea.y(); y < bottom; ++y) {
        pixel = image.pixel(x, y);
        brightness = qGray(pixel);
        // Only take into account sufficiently bright pixels, reduces background
        if ( brightness > threshold ) {
          red += qRed(pixel) * brightness;
          green += qGreen(pixel) * brightness;
          blue += qBlue(pixel) * brightness;
          totalBrightness += brightness;
        }
      }
    }

    average.setRgb(
          red/totalBrightness
        , green/totalBrightness
        , blue/totalBrightness
    );
  }

  return average;

}

void DefoMeasurement::setImageLocation(const QString& imageLocation) {

  imageLocation_ = imageLocation;
}
  
void DefoMeasurement::readExifData() {

  DefoExifReader reader(imageLocation_);
  reader.read();

  exifFocalLength_ = reader.getFloatValue("Exif.Photo.FocalLength");
  exifExposureTime_ = reader.getFloatValue("Exif.Photo.ExposureTime");
  exifExposureTimeString_ = reader.getStringValue("Exif.Photo.ExposureTime");
  exifAperture_ = reader.getLongValue("Exif.Photo.FNumber");
  exifISO_ = reader.getLongValue("Exif.Photo.ISOSpeedRatings");
}

void DefoMeasurement::acquireData(const DefoPointRecognitionModel* model) {

  pointRecognitionThresholds_.clear();

  pointRecognitionThresholds_.push_back(
     model->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_1)
  );
  pointRecognitionThresholds_.push_back(
     model->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_2)
  );
  pointRecognitionThresholds_.push_back(
     model->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_3)
  );
  
  pointRecognitionHalfSquareWidth_ = model->getHalfSquareWidth();
}

void DefoMeasurement::acquireData(const DefoConradModel* model) {

  conradState_ = model->getDeviceState();
  if (conradState_ != READY) return;

  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_1));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_2));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_3));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_4));
  panelStates_.push_back(model->getSwitchState(DefoConradModel::LIGHT_PANEL_5));
  ledState_ = model->getSwitchState(DefoConradModel::CALIBRATION_LEDS);
}

void DefoMeasurement::acquireData(const DefoJulaboModel* model) {

  julaboState_ = model->getDeviceState();
  if (julaboState_ != READY) return;

  circulatorState_ = model->isCirculatorEnabled();
  bathTemperature_ = model->getBathTemperature();
}

void DefoMeasurement::acquireData(const DefoKeithleyModel* model) {

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
      stream.writeAttribute("on", QString().setNum(panelStates_[0]==READY));
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

void DefoMeasurement::read(const QDir&path) {

  pointRecognitionHalfSquareWidth_ = 15;

  QString fileLocation = path.absoluteFilePath("%1.xml");
  fileLocation = fileLocation.arg(timestamp_.toString("yyyyMMddhhmmss"));

  QFile file(fileLocation);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoMeasurement") {
      // consistency check
      QString timestamp = stream.attributes().value("timestamp").toString();
      QDateTime dt = QDateTime::fromString(timestamp, "yyyyMMddhhmmss");
      if (dt!=getTimeStamp()) {
        // do something
      }
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
}

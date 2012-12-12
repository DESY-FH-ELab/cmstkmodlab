#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "DefoMeasurementListModel.h"

DefoMeasurementListModel::DefoMeasurementListModel(QObject *parent) :
    QObject(parent)
{}

/**
  \brief Adds a deformation measurement to the list.
  \arg measurement DefoMeasurement to be added.
  \arg select If true, the added measurement will be set as the current
  selection.
  */
void DefoMeasurementListModel::addMeasurement(
    DefoMeasurement* measurement
) {  

  QMutexLocker locker(&mutex_);

  // Store measurement* in list
  measurementList_.push_back(measurement);
  // Set initial point collection to NULL, i.e. non existant
  points_[measurement] = NULL;
//  std::cout << "Measurement on "
//            << measurementList_.back().getTimeStamp().toString().toStdString()
//            << std::endl;
  emit measurementCountChanged( getMeasurementCount() );

}


/// Returns the number of measurements currently stored.
int DefoMeasurementListModel::getMeasurementCount() const {
  return measurementList_.size();
}

/// Returns measurement at index i.
const DefoMeasurement* DefoMeasurementListModel::getMeasurement(int i) const {
  return measurementList_.at(i);
}

/// Returns measurement at index i.
DefoMeasurement* DefoMeasurementListModel::getMeasurement(int i) {
  return measurementList_.at(i);
}

/**
  Returns the point collection associated with the measurement, or NULL if
  the image has not been scanned yet (or the measurement is not in the list).
  */
const DefoPointCollection* DefoMeasurementListModel::getMeasurementPoints(
    DefoMeasurement* measurement
) {

  PointMap::const_iterator it = points_.find(measurement);

  if ( it == points_.end() )
    return NULL;
  else
    return it->second;

}


/**
  Sets the current point collection for the given measurement.
  If the measurement is not already present, this will add the measurement to
  the list!
  If adding points to the current set, please use appendMeasurementPoints,
  which makes the whole procedure thread safe.
  */
void DefoMeasurementListModel::setMeasurementPoints(
    DefoMeasurement *measurement
  , const DefoPointCollection *points
) {

  if ( points_.find(measurement) == points_.end() )
    addMeasurement(measurement);

  points_[measurement] = points;
  emit pointsUpdated(measurement);

}

/**
  Append the collection of points to the current collection of points.
  This is equivalent to calling getMeasurementPoints, adding points to a copy
  of that collection and then calling setMeasurementPoints with the extended
  copy, but this function makes the whole proces thread safe.
  */
void DefoMeasurementListModel::appendMeasurementPoints(
    DefoMeasurement *measurement
  , const DefoPointCollection *points
) {

  QMutexLocker locker(&mutex_);

  if ( points != NULL ) {

    const DefoPointCollection* currentList = getMeasurementPoints(measurement);
    if ( currentList != NULL ) {
      // Copy the current point collection
      DefoPointCollection* newPoints = new DefoPointCollection(*currentList);
      // Insert given points
      newPoints->insert(newPoints->end(), points->begin(), points->end());
      // Set new points
      setMeasurementPoints(measurement, newPoints);
    }
    else // Currently empty, so just set list
      setMeasurementPoints(measurement, points);

  }

}

void DefoMeasurementListModel::write(const QDir& path)
{
  QString fileLocation = path.absoluteFilePath("measurements.odmx");

  QFile file(fileLocation);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoMeasurements");

  const DefoMeasurement* measurement;
  int count = 1;
  for (int i = 0; i < this->getMeasurementCount(); ++i) {
    measurement = this->getMeasurement(i);
    if (measurement->isPreview()) continue;

    stream.writeStartElement("DefoMeasurement");
    stream.writeAttribute("index", QString().setNum(count++));
    stream.writeAttribute("timestamp", measurement->getTimeStamp().toString("yyyyMMddhhmmss"));
    stream.writeEndElement();
  }

  stream.writeEndElement();
  
  stream.writeEndDocument();
}

void DefoMeasurementListModel::writePoints(const QDir& path)
{
  QString filename = "points_%1.xml";
  for (int i = 0; i < this->getMeasurementCount(); ++i) {
    DefoMeasurement* measurement = this->getMeasurement(i);
    if (measurement->isPreview()) continue;

    const DefoPointCollection* points =this->getMeasurementPoints(measurement);
    if (!points || points->size()==0) continue;

    QString fileLocation = path.absoluteFilePath(filename.arg(measurement->getTimeStamp().toString("yyyyMMddhhmmss")));

    QFile file(fileLocation);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
      continue;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("DefoPoints");
    stream.writeAttribute("count", QString().setNum(points->size()));

    for (DefoPointCollection::const_iterator it = points->begin();
         it != points->end();
         ++it) {
      stream.writeStartElement("DefoPoint");
      stream.writeAttribute("x", QString().setNum(it->getX()));
      stream.writeAttribute("y", QString().setNum(it->getY()));
      int h, s ,v;
      QColor c = it->getColor();
      h = c.hsvHue();
      s = c.hsvSaturation();
      v = c.value();
      stream.writeAttribute("H", QString().setNum(h));
      stream.writeAttribute("S", QString().setNum(s));
      stream.writeAttribute("V", QString().setNum(v));
      stream.writeEndElement();
    }

    stream.writeEndElement();

    stream.writeEndDocument();
  }
}

void DefoMeasurementListModel::clear() {
  measurementList_.clear();
  points_.clear();
  emit measurementCountChanged(0);
}

void DefoMeasurementListModel::read(const QString& filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);
  DefoMeasurement* measurement;
  QDir basepath(filename);
  basepath.cdUp();

  while (!stream.atEnd()) {
    stream.readNextStartElement();
    if (stream.isStartElement() && stream.name()=="DefoMeasurement") {
      QString timestamp = stream.attributes().value("timestamp").toString();
      QDateTime dt = QDateTime::fromString(timestamp, "yyyyMMddhhmmss");

      QString imageLocation = basepath.absoluteFilePath("%1.jpg");
      imageLocation = imageLocation.arg(dt.toString("yyyyMMddhhmmss"));
      measurement = new DefoMeasurement(imageLocation, false);
      measurement->setTimeStamp(dt);

      QString dataLocation = basepath.absoluteFilePath("%1.xml");
      dataLocation = dataLocation.arg(dt.toString("yyyyMMddhhmmss"));
      measurement->read(dataLocation);

      addMeasurement(measurement);
    }
  }
}

void DefoMeasurementListModel::readPoints(const QDir& path)
{
  QString filename = "points_%1.xml";
  for (int i = 0; i < this->getMeasurementCount(); ++i) {
    DefoMeasurement* measurement = this->getMeasurement(i);
    if (measurement->isPreview()) continue;

    QString fileLocation = path.absoluteFilePath(filename.arg(measurement->getTimeStamp().toString("yyyyMMddhhmmss")));

    QFile file(fileLocation);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      continue;

    DefoPointCollection* points = new DefoPointCollection;

    QXmlStreamReader stream(&file);
    while (!stream.atEnd()) {
      stream.readNextStartElement();
      if (stream.isStartElement() && stream.name()=="DefoPoint") {
        float x = stream.attributes().value("x").toString().toFloat();
        float y = stream.attributes().value("y").toString().toFloat();
        int H = stream.attributes().value("H").toString().toInt();
        int S = stream.attributes().value("S").toString().toInt();
        int V = stream.attributes().value("V").toString().toInt();
        DefoPoint p(x, y);
        QColor c;
        c.setHsv(H, S, V);
        p.setColor(c);

        points->push_back(p);
      }
    }

    if (points->size()==0) {
      delete points;
      continue;
    }

    this->setMeasurementPoints(measurement, points);
  }
}

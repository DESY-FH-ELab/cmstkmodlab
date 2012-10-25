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
    const DefoMeasurementBase* measurement
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
const DefoMeasurementBase* DefoMeasurementListModel::getMeasurement(int i) const {
  return measurementList_.at(i);
}

/**
  Returns the point collection associated with the measurement, or NULL if
  the image has not been scanned yet (or the measurement is not in the list).
  */
const DefoPointCollection* DefoMeasurementListModel::getMeasurementPoints(
    const DefoMeasurementBase* measurement
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
    const DefoMeasurementBase *measurement
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
    const DefoMeasurementBase *measurement
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

  const DefoMeasurementBase* measurement;
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

void DefoMeasurementListModel::clear() {

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
      measurement = new DefoMeasurement(imageLocation);
      measurement->setTimeStamp(dt);

      QString dataLocation = basepath.absoluteFilePath("%1.xml");
      dataLocation = dataLocation.arg(dt.toString("yyyyMMddhhmmss"));
      measurement->read(dataLocation);

      addMeasurement(measurement);
    }
  }
}

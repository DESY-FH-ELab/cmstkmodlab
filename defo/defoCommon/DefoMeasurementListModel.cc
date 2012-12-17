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
    DefoPointCollection* newPoints;
    
    if ( currentList != NULL ) {
      newPoints = new DefoPointCollection(*currentList);
    } else {
      newPoints = new DefoPointCollection();
    }
    
    // Insert given points
    newPoints->insert(newPoints->end(), points->begin(), points->end());

    cleanupPoints(newPoints);

    // Set new points
    setMeasurementPoints(measurement, newPoints);
  }
}

void DefoMeasurementListModel::cleanupPoints(DefoPointCollection* points)
{
  double d;
  for (DefoPointCollection::iterator it1 = points->begin();
       it1 != points->end();
       ++it1) {
    if ((*it1).isValid()==false) continue;

    for (DefoPointCollection::iterator it2 = it1+1;
         it2 != points->end();
         ++it2) {
      if ((*it2).isValid()==false) continue;

      d = it1->getDistance(*it2);
      if (d<1.0) {
        (*it2).setValid(false);
      }
    }
  }

  for (DefoPointCollection::iterator it1 = points->begin();
       it1 != points->end();) {
    if ((*it1).isValid()==false) {
      points->erase(it1);
    } else {
      ++it1;
    }
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
      stream.writeAttribute("x", QString().setNum(it->getX(), 'e', 6));
      stream.writeAttribute("y", QString().setNum(it->getY(), 'e', 6));
      float h, s ,v;
      QColor c = it->getColor();
      h = c.hsvHueF();
      s = c.hsvSaturationF();
      v = c.valueF();
      stream.writeAttribute("H", QString().setNum(h, 'e', 6));
      stream.writeAttribute("S", QString().setNum(s, 'e', 6));
      stream.writeAttribute("V", QString().setNum(v, 'e', 6));

      stream.writeAttribute("indexed", QString().setNum(it->isIndexed()));
      if (it->isIndexed()) {
        stream.writeAttribute("ix", QString().setNum(it->getIndex().first));
        stream.writeAttribute("iy", QString().setNum(it->getIndex().second));
      }

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

      measurement->read(basepath);

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
        double x = stream.attributes().value("x").toString().toDouble();
        double y = stream.attributes().value("y").toString().toDouble();
        float H = stream.attributes().value("H").toString().toFloat();
        float S = stream.attributes().value("S").toString().toFloat();
        float V = stream.attributes().value("V").toString().toFloat();
        DefoPoint p(x, y);
        QColor c;
        c.setHsvF(H, S, V);
        p.setColor(c);

        p.setValid(true);

        bool indexed = (stream.attributes().value("indexed").toString().toInt()==1);
        p.unindex();
        if (indexed) {
          int ix = stream.attributes().value("ix").toString().toInt();
          int iy = stream.attributes().value("iy").toString().toInt();
          p.setIndex(ix, iy);
        }

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

DefoMeasurementPairListModel::DefoMeasurementPairListModel(QObject *parent) :
    QObject(parent)
{}

/**
  \brief Adds a deformation measurement pair to the list.
  \arg measurementPair DefoMeasurementPair to be added.
  \arg select If true, the added measurement will be set as the current
  selection.
  */
void DefoMeasurementPairListModel::addMeasurementPair(
    DefoMeasurement* reference
  , DefoMeasurement* deformed
) {

  QMutexLocker locker(&mutex_);

  // Store measurementPair* in list
  measurementPairList_.push_back(new DefoMeasurementPair(reference, deformed));

  emit measurementPairCountChanged( getMeasurementPairCount() );
}

/**
  \brief Adds a deformation measurement pair to the list.
  \arg measurementPair DefoMeasurementPair to be added.
  \arg select If true, the added measurement will be set as the current
  selection.
  */
void DefoMeasurementPairListModel::addMeasurementPair(
    DefoMeasurementPair* measurementPair
) {

  QMutexLocker locker(&mutex_);

  // Store measurementPair* in list
  measurementPairList_.push_back(measurementPair);

  emit measurementPairCountChanged( getMeasurementPairCount() );
}

/// Returns the number of measurements currently stored.
int DefoMeasurementPairListModel::getMeasurementPairCount() const {
  return measurementPairList_.size();
}

/// Returns measurement at index i.
const DefoMeasurementPair* DefoMeasurementPairListModel::getMeasurementPair(int i) const {
  return measurementPairList_.at(i);
}

/// Returns measurement at index i.
DefoMeasurementPair* DefoMeasurementPairListModel::getMeasurementPair(int i) {
  return measurementPairList_.at(i);
}

DefoMeasurementPair* DefoMeasurementPairListModel::findMeasurementPair(DefoMeasurement* reference,
                                                                       DefoMeasurement* deformed) {

  for (std::vector<DefoMeasurementPair*>::iterator it = measurementPairList_.begin();
       it != measurementPairList_.end();
       ++it) {
    DefoMeasurementPair* pair = *it;
    if (pair->first==reference && pair->second==deformed)
      return pair;
  }

  return 0;
 }

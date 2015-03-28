#include <fstream>

#include <nqlogger.h>

#include "DefoOfflinePreparationModel.h"
#include "DefoPointSaver.h"

DefoOfflinePreparationModel::DefoOfflinePreparationModel(DefoMeasurementListModel * listModel,
                                                         DefoMeasurementSelectionModel* refSelectionModel,
                                                         DefoMeasurementSelectionModel* defoSelectionModel,
                                                         DefoAlignmentModel* alignmentModel,
                                                         DefoPointIndexerModel* pointIndexerModel,
                                                         DefoColorSelectionModel* refColorModel,
                                                         DefoColorSelectionModel* defoColorModel,
                                                         DefoMeasurementPairListModel* pairListModel,
                                                         DefoMeasurementPairSelectionModel* pairSelectionModel,
                                                         QObject *parent) :
    QObject(parent),
    listModel_(listModel),
    pairListModel_(pairListModel),
    pairSelectionModel_(pairSelectionModel)
{
  angle_ = 0.0;
  refMeasurement_ = 0;
  defoMeasurement_ = 0;
  pointIndexer_ = 0;

  connect(refSelectionModel, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(refSelectionChanged(DefoMeasurement*)));

  connect(defoSelectionModel, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(defoSelectionChanged(DefoMeasurement*)));

  connect(listModel_, SIGNAL(pointsUpdated(const DefoMeasurement*)),
          this, SLOT(pointsUpdated(const DefoMeasurement*)));

  connect(alignmentModel, SIGNAL(alignmentChanged(double)),
          this, SLOT(alignmentChanged(double)));

  connect(pointIndexerModel, SIGNAL(pointIndexerChanged(DefoVPointIndexer*)),
          this, SLOT(pointIndexerChanged(DefoVPointIndexer*)));

  connect(refColorModel, SIGNAL(colorChanged(float,float)),
          this, SLOT(refColorChanged(float,float)));

  connect(defoColorModel, SIGNAL(colorChanged(float,float)),
          this, SLOT(defoColorChanged(float,float)));
}

void DefoOfflinePreparationModel::setCurrentDir(QDir& dir)
{
    currentDir_ = dir;
}

void DefoOfflinePreparationModel::refSelectionChanged(DefoMeasurement* measurement)
{
  refMeasurement_ = measurement;
}

void DefoOfflinePreparationModel::defoSelectionChanged(DefoMeasurement* measurement)
{
  defoMeasurement_ = measurement;
}

void DefoOfflinePreparationModel::pointsUpdated(const DefoMeasurement* /*measurement*/)
{

}

void DefoOfflinePreparationModel::alignmentChanged(double angle)
{
  angle_ = angle;
}

void DefoOfflinePreparationModel::pointIndexerChanged(DefoVPointIndexer * indexer)
{
  pointIndexer_ = indexer;
}

void DefoOfflinePreparationModel::refColorChanged(float hue, float saturation)
{
  refColor_.setHsvF(hue, saturation, 1.0, 1.0);
}

void DefoOfflinePreparationModel::defoColorChanged(float hue, float saturation)
{
  defoColor_.setHsvF(hue, saturation, 1.0, 1.0);
}

void DefoOfflinePreparationModel::prepare()
{
  if (refMeasurement_==0 || defoMeasurement_==0) {
    NQLogWarning("DefoOfflinePreparationModel") 
      << "reco: reference and deformed measurements not selected";
    return;
  }

  if (pointIndexer_==0) {
    NQLogWarning("DefoOfflinePreparationModel") 
      << "reco: point indexer not available";
    return;
  }

  const DefoPointCollection* refPoints = listModel_->getMeasurementPoints(refMeasurement_);
  if (!refPoints || refPoints->size()==0) {
     NQLogWarning("DefoOfflinePreparationModel") 
       << "reco: reference measurement does not contain points";
    return;
  }

  const DefoPointCollection* defoPoints = listModel_->getMeasurementPoints(defoMeasurement_);
  if (!defoPoints || defoPoints->size()==0) {
    NQLogWarning("DefoOfflinePreparationModel") 
      << "reco: deformed measurement does not contain points";
    return;
  }

  if (!alignPoints(refPoints, refCollection_)) {
    NQLogWarning("DefoOfflinePreparationModel") 
      << "reco: reference points could not be aligned";
    return;
  }

  if (!alignPoints(defoPoints, defoCollection_)) {
     NQLogWarning("DefoOfflinePreparationModel") 
       << "reco: deformed points could not be aligned";
    return;
  }

  pointIndexer_->indexPoints(&refCollection_, refColor_);

  pointIndexer_->indexPoints(&defoCollection_, defoColor_);

  QString filename = "offlinePoints_%1.xml";
  QString fileLocation;

  fileLocation = currentDir_.absoluteFilePath(filename.arg(refMeasurement_->getTimeStamp().toString("yyyyMMddhhmmss")));
  DefoPointSaver refSaver(fileLocation);
  refSaver.writeXMLPoints(refCollection_);

  fileLocation = currentDir_.absoluteFilePath(filename.arg(defoMeasurement_->getTimeStamp().toString("yyyyMMddhhmmss")));
  DefoPointSaver defoSaver(fileLocation);
  defoSaver.writeXMLPoints(defoCollection_);
}

bool DefoOfflinePreparationModel::alignPoints(const DefoPointCollection* original,
                                              DefoPointCollection& aligned)
{
  aligned.clear();

  if (angle_==0.0) {
    for (DefoPointCollection::const_iterator it = original->begin();
         it!=original->end();
         ++it) {
      aligned.push_back(*it);
    }
    return true;
  } else {

    double c = std::cos(angle_);
    double s = std::sin(angle_);

    for (DefoPointCollection::const_iterator it = original->begin();
         it!=original->end();
         ++it) {
      const DefoPoint& p = *it;
      DefoPoint np(p);
      np.setX(p.getX()*c - p.getY()*s);
      np.setY(p.getX()*s + p.getY()*c);

      aligned.push_back(np);
    }
  }

  return true;
}

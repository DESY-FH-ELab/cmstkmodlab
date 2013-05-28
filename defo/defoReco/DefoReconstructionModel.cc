#include <iostream>
#include <fstream>

#include "DefoReconstructionModel.h"
#include "DefoPointSaver.h"

DefoReconstructionModel::DefoReconstructionModel(
    DefoMeasurementListModel * listModel
  , DefoMeasurementSelectionModel* refSelectionModel
  , DefoMeasurementSelectionModel* defoSelectionModel
  , DefoAlignmentModel* alignmentModel
  , DefoPointIndexerModel* pointIndexerModel
  , DefoColorSelectionModel* refColorModel
  , DefoColorSelectionModel* defoColorModel
  , DefoMeasurementPairListModel* pairListModel
  , DefoMeasurementPairSelectionModel* pairSelectionModel
  , DefoGeometryModel* geometryModel
  , QObject *parent
  ) :
    QObject(parent),
    listModel_(listModel),
    pairListModel_(pairListModel),
    pairSelectionModel_(pairSelectionModel),
    geometryModel_(geometryModel)
{
  angle_ = 0.0;
  refMeasurement_ = 0;
  defoMeasurement_ = 0;
  pointIndexer_ = 0;

  connect(refSelectionModel,
          SIGNAL(selectionChanged(DefoMeasurement*)),
          this,
          SLOT(refSelectionChanged(DefoMeasurement*)));

  connect(defoSelectionModel,
          SIGNAL(selectionChanged(DefoMeasurement*)),
          this,
          SLOT(defoSelectionChanged(DefoMeasurement*)));

  connect(listModel_,
          SIGNAL(pointsUpdated(const DefoMeasurement*)),
          this,
          SLOT(pointsUpdated(const DefoMeasurement*)));

  connect(alignmentModel,
          SIGNAL(alignmentChanged(double)),
          this,
          SLOT(alignmentChanged(double)));

  connect(pointIndexerModel,
          SIGNAL(pointIndexerChanged(DefoVPointIndexer*)),
          this,
          SLOT(pointIndexerChanged(DefoVPointIndexer*)));

  connect(refColorModel,
          SIGNAL(colorChanged(float,float)),
          this,
          SLOT(refColorChanged(float,float)));

  connect(defoColorModel,
          SIGNAL(colorChanged(float,float)),
          this,
          SLOT(defoColorChanged(float,float)));

  reco_ = new DefoRecoSurface(this);

  reco_->setPitchX(22.3/5184.); // mm/pixel
  reco_->setPitchY(14.9/3456.); // mm/pixel

  connect(reco_,
          SIGNAL(incrementRecoProgress()),
          this,
          SLOT(incrementRecoProgress()));
}

void DefoReconstructionModel::refSelectionChanged(DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::referenceSelectionChanged(DefoMeasurement* measurement)" << std::endl;
  refMeasurement_ = measurement;
  emit setupChanged();
}

void DefoReconstructionModel::defoSelectionChanged(DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::defoSelectionChanged(DefoMeasurement* measurement)" << std::endl;
  defoMeasurement_ = measurement;
  emit setupChanged();
}

void DefoReconstructionModel::pointsUpdated(const DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::pointsUpdated(const DefoMeasurement* measurement)" << std::endl;
  if (measurement==refMeasurement_) emit setupChanged();
  if (measurement==defoMeasurement_) emit setupChanged();
}

void DefoReconstructionModel::alignmentChanged(double angle) {
  std::cout << "void DefoReconstructionModel::alignmentChanged(double angle)" << std::endl;
  angle_ = angle;
  emit setupChanged();
}

void DefoReconstructionModel::pointIndexerChanged(DefoVPointIndexer * indexer) {
  std::cout << "void DefoReconstructionModel::pointIndexerChanged(DefoVPointIndexer * indexer)" << std::endl;
  pointIndexer_ = indexer;
  emit setupChanged();
}

void DefoReconstructionModel::refColorChanged(float hue, float saturation) {
  std::cout << "void DefoReconstructionModel::refColorChanged(float hue, float saturation)" << std::endl;
  refColor_.setHsvF(hue, saturation, 1.0, 1.0);
  emit setupChanged();
}

void DefoReconstructionModel::defoColorChanged(float hue, float saturation) {
  std::cout << "void DefoReconstructionModel::defoColorChanged(float hue, float saturation)" << std::endl;
  defoColor_.setHsvF(hue, saturation, 1.0, 1.0);
  emit setupChanged();
}

void DefoReconstructionModel::geometryChanged() {
  std::cout << "void DefoReconstructionModel::geometryChanged()" << std::endl;

  // geometryModel_->getAngle1();
  // geometryModel_->getAngle2();
  // geometryModel_->getDistance();
  // geometryModel_->getHeight1();
  // geometryModel_->getHeight2();

  reco_->setNominalGridDistance(1.802*1000.);
  reco_->setNominalCameraDistance(1.822*1000.);
  //reco_->setNominalViewingAngle();

  emit setupChanged();
}

void DefoReconstructionModel::incrementRecoProgress() {
  emit incrementProgress();
}

void DefoReconstructionModel::reconstruct() {
  std::cout << "void DefoReconstructionModel::reconstruct()" << std::endl;

  emit recoProgressChanged(0);

  if (refMeasurement_==0 || defoMeasurement_==0) {
    std::cout << "reco: reference and deformed measurements not selected" << std::endl;
    return;
  }

  if (pointIndexer_==0) {
    std::cout << "reco: point indexer not available" << std::endl;
    return;
  }

  const DefoPointCollection* refPoints = listModel_->getMeasurementPoints(refMeasurement_);
  if (!refPoints || refPoints->size()==0) {
    std::cout << "reco: reference measurement does not contain points" << std::endl;
    return;
  }

  const DefoPointCollection* defoPoints = listModel_->getMeasurementPoints(defoMeasurement_);
  if (!defoPoints || defoPoints->size()==0) {
    std::cout << "reco: deformed measurement does not contain points" << std::endl;
    return;
  }

  if (!alignPoints(refPoints, refCollection_)) {
    std::cout << "reco: reference points could not be aligned" << std::endl;
    return;
  }
  emit incrementProgress();

  if (!alignPoints(defoPoints, defoCollection_)) {
    std::cout << "reco: deformed points could not be aligned" << std::endl;
    return;
  }
  emit incrementProgress();

  pointIndexer_->indexPoints(&refCollection_, refColor_);
  emit incrementProgress();

  pointIndexer_->indexPoints(&defoCollection_, defoColor_);
  emit incrementProgress();

  DefoPointSaver refSaver("./refPoints.txt");
  refSaver.writePoints(refCollection_);
  DefoPointSaver defoSaver("./defoPoints.txt");
  defoSaver.writePoints(defoCollection_);
  emit incrementProgress();

  reco_->setFocalLength(refMeasurement_->getFocalLength());
  emit incrementProgress();

  DefoSurface surface = reco_->reconstruct(defoCollection_, refCollection_);

  bool newPair = false;
  DefoMeasurementPair * measurementPair = pairListModel_->findMeasurementPair(refMeasurement_,
                                                                              defoMeasurement_);
  if (measurementPair==0) {
    newPair = true;
    measurementPair = new DefoMeasurementPair(refMeasurement_, defoMeasurement_);
  }

  measurementPair->setSurface(surface);

  if (newPair) {
    pairListModel_->addMeasurementPair(measurementPair);
  }
}

bool DefoReconstructionModel::alignPoints(const DefoPointCollection* original,
                                          DefoPointCollection& aligned) {

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

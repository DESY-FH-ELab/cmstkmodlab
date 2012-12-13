#include <iostream>
#include <fstream>

#include "DefoReconstructionModel.h"

DefoReconstructionModel::DefoReconstructionModel(
    DefoMeasurementListModel * listModel
  , DefoMeasurementSelectionModel* refSelectionModel
  , DefoMeasurementSelectionModel* defoSelectionModel
  , DefoAlignmentModel* alignmentModel
  , DefoColorSelectionModel* refColorModel
  , DefoColorSelectionModel* defoColorModel
  , QObject *parent
  ) :
    QObject(parent),
    listModel_(listModel)
{
  angle_ = 0.0;
  refMeasurement_ = 0;
  defoMeasurement_ = 0;

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

  connect(refColorModel,
          SIGNAL(colorChanged(float,float)),
          this,
          SLOT(refColorChanged(float,float)));

  connect(defoColorModel,
          SIGNAL(colorChanged(float,float)),
          this,
          SLOT(defoColorChanged(float,float)));
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

void DefoReconstructionModel::reconstruct() {
  std::cout << "void DefoReconstructionModel::reconstruct()" << std::endl;

  if (refMeasurement_==0 || defoMeasurement_==0) {
    std::cout << "reco: reference and deformed measurements not selected" << std::endl;
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

  if (!alignPoints(defoPoints, defoCollection_)) {
    std::cout << "reco: deformed points could not be aligned" << std::endl;
    return;
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

    std::ofstream ofile("/home/tkmodlab/software/cmstkmodlab/trunk/defo/defoReco/points.txt");

    for (DefoPointCollection::const_iterator it = original->begin();
         it!=original->end();
         ++it) {
      const DefoPoint& p = *it;
      DefoPoint np(p);
      np.setX(p.getX()*c - p.getY()*s);
      np.setY(p.getX()*s + p.getY()*c);

      ofile << p.getX() << " " << np.getX() << " " << p.getY() << " " << np.getY() << " " << np.getColor().hsvHue() << std::endl;

      aligned.push_back(np);
    }
  }

  return true;
}

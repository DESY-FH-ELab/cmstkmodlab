#include <iostream>

#include "DefoReconstructionModel.h"

DefoReconstructionModel::DefoReconstructionModel(
    DefoMeasurementListModel * listModel
  , QObject *parent
  ) :
    QObject(parent),
    listModel_(listModel)
{
  angle_ = 0.0;
}

void DefoReconstructionModel::referenceSelectionChanged(DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::referenceSelectionChanged(DefoMeasurement* measurement)" << std::endl;
  referenceMeasurement_ = measurement;
  emit setupChanged();
}

void DefoReconstructionModel::defoSelectionChanged(DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::defoSelectionChanged(DefoMeasurement* measurement)" << std::endl;
  defoMeasurement_ = measurement;
  emit setupChanged();
}

void DefoReconstructionModel::pointsUpdated(const DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::pointsUpdated(const DefoMeasurement* measurement)" << std::endl;
  if (measurement==referenceMeasurement_) emit setupChanged();
  if (measurement==defoMeasurement_) emit setupChanged();
}

void DefoReconstructionModel::alignmentChanged(double angle)
{
  std::cout << "void DefoReconstructionModel::alignmentChanged(double angle)" << std::endl;
  angle_ = angle;
  emit setupChanged();
}

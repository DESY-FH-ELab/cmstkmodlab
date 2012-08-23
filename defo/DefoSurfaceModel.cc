#include "DefoSurfaceModel.h"

DefoSurfaceModel::DefoSurfaceModel(QObject *parent) :
    QObject(parent)
{
  reference_ = NULL;
}

DefoSurfaceModel::~DefoSurfaceModel() {
//  delete reference_;
}

/**
  * Returns whether a reference measurement is currently set.
  */
bool DefoSurfaceModel::hasReferenceMeasurement() const {
  return reference_ != NULL;
}

/**
  * Returns a reference to the current reference surface or NULL
  * if none is set.
  */
const DefoMeasurement * DefoSurfaceModel::getReferenceMeasurement() const {
  return reference_;
}

/**
  * Sets the current reference measurement.
  */
void DefoSurfaceModel::setReferenceMeasurement(DefoMeasurement *reference) {
//  delete reference_;
  reference_ = reference;

//  QString time = reference_->getTimeStamp().toString("dd.MM.yyyy");
//  std::cout << time.toStdString() << std::endl;

  emit referenceChanged();
}

/**
  * Adds a deformation measurement to the model.
  */
void DefoSurfaceModel::addMeasurement(const DefoMeasurement &measurement) {
  measurementList_.push_back(measurement);
  emit measurementCountChanged();
}

/**
  * Returns the number of measurements currently stored, excluding the
  * reference measurement.
  */
int DefoSurfaceModel::getMeasurementCount() const {
  return measurementList_.size();
}

/**
  * Returns measurement at index i.
  */
const DefoMeasurement& DefoSurfaceModel::getMeasurement(int i) const {
  return measurementList_.at(i);
}

/**
  * Removes surface at index i.
  */
void DefoSurfaceModel::removeMeasurement(int i) {
//  if (index < getSurfaceCount()) {
//    deformedList_.erase(deformedList_.begin() + i);
//    emit measurementCountChanged();
  //  }
}

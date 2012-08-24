#include "DefoMeasurementListModel.h"

DefoMeasurementListModel::DefoMeasurementListModel(QObject *parent) :
    QObject(parent)
  , selectedIndex_(-1)
{}

//DefoMeasurementListModel::~DefoMeasurementListModel() {
////  delete reference_;
//}

///**
//  * Returns whether a reference measurement is currently set.
//  */
//bool DefoMeasurementListModel::hasReferenceMeasurement() const {
//  return reference_ != NULL;
//}

///**
//  * Returns a reference to the current reference surface or NULL
//  * if none is set.
//  */
//const DefoMeasurement * DefoMeasurementListModel::getReferenceMeasurement() const {
//  return reference_;
//}

///**
//  * Sets the current reference measurement.
//  */
//void DefoMeasurementListModel::setReferenceMeasurement(DefoMeasurement *reference) {
////  delete reference_;
//  reference_ = reference;

////  QString time = reference_->getTimeStamp().toString("dd.MM.yyyy");
////  std::cout << time.toStdString() << std::endl;

//  emit referenceChanged();
//}

/**
  \brief Adds a deformation measurement to the list.
  \arg measurement DefoMeasurement to be added.
  \arg select If true, the added measurement will be set as the current
  selection.
  */
void DefoMeasurementListModel::addMeasurement(
    const DefoMeasurement &measurement
  , bool select
) {

  measurementList_.push_back(measurement);
//  std::cout << "Measurement on "
//            << measurementList_.back().getTimeStamp().toString().toStdString()
//            << std::endl;
  emit measurementCountChanged( getMeasurementCount() );

  // Select if wanted or first measurement
  if (select || getMeasurementCount() == 0)
    setSelectionIndex(measurementList_.size() - 1);

}


/// Returns the number of measurements currently stored.
int DefoMeasurementListModel::getMeasurementCount() const {
  return measurementList_.size();
}

/// Returns measurement at index i.
const DefoMeasurement& DefoMeasurementListModel::getMeasurement(int i) const {
  return measurementList_.at(i);
}

/**
  Sets the currently selected measurement.
  \arg index Index of the measurement to be selected.
  */
void DefoMeasurementListModel::setSelectionIndex(int index) {

  if ( index != selectedIndex_ ) {

    // Only store if the new value makes sense.
    if ( index > -1 && index < getMeasurementCount() )
      selectedIndex_ = index;

    emit selectionChanged(selectedIndex_);
  }

}

const DefoMeasurement& DefoMeasurementListModel::getSelection() const {
  return measurementList_.at(selectedIndex_);
}

///// Removes surface at index i.
//void DefoMeasurementListModel::removeMeasurement(int i) {
////  if (index < getSurfaceCount()) {
////    deformedList_.erase(deformedList_.begin() + i);
////    emit measurementCountChanged();
//  //  }
//}

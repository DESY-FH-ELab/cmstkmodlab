#include "DefoPointFinder.h"

DefoPointFinder::DefoPointFinder(
    DefoMeasurementListModel *listModel
  , DefoPointRecognitionModel *pointModel
  , const DefoMeasurement *measurement
  , const QRect &searchRectangle
) :
    listModel_(listModel)
  , pointModel_(pointModel)
  , measurement_(measurement)
  , searchArea_(searchRectangle)
{}

void DefoPointFinder::run() {

  const DefoPointCollection* points = measurement_->findPoints(
      &searchArea_
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_1)
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_2)
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_3)
    , pointModel_->getHalfSquareWidth()
  );

  listModel_->appendMeasurementPoints(measurement_, points);

}

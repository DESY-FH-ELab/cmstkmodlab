
#include "DefoPointFinder.h"

DefoPointFinder::DefoPointFinder(
    DefoMeasurementListModel *listModel
  , DefoPointRecognitionModel *pointModel
  , DefoMeasurement *measurement
  , const QRect &searchRectangle
  , DefoROIModel * roiModel
) :
    listModel_(listModel)
  , pointModel_(pointModel)
  , measurement_(measurement)
  , searchArea_(searchRectangle)
  , roiModel_(roiModel)
{}

void DefoPointFinder::run() {

  QPolygonF roi;

  if (roiModel_) {
    QImage image = measurement_->getImage();

    for (int j=0;j<roiModel_->size();++j) {
      const QPointF& p = roiModel_->at(j);
      QPointF np(p.x()*image.width(), p.y()*image.height());
      roi.push_back(np);
    }
  }

  const DefoPointCollection* points = measurement_->findPoints(
      &searchArea_
    , &roi
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_1)
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_2)
    , pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_3)
    , pointModel_->getHalfSquareWidth()
  );

  listModel_->appendMeasurementPoints(measurement_, points);
}

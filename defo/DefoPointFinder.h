#ifndef DEFOPOINTFINDER_H
#define DEFOPOINTFINDER_H

#include <QThread>
#include "DefoMeasurement.h"
#include "DefoMeasurementListModel.h"
#include "DefoPointRecognitionModel.h"
#include "DefoROIModel.h"

class DefoPointFinder : public QThread {

  Q_OBJECT
public:
  explicit DefoPointFinder(
      DefoMeasurementListModel *listModel
    , DefoPointRecognitionModel *pointModel
    , DefoMeasurement *measurement
    , const QRect& searchRectangle
    , DefoROIModel * roiModel = 0
  );

protected:
  DefoMeasurementListModel* listModel_;
  DefoPointRecognitionModel* pointModel_;
  DefoMeasurement* measurement_;
  const QRect searchArea_;
  DefoROIModel* roiModel_;

  void run();
};

#endif // DEFOPOINTFINDER_H

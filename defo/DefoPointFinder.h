#ifndef DEFOPOINTFINDER_H
#define DEFOPOINTFINDER_H

#include <QThread>
#include "DefoMeasurement.h"
#include "DefoMeasurementListModel.h"
#include "DefoPointRecognitionModel.h"

class DefoPointFinder : public QThread {

  Q_OBJECT
public:
  explicit DefoPointFinder(
      DefoMeasurementListModel *listModel
    , DefoPointRecognitionModel *pointModel
    , const DefoMeasurementBase *measurement
    , const QRect& searchRectangle
  );

protected:
  DefoMeasurementListModel* listModel_;
  DefoPointRecognitionModel* pointModel_;
  const DefoMeasurementBase* measurement_;
  const QRect searchArea_;

  void run();

};

#endif // DEFOPOINTFINDER_H

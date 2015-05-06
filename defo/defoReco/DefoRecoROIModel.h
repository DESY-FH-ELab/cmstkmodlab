#ifndef DEFORECOROIMODEL_H
#define DEFORECOROIMODEL_H

#include <QObject>
#include <QPolygonF>

#include <DefoMeasurementSelectionModel.h>
#include <DefoROIModel.h>

class DefoRecoROIModel : public DefoROIModel
{
    Q_OBJECT

public:

  explicit DefoRecoROIModel(DefoMeasurementSelectionModel *selectionModel,
                            QObject *parent = 0);

  explicit DefoRecoROIModel(DefoMeasurementSelectionModel *selectionModel,
                            DefoRecoROIModel *other,
                            QObject *parent = 0);

protected:

  DefoMeasurementSelectionModel *selectionModel_;
  DefoRecoROIModel *otherROIModel_;

protected slots:

  void selectionChanged(DefoMeasurement*);
  void otherROIChanged(bool);
};

#endif // DEFORECOROIMODEL_H

#ifndef DEFORECONSTRUCTIONMODEL_H
#define DEFORECONSTRUCTIONMODEL_H

#include <QObject>
#include <QImage>
#include <QSize>

#include "DefoMeasurement.h"
#include "DefoMeasurementListModel.h"

class DefoReconstructionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoReconstructionModel(DefoMeasurementListModel * listModel
                                 , QObject *parent = 0);

  double getAngle() const { return angle_; }

public slots:

  void referenceSelectionChanged(DefoMeasurement*);
  void defoSelectionChanged(DefoMeasurement*);
  void pointsUpdated(const DefoMeasurement*);
  void alignmentChanged(double angle);

protected:

  DefoMeasurementListModel* listModel_;
  DefoMeasurement* referenceMeasurement_;
  DefoMeasurement* defoMeasurement_;
  float angle_;

signals:

  void setupChanged();
};

#endif // DEFORECOIMAGEZOOMMODEL_H

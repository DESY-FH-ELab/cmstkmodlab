#ifndef DEFORECONSTRUCTIONMODEL_H
#define DEFORECONSTRUCTIONMODEL_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QColor>

#include "DefoMeasurement.h"
#include "DefoPoint.h"
#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoAlignmentModel.h"
#include "DefoColorSelectionModel.h"

class DefoReconstructionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoReconstructionModel(DefoMeasurementListModel* listModel
                                 , DefoMeasurementSelectionModel* refSelectionModel
                                 , DefoMeasurementSelectionModel* defoSelectionModel
                                 , DefoAlignmentModel* alignmentModel
                                 , DefoColorSelectionModel* refColorModel
                                 , DefoColorSelectionModel* defoColorModel
                                 , QObject *parent = 0);

  double getAngle() const { return angle_; }

public slots:

  void refSelectionChanged(DefoMeasurement*);
  void defoSelectionChanged(DefoMeasurement*);
  void pointsUpdated(const DefoMeasurement*);
  void alignmentChanged(double angle);
  void refColorChanged(float hue, float saturation);
  void defoColorChanged(float hue, float saturation);

  void reconstruct();

protected:

  bool alignPoints(const DefoPointCollection* original,
                   DefoPointCollection& aligned);

  DefoMeasurementListModel* listModel_;
  DefoMeasurement* refMeasurement_;
  DefoMeasurement* defoMeasurement_;
  float angle_;
  DefoPointCollection refCollection_;
  DefoPointCollection defoCollection_;
  QColor refColor_;
  QColor defoColor_;

signals:

  void setupChanged();
};

#endif // DEFORECOIMAGEZOOMMODEL_H

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
#include "DefoPointIndexerModel.h"
#include "DefoColorSelectionModel.h"
#include "DefoGeometryModel.h"
#include "DefoRecoSurface.h"

class DefoReconstructionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoReconstructionModel(DefoMeasurementListModel* listModel
                                 , DefoMeasurementSelectionModel* refSelectionModel
                                 , DefoMeasurementSelectionModel* defoSelectionModel
                                 , DefoAlignmentModel* alignmentModel
                                 , DefoPointIndexerModel* pointIndexerModel
                                 , DefoColorSelectionModel* refColorModel
                                 , DefoColorSelectionModel* defoColorModel
                                 , DefoMeasurementPairListModel* pairListModel
                                 , DefoMeasurementPairSelectionModel* pairSelectionModel
                                 , DefoGeometryModel* geometryModel
                                 , QObject *parent = 0);

  double getAngle() const { return angle_; }

public slots:

  void refSelectionChanged(DefoMeasurement*);
  void defoSelectionChanged(DefoMeasurement*);
  void pointsUpdated(const DefoMeasurement*);
  void alignmentChanged(double angle);
  void pointIndexerChanged(DefoVPointIndexer *);
  void refColorChanged(float hue, float saturation);
  void defoColorChanged(float hue, float saturation);
  void geometryChanged();

  void reconstruct();
  void incrementRecoProgress();

protected:

  bool alignPoints(const DefoPointCollection* original,
                   DefoPointCollection& aligned);

  DefoMeasurementListModel* listModel_;
  DefoMeasurement* refMeasurement_;
  DefoMeasurement* defoMeasurement_;
  DefoMeasurementPairListModel* pairListModel_;
  DefoMeasurementPairSelectionModel* pairSelectionModel_;
  DefoGeometryModel* geometryModel_;
  float angle_;
  DefoPointCollection refCollection_;
  DefoPointCollection defoCollection_;
  DefoVPointIndexer* pointIndexer_;
  QColor refColor_;
  QColor defoColor_;

  DefoRecoSurface *reco_;

signals:

  void setupChanged();
  void recoProgressChanged(int);
  void incrementProgress();
};

#endif // DEFORECONSTRUCTIONMODEL_H

#ifndef DEFOOFFLINEPREPARATIONMODEL_H
#define DEFOOFFLINEPREPARATIONMODEL_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QColor>
#include <QDir>

#include "DefoMeasurement.h"
#include "DefoPoint.h"
#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoAlignmentModel.h"
#include "DefoPointIndexerModel.h"
#include "DefoColorSelectionModel.h"

class DefoOfflinePreparationModel : public QObject
{
    Q_OBJECT
public:
    explicit DefoOfflinePreparationModel(DefoMeasurementListModel* listModel
                                         , DefoMeasurementSelectionModel* refSelectionModel
                                         , DefoMeasurementSelectionModel* defoSelectionModel
                                         , DefoAlignmentModel* alignmentModel
                                         , DefoPointIndexerModel* pointIndexerModel
                                         , DefoColorSelectionModel* refColorModel
                                         , DefoColorSelectionModel* defoColorModel
                                         , DefoMeasurementPairListModel* pairListModel
                                         , DefoMeasurementPairSelectionModel* pairSelectionModel
                                         , QObject *parent = 0);

    double getAngle() const { return angle_; }

public slots:

  void setCurrentDir(QDir& dir);
  void refSelectionChanged(DefoMeasurement*);
  void defoSelectionChanged(DefoMeasurement*);
  void pointsUpdated(const DefoMeasurement*);
  void alignmentChanged(double angle);
  void pointIndexerChanged(DefoVPointIndexer *);
  void refColorChanged(float hue, float saturation);
  void defoColorChanged(float hue, float saturation);

  void prepare();

protected:

  bool alignPoints(const DefoPointCollection* original,
                   DefoPointCollection& aligned);

  QDir currentDir_;
  DefoMeasurementListModel* listModel_;
  DefoMeasurement* refMeasurement_;
  DefoMeasurement* defoMeasurement_;
  DefoMeasurementPairListModel* pairListModel_;
  DefoMeasurementPairSelectionModel* pairSelectionModel_;
  float angle_;
  DefoPointCollection refCollection_;
  DefoPointCollection defoCollection_;
  DefoVPointIndexer* pointIndexer_;
  QColor refColor_;
  QColor defoColor_;
};

#endif // DEFOOFFLINEPREPARATIONMODEL_H

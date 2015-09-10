#ifndef DEFOPOINTFINDER_H
#define DEFOPOINTFINDER_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QPolygonF>

#include <TGraph2D.h>
#include <TF2.h>

#include "DefoMeasurement.h"
#include "DefoMeasurementListModel.h"
#include "DefoPointRecognitionModel.h"
#include "DefoROIModel.h"

class DefoPointFinder : public QThread
{
  Q_OBJECT

public:

  explicit DefoPointFinder(int block,
                           QMutex* mutex,
                           DefoMeasurementListModel *listModel,
                           DefoPointRecognitionModel *pointModel,
                           DefoMeasurement *measurement,
                           const QRect& searchRectangle,
                           bool do2Dfit = false,
                           DefoROIModel * roiModel = 0);
  ~DefoPointFinder();

  void run();

protected:

  int block_;
  QMutex* mutex_;
  DefoMeasurementListModel* listModel_;
  DefoPointRecognitionModel* pointModel_;
  DefoMeasurement* measurement_;
  const QRect searchArea_;
  bool do2Dfit_;
  DefoROIModel* roiModel_;

  TGraph2D *gr2D_;
  TF2* fitFunc_;

  QImage image_;

  const DefoPointCollection* findPoints(const QRect* searchArea,
                                        const QPolygonF* roi,
                                        int step1Threshold,
                                        int step2Threshold,
                                        int step3Threshold,
                                        int halfSquareWidth);
 
  DefoPoint getCenterOfGravity(const QRect &area,
                               int threshold) const;
 
  DefoPoint getFitPosition(const DefoPoint& intermediate,
			   const QRect &area,
			   int threshold) const;

  void determinePointColors(DefoPointCollection* points,
                            int halfSquareWidth,
                            int threshold) const;

  const QColor getAverageColor(const QRect& area,
                               int threshold) const;

signals:

  void pointsFound(DefoMeasurement*, const DefoPointCollection*);
};

#endif // DEFOPOINTFINDER_H

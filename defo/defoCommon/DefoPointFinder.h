#ifndef DEFOPOINTFINDER_H
#define DEFOPOINTFINDER_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QPolygonF>

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
			   DefoROIModel * roiModel = 0);
  ~DefoPointFinder();

protected:

  int block_;
  QMutex* mutex_;
  DefoMeasurementListModel* listModel_;
  DefoPointRecognitionModel* pointModel_;
  DefoMeasurement* measurement_;
  const QRect searchArea_;
  DefoROIModel* roiModel_;

  QImage image_;

  void run();

  const DefoPointCollection* findPoints(const QRect* searchArea,
                                        const QPolygonF* roi,
                                        int step1Threshold,
                                        int step2Threshold,
                                        int step3Threshold,
                                        int halfSquareWidth);
 
  DefoPoint getCenterOfGravity(const QRect &area,
                               int threshold) const;

  void determinePointColors(DefoPointCollection* points,
                            int halfSquareWidth,
                            int threshold) const;

  const QColor getAverageColor(const QRect& area,
                               int threshold) const;
};

#endif // DEFOPOINTFINDER_H

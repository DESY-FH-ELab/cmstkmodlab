#ifndef DEFORECOMAINWINDOW_H
#define DEFORECOMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>

#include "DefoRecoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoROIModel.h"
#include "DefoAlignmentModel.h"

#include "DefoMeasurementListTreeWidget.h"
#include "DefoMeasurementInfoListTreeWidget.h"
#include "DefoMeasurementCommentTextView.h"
#include "DefoImageWidget.h"

#include "DefoPointRecognitionModel.h"
#include "DefoPointRecognitionWidget.h"

#include "DefoPointIndexerModel.h"
#include "DefoColorSelectionModel.h"

#include "DefoOfflinePreparationModel.h"

#include "DefoReconstructionModel.h"
#include "DefoReconstructionWidget.h"

#include "DefoGeometryModel.h"
#include "DefoGeometryWidget.h"

#include "DefoCalibrationModel.h"
#include "DefoCalibrationWidget.h"

#include "Defo2DSplineInterpolationModel.h"
#include "Defo2DSplineInterpolationWidget.h"

class DefoRecoMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit DefoRecoMainWindow(QWidget *parent = 0);

protected:

  QDir currentDir_;
  QList<QDir> tempDirs_;

  QTabWidget* tabWidget_;

  DefoRecoMeasurementListModel *listModel_;
  DefoMeasurementSelectionModel *selectionModel_;

  DefoMeasurementSelectionModel *roiSelectionModel_;
  DefoROIModel *roiModel_;

  DefoMeasurementSelectionModel *alignmentSelectionModel_;
  DefoAlignmentModel *alignmentModel_;

  DefoMeasurementSelectionModel *refSelectionModel_;
  DefoMeasurementSelectionModel *defoSelectionModel_;

  DefoPointRecognitionModel* refPointModel_;
  DefoPointRecognitionModel* defoPointModel_;

  DefoPointIndexerModel* pointIndexerModel_;
  DefoColorSelectionModel* refColorModel_;
  DefoColorSelectionModel* defoColorModel_;

  DefoGeometryModel* geometryModel_;
  DefoCalibrationModel* calibrationModel_;
  Defo2DSplineInterpolationModel* interpolationModel_;

  DefoMeasurementPairListModel* measurementPairListModel_;
  DefoMeasurementPairSelectionModel* measurementPairSelectionModel_;

  DefoOfflinePreparationModel* offlineModel_;
  DefoReconstructionModel* reconstructionModel_;

protected slots:

  void quit();
  void loadMeasurementButtonClicked();
  void saveMeasurementButtonClicked();
  void exportMeasurementButtonClicked();
};

#endif // DEFORECOMAINWINDOW_H

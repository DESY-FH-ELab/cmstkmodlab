#ifndef DEFORECOPOINTRECOGNICTIONWIDGET_H
#define DEFORECOPOINTRECOGNICTIONWIDGET_H

#include <QTabWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QString>
#include <QMutex>

#include "DefoMeasurementSelectionModel.h"
#include "DefoPointRecognitionModel.h"
#include "DefoROIModel.h"
#include "DefoImageWidget.h"
#include "DefoRecoImageWidget.h"

class DefoRecoPointRecognitionWidget : public QTabWidget
{
  Q_OBJECT

public:

  explicit DefoRecoPointRecognitionWidget(DefoMeasurementListModel* listModel,
					  DefoMeasurementSelectionModel* selectionModel,
					  DefoPointRecognitionModel* pointModel,
					  DefoROIModel* roiModel,
					  QWidget *parent = 0);

public slots:

  virtual void selectionChanged(DefoMeasurement* measurement);
  void thresholdValueChanged(DefoPointRecognitionModel::Threshold threshold,
                             int value);
  void halfSquareWidthChanged(int value);

protected:

  // Models
  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoPointRecognitionModel* pointModel_;
  DefoROIModel* roiModel_;

  // Widgets
  DefoImageThresholdsWidget* thresholdImage_;
  DefoImagePointsWidget * pointsImage_;
  QCheckBox* fitPoints_;
  QPushButton* findPoints_;
  QPushButton* savePoints_;

  QMutex mutex_;

protected slots:

  void findPointsButtonClicked();
  void savePointsButtonClicked();
  void controlStateChanged(bool);
};

#endif // DEFORECOPOINTRECOGNICTIONWIDGET_H

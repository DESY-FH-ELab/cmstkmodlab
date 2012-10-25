#ifndef DEFOPOINTRECOGNICTIONWIDGET_H
#define DEFOPOINTRECOGNICTIONWIDGET_H

#include <QTabWidget>
#include <QPushButton>

#include "DefoMeasurementSelectionModel.h"
#include "DefoPointRecognitionModel.h"
#include "DefoImageWidget.h"

class DefoPointRecognitionWidget : public QTabWidget
{
  Q_OBJECT
public:
  explicit DefoPointRecognitionWidget(
      DefoMeasurementListModel* listModel
    , DefoMeasurementSelectionModel* selectionModel
    , DefoPointRecognitionModel* pointModel
    , QWidget *parent = 0
  );

protected:
  // Models
  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoPointRecognitionModel* pointModel_;

  // Widgets
  DefoImageThresholdsWidget* thresholdImage_;
  DefoImagePointsWidget * pointsImage_;
  QPushButton* findPoints_;
  QPushButton* savePoints_;

protected slots:
  void findPointsButtonClicked();
  void savePointsButtonClicked();
  void controlStateChanged(bool);
};

#endif // DEFOCAMERAWIDGET_H

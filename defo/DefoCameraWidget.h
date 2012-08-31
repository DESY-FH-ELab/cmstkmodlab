#ifndef DEFOCAMERAWIDGET_H
#define DEFOCAMERAWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>

#include "DefoState.h"
#include "DefoCameraModel.h"
#include "DefoCameraOptionComboBox.h"

#include "DefoConradModel.h"
#include "DefoConradCheckbox.h"

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoMeasurementListComboBox.h"
#include "DefoImageWidget.h"

class DefoCameraWidget : public QWidget
{
  Q_OBJECT
public:
  explicit DefoCameraWidget(
      DefoCameraModel* cameraModel
    , DefoConradModel* conradModel
    , DefoMeasurementListModel* listModel
    , DefoMeasurementSelectionModel* selectionModel
    , QWidget *parent = 0
  );

protected:
  // Models
  DefoCameraModel* cameraModel_;
  DefoConradModel* conradModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoMeasurementListModel* listModel_;

  // Widgets
  QCheckBox* enableCheckBox_;
  QPushButton* pictureButton_;
  QWidget* buttons_;

protected slots:
  void deviceStateChanged(State newState);
  void openButtonClicked();
//  void saveButtonClicked();

};

#endif // DEFOCAMERAWIDGET_H

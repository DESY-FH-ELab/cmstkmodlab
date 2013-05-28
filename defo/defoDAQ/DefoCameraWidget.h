#ifndef DEFOCAMERAWIDGET_H
#define DEFOCAMERAWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QStackedWidget>
#include <QFrame>
#include <QPlainTextEdit>
#include <QSpinBox>

#include "DefoState.h"
#include "DefoCameraModel.h"
#include "DefoCameraOptionComboBox.h"

#include "DefoConradModel.h"
#include "DefoConradCheckbox.h"

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoMeasurementListComboBox.h"
#include "DefoImageWidget.h"

class DefoImageCommentEdit : public QPlainTextEdit {
public:
    DefoImageCommentEdit(QWidget* parent) : QPlainTextEdit(parent) { }
    virtual QSize sizeHint() const { return QSize(500, 50); }
};

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
  QPushButton* previewButton_;
  QPushButton* pictureButton_;
  QCheckBox* liveviewCheckBox_;
  QWidget* buttons_;
  DefoImageCommentEdit* commentEditor_;
  QSpinBox* calibAmplitudeSpinBox_;
  QStackedWidget* imageStack_;
  QFrame* buttonsNRawImage_;
  QWidget* buttonsCamera_;
  DefoRawImageWidget* rawImage_;
  QFrame* liveImageFrame_;
  DefoLiveViewImageWidget* liveImage_;

protected slots:
  void deviceStateChanged(State newState);
  void controlStateChanged(bool);
  void liveViewModeChanged(bool enabled);
  void previewButtonClicked();
  void pictureButtonClicked();
};

#endif // DEFOCAMERAWIDGET_H

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>

#include "DefoConradModel.h"
#include "DefoConradWidget.h"

#include "DefoJulaboModel.h"
#include "DefoJulaboWidget.h"

#include "DefoKeithleyModel.h"
#include "DefoKeithleyWidget.h"

#include "DefoCameraModel.h"
#include "DefoCameraWidget.h"

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoMeasurementListComboBox.h"
#include "DefoImageWidget.h"

#include "DefoPointRecognitionModel.h"
#include "DefoThresholdSpinBox.h"
#include "DefoPointFinder.h"


class CoordinateSaver : public QFile
{
  Q_OBJECT
public:
  explicit CoordinateSaver(const QString& filename, QObject* parent = 0);
  ~CoordinateSaver();
  void writePoint(double x, double y);

protected:
  const static QString LINE_FORMAT;

};

class Photographer : public QObject
{
  Q_OBJECT
public:
  explicit Photographer(
      DefoMeasurementListModel* listModel
    , DefoCameraModel* cameraModel
    , int interval
    , QObject* parent = 0
  );

protected:
  QTimer timer_;
  int interval_;
  DefoMeasurementListModel* listModel_;
  DefoCameraModel* cameraModel_;

protected slots:
  void takePicture();
  void setCameraState(State state);

};

class TestWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit TestWindow(QWidget *parent = 0);

public slots:
  void pointButtonClicked();
  void newCameraImage(QString location);

protected:
  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoCameraModel* cameraModel_;
  DefoPointRecognitionModel* pointModel_;

  DefoImagePointsWidget* pointsImage_;

  QDir currentDir_;

  Photographer* photographer;

protected slots:
  void writePoints();

};

#endif // TESTWINDOW_H

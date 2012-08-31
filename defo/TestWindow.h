#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>
#include <QThread>

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

//  QTimer* timer_;
//  int pictureInterval_;

protected slots:
//  void timedPicture();
  void writePoints();

};

class CoordinateSaver : public QFile {

  Q_OBJECT
public:
  explicit CoordinateSaver(const QString& filename, QObject* parent = 0);
  ~CoordinateSaver();
  void writePoint(double x, double y);

protected:
  const static QString LINE_FORMAT;

};

class PointFinder : public QThread {

  Q_OBJECT
public:
  explicit PointFinder(
      DefoMeasurementListModel *listModel
    , DefoPointRecognitionModel *pointModel
    , const DefoMeasurement *measurement
    , const QRect& searchRectangle
  );

protected:
  DefoMeasurementListModel* listModel_;
  DefoPointRecognitionModel *pointModel_;
  const DefoMeasurement* measurement_;
  const QRect searchArea_;

  void run();

};

#endif // TESTWINDOW_H

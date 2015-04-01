#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>

#include <QMutex>

#include "DefoConradModel.h"
#include "DefoConradWidget.h"

#include "DefoJulaboModel.h"
#include "DefoJulaboWidget.h"

#include "KeithleyModel.h"
#include "KeithleyWidget.h"

#include "DefoCameraModel.h"
#include "DefoCameraWidget.h"

#include "DefoScriptModel.h"
#include "DefoScriptWidget.h"

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoMeasurementListComboBox.h"
#include "DefoImageWidget.h"

#include "DefoPointRecognitionModel.h"
#include "DefoThresholdSpinBox.h"
#include "DefoPointFinder.h"
#include "DefoPoint.h"


class CoordinateSaver : public QFile
{
  Q_OBJECT
public:
  explicit CoordinateSaver(const QString& filename, QObject* parent = 0);
  ~CoordinateSaver();
  void writePoint(const DefoPoint &point);

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
  DefoMeasurementListModel* listModel_;
  DefoCameraModel* cameraModel_;
  int interval_;
  QTimer timer_;

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
  void newCameraImage(QString location, bool keep);

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

class ScriptWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ScriptWindow(QWidget *parent = 0);

public slots:

protected:
  DefoScriptModel* scriptModel_;

protected slots:
};

#endif // TESTWINDOW_H

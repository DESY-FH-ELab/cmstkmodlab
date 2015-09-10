#ifndef DEFOMAINWINDOW_H
#define DEFOMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
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

#include "DefoKeithleyModel.h"
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
#include "DefoPointRecognitionWidget.h"

#include "DefoDAQModel.h"
#include "DefoDAQThread.h"
#include "DefoDAQServer.h"
#include "DefoDAQStreamer.h"

class DefoMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit DefoMainWindow(QWidget *parent = 0);

public slots:

  void quit();
  void exportMeasurement();
  void prepareNewMeasurement();
  void newCameraImage(QString location, bool keep);
  void newCameraImages(QStringList locations);
  void setControlsEnabled(bool);

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  QPushButton *exportMeasurementButton_;
  QPushButton *newMeasurementButton_;

  DefoConradModel* conradModel_;
  DefoJulaboModel* julaboModel_;
  DefoKeithleyModel* keithleyModel_;
  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoCameraModel* cameraModel_;
  DefoPointRecognitionModel* pointModel_;

  DefoScriptModel* scriptModel_;

  DefoDAQModel* daqModel_;
  DefoDAQThread* daqThread_;
  DefoDAQServer* daqServer_;
  DefoDAQStreamer* daqStreamer_;

  QMutex mutex_;
};

#endif // DEFOMAINWINDOW_H

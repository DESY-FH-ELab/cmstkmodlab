#ifndef DEFOMAINWINDOW_H
#define DEFOMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
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

class DefoMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit DefoMainWindow(QWidget *parent = 0);

public slots:
  void newCameraImage(QString location, bool keep);
  void pointButtonClicked();

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  DefoConradModel* conradModel_;
  DefoJulaboModel* julaboModel_;
  DefoKeithleyModel* keithleyModel_;
  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoCameraModel* cameraModel_;
  DefoPointRecognitionModel* pointModel_;

  DefoScriptModel* scriptModel_;

protected slots:
  //void writePoints();

};

#endif // DEFOMAINWINDOW_H

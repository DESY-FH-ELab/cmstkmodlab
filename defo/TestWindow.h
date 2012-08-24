#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>

#include "DefoConradModel.h"
#include "DefoConradWidget.h"

#include "DefoJulaboModel.h"
#include "DefoJulaboWidget.h"

#include "DefoKeithleyModel.h"
#include "DefoKeithleyWidget.h"

#include "DefoCameraModel.h"
#include "DefoCameraOptionComboBox.h"

#include "DefoMeasurementListModel.h"
#include "DefoImageWidget.h"

#include "DefoPointRecognitionModel.h"
#include "DefoThresholdSlider.h"

class TestWindow : public QMainWindow
{
    Q_OBJECT
public:
  explicit TestWindow(QWidget *parent = 0);

public slots:
  void fileButtonClicked();
  void newCameraImage(QImage newImage);

protected:
  DefoMeasurementListModel* listModel_;
  DefoCameraModel* cameraModel_;

};

#endif // TESTWINDOW_H

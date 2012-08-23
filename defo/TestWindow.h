#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QImage>
#include <QComboBox>

#include "DefoImageWidget.h"
#include "DefoSurfaceModel.h"

#include "DefoConradModel.h"
#include "DefoConradWidget.h"

#include "DefoJulaboModel.h"
#include "DefoJulaboWidget.h"

#include "DefoKeithleyModel.h"
#include "DefoKeithleyWidget.h"

#include "DefoCameraModel.h"
#include "DefoCameraOptionComboBox.h"

class TestWindow : public QMainWindow
{
    Q_OBJECT
public:
  explicit TestWindow(QWidget *parent = 0);
  ~TestWindow();

protected:
  DefoSurfaceModel* model_;
  DefoCameraModel* cameraModel_;
  EOS550D* camera_;

signals:

public slots:
  void fileButtonClicked();
  void cameraButtonClicked();
  void newCameraImage(QImage newImage);

};

#endif // TESTWINDOW_H

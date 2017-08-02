/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef DEFOIMAGEWIDGET_H
#define DEFOIMAGEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QDateTime>
#include <QSize>

#include "DefoCameraModel.h"
#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoImageZoomModel.h"
#include "DefoPointRecognitionModel.h"

class DefoImageBaseWidget : public QWidget
{
  Q_OBJECT

public:

  DefoImageBaseWidget(QWidget *parent = 0);

protected:

  virtual void paintEvent(QPaintEvent *event) = 0;

  /// Subroutine for paintEvent that prepares the image to be drawn.
  virtual QImage prepareImage(const QImage& image) const = 0;

  static const QSize MINIMUM_SIZE;

  QSize getImageDrawingSize(const QImage& image) const;
};

class DefoImageWidget : public DefoImageBaseWidget
{
  Q_OBJECT

public:

  explicit DefoImageWidget(DefoMeasurementSelectionModel* model,
                           QWidget *parent = 0);

public slots:

  virtual void selectionChanged(DefoMeasurement* measurement);

protected:

  DefoMeasurementSelectionModel* selectionModel_;

  void paintEvent(QPaintEvent *event);

  /// Subroutine for paintEvent that prepares the image to be drawn.
  virtual QImage prepareImage(const QImage& image) const = 0;
};

class DefoLiveViewImageWidget : public DefoImageBaseWidget
{
  Q_OBJECT

public:

  enum MarkerMode {
    None = 0,
    Boxed = 1,
    Axis = 2,
    MarkerModeMax = 3
  };

  explicit DefoLiveViewImageWidget(DefoCameraModel* model,
                                   QWidget *parent = 0);

public slots:

  virtual void newLiveViewImage(QString location);

protected:

  DefoCameraModel* cameraModel_;
  void paintEvent(QPaintEvent *event);

  /// Subroutine for paintEvent that prepares the image to be drawn.
  QImage prepareImage(const QImage& image) const;

  MarkerMode markerMode_;
  virtual void keyReleaseEvent(QKeyEvent * event);
};

/// Displays the raw camera image.
class DefoRawImageWidget : public DefoImageWidget {

  Q_OBJECT

public:

  explicit DefoRawImageWidget(DefoMeasurementSelectionModel* model,
                              QWidget *parent = 0);

  QImage prepareImage(const QImage& image) const;

};

/// Displays a visualisation of the current point recognition thresholds.
class DefoImageThresholdsWidget : public DefoImageWidget
{

  Q_OBJECT

public:

  explicit DefoImageThresholdsWidget(DefoMeasurementSelectionModel* selectionModel,
                                     DefoPointRecognitionModel* recognitionModel,
                                     QWidget *parent = 0);

  QImage prepareImage(const QImage& image) const;

public slots:

  void thresholdChanged(DefoPointRecognitionModel::Threshold threshold,
                        int value);

  void selectionChanged(DefoMeasurement* measurement);
  void controlStateChanged(bool);

protected:

  DefoPointRecognitionModel* recognitionModel_;
  QImage imageCache_;

  void updateCache();
};

/*
  POINT RECOGNITION DISPLAY
 */
class DefoImagePointsWidget : public DefoRawImageWidget {

  Q_OBJECT

public:

  explicit DefoImagePointsWidget(DefoMeasurementListModel *listModel,
                                 DefoMeasurementSelectionModel *selectionModel,
                                 QWidget *parent);

protected:

  DefoMeasurementListModel* listModel_;
  void paintEvent(QPaintEvent *event);

protected slots:

  void pointsUpdated(const DefoMeasurement* measurement);
};

#endif // DEFOIMAGEWIDGET_H

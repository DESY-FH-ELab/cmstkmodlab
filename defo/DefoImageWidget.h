#ifndef DEFOIMAGEWIDGET_H
#define DEFOIMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDateTime>
#include <QSize>

#include "DefoMeasurementListModel.h"
#include "DefoPointRecognitionModel.h"

class DefoImageWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoImageWidget(
      DefoMeasurementListModel* model
    , QWidget *parent = 0
  );

protected:
  DefoMeasurementListModel* measurementListModel_;
  void paintEvent(QPaintEvent *event);

  /// Subroutine for paintEvent that prepares the image to be drawn.
  virtual QImage prepareImage(const QImage& image) const = 0;

  static const QSize MINIMUM_SIZE;
  QSize getImageDrawingSize(const QImage& image) const;

public slots:
  virtual void selectionChanged(int index);

};

/// Displays the raw camera image.
class DefoRawImageWidget : public DefoImageWidget {

    Q_OBJECT
public:
  explicit DefoRawImageWidget(
      DefoMeasurementListModel* model
    , QWidget *parent = 0
  );

  QImage prepareImage(const QImage& image) const;

};

/// Displays a visualisation of the current point recognition thresholds.
class DefoImageThresholdsWidget : public DefoImageWidget {

  Q_OBJECT
public:
  explicit DefoImageThresholdsWidget(
      DefoMeasurementListModel* listModel
    , DefoPointRecognitionModel* recognitionModel
    , QWidget *parent = 0
  );

  QImage prepareImage(const QImage& image) const;

public slots:
  void thresholdChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void selectionChanged(int index);

protected:
  DefoPointRecognitionModel* recognitionModel_;
  QImage imageCache_;
  void updateCache();

};

#endif // DEFOIMAGEWIDGET_H

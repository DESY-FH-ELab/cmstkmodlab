#ifndef DEFORECOIMAGEWIDGET_H
#define DEFORECOIMAGEWIDGET_H

#include <QScrollArea>
#include <QWidget>
#include <QPainter>
#include <QSize>

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoImageZoomModel.h"
#include "DefoROIModel.h"
#include "DefoAlignmentModel.h"

class DefoRecoImageContentWidget : public QWidget
{
  Q_OBJECT

public:

  explicit DefoRecoImageContentWidget(DefoMeasurementSelectionModel* model,
                                      DefoImageZoomModel* zoomModel,
                                      QWidget *parent = 0);
  
  virtual QSize getImageDrawingSize(const QImage& image) const;
  virtual QImage prepareImage(const QImage& image) const;

  virtual void paintEvent(QPaintEvent *event);

public slots:

  virtual void selectionChanged(DefoMeasurement* measurement);

protected:

  DefoMeasurementSelectionModel* selectionModel_;
  DefoImageZoomModel* zoomModel_;
};

class DefoRecoRawImageContentWidget : public DefoRecoImageContentWidget
{
  Q_OBJECT

public:

  explicit DefoRecoRawImageContentWidget(DefoMeasurementSelectionModel* model,
                                         DefoRecoImageZoomModel* zoomModel,
                                         QWidget *parent = 0);
};

class DefoRecoROIImageContentWidget : public DefoRecoImageContentWidget
{
  Q_OBJECT

public:

  explicit DefoRecoROIImageContentWidget(DefoMeasurementSelectionModel* model,
                                         DefoRecoImageZoomModel* zoomModel,
                                         DefoROIModel* roiModel,
                                         QWidget *parent = 0);
  
  void paintEvent(QPaintEvent *event);

protected:

  DefoROIModel* roiModel_;
};

class DefoRecoImageThresholdsContentWidget : public DefoRecoImageContentWidget
{
  Q_OBJECT

public:

  explicit DefoRecoImageThresholdsContentWidget(DefoMeasurementSelectionModel* model,
                                                DefoRecoImageZoomModel* zoomModel,
                                                DefoPointRecognitionModel* recognitionModel,
                                                DefoROIModel* roiModel,
                                                QWidget *parent = 0);

  QImage prepareImage(const QImage& image) const;

public slots:

  void thresholdChanged(DefoPointRecognitionModel::Threshold threshold,
                        int value);
  void selectionChanged(DefoMeasurement* measurement);
  void roiChanged();

protected:

  virtual bool event(QEvent* event);

  DefoROIModel* roiModel_;
  DefoPointRecognitionModel* recognitionModel_;
  QImage imageCache_;
  void updateCache();
  bool needsUpdate_;
};

class DefoRecoImagePointsContentWidget : public DefoRecoImageContentWidget
{
  Q_OBJECT

public:

  explicit DefoRecoImagePointsContentWidget(DefoMeasurementListModel* listModel,
                                            DefoMeasurementSelectionModel* selectionModel,
                                            DefoRecoImageZoomModel* zoomModel,
                                            QWidget *parent = 0);

  void paintEvent(QPaintEvent *event);

protected:

  DefoMeasurementListModel* listModel_;

protected slots:

void pointsUpdated(const DefoMeasurement* measurement);
};

class DefoRecoAlignmentImageContentWidget : public DefoRecoImageContentWidget
{
  Q_OBJECT

public:

  explicit DefoRecoAlignmentImageContentWidget(DefoMeasurementSelectionModel* model,
                                               DefoRecoImageZoomModel* zoomModel,
                                               DefoAlignmentModel* alignmentModel,
                                               QWidget *parent = 0);

  void paintEvent(QPaintEvent *event);

protected:

  DefoAlignmentModel* alignmentModel_;
};

class DefoRecoImageWidget : public QScrollArea
{
  Q_OBJECT

public:

  DefoRecoImageWidget(DefoMeasurementSelectionModel* model,
                      QWidget *parent = 0);

  void resizeEvent(QResizeEvent *event);
  virtual void keyReleaseEvent(QKeyEvent * event);

public slots:

  void selectionChanged(DefoMeasurement*);
  void zoomFactorChanged(float newScaling, float oldScaling);

protected:

  DefoMeasurementSelectionModel* selectionModel_;
  DefoRecoImageZoomModel* zoomModel_;
  static const QSize MINIMUM_SIZE;
};

class DefoRecoRawImageWidget : public DefoRecoImageWidget
{
  Q_OBJECT

public:

  DefoRecoRawImageWidget(DefoMeasurementSelectionModel* model,
                         QWidget *parent = 0);
};

class DefoRecoROIImageWidget : public DefoRecoImageWidget
{
  Q_OBJECT

public:

  DefoRecoROIImageWidget(DefoMeasurementSelectionModel* model,
                         DefoROIModel* roiModel,
                         QWidget *parent = 0);

  virtual void mouseMoveEvent(QMouseEvent * e);
  virtual void mousePressEvent(QMouseEvent * e);
  virtual void mouseReleaseEvent(QMouseEvent * e);

public slots:

  void roiChanged();
  void showContextMenu(const QPoint&);

protected:

  DefoROIModel* roiModel_;
};

class DefoRecoImageThresholdsWidget : public DefoRecoImageWidget
{
  Q_OBJECT

public:

  DefoRecoImageThresholdsWidget(DefoMeasurementSelectionModel* model,
                                DefoPointRecognitionModel* recognitionModel,
                                DefoROIModel* roiModel,
                                QWidget *parent = 0);
};

class DefoRecoImagePointsWidget : public DefoRecoImageWidget
{
  Q_OBJECT

public:

  explicit DefoRecoImagePointsWidget(DefoMeasurementListModel *listModel,
                                     DefoMeasurementSelectionModel *selectionModel,
                                     QWidget *parent);
};

class DefoRecoAlignmentImageWidget : public DefoRecoImageWidget
{
  Q_OBJECT

public:

  DefoRecoAlignmentImageWidget(DefoMeasurementSelectionModel* model,
                               DefoAlignmentModel* alignmentModel,
                               QWidget *parent = 0);

  virtual void mouseMoveEvent(QMouseEvent * e);
  virtual void mousePressEvent(QMouseEvent * e);
  virtual void mouseReleaseEvent(QMouseEvent * e);
  virtual void keyReleaseEvent(QKeyEvent * event);

public slots:

  void alignmentChanged(double angle);

protected:

  DefoAlignmentModel* alignmentModel_;
};

#endif // DEFORECOIMAGEWIDGET_H

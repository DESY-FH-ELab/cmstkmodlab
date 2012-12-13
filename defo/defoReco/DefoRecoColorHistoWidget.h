#ifndef DEFORECOCOLORHISTOWIDGET_H
#define DEFORECOCOLORHISTOWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QSize>

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoColorSelectionModel.h"

class DefoRecoColorHistoWidget : public QWidget {
  Q_OBJECT
public:
  explicit DefoRecoColorHistoWidget(
      DefoMeasurementListModel* listModel
    , DefoMeasurementSelectionModel* selectionModel
    , DefoColorSelectionModel* colorModel
    , QWidget *parent = 0
  );

  virtual void paintEvent(QPaintEvent *event);

public slots:
  void pointsUpdated(const DefoMeasurement*);
  void selectionChanged(DefoMeasurement* measurement);
  void colorChanged(float hue, float saturation);

protected:

  void mouseMoveEvent(QMouseEvent * e);

  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoColorSelectionModel* colorModel_;
};

#endif // DEFORECOCOLORHISTOWIDGET_H

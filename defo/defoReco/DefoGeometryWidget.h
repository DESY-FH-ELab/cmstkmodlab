#ifndef DEFOGEOMETRYWIDGET_H
#define DEFOGEOMETRYWIDGET_H

#include <QWidget>
#include <QSvgWidget>
#include <QDoubleSpinBox>

#include "DefoGeometryModel.h"

class DefoGeometryWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoGeometryWidget(
      DefoGeometryModel* geometryModel
    , QWidget *parent = 0
  );

protected:

  DefoGeometryModel* geometryModel_;

  QSvgWidget * sketch_ ;
  void prepareSketch();

  QDoubleSpinBox * angle1SpinBox_;
  QDoubleSpinBox * angle2SpinBox_;
  QDoubleSpinBox * distanceSpinBox_;
  QDoubleSpinBox * heightSpinBox_;

protected slots:

  void angle1Changed(double);
  void angle2Changed(double);
  void distanceChanged(double);
  void heightChanged(double);
  void geometryChanged();
};

#endif // DEFOGEOMETRYWIDGET_H
